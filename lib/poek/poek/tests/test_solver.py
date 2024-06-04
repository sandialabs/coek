import pytest

from poek import *


def test_available1():
    opt = solver("bad_coek_solvername")
    assert not opt.available


def test_available2():
    m = model()
    x0 = m.add_variable(lower=0)
    x1 = m.add_variable(lower=0)
    m.add_objective(x0 + x1)

    opt = solver("bad_coek_solvername")
    res = opt.solve(m)
    assert res.termination_condition == "solver_not_available"


def test_available3():
    m = model()
    x0 = m.add_variable(lower=0)
    x1 = m.add_variable(lower=0)
    m.add_objective(x0 + x1)
    nlp = nlp_model(m, "cppad")

    opt = nlp_solver("bad_coek_solvername")
    res = opt.solve(nlp)
    assert res.termination_condition == "solver_not_available"


def test_nlpsolver():
    m = model()
    v1 = m.add_variable(name="v1", value=1)
    v2 = m.add_variable(name="v2", value=2)
    v3 = m.add_variable(name="v3", value=3)

    e = v1 + v2
    m.add_objective((1 - e) + 3 * e + e * e + v3)
    m.add_constraint(v1 * v1 + 1 <= 3 * v2)
    m.add_constraint(v1**2 >= 3 * v2)
    m.add_constraint(1 < v2)
    m.add_constraint(v1 > 1)
    m.add_constraint(v1 == 3 * v2 - 3)

    nlp = nlp_model(m, "cppad")
    assert nlp.num_variables() == 3
    assert nlp.num_objectives() == 1
    assert nlp.num_constraints() == 5


def test_rosenbr():
    m = model()
    x0 = m.add_variable(value=1)
    x1 = m.add_variable(value=2)

    m.add_objective(100 * (x1 - x0**2) ** 2 + (x0 - 1) ** 2)

    nlp = nlp_model(m, "cppad")
    opt = nlp_solver("ipopt")
    res = opt.solve(nlp)
    assert res.solver_name == "ipopt"
    if opt.available:
        assert check_optimal_termination(res)
        assert x0.value == pytest.approx(1.0)
        assert x1.value == pytest.approx(1.0)
    else:
        assert res.termination_condition == "solver_not_available"
