# TODO
# Formulated in pyomo by Logan Barnes. Taken from:

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

#   Source: problem 111 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.
#   This problem has been modified on 20 Oct 92 by Todd Plantenga as follows.
#   The bound constraints, which are inactive at the solution,
#   are removed.

#   SIF input: Nick Gould, August 1991 and T. Plantenga, October 1992.

#   classification OOR2-AN-10-3

import poek as pk

exp = pk.exp
log = pk.log

model = pk.model()

N = list(range(1, 11))

x = model.add_variable(index=N, value=-2.3)
c = {
    1: -6.089,
    2: -17.164,
    3: -34.054,
    4: -5.914,
    5: -24.721,
    6: -14.986,
    7: -24.100,
    8: -10.708,
    9: -26.662,
    10: -22.179,
}

model.add_objective(sum(exp(x[j]) * (c[j] + x[j] - log(sum(exp(x[k]) for k in N))) for j in N))

model.add_constraint(exp(x[1]) + 2 * exp(x[2]) + 2 * exp(x[3]) + exp(x[6]) + exp(x[10]) == 2.0)
model.add_constraint(exp(x[4]) + 2 * exp(x[5]) + exp(x[6]) + exp(x[7]) == 1)
model.add_constraint(exp(x[3]) + exp(x[7]) + exp(x[8]) + 2 * exp(x[9]) + exp(x[10]) == 1)
