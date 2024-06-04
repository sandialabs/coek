import pytest
import coek_utils

#
# Parameter
#

# double
def p1(d):
    d["p1"] = 1.1


# int
def p2(d):
    d["p2"] = 1


# string
def p3(d):
    d["p3"] = "1"


# string
def p3(d):
    d["p3"] = "1"


# tuple
def p4(d):
    d["p4"] = ("bar", 1.1)


#
# Set
#

# set of doubles
def s1(d):
    d["s1"] = [1.1, 2.1]


def s2(d):
    d["s2"] = set([1.1, 2.1])


# set of ints
def s3(d):
    d["s3"] = [1, 2]


def s4(d):
    d["s4"] = set([1, 2])


# set of strings
def s5(d):
    d["s5"] = ["bar", "foo"]


def s6(d):
    d["s6"] = set(["foo", "bar"])


# set of tupls
def s7(d):
    d["s7"] = [(1, 1), (2, 2)]


def s8(d):
    d["s8"] = set([(1, 1), (2, 2)])


#
# Indexed parameters
#

# map from ints to doubles
def ip1(d):
    d["ip1"] = {1: 1.1}


# map from ints to ints
def ip2(d):
    d["ip2"] = {1: 1}


# map from ints to strings
def ip3(d):
    d["ip3"] = {1: "1"}


# map from tuples to tuples
def ip4(d):
    d["ip4"] = {(1, "1"): (1, 1.1, "1")}


# map from tuples to string
def ip5(d):
    d["ip5"] = {(1, "1"): "1"}


# map from ints to tuples
def ip6(d):
    d["ip6"] = {1: (1, 1.1, "1")}


#
# Indexed sets
#

# map from ints to set of strings
def is1(d):
    d["is1"] = {1: ["1", "2", "3"]}


# map from ints to set of strings
def is2(d):
    d["is2"] = {1: set(["1", "2", "3"])}


# map from tuples to set of strings
def is3(d):
    d["is3"] = {(1, "1"): ["1", "2", "3"]}


# map from tuples to set of strings
def is4(d):
    d["is4"] = {(1, "1"): set(["1", "2", "3"])}


# map from ints to set of strings
def is5(d):
    d["is5"] = {1: {"1", "2", "3"}, 2: set()}


# map from ints to set of strings
def is6(d):
    d["is6"] = {1: set(), 2: {"1", "2", "3"}}


# map from ints to set of tuples
def is7(d):
    d["is7"] = {1: {(1, 1), (2, 2)}}


# map from tuples to set of tuples
def is8(d):
    d["is8"] = {(1, "a"): {(1, "a", 1.1), (1, "a", 2.2)}, (2, "b"): {(2, "b", 1.1), (2, "b", 2.2)}}


#
# Tests
#


def test_p1():
    d = {}
    p1(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"p1": 1.1}'


def test_p2():
    d = {}
    p2(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"p2": 1}'


def test_p3():
    d = {}
    p3(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"p3": "1"}'


def test_p4():
    d = {}
    p4(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"p4": ["bar", 1.1]}'


