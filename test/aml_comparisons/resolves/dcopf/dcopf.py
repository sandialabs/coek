from egret.data.model_data import ModelData
from egret.model_library.transmission import tx_utils
import enum
import pyomo.environ as pyomo_environ
import pyomo_coek.components_only as components_only
import pyomo_coek.full_integration as expression_wrappers
import math
import sys
from pyomo.contrib import appsi


class AML(str, enum.Enum):
    pyomo = "pyomo"
    hybrid_components_only = "hybrid_components_only"
    hybrid_expression_wrappers = "hybrid_expression_wrappers"


def declare_load_params(pe, m, md: ModelData, index_set):
    m.pl = pe.Param(index_set, mutable=True)

    for b in index_set:
        m.pl[b].value = 0

    for load_name, load in md.data["elements"]["load"].items():
        b = load["bus"]
        if b in index_set and "p_load" in load:
            m.pl[b].value += load["p_load"]


def declare_var_pg(pe, m, md: ModelData, index_set):
    m.pg = pe.Var(index_set)

    for g in index_set:
        gen = md.data["elements"]["generator"][g]
        pmin = gen["p_min"]
        pmax = gen["p_max"]
        m.pg[g].setlb(pmin)
        m.pg[g].setub(pmax)


def declare_var_pf(pe, m, md: ModelData, index_set):
    m.pf = pe.Var(index_set)

    for b in index_set:
        branch = md.data["elements"]["branch"][b]
        smax = branch["rating_long_term"]
        if smax is not None:
            m.pf[b].setlb(-smax)
            m.pf[b].setub(smax)


def declare_var_va(pe, m, md: ModelData, index_set):
    m.va = pe.Var(index_set)

    ref_bus = md.data["system"]["reference_bus"]
    ref_angle = md.data["system"]["reference_bus_angle"]
    m.va[ref_bus].fix(math.radians(ref_angle))


def declare_power_flow_con(pe, m, md: ModelData, index_set):
    m.power_flow_con = pe.Constraint(index_set)

    for bname in index_set:
        branch = md.data["elements"]["branch"][bname]
        from_bus = branch["from_bus"]
        to_bus = branch["to_bus"]

        tau = 1
        shift = 0

        if branch["branch_type"] == "transformer":
            tau = branch["transformer_tap_ratio"]
            shift = math.radians(branch["transformer_phase_shift"])
        else:
            assert branch["branch_type"] == "line"

        x = branch["reactance"]
        b = -1 / (tau * x)

        m.power_flow_con[bname] = m.pf[bname] == b * (
            m.va[from_bus] - m.va[to_bus] + shift
        )


def declare_power_balance(pe, m, md: ModelData, index_set):
    m.power_balance = pe.Constraint(index_set)

    exprs = dict()
    for b in index_set:
        exprs[b] = 0

    for branch_name, branch in md.data["elements"]["branch"].items():
        exprs[branch["from_bus"]] -= m.pf[branch_name]
        exprs[branch["to_bus"]] += m.pf[branch_name]

    for gen_name, gen in md.data["elements"]["generator"].items():
        exprs[gen["bus"]] += m.pg[gen_name]

    for b in index_set:
        exprs[b] -= m.load_factor * m.pl[b]

    if "shunt" in md.data["elements"]:
        for shunt_name, shunt in md.data["elements"]["shunt"].items():
            if shunt["shunt_type"] == "fixed":
                exprs[shunt["bus"]] -= shunt["gs"]

    for b in index_set:
        m.power_balance[b] = exprs[b] == 0


def declare_cost_objective(pe, m, md: ModelData, index_set):
    obj_expr = 0
    for g in index_set:
        gen = md.data["elements"]["generator"][g]
        if "p_cost" not in gen:
            continue
        assert gen["p_cost"]["cost_curve_type"] == "polynomial"
        cost_vals = gen["p_cost"]["values"]

        for exp, coef in cost_vals.items():
            obj_expr += coef * m.pg[g] ** exp

    m.objective = pe.Objective(expr=obj_expr)


def _get_pe(aml: AML):
    if aml == AML.pyomo:
        pe = pyomo_environ
    elif aml == AML.hybrid_components_only:
        pe = components_only
    elif aml == AML.hybrid_expression_wrappers:
        pe = expression_wrappers
    else:
        raise ValueError(f"Unrecognized AML: {aml}")
    return pe


def build_dcopf_model(fname: str, aml: AML):
    pe = _get_pe(aml)

    md = ModelData.read(fname)
    md = md.clone_in_service()
    tx_utils.scale_ModelData_to_pu(md, inplace=True)

    m = pe.ConcreteModel()

    m.bus_set = pe.Set(initialize=list(md.data["elements"]["bus"].keys()))
    m.gen_set = pe.Set(initialize=list(md.data["elements"]["generator"].keys()))
    m.branch_set = pe.Set(initialize=list(md.data["elements"]["branch"].keys()))

    declare_load_params(pe, m, md, m.bus_set)
    m.load_factor = pe.Var(initialize=1)
    m.load_factor.fix()

    declare_var_va(pe, m, md, m.bus_set)
    declare_var_pg(pe, m, md, m.gen_set)
    declare_var_pf(pe, m, md, m.branch_set)

    declare_power_flow_con(pe, m, md, m.branch_set)
    declare_power_balance(pe, m, md, m.bus_set)

    declare_cost_objective(pe, m, md, m.gen_set)

    return m


def build_egret_model(fname: str):
    from egret.models.dcopf import create_btheta_dcopf_model

    md = ModelData.read(fname)
    m, _ = create_btheta_dcopf_model(md)
    return m


def main():
    m = build_dcopf_model(sys.argv[1], AML.pyomo)
    opt = appsi.solvers.Gurobi()
    res = opt.solve(m)
    print(res)


def run_egret():
    m = build_egret_model(sys.argv[1])
    opt = appsi.solvers.Gurobi()
    res = opt.solve(m)
    print(res)


if __name__ == "__main__":
    main()
    # run_egret()
