import pyomo.environ as pyo

m = pyo.ConcreteModel()

m.a = pyo.Var(bounds=(0, 1), within=pyo.Integers, initialize=0)
m.b = pyo.Var(bounds=(0, 1), within=pyo.Binary, initialize=0)
m.c = pyo.Var(bounds=(0, None))
m.d = pyo.Var(bounds=(None, 0))
m.e = pyo.Var()
m.q = pyo.Param(initialize=2)

m.o = pyo.Objective(expr=3 * m.a + m.q, sense=pyo.maximize)

m.c1 = pyo.Constraint(expr=3 * m.b + m.q - m.a <= 0)

m.c2 = pyo.Constraint(expr=3 * m.b + m.b == 0)

m.c3 = pyo.Constraint(expr=3 * m.b * m.a + m.q + m.b * m.b + m.b * m.b == 0)

m.c4 = pyo.Constraint(expr=3 * m.b * m.b + m.q - m.a * m.b - m.a * m.a <= 0)

m.c5 = pyo.Constraint(expr=pyo.inequality(-7, 3 * m.b * m.b + m.q - m.a * m.b - m.a * m.a, 7))

m.c6 = pyo.Constraint(expr=m.c + m.d == 0)

m.c7 = pyo.Constraint(expr=m.e + 3 * m.d == 1)

m.c8 = pyo.Constraint(expr=pyo.inequality(7, 3 * m.b + m.q - m.a, 7))

m.e.fix(1.0)

m.write("testing1.nl")
