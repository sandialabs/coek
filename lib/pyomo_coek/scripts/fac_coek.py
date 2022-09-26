from pyomo.environ import ConcreteModel, RangeSet, Param, SolverFactory
from pyomo_coek import Var, Objective, Constraint, Gurobi
from pyomo.common.timing import HierarchicalTimer


def create_model(size):
    m = ConcreteModel()

    m.Grid = RangeSet(0, size)
    m.Facs = RangeSet(1, size)
    m.Dims = RangeSet(1, 2)

    # TODO: Add comments to define these variables
    m.d = Var()
    m.y = Var(m.Facs, m.Dims, bounds=(0.0, 1.0))
    m.z = Var(m.Grid, m.Grid, m.Facs, bounds=(0, 1))
    m.s = Var(m.Grid, m.Grid, m.Facs, bounds=(0.0, None))
    m.r = Var(m.Grid, m.Grid, m.Facs, m.Dims)

    m.obj = Objective(expr = m.d)

    def assmt_rule(mod, i, j):
        return sum(mod.z[i,j,f] for f in mod.Facs) == 1
    m.assmt = Constraint(m.Grid, m.Grid, rule=assmt_rule)

    m.M = Param(initialize=2*1.414, mutable=True)

    def quadrhs_rule(mod,i,j,f):
        e = mod.s[i,j,f] == mod.d + mod.M.value*(1 - mod.z[i,j,f])
        return e
    m.quadrhs = Constraint(m.Grid, m.Grid, m.Facs, rule=quadrhs_rule)

    def quaddistk_rule(mod,i,j,f,dim):
        ij = (i,j)
        return mod.r[i,j,f,dim] == ij[dim-1]/size - mod.y[f,dim]
    m.quaddistk = Constraint(m.Grid, m.Grid, m.Facs, m.Dims, rule=quaddistk_rule)

    def quaddist_rule(mod,i,j,f):
        return sum(mod.r[i,j,f,dim]**2 for dim in mod.Dims) <= mod.s[i,j,f]**2
    m.quaddist = Constraint(m.Grid, m.Grid, m.Facs, rule=quaddist_rule)

    return m


def main():
    timer = HierarchicalTimer()
    timer.start('all')
    timer.start('model construction')
    m = create_model(25)
    timer.stop('model construction')
    timer.start('solve')
    opt = Gurobi()
    res = opt.solve(m, timer=timer)
    timer.stop('solve')
    timer.stop('all')
    print(timer)


if __name__ == '__main__':
    main()
