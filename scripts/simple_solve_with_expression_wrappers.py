import pyomo_coek.full_integration as pe
#import pyomo.environ as pe


def main():
    m = pe.ConcreteModel()
    m.x = pe.Var()
    m.y = pe.Var()
    m.obj = pe.Objective(expr=m.x**2 + m.y**2)
    m.c1 = pe.Constraint(expr=m.y >= (m.x - 1)**2)
    opt = pe.Gurobi()
    #opt = pe.SolverFactory('appsi_gurobi')
    opt.solve(m)
    print(m.x.value, m.y.value)
    print(m.obj.expr())


if __name__ == '__main__':
    main()
