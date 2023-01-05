import pyomo.environ as pe
import pyomo_coek as pc
from pyomo.common.timing import HierarchicalTimer


def main():
    timer = HierarchicalTimer()
    timer.start("all")
    timer.start("construct model")
    m = pe.ConcreteModel()
    m.x = pc.Var()
    m.y = pc.Var()

    m.c1 = pc.Constraint(expr=m.y >= (m.x - 1) ** 2)

    m.obj = pc.Objective(expr=m.x**2 + m.y**2)
    timer.stop("construct model")

    timer.start("solve")
    opt = pc.Gurobi()
    res = opt.solve(m, timer=timer)
    timer.stop("solve")
    timer.stop("all")
    m.pprint()
    print(res)
    print(timer)


if __name__ == "__main__":
    main()
