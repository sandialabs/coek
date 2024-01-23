# content of conftest.py

import pytest


@pytest.fixture(
    scope="session",
    params=["pyomo_coek.components_only", "pyomo_coek.full_integration"],
)
def pyomo_module(request):
    return pytest.importorskip(request.param)
