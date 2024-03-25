import pytest

from poek import *


def test_available():
    opt = solver("bad_coek_solvername")
    assert not opt.available

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
