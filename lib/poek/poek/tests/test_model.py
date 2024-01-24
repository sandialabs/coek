import pyutilib.th as unittest

from poek import *


class TestModel(unittest.TestCase):
    def test_model1(self):
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
        self.assertEqual(nlp.num_variables(), 3)
        self.assertEqual(nlp.num_objectives(), 1)
        self.assertEqual(nlp.num_constraints(), 5)

        e = m.get_objective()
        self.assertEqual(
            e.to_list(),
            [
                "min",
                [
                    "+",
                    "1.000000",
                    ["-", ["+", "v1", "v2"]],
                    ["*", "3.000000", ["+", "v1", "v2"]],
                    ["*", ["+", "v1", "v2"], ["+", "v1", "v2"]],
                    "v3",
                ],
            ],
        )

        e = m.get_constraint(0)
        self.assertEqual(
            e.to_list(),
            [
                "<=",
                "-Inf",
                ["+", ["*", "v1", "v1"], "1.000000", ["*", "-3.000000", "v2"]],
                "0.000000",
            ],
        )
        self.assertEqual(e.value, -4)
        e = m.get_constraint(1)
        self.assertEqual(e.value, 5)
        e = m.get_constraint(2)
        self.assertEqual(e.value, 2)
        e = m.get_constraint(3)
        self.assertEqual(e.value, 1)
        e = m.get_constraint(4)
        self.assertEqual(e.value, -2)

        self.assertEqual(nlp.compute_f(), 19)
        self.assertEqual(nlp.compute_c(), [-4, 5, 2, 1, -2])
        self.assertEqual(nlp.compute_df(), [8, 8, 1])
        self.assertEqual(nlp.compute_dc(0), [2, -3, 0])
        self.assertEqual(nlp.compute_dc(1), [-2, 3, 0])
        self.assertEqual(nlp.compute_dc(2), [0, 1, 0])
        self.assertEqual(nlp.compute_dc(3), [1, 0, 0])
        self.assertEqual(nlp.compute_dc(4), [1, -3, 0])

    def test_constant_obj1(self):
        m = model()
        p = parameter(value=2)
        m.add_objective(p)

    def test_constant_obj2(self):
        m = model()
        m.add_objective(2)

    def test_variable_obj(self):
        m = model()
        v = variable(name="v")
        m.add_variable(v)
        m.add_objective(v)

    def test_write_nl(self):
        m = model()
        v1 = m.add_variable(name="v1", value=1)
        v2 = m.add_variable(name="v2", value=2)
        v3 = m.add_variable(name="v3", value=3)
        # v4 = variable(name="v3", value=3)

        e = v1 + v2
        m.add_objective((1 - e) + 3 * e + e * e + v3)
        m.add_constraint(v1 * v1 + 1 <= 3 * v2)
        m.add_constraint(v1**2 >= 3 * v2)
        m.add_constraint(1 < v2)
        m.add_constraint(v1 > 1)
        m.add_constraint(v1 == 3 * v3 - 3)

        m.write("test1.nl")


if __name__ == "__main__":  # pragma:nocover
    unittest.main()
