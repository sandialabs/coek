# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
# Taken from:

# AMPL Model by Hande Y. Benson
#
# Copyright (C) 2001 Princeton University
# All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice appear in all
# copies and that the copyright notice and this
# permission notice appear in all supporting documentation.

#   Source: modified version of problem 19 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.
#   that is meant to simulate the Lagrangian barrier objective function
#   for particular values of the shifts and multipliers

#   SIF input: A.R. Conn August 1993

#   classification OUR2-AN-2-0

import poek as pk

log = pk.log


model = pk.model()

x1 = model.add_variable(value=15.0)
x2 = model.add_variable(value=-1.0)

if -((x1 - 5) ** 2) - (x2 - 5) ** 2 + 200 + 1 <= 0.0:
    obj1 = 1e10 * (-((x1 - 5) ** 2) - (x2 - 5) ** 2 + 200) ** 2
else:
    obj1 = -log(-((x1 - 5) ** 2) - (x2 - 5) ** 2 + 200 + 1)
if (x1 - 5) ** 2 + (x2 - 5) ** 2 - 100 + 1 <= 0.0:
    obj2 = 1e10 * ((x1 - 5) ** 2 + (x2 - 5) ** 2 - 100) ** 2
else:
    obj2 = -log((x1 - 5) ** 2 + (x2 - 5) ** 2 - 100 + 1)
if (x2 - 5) ** 2 + (x1 - 6) ** 2 + 1 <= 0.0:
    obj3 = 1e10 * ((x2 - 5) ** 2 + (x1 - 6) ** 2) ** 2
else:
    obj3 = -log((x2 - 5) ** 2 + (x1 - 6) ** 2 + 1)
if -((x2 - 5) ** 2) - (x1 - 6) ** 2 + 82.81 + 1 <= 0.0:
    obj4 = 1e10 * (-((x2 - 5) ** 2) - (x1 - 6) ** 2 + 82.81) ** 2
else:
    obj4 = -log(-((x2 - 5) ** 2) - (x1 - 6) ** 2 + 82.81 + 1)
if 100 - x1 + 1 <= 0.0:
    obj5 = 1e10 * (100 - x1) ** 2
else:
    obj5 = -log(100 - x1 + 1)
if x1 - 13 + 1 <= 0.0:
    obj6 = 1e10 * (x1 - 13) ** 2
else:
    obj6 = -log(x1 - 13 + 1)
if 100 - x2 + 1 <= 0.0:
    obj7 = 1e10 * (100 - x2) ** 2
else:
    obj7 = -log(100 - x2 + 1)
if x2 + 1 <= 0.0:
    obj8 = 1e10 * (x2) ** 2
else:
    obj8 = -log(x2 + 1)

model.add_objective(
    (x1 - 10) ** 3
    + (x2 - 20) ** 3
    + obj1
    + obj2
    + obj3
    + obj4
    + obj5
    + obj6
    + obj7
    + obj8
)