def test_s1():
    d = {}
    s1(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s1": {"data": [1.1, 2.1], "set_type": "d"}}'


def test_s2():
    d = {}
    s2(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s2": {"data": [1.1, 2.1], "set_type": "d"}}'


def test_s3():
    d = {}
    s3(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s3": {"data": [1, 2], "set_type": "i"}}'


def test_s4():
    d = {}
    s4(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s4": {"data": [1, 2], "set_type": "i"}}'


def test_s5():
    d = {}
    s5(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s5": {"data": ["bar", "foo"], "set_type": "s"}}'


def test_s6():
    d = {}
    s6(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s6": {"data": ["bar", "foo"], "set_type": "s"}}'


def test_s7():
    d = {}
    s7(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s7": {"data": [[1, 1], [2, 2]], "set_type": ["i", "i"]}}'


def test_s8():
    d = {}
    s8(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"s8": {"data": [[1, 1], [2, 2]], "set_type": ["i", "i"]}}'


def test_ip1():
    d = {}
    ip1(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"ip1": {"data": [[1, 1.1]], "key_type": "i", "param_type": "d"}}'


def test_ip2():
    d = {}
    ip2(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"ip2": {"data": [[1, 1]], "key_type": "i", "param_type": "i"}}'


def test_ip3():
    d = {}
    ip3(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"ip3": {"data": [[1, "1"]], "key_type": "i", "param_type": "s"}}'


def test_ip4():
    d = {}
    ip4(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"ip4": {"data": [[[1, "1"], [1, 1.1, "1"]]], "key_type": ["i", "s"], "param_type": ["i", "d", "s"]}}'
    )


def test_ip5():
    d = {}
    ip5(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"ip5": {"data": [[[1, "1"], "1"]], "key_type": ["i", "s"], "param_type": "s"}}'


def test_ip6():
    d = {}
    ip6(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"ip6": {"data": [[1, [1, 1.1, "1"]]], "key_type": "i", "param_type": ["i", "d", "s"]}}'
    )


def test_is1():
    d = {}
    is1(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"is1": {"data": [[1, ["1", "2", "3"]]], "key_type": "i", "set_type": "s"}}'


def test_is2():
    d = {}
    is2(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert ans == '{"is2": {"data": [[1, ["1", "2", "3"]]], "key_type": "i", "set_type": "s"}}'


def test_is3():
    d = {}
    is3(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"is3": {"data": [[[1, "1"], ["1", "2", "3"]]], "key_type": ["i", "s"], "set_type": "s"}}'
    )


def test_is4():
    d = {}
    is4(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"is4": {"data": [[[1, "1"], ["1", "2", "3"]]], "key_type": ["i", "s"], "set_type": "s"}}'
    )


def test_is5():
    d = {}
    is5(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"is5": {"data": [[1, ["1", "2", "3"]], [2, []]], "key_type": "i", "set_type": "s"}}'
    )


def test_is6():
    d = {}
    is6(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"is6": {"data": [[1, []], [2, ["1", "2", "3"]]], "key_type": "i", "set_type": "s"}}'
    )


def test_is7():
    d = {}
    is7(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans == '{"is7": {"data": [[1, [[1, 1], [2, 2]]]], "key_type": "i", "set_type": ["i", "i"]}}'
    )


def test_is8():
    d = {}
    is8(d)
    ans = coek_utils.dump_to_json_string(d, sort_keys=True)
    assert (
        ans
        == '{"is8": {"data": [[[1, "a"], [[1, "a", 1.1], [1, "a", 2.2]]], [[2, "b"], [[2, "b", 1.1], [2, "b", 2.2]]]], "key_type": ["i", "s"], "set_type": ["i", "s", "d"]}}'
    )


json_all = """{
  "ip1": {
    "data": [
      [
        1,
        1.1
      ]
    ],
    "key_type": "i",
    "param_type": "d"
  },
  "ip2": {
    "data": [
      [
        1,
        1
      ]
    ],
    "key_type": "i",
    "param_type": "i"
  },
  "ip3": {
    "data": [
      [
        1,
        "1"
      ]
    ],
    "key_type": "i",
    "param_type": "s"
  },
  "ip4": {
    "data": [
      [
        [
          1,
          "1"
        ],
        [
          1,
          1.1,
          "1"
        ]
      ]
    ],
    "key_type": [
      "i",
      "s"
    ],
    "param_type": [
      "i",
      "d",
      "s"
    ]
  },
  "ip5": {
    "data": [
      [
        [
          1,
          "1"
        ],
        "1"
      ]
    ],
    "key_type": [
      "i",
      "s"
    ],
    "param_type": "s"
  },
  "ip6": {
    "data": [
      [
        1,
        [
          1,
          1.1,
          "1"
        ]
      ]
    ],
    "key_type": "i",
    "param_type": [
      "i",
      "d",
      "s"
    ]
  },
  "is1": {
    "data": [
      [
        1,
        [
          "1",
          "2",
          "3"
        ]
      ]
    ],
    "key_type": "i",
    "set_type": "s"
  },
  "is2": {
    "data": [
      [
        1,
        [
          "1",
          "2",
          "3"
        ]
      ]
    ],
    "key_type": "i",
    "set_type": "s"
  },
  "is3": {
    "data": [
      [
        [
          1,
          "1"
        ],
        [
          "1",
          "2",
          "3"
        ]
      ]
    ],
    "key_type": [
      "i",
      "s"
    ],
    "set_type": "s"
  },
  "is4": {
    "data": [
      [
        [
          1,
          "1"
        ],
        [
          "1",
          "2",
          "3"
        ]
      ]
    ],
    "key_type": [
      "i",
      "s"
    ],
    "set_type": "s"
  },
  "is5": {
    "data": [
      [
        1,
        [
          "1",
          "2",
          "3"
        ]
      ],
      [
        2,
        []
      ]
    ],
    "key_type": "i",
    "set_type": "s"
  },
  "is6": {
    "data": [
      [
        1,
        []
      ],
      [
        2,
        [
          "1",
          "2",
          "3"
        ]
      ]
    ],
    "key_type": "i",
    "set_type": "s"
  },
  "is7": {
    "data": [
      [
        1,
        [
          [
            1,
            1
          ],
          [
            2,
            2
          ]
        ]
      ]
    ],
    "key_type": "i",
    "set_type": [
      "i",
      "i"
    ]
  },
  "is8": {
    "data": [
      [
        [
          1,
          "a"
        ],
        [
          [
            1,
            "a",
            1.1
          ],
          [
            1,
            "a",
            2.2
          ]
        ]
      ],
      [
        [
          2,
          "b"
        ],
        [
          [
            2,
            "b",
            1.1
          ],
          [
            2,
            "b",
            2.2
          ]
        ]
      ]
    ],
    "key_type": [
      "i",
      "s"
    ],
    "set_type": [
      "i",
      "s",
      "d"
    ]
  },
  "p1": 1.1,
  "p2": 1,
  "p3": "1",
  "p4": [
    "bar",
    1.1
  ],
  "s1": {
    "data": [
      1.1,
      2.1
    ],
    "set_type": "d"
  },
  "s2": {
    "data": [
      1.1,
      2.1
    ],
    "set_type": "d"
  },
  "s3": {
    "data": [
      1,
      2
    ],
    "set_type": "i"
  },
  "s4": {
    "data": [
      1,
      2
    ],
    "set_type": "i"
  },
  "s5": {
    "data": [
      "bar",
      "foo"
    ],
    "set_type": "s"
  },
  "s6": {
    "data": [
      "bar",
      "foo"
    ],
    "set_type": "s"
  },
  "s7": {
    "data": [
      [
        1,
        1
      ],
      [
        2,
        2
      ]
    ],
    "set_type": [
      "i",
      "i"
    ]
  },
  "s8": {
    "data": [
      [
        1,
        1
      ],
      [
        2,
        2
      ]
    ],
    "set_type": [
      "i",
      "i"
    ]
  }
}"""


def test_all():
    d = {}

    p1(d)
    p2(d)
    p3(d)
    p3(d)
    p4(d)
    s1(d)
    s2(d)
    s3(d)
    s4(d)
    s5(d)
    s6(d)
    s7(d)
    s8(d)
    ip1(d)
    ip2(d)
    ip3(d)
    ip4(d)
    ip5(d)
    ip6(d)
    is1(d)
    is2(d)
    is3(d)
    is4(d)
    is5(d)
    is6(d)
    is7(d)
    is8(d)

    ans = coek_utils.dump_to_json_string(d, sort_keys=True, indent=2)
    # print(ans)
    assert ans == json_all


def test_load_p1():
    d = {}
    p1(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["p1"] == ans["p1"]


def test_load_p2():
    d = {}
    p2(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["p2"] == ans["p2"]


def test_load_p3():
    d = {}
    p3(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["p3"] == ans["p3"]


def test_load_p4():
    d = {}
    p4(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["p4"] == ans["p4"]


# NOTE: s1, s3, s5 and s7 use list representations for sets.  The
# JSON decoder returns a set() representation, which is more accurate.
# Hence, there is a difference in the output.


def test_load_s2():
    d = {}
    s2(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["s2"] == ans["s2"]


def test_load_s4():
    d = {}
    s4(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["s4"] == ans["s4"]


def test_load_s6():
    d = {}
    s6(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["s6"] == ans["s6"]


def test_load_s8():
    d = {}
    s8(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["s8"] == ans["s8"]


def test_load_ip1():
    d = {}
    ip1(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip1"] == ans["ip1"]


def test_load_ip2():
    d = {}
    ip2(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip2"] == ans["ip2"]


def test_load_ip3():
    d = {}
    ip3(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip3"] == ans["ip3"]


def test_load_ip4():
    d = {}
    ip4(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip4"] == ans["ip4"]


def test_load_ip5():
    d = {}
    ip5(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip5"] == ans["ip5"]


def test_load_ip6():
    d = {}
    ip6(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["ip6"] == ans["ip6"]


def test_load_is2():
    d = {}
    is2(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is2"] == ans["is2"]


def test_load_is4():
    d = {}
    is4(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is4"] == ans["is4"]


def test_load_is5():
    d = {}
    is5(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is5"] == ans["is5"]


def test_load_is6():
    d = {}
    is6(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is6"] == ans["is6"]


def test_load_is7():
    d = {}
    is7(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is7"] == ans["is7"]


def test_load_is8():
    d = {}
    is8(d)
    ans = coek_utils.load_from_json_string(json_all)
    assert d["is8"] == ans["is8"]
