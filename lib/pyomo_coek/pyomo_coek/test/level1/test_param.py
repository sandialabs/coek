import pytest

#
# Test fixtures
#

@pytest.fixture
def model_M(pyomo_module):
    return pyomo_module.ConcreteModel()

@pytest.fixture
def var_a(model_M, pyomo_module):
    model_M.a = pyomo_module.Var(initialize=0.0)
    return model_M.a

@pytest.fixture
def var_b(model_M, pyomo_module):
    model_M.b = pyomo_module.Var()
    return model_M.b
    
@pytest.fixture
def var_c(model_M, pyomo_module):
    model_M.c = pyomo_module.Var()
    return model_M.c
    
@pytest.fixture
def var_d(model_M, pyomo_module):
    model_M.d = pyomo_module.Var()
    return model_M.d
    
@pytest.fixture
def var_v(model_M, pyomo_module):
    model_M.v = pyomo_module.Var()
    return model_M.v
    
@pytest.fixture
def param_p(model_M, pyomo_module):
    model_M.p = pyomo_module.Param(initialize=0, mutable=True)
    return model_M.p

@pytest.fixture
def param_q(model_M, pyomo_module):
    model_M.q = pyomo_module.Param(initialize=0, mutable=True)
    return model_M.q

@pytest.fixture
def param_r(model_M, pyomo_module):
    model_M.r = pyomo_module.Param(initialize=1, mutable=True)
    return model_M.r

#
# Value tests
#

def test_var_value(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.p = pe.Var(initialize=2)
    assert pe.value(M.p) == 2
    M.p.value = 3
    assert pe.value(M.p) == 3

def test_expr_value(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.p = pe.Var(initialize=2)
    assert pe.value(M.p) == 2
    e = M.p + 3
    assert pe.value(e) == 5
    with pytest.raises(AttributeError) as einfo:
        e.value = 0

def test_con_value(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.p = pe.Var(initialize=2)
    assert pe.value(M.p) == 2
    e = M.p <= 3
    assert pe.value(e) == True 
    with pytest.raises(AttributeError) as einfo:
        e.value = 0

def test_param1_value(pyomo_module):
    pe = pyomo_module
    M = pe.ConcreteModel()
    M.p = pe.Param(initialize=2, mutable=True)
    assert pe.value(M.p) == 2
    M.p.value = 3
    assert pe.value(M.p) == 3

