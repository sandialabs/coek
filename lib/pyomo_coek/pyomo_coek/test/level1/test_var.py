#
# Test variable API
#
import pytest

#
# Test fixtures
#


@pytest.fixture
def model_M(pyomo_module):
    return pyomo_module.ConcreteModel()


@pytest.fixture
def M_v(model_M, pyomo_module):
    model_M.v = pyomo_module.Var()
    return model_M.v


#
# Var value
#


def test_var_uninitialized(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var()
    # Uninitialized variable has value None
    assert M.v.value == None
    M.v.value = 1.0
    # Variable value can be set
    assert M.v.value == 1.0


def test_var_initialized(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=1.0)
    # Uninitialized variable has value 1.0
    assert M.v.value == 1.0


#
# Var fixed
#


def test_var_fixed(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=2.0)
    M.v.fixed = True
    assert M.v.fixed == True
    assert M.v.value == 2.0
    # Can still set the value of a fixed variable
    M.v.value = 1.0
    assert M.v.value == 1.0


def test_var_fix1(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=1.0)
    assert M.v.fixed == False
    assert M.v.value == 1.0
    M.v.fix()
    assert M.v.fixed == True
    assert M.v.value == 1.0  # Q: Why not None?


def test_var_fix2(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=1.0)
    M.v.fixed = True
    assert M.v.fixed == True
    assert M.v.value == 1.0
    M.v.fix(False)
    assert M.v.fixed == True
    assert M.v.value == 0.0


def test_var_fix3(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=1.0)
    M.v.fixed = True
    assert M.v.fixed == True
    assert M.v.value == 1.0
    M.v.unfix()
    assert M.v.fixed == False
    assert M.v.value == 1.0


def test_var_fix4(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(initialize=1.0)
    assert M.v.fixed == False
    assert M.v.value == 1.0
    M.v.fix(None)
    assert M.v.fixed == True
    assert M.v.value == None


#
# Var bounds
#


def test_var_lb(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var()
    assert M.v.lb == None
    M.v.lb = 1.0
    assert M.v.lb == 1.0


def test_var_ub(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var()
    assert M.v.ub == None
    M.v.ub = 1.0
    assert M.v.ub == 1.0


def test_var_bounds1(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(bounds=(1.0, 2.0))
    assert M.v.lb == 1.0
    assert M.v.ub == 2.0


def test_var_bounds2(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(bounds=(1.0, None))
    assert M.v.lb == 1.0
    assert M.v.ub == None


def test_var_bounds3(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.v = pe.Var(bounds=(None, 1.0))
    assert M.v.lb == None
    assert M.v.ub == 1.0
