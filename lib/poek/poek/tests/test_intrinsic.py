import math
import pytest

from poek import *


@pytest.fixture
def var_v():
    return variable()


@pytest.fixture
def param_p():
    return parameter(value=0)


def test_ceil(var_v):
    v = var_v
    e = ceil(v)
    v.value = 1.5
    assert e.value == pytest.approx(2.0)
    v.value = -1.5
    assert e.value == pytest.approx(-1.0)


def test_floor(var_v):
    v = var_v
    e = floor(v)
    v.value = 1.5
    assert e.value == pytest.approx(1.0)
    v.value = -1.5
    assert e.value == pytest.approx(-2.0)


def test_exp(var_v):
    v = var_v
    e = exp(v)
    v.value = 1
    assert e.value == pytest.approx(math.e)
    v.value = 0
    assert e.value == pytest.approx(1.0)


def test_log(var_v):
    v = var_v
    e = log(v)
    v.value = 1
    assert e.value == pytest.approx(0)
    v.value = math.e
    assert e.value == pytest.approx(1)


def test_log10(var_v):
    v = var_v
    e = log10(v)
    v.value = 1
    assert e.value == pytest.approx(0)
    v.value = 10
    assert e.value == pytest.approx(1)


def test_pow(var_v, param_p):
    v = var_v
    p = param_p
    e = v**p
    v.value = 2
    p.value = 0
    assert e.value == pytest.approx(1.0)
    v.value = 2
    p.value = 1
    assert e.value == pytest.approx(2.0)


def test_sqrt(var_v):
    v = var_v
    e = sqrt(v)
    v.value = 1
    assert e.value == pytest.approx(1.0)
    v.value = 4
    assert e.value == pytest.approx(2.0)


def test_sin(var_v):
    v = var_v
    e = sin(v)
    v.value = 0
    assert e.value == pytest.approx(0.0)
    v.value = math.pi / 2.0
    assert e.value == pytest.approx(1.0)


def test_cos(var_v):
    v = var_v
    e = cos(v)
    v.value = 0
    assert e.value == pytest.approx(1.0)
    v.value = math.pi / 2.0
    assert e.value == pytest.approx(0.0)


def test_tan(var_v):
    v = var_v
    e = tan(v)
    v.value = 0
    assert e.value == pytest.approx(0.0)
    v.value = math.pi / 4.0
    assert e.value == pytest.approx(1.0)


def test_asin(var_v):
    v = var_v
    e = asin(v)
    v.value = 0
    assert e.value == pytest.approx(0.0)
    v.value = 1.0
    assert e.value == pytest.approx(math.pi / 2.0)


def test_acos(var_v):
    v = var_v
    e = acos(v)
    v.value = 1.0
    assert e.value == pytest.approx(0.0)
    v.value = 0.0
    assert e.value == pytest.approx(math.pi / 2.0)


def test_atan(var_v):
    v = var_v
    e = atan(v)
    v.value = 0
    assert e.value == pytest.approx(0.0)
    v.value = 1.0
    assert e.value == pytest.approx(math.pi / 4.0)


def test_sinh(var_v):
    v = var_v
    e = sinh(v)
    v.value = 0.0
    assert e.value == pytest.approx(0.0)
    v.value = 1.0
    assert e.value == pytest.approx((math.e - 1.0 / math.e) / 2.0)


def test_cosh(var_v):
    v = var_v
    e = cosh(v)
    v.value = 0.0
    assert e.value == pytest.approx(1.0)
    v.value = 1.0
    assert e.value == pytest.approx((math.e + 1.0 / math.e) / 2.0)


def test_tanh(var_v):
    v = var_v
    e = tanh(v)
    v.value = 0.0
    assert e.value == pytest.approx(0.0)
    v.value = 1.0
    assert e.value == pytest.approx((math.e - 1.0 / math.e) / (math.e + 1.0 / math.e))


def test_asinh(var_v):
    v = var_v
    e = asinh(v)
    v.value = 0.0
    assert e.value == pytest.approx(0.0)
    v.value = (math.e - 1.0 / math.e) / 2.0
    assert e.value == pytest.approx(1.0)


def test_acosh(var_v):
    v = var_v
    e = acosh(v)
    v.value = 1.0
    assert e.value == pytest.approx(0.0)
    v.value = (math.e + 1.0 / math.e) / 2.0
    assert e.value == pytest.approx(1.0)


def test_atanh(var_v):
    v = var_v
    e = atanh(v)
    v.value = 0.0
    assert e.value == pytest.approx(0.0)
    v.value = (math.e - 1.0 / math.e) / (math.e + 1.0 / math.e)
    assert e.value == pytest.approx(1.0)
