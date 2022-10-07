import argparse
from pyomo.contrib import appsi
import numpy as np
from dcopf import AML, build_dcopf_model, _get_pe
from pyomo.common.timing import TicTocTimer
import tqdm


class Options(object):
    def __init__(self):
        self.case: str = "pglib-opf-master/pglib_opf_case118_ieee.m"
        self.aml: AML = AML.pyomo
        self.solver: str = "gurobi"
        self.n_resolves: int = 10


def parse_args() -> Options:
    parser = argparse.ArgumentParser()
    parser.add_argument("--case", dest="case", required=True)
    parser.add_argument("--aml", dest="aml", default=AML.pyomo, type=AML)
    parser.add_argument("--n_resolves", dest="n_resolves", default=10, type=int)
    parser.add_argument("--solver", dest="solver", default="gurobi", type=str)
    args = parser.parse_args()
    res = Options()
    res.case = args.case
    res.aml = args.aml
    res.n_resolves = args.n_resolves
    res.solver = args.solver
    return res


def get_solver(options: Options):
    pe = _get_pe(options.aml)
    if options.aml == AML.pyomo:
        solver = pe.SolverFactory(options.solver)
    elif options.aml in {AML.hybrid_components_only, AML.hybrid_expression_wrappers}:
        assert options.solver in {"gurobi", "ipopt"}
        if options.solver == "gurobi":
            solver = pe.Gurobi()
        else:
            solver = pe.Ipopt()
        solver.config.stream_solver = False
    else:
        raise ValueError(f"unexpected aml: {options.aml}")
    return solver


def get_min_and_max_load(options: Options):
    pe = _get_pe(options.aml)
    m = build_dcopf_model(options.case, options.aml)
    m.load_factor.unfix()
    m.load_factor.setlb(0.1)
    m.load_factor.setub(10)

    m.objective.deactivate()
    m.scale_obj = pe.Objective(expr=m.load_factor)

    opt = get_solver(options)
    res = opt.solve(m)
    min_load_factor = m.load_factor.value

    m.scale_obj.sense = pe.maximize
    res = opt.solve(m)
    max_load_factor = m.load_factor.value

    return min_load_factor, max_load_factor


def param_updates(options: Options):
    min_load_factor, max_load_factor = get_min_and_max_load(options)
    m = build_dcopf_model(options.case, options.aml)

    base_loads = dict()
    for k, v in m.pl.items():
        base_loads[k] = v.value

    opt = get_solver(options)
    if options.aml in {AML.hybrid_components_only, AML.hybrid_expression_wrappers}:
        opt.load(m)

    if isinstance(opt, appsi.base.PersistentSolver):
        opt.update_config.check_for_new_or_removed_constraints = False
        opt.update_config.check_for_new_or_removed_vars = False
        opt.update_config.check_for_new_or_removed_params = False
        opt.update_config.check_for_new_objective = False
        opt.update_config.update_constraints = False
        opt.update_config.update_vars = False
        opt.update_config.update_params = True
        opt.update_config.update_named_expressions = False
        opt.update_config.update_objective = False
        opt.update_config.treat_fixed_vars_as_params = True

    load_list = [
        float(i) for i in np.linspace(min_load_factor, max_load_factor, options.n_resolves)
    ]
    obj_list = list()
    for load_factor in load_list:
        for k, p in m.pl.items():
            p.set_value(base_loads[k] * load_factor, idx=k)
        if options.aml in {AML.hybrid_components_only, AML.hybrid_expression_wrappers}:
            res = opt.resolve()
        else:
            res = opt.solve(m)
        obj_list.append(m.objective.expr())

    return obj_list


def main():
    timer = TicTocTimer()
    timer.tic("start")
    options = parse_args()
    obj_list = param_updates(options)
    print(obj_list)
    timer.toc("done")


if __name__ == "__main__":
    main()
