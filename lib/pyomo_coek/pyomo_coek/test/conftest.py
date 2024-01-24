# content of conftest.py

import pytest


@pytest.fixture(
    scope="session",
    #params=["pyomo_coek.components_only", "pyomo_coek.full_integration"],
    #params=["pyomo_coek.components_only"],
    params=["pyomo_coek.pyomo_wrapper", "pyomo_coek.components_only"],
    #params=["pyomo_coek.pyomo_wrapper"],
)
def pyomo_module(request):
    return pytest.importorskip(request.param)
