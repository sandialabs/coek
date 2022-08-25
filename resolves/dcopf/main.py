import pyomo.environ as pe
from egret.data.model_data import ModelData
from egret.models.dcopf import create_btheta_dcopf_model
import argparse
import time
from pyomo.contrib import appsi
from pyomo.common.timing import HierarchicalTimer
import numpy as np
from pyomo.core.expr.visitor import replace_expressions, ExpressionValueVisitor
import cProfile
import pstats
import io
import line_profiler
from pyomo.core.base.param import _ParamData
from typing import Optional
import csv


class Options(object):
    def __init__(self):
        self.case: str = 'pglib-opf-master/pglib_opf_case118_ieee.m'
        self.solver: str = 'appsi_gurobi'
        self.n_resolves: int = 10
        self.replace_with_params = True


def parse_args() -> Options:
    parser = argparse.ArgumentParser()
    parser.add_argument('--case', dest='case', required=True)
    parser.add_argument('--solver', dest='solver', default='gurobi_direct')
    parser.add_argument('--n_resolves', dest='n_resolves', default=100, type=int)
    args = parser.parse_args()
    res = Options()
    res.case = args.case
    res.solver = args.solver
    res.n_resolves = args.n_resolves
    return res


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
