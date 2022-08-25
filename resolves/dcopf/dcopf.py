from egret.data.model_data import ModelData
from egret.model_library.transmission import tx_utils
import numpy as np
import csv
import enum
import pyomo.environ as pyomo_environ
import pyomo_coek.components_only as components_only
import pyomo_coek.full_integration as expression_wrappers
import math


class AML(str, enum.Enum):
    pyomo = 'pyomo'
    hybrid_components_only = 'hybrid_components_only'
    hybrid_expression_wrappers = 'hybrid_expression_wrappers'


def declare_load_params(pe, m, md: ModelData, index_set):
    m.pl = pe.Param(index_set, mutable=True)

    for b in index_set:
        m.pl[b].value = 0

    for load_name, load in md.data['elements']['load'].items():
        b = load['bus']
        if b in index_set and 'p_load' in load:
            m.pl[b].value += load['p_load']


def declare_var_pg(pe, m, md: ModelData, index_set):
    m.pg = pe.Var(index_set)

    for g in index_set:
        gen = md.data['elements']['generator'][g]
        pmin = gen['p_min']
        pmax = gen['p_max']
        m.pg[g].setlb(pmin)
        m.pg[g].setub(pmax)


def declare_var_pf(pe, m, md: ModelData, index_set):
    m.pf = pe.Var(index_set)

    for b in index_set:
        branch = md.data['elements']['branch'][b]
        smax = branch['rating_long_term']
        if smax is not None:
            m.pf[b].setlb(-smax)
            m.pf[b].setub(smax)


def declare_var_va(pe, m, md: ModelData, index_set):
    m.va = pe.Var(index_set)

    ref_bus = md.data['system']['reference_bus']
    ref_angle = md.data['system']['reference_bus_angle']
    m.va[ref_bus].fix(math.radians(ref_angle))


def declare_power_flow_con(pe, m, md: ModelData, index_set):
    m.power_flow_con = pe.Constraint(index_set)

    for bname in index_set:
        branch = md.data['elements']['branch'][bname]
        from_bus = branch['from_bus']
        to_bus = branch['to_bus']

        tau = 1
        shift = 0

        print(branch['branch_type'])
        if branch['branch_type'] == 'transformer':
            tau = branch['transformer_tap_ratio']
            shift = math.radians(branch['transformer_phase_shift'])

        x = branch['reactance']
        b = -1/(tau*x)

        m.power_flow_con[bname] = m.pf[bname] == b * (m.va[from_bus] - m.va[to_bus] + shift)


def declare_power_balance(pe, m, md: ModelData, index_set):
    m.power_balance = pe.Constraint(index_set)

    exprs = dict()
    for b in index_set:
        exprs[b] = 0

    for branch_name, branch in md.data['elements']['branch'].items():
        exprs[branch['from_bus']] -= m.pf[branch_name]
        exprs[branch['to_bus']] += m.pf[branch_name]

    for gen_name, gen in md.data['elements']['generator'].items():
        exprs[gen['bus']] += m.pg[gen_name]

    for b in index_set:
        exprs[b] -= m.pl[b]

    if 'shunt' in md.data['elements']:
        for shunt_name, shunt in md.data['elements']['shunt'].items():
            if shunt['shunt_type'] == 'fixed':
                exprs[shunt['bus']] -= shunt['gs']

    for b in index_set:
        m.power_balance[b] = exprs[b] == 0


def build_dcopf_model(fname: str, aml: AML):
    if aml == AML.pyomo:
        pe = pyomo_environ
    elif aml == AML.hybrid_components_only:
        pe = components_only
    elif aml == AML.hybrid_expression_wrappers:
        pe = expression_wrappers
    else:
        raise ValueError(f'Unrecognized AML: {aml}')

    md = ModelData.read(fname)
    md = md.clone_in_service()
    tx_utils.scale_ModelData_to_pu(md, inplace=True)

    m = pe.ConcreteModel()

    m.bus_set = pe.Set(initialize=list(md.data['elements']['bus'].keys()))
    m.gen_set = pe.Set(initialize=list(md.data['elements']['generator'].keys()))
    m.branch_set = pe.Set(initialize=list(md.data['elements']['branch'].keys()))

    declare_load_params(pe, m, md, m.bus_set)

    declare_var_va(pe, m, md, m.bus_set)
    declare_var_pg(pe, m, md, m.gen_set)
    declare_var_pf(pe, m, md, m.branch_set)

    declare_power_flow_con(pe, m, md, m.branch_set)
    declare_power_balance(pe, m, md, m.bus_set)



