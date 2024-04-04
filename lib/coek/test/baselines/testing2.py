import pyomo.environ as pyo

m = pyo.ConcreteModel()

m.a = pyo.Var(bounds=(0, 1), within=pyo.Integers, initialize=0)
m.b = pyo.Var(bounds=(0, 1), within=pyo.Binary, initialize=0)
m.q = pyo.Param(initialize=2)

m.o = pyo.Objective(
    expr=3 * m.a + m.q + m.a * m.a * m.a * (-m.a + m.b + 3 * m.a + 3 * m.b + pyo.sin(-pyo.cos(m.a)))
)

m.b.fix(1)

m.write("testing2.nl")
