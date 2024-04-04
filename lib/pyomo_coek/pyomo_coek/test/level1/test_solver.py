#
# Test variable API
#
import pytest

def test_available1(pyomo_module):
    opt = pyomo_module.SolverFactory("bad_pyomo_solvername")
    assert not opt.available(False)

def test_available2(pyomo_module):
    m = pyomo_module.ConcreteModel()
    m.x0 = pyomo_module.Var(bounds=(0,None))
    m.x1 = pyomo_module.Var(bounds=(0,None))
    m.o = pyomo_module.Objective(expr=m.x0+m.x1)

    opt = pyomo_module.SolverFactory("bad_pyomo_solvername")
    with pytest.raises(RuntimeError) as e_info:
        res = opt.solve(m)

def test_gurobi(pyomo_module):
    m = pyomo_module.ConcreteModel()
    m.x0 = pyomo_module.Var(bounds=(0,None))
    m.x1 = pyomo_module.Var(bounds=(0,None))
    m.o = pyomo_module.Objective(expr=m.x0+m.x1)

    opt = pyomo_module.SolverFactory("coek_gurobi")
    if opt.available(False):
        res = opt.solve(m)
        pyo.check_optimal_termination(m)

def test_ipopt(pyomo_module):
    m = pyomo_module.ConcreteModel()
    m.x0 = pyomo_module.Var(bounds=(0,None))
    m.x1 = pyomo_module.Var(bounds=(0,None))
    m.o = pyomo_module.Objective(expr=m.x0+m.x1)

    opt = pyomo_module.SolverFactory("coek_ipopt")
    if opt.available(False):
        res = opt.solve(m)
        pyo.check_optimal_termination(m)