def get_min_and_max_load(options: Options):
    md = ModelData.read(options.case)
    m, scaled_md = create_btheta_dcopf_model(md)
    m.scale = pe.Var(bounds=(0.1, 10))
    m.load_scale_cons = pe.ConstraintList()

    for k, v in m.pl.items():
        v.unfix()
        m.load_scale_cons.add(v == m.scale * v.value)
        
    m.obj.deactivate()
    m.scale_obj = pe.Objective(expr=m.scale)

    opt = pe.SolverFactory(options.solver)
    res = opt.solve(m, tee=False, keepfiles=True)
    pe.assert_optimal_termination(res)
    min_scale = m.scale.value

    m.scale_obj.sense = pe.maximize
    res = opt.solve(m, tee=False)
    pe.assert_optimal_termination(res)
    max_scale = m.scale.value

    return min_scale, max_scale


def param_updates(options: Options):
    timer = HierarchicalTimer()
    timer.start('load_data')
    md = ModelData.read(options.case)
    timer.stop('load_data')
    timer.start('build_model')
    m, scaled_md = create_btheta_dcopf_model(md)
    timer.stop('build_model')
    timer.start('get min and max load')
    min_scale, max_scale = get_min_and_max_load(options)
    _min_scale = max(min(0.5*(min_scale + max_scale), 0.8), min_scale)
    _max_scale = min(max(0.5*(min_scale + max_scale), 1.2), max_scale)
    min_scale = _min_scale
    max_scale = _max_scale
    timer.stop('get min and max load')

    base_loads = dict()
    for k, v in m.pl.items():
        base_loads[k] = v.value

    if options.replace_with_params:
        pl_set = list(m.pl.keys())
        m.pl_param = pe.Param(pl_set, mutable=True, initialize=base_loads)
        replace_dict = dict()
        for k, v in m.pl.items():
            replace_dict[id(v)] = m.pl_param[k]

        for con in m.component_data_objects(pe.Constraint, descend_into=True, active=True):
            new_body = replace_expressions(con.body, replace_dict)
            if con.equality:
                con.set_value(new_body == con.lower)
            else:
                con.set_value((con.lower, new_body, con.upper))

    timer.start('create solver')
    #opt = pe.SolverFactory(options.solver)
    opt = appsi.solvers.Ipopt()
    timer.stop('create solver')

    if isinstance(opt, appsi.base.PersistentSolver):
        opt.update_config.check_for_new_or_removed_constraints = False
        opt.update_config.update_constraints = False
        opt.update_config.check_for_new_or_removed_vars = False
        opt.update_config.update_vars = False
        opt.config.stream_solver = False
        
    print('***** first solve ***********')
    timer.start('first solve')
    res = opt.solve(m)
    #pe.assert_optimal_termination(res)
    timer.stop('first solve')

    load_list = [float(i) for i in np.linspace(min_scale, max_scale, options.n_resolves)]
    obj_list = list()
    timer.start('resolves')
    for load in load_list:
        if options.replace_with_params:
            timer.start('update param vals')
            for k, p in m.pl_param.items():
                p.set_value(base_loads[k] * load, idx=k)
            timer.stop('update param vals')
        else:
            timer.start('update var vals')
            for k, v in m.pl.items():
                v.value = base_loads[k] * load
            timer.stop('update var vals')
        timer.start('actual solve')
        res = opt.solve(m, timer=timer)
        timer.stop('actual solve')
        #pe.assert_optimal_termination(res)
        obj_list.append(pe.value(m.obj))
    timer.stop('resolves')
    
    print(timer)
    return obj_list, timer


def main():
    options = parse_args()

    ipopt_options = Options()
    ipopt_options.solver = 'appsi_ipopt'
    ipopt_options.case = options.case
    ipopt_options.replace_with_params = False
    ipopt_options.n_resolves = options.n_resolves
    ipopt_obj_list, ipopt_timer = param_updates(ipopt_options)
    ipopt_obj_list = np.array(ipopt_obj_list)
    quit()

    first_solve_times = dict()
    resolve_times = dict()
    solvers_to_test = ['cbc', 'ipopt', 'gurobi', 'gurobi_direct', 'scip', 'appsi_cbc', 'appsi_ipopt', 'appsi_gurobi', 'appsi_highs']
    #solvers_to_test = ['appsi_gurobi']
    for solver in solvers_to_test:
        print(solver)
        options.solver = solver
        obj_list, timer = param_updates(options)
        obj_list = np.array(obj_list)
        if not np.allclose(obj_list, ipopt_obj_list):
            raise RuntimeError('failed\n' + str(np.abs(obj_list - ipopt_obj_list)))
        first_solve_times[solver] = timer.get_total_time('first solve')
        resolve_times[solver] = timer.get_total_time('resolves')

    f = open('param_update_results.csv', 'w')
    writer = csv.writer(f)
    writer.writerow(['solver', 'first solve time', 'resolve time'])
    for solver in solvers_to_test:
        writer.writerow([solver, first_solve_times[solver], resolve_times[solver]])


if __name__ == '__main__':
    main()
