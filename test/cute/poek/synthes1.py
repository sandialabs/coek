# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
# Taken from:
#
# **************************
# SET UP THE INITIAL DATA *
# **************************
#   Problem :
#   *********
#   Source: Test problem 1 (Synthesis of processing system) in
#   M. Duran & I.E. Grossmann,
#   "An outer approximation algorithm for a class of mixed integer nonlinear
#    programs", Mathematical Programming 36, pp. 307-339, 1986.
#   SIF input: S. Leyffer, October 1997
#   classification OOR2-AN-6-6

import poek as pk

log = pk.log


model = pk.model()

x1 = model.add_variable(lb=0.0, ub=2.0)
x2 = model.add_variable(lb=0.0, ub=2.0)
x3 = model.add_variable(lb=0.0, ub=1.0)
y1 = model.add_variable(lb=0.0, ub=1.0)
y2 = model.add_variable(lb=0.0, ub=1.0)
y3 = model.add_variable(lb=0.0, ub=1.0)

e = (
    -18.0 * log(x2 + 1.0)
    - 19.2 * log(x1 - x2 + 1.0)
    + 5.0 * y1
    + 6.0 * y2
    + 8.0 * y3
    + 10.0 * x1
    - 7.0 * x3
    + 10.0
)
model.add_objective(e)

model.add_constraint(0 <= 0.8 * log(x2 + 1.0) + 0.96 * log(x1 - x2 + 1.0) - 0.8 * x3)

model.add_constraint(0 <= log(x2 + 1.0) + 1.2 * log(x1 - x2 + 1.0) - x3 - 2.0 * y3 + 2.0)

model.add_constraint(0 >= x2 - x1)

model.add_constraint(0 >= x2 - 2.0 * y1)

model.add_constraint(0 >= -x2 + x1 - 2.0 * y2)

model.add_constraint(0 >= y1 + y2 - 1.0)
