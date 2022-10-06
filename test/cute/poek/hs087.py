# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from:
# hs87.mod  LOR2-RN-11-24
# Original AMPL coding by Elena Bobrovnikova (summer 1996 at Bell Labs).

# The problem given here is stated in the notation of Hock & Schittkowski,
# but is corrected to conform to the problem as stated in
#   D. M. Himmelblau, Applied Nonlinear Programming,
#   McGraw-Hill, 1972, pp. 413-414
# except that jump-discontinuities in the objective function are
# omitted by stating it as the sum of two piecewise-linear terms.
# Errors in the problem statement by Hock & Schittkowski are noted in
# comments below.  (dmg, 19970820)

# Nonlinear electrical network

# Ref.: A.R.Colville. A Comparative Study on Nonlinear Programming
# Codes. IBM Scientific Center Report 320-2949, no.6, 1968.

# Ref.: W. Hock and K. Schittkowski, Test Examples for Nonlinear Programming
# Codes.  Lecture Notes in Economics and Mathematical Systems, v. 187,
# Springer-Verlag, New York, 1981, p. 106.

# Number of variables:  11 (6 before presolve and linearization of pl terms)
# Number of constraints:  24 (16 before presolve and linearization of pl terms)
# Objective convex piece-wise linear
# Nonlinear constraints

import poek as pk
from math import cos, sin


model = pk.model()

a = 131.078
b = 1.48477
c = 0.90798
d = cos(1.47588)
e = sin(1.47588)
lim1 = 300
lim2 = 100
lim3 = 200
rate1 = 30
rate2 = 31
rate3 = 28
rate4 = 29
rate5 = 30

N = list(range(1, 7))

z1 = model.add_variable()
z2 = model.add_variable()
x1 = model.add_variable()
x2 = model.add_variable()
x3 = model.add_variable()
x4 = model.add_variable()
x5 = model.add_variable()
x6 = model.add_variable()
x1.set_lb(0.0)
x1.set_ub(400.0)
x2.set_lb(0.0)
x2.set_ub(1000.0)
x3.set_lb(340.0)
x3.set_ub(420.0)
x4.set_lb(340.0)
x4.set_ub(420.0)
x5.set_lb(-1000.0)
x5.set_ub(1000.0)
x6.set_lb(0.0)
x6.set_ub(0.5236)

x1.value = 390.0
x2.value = 1000.0
x3.value = 419.5
x4.value = 340.5
x5.value = 198.175
x6.value = 0.5

model.add_objective(z1 + z2)


def f1(model, x):
    if x == 0:
        return 0.0
    elif x == 300:
        return 300 * 30.0
    elif x == 400:
        return 30.0 * 300.0 + 31.0 * 100.0


def f2(model, x):
    if x == 0:
        return 0.0
    elif x == 100:
        return 28.0 * 100.0
    elif x == 200:
        return 28.0 * 100.0 + 29 * 100.0
    elif x == 1000:
        return 28.0 * 100.0 + 29 * 100.0 + 30.0 * 800


piecew1 = Piecewise(z1, x1, pw_constr_type="LB", pw_pts=[0.0, lim1, 400.0], f_rule=f1)
piecew2 = Piecewise(z2, x2, pw_constr_type="LB", pw_pts=[0.0, lim2, lim3, 1000.0], f_rule=f2)

e1 = Constraint(expr=x1 == 300 - x3 * x4 * cos(b - x6) / a + c * x3**2 * d / a)
e2 = Constraint(expr=x2 == -x3 * x4 * cos(b + x6) / a + c * x4**2 * d / a)
e3 = Constraint(expr=x5 == -x3 * x4 * sin(b + x6) / a + c * x4**2 * e / a)
e4 = Constraint(expr=200 - x3 * x4 * sin(b - x6) / a + c * x3**2 * e / a == 0)
