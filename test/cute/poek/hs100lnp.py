# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
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

#   Source: problem 100 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.
#   This problem has been modified 20 Oct 92 by Todd Plantenga as follows.
#   The nonlinear inequality constraints are removed (if inactive
#   at the solution) or changed to equalities (if active).

#   SIF input: Ph. Toint, April 1991 and T. Plantenga, October 1992.

#   classification OOR2-AN-7-2

import poek as pk


model = pk.model()

N = list(range(1, 8))

x = model.add_variable(index=N)
x[1].value = 1.0
x[2].value = 2.0
x[3].value = 0.0
x[4].value = 4.0
x[5].value = 0.0
x[6].value = 1.0
x[7].value = 1.0

model.add_objective(
    (x[1] - 10) ** 2
    + 5.0 * (x[2] - 12) ** 2
    + x[3] ** 4
    + 3.0 * (x[4] - 11) ** 2
    + 10.0 * x[5] ** 6
    + 7.0 * x[6] ** 2
    + x[7] ** 4
    - 4.0 * x[6] * x[7]
    - 10.0 * x[6]
    - 8.0 * x[7]
)

model.add_constraint(2 * x[1] ** 2 + 3 * x[2] ** 4 + x[3] + 4 * x[4] ** 2 + 5 * x[5] == 127.0)
model.add_constraint(
    -4 * x[1] ** 2 - x[2] ** 2 + 3 * x[1] * x[2] - 2 * x[3] ** 2 - 5 * x[6] + 11 * x[7] == 0
)
