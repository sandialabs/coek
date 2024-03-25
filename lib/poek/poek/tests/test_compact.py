#
# This API is not polished.  So this is just a draft set of tests
#

import pytest

from poek import *

try:
    from poek import compact_model

    compact_model_available = True
except:
    compact_model_available = False


@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact1(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ExpressionSequence(Forall(i).In(A), y[i])
    i = 1
    for e in tmp:
        assert e.to_list() == ["y[%d]" % i]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact2(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ExpressionSequence(Forall(i).In(A), y[i] + 1)
    i = 1
    for e in tmp:
        assert e.to_list() == ["+", "y[%d]" % i, to_string(1.0)]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact3(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ExpressionSequence(Forall(i).In(A), y[i] + i)
    i = 1
    for e in tmp:
        assert e.to_list() == ["+", "y[%d]" % i, to_string(i * 1.0)]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact3(self):
    m = compact_model()
    A = RangeSet(1, 4)
    B = RangeSet(1, 6)
    y = variable(B, name="y").generate_names()
    i = index("i")
    #
    tmp = ExpressionSequence(Forall(i).In(A), y[i] + i * y[i + 2])
    i = 1
    for e in tmp:
        assert e.to_list() ==\
            [
                "+",
                "y[%d]" % i,
                [
                    "*",
                    to_string(i * 1.0),
                    "y[%d]" % (i + 2),
                ],
            ]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact4(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = Sum(y[i], Forall(i).In(A))
    e = tmp.expand()
    assert e.to_list() == ["+", "y[1]", "y[2]", "y[3]", "y[4]"]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact5(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    #
    tmp = Sum(x[i, i], Forall(i).In(A))
    e = tmp.expand()
    assert e.to_list() == ["+", "x[1,1]", "x[2,2]", "x[3,3]", "x[4,4]"]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact6(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(x[i, i], Forall(i, j).In(A * A))
    e = tmp.expand()
    assert e.to_list() == \
        [
            "+",
            "x[1,1]",
            "x[1,1]",
            "x[1,1]",
            "x[1,1]",
            "x[2,2]",
            "x[2,2]",
            "x[2,2]",
            "x[2,2]",
            "x[3,3]",
            "x[3,3]",
            "x[3,3]",
            "x[3,3]",
            "x[4,4]",
            "x[4,4]",
            "x[4,4]",
            "x[4,4]",
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact7(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(x[i, i], Forall(i).In(A).Forall(j).In(A))
    e = tmp.expand()
    assert e.to_list() ==\
        [
            "+",
            "x[1,1]",
            "x[1,1]",
            "x[1,1]",
            "x[1,1]",
            "x[2,2]",
            "x[2,2]",
            "x[2,2]",
            "x[2,2]",
            "x[3,3]",
            "x[3,3]",
            "x[3,3]",
            "x[3,3]",
            "x[4,4]",
            "x[4,4]",
            "x[4,4]",
            "x[4,4]",
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact8(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(y[i] * x[i, i], Forall(i).In(A))
    e = tmp.expand()
    assert e.to_list() ==\
        [
            "+",
            ["*", "y[1]", "x[1,1]"],
            ["*", "y[2]", "x[2,2]"],
            ["*", "y[3]", "x[3,3]"],
            ["*", "y[4]", "x[4,4]"],
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_eseq_compact9(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()          # TODO - push this logic into the model
    x = variable(A * A, name="x").generate_names()      # TODO - push this logic into the model
    i = index("i")
    j = index("j")
    #
    tmp = Sum(y[i] * Sum(x[i, j], Forall(j).In(A)), Forall(i).In(A))
    e = tmp.expand()
    assert e.to_list() ==\
        [
            "+",
            ["*", "y[1]", ["+", "x[1,1]", "x[1,2]", "x[1,3]", "x[1,4]"]],
            ["*", "y[2]", ["+", "x[2,1]", "x[2,2]", "x[2,3]", "x[2,4]"]],
            ["*", "y[3]", ["+", "x[3,1]", "x[3,2]", "x[3,3]", "x[3,4]"]],
            ["*", "y[4]", ["+", "x[4,1]", "x[4,2]", "x[4,3]", "x[4,4]"]],
        ]


@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact1(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ConstraintSequence(Forall(i).In(A), y[i] == 0)
    i = 1
    for e in tmp:
        assert e.to_list() == ["==", "y[%d]" % i, to_string(0.0)]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact2(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ConstraintSequence(Forall(i).In(A), y[i] + 1 == 0)
    i = 1
    for e in tmp:
        assert e.to_list() == ["==", ["+", "y[%d]" % i, to_string(1.0)], to_string(0.0)]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact3(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = ConstraintSequence(Forall(i).In(A), y[i] + i == 0)
    i = 1
    for e in tmp:
        assert e.to_list() == ["==", ["+", "y[%d]" % i, to_string(i * 1.0)], to_string(0.0)]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact3(self):
    m = compact_model()
    A = RangeSet(1, 4)
    B = RangeSet(1, 6)
    y = variable(B, name="y").generate_names()
    i = index("i")
    #
    tmp = ConstraintSequence(Forall(i).In(A), y[i] + i * y[i + 2] == 0)
    i = 1
    for e in tmp:
        assert e.to_list() ==\
            [
                "==",
                ["+", "y[%d]" % i, ["*", to_string(i * 1.0), "y[%d]" % (i + 2)]],
                to_string(0.0),
            ]
        i += 1

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact4(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    i = index("i")
    #
    tmp = Sum(y[i], Forall(i).In(A)) == 0
    e = tmp.expand()
    assert e.to_list() == ["==", ["+", "y[1]", "y[2]", "y[3]", "y[4]"], to_string(0.0)]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact5(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    #
    tmp = Sum(x[i, i], Forall(i).In(A)) == 0
    e = tmp.expand()
    assert e.to_list() == ["==", ["+", "x[1,1]", "x[2,2]", "x[3,3]", "x[4,4]"], to_string(0.0)]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact6(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(x[i, i], Forall(i, j).In(A * A)) == 0
    e = tmp.expand()
    assert e.to_list() ==\
        [
            "==",
            [
                "+",
                "x[1,1]",
                "x[1,1]",
                "x[1,1]",
                "x[1,1]",
                "x[2,2]",
                "x[2,2]",
                "x[2,2]",
                "x[2,2]",
                "x[3,3]",
                "x[3,3]",
                "x[3,3]",
                "x[3,3]",
                "x[4,4]",
                "x[4,4]",
                "x[4,4]",
                "x[4,4]",
            ],
            to_string(0.0),
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact7(self):
    m = compact_model()
    A = RangeSet(1, 4)
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(x[i, i], Forall(i).In(A).Forall(j).In(A)) == 0
    e = tmp.expand()
    assert e.to_list() == \
        [
            "==",
            [
                "+",
                "x[1,1]",
                "x[1,1]",
                "x[1,1]",
                "x[1,1]",
                "x[2,2]",
                "x[2,2]",
                "x[2,2]",
                "x[2,2]",
                "x[3,3]",
                "x[3,3]",
                "x[3,3]",
                "x[3,3]",
                "x[4,4]",
                "x[4,4]",
                "x[4,4]",
                "x[4,4]",
            ],
            to_string(0.0),
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact8(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(y[i] * x[i, i], Forall(i).In(A)) == 0
    e = tmp.expand()
    assert e.to_list() == \
        [
            "==",
            [
                "+",
                ["*", "y[1]", "x[1,1]"],
                ["*", "y[2]", "x[2,2]"],
                ["*", "y[3]", "x[3,3]"],
                ["*", "y[4]", "x[4,4]"],
            ],
            to_string(0.0),
        ]

@pytest.mark.skipif(not compact_model_available, reason="pycoek not compiled with compact models")
def test_cseq_compact9(self):
    m = compact_model()
    A = RangeSet(1, 4)
    y = variable(A, name="y").generate_names()
    x = variable(A * A, name="x").generate_names()
    i = index("i")
    j = index("j")
    #
    tmp = Sum(y[i] * Sum(x[i, j], Forall(j).In(A)), Forall(i).In(A)) == 0
    e = tmp.expand()
    assert e.to_list() == \
        [
            "==",
            [
                "+",
                ["*", "y[1]", ["+", "x[1,1]", "x[1,2]", "x[1,3]", "x[1,4]"]],
                ["*", "y[2]", ["+", "x[2,1]", "x[2,2]", "x[2,3]", "x[2,4]"]],
                ["*", "y[3]", ["+", "x[3,1]", "x[3,2]", "x[3,3]", "x[3,4]"]],
                ["*", "y[4]", ["+", "x[4,1]", "x[4,2]", "x[4,3]", "x[4,4]"]],
            ],
            to_string(0.0),
        ]
