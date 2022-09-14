import sys
import pyomo_coek.components_only as pe


def fac_linear(G):
    ones = [1] * G

    model = pe.ConcreteModel()

    F = G

    model.Grid = pe.RangeSet(0, G)
    model.Facs = pe.RangeSet(1, F)
    model.Dims = pe.RangeSet(1, 2)

    model.d = pe.Var()
    model.y = pe.Var(model.Facs, model.Dims, bounds=(0.0, 1.0))
    model.z = pe.Var(model.Grid, model.Grid, model.Facs, within=pe.Binary)
    model.s = pe.Var(model.Grid, model.Grid, model.Facs, bounds=(0.0, None))
    model.r = pe.Var(model.Grid, model.Grid, model.Facs, model.Dims)

    def obj_rule(mod):
        return 1.0 * mod.d

    model.obj = pe.Objective(rule=obj_rule)

    def assmt_rule(mod, i, j):
        return (
            pe.construct_linear_expression(
                linear_coefs=ones, linear_vars=[mod.z[i, j, f] for f in mod.Facs]
            )
            == 1
        )

    model.assmt = pe.Constraint(model.Grid, model.Grid, rule=assmt_rule)

    M = 2 * 1.414

    def quadrhs_rule(mod, i, j, f):
        return mod.s[i, j, f] == mod.d + M * (1 - mod.z[i, j, f])

    model.quadrhs = pe.Constraint(model.Grid, model.Grid, model.Facs, rule=quadrhs_rule)

    def quaddistk1_rule(mod, i, j, f):
        return mod.r[i, j, f, 1] == (1.0 * i) / G - mod.y[f, 1]

    model.quaddistk1 = pe.Constraint(
        model.Grid, model.Grid, model.Facs, rule=quaddistk1_rule
    )

    def quaddistk2_rule(mod, i, j, f):
        return mod.r[i, j, f, 2] == (1.0 * j) / G - mod.y[f, 2]

    model.quaddistk2 = pe.Constraint(
        model.Grid, model.Grid, model.Facs, rule=quaddistk2_rule
    )

    def quaddist_rule(mod, i, j, f):
        return mod.r[i, j, f, 1] ** 2 + mod.r[i, j, f, 2] ** 2 <= mod.s[i, j, f] ** 2

    model.quaddist = pe.Constraint(
        model.Grid, model.Grid, model.Facs, rule=quaddist_rule
    )

    return model
