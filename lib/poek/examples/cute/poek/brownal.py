# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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

#   Source: Problem 27 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#79
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10

x = model.add_variable(index=list(range(1, N + 1)), value=0.5)

expr = 1.0
for j in range(1, N + 1):
    expr *= x[j]
expr -= 1.0
model.add_objective(
    sum((x[i] + sum(x[j] for j in range(1, N + 1)) - (N + 1)) ** 2 for i in range(1, N))
    + expr**2
)
