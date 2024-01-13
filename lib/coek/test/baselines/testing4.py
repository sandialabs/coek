import pyomo.environ as pyo

m = pyo.ConcreteModel()

m.x = pyo.Var(bounds=(0,1), within=pyo.Binary, initialize=0)
m.y = pyo.Var(bounds=(0,1), within=pyo.Binary, initialize=0)
m.z = pyo.Var(bounds=(0,1), within=pyo.Binary, initialize=0)
m.a = pyo.Var(bounds=(0,1), within=pyo.Integers, initialize=0)
m.b = pyo.Var(bounds=(0,1), within=pyo.Binary, initialize=0)

m.o = pyo.Objective(expr=m.a + pyo.cos(m.x) + pyo.cos(m.y))
m.c = pyo.Constraint(expr=m.b + pyo.cos(m.y) + pyo.cos(m.z) == 1)

m.write("testing4.nl")
