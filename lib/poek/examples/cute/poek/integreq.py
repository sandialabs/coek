# TODO
#  Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
#  Taken from:

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

#   Source:  Problem 29 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Feb 1990.

#   classification NOR2-AN-V-V

import poek as pk


model = pk.model()

N = 100
h = 1.0 / (N + 1)
M = list(range(1, N + 1))
L = list(range(0, N + 2))
t = {i: i * h for i in M}

x = model.add_variable(index=L)
for i in L:
    x[i].value = t[i] * (t[i] - 1.0) if (i >= 1) and (i <= N) else 0.0

x[0].value = 0.0
x[0].fixed = True
x[N + 1].value = 0.0
x[N + 1].fixed = True

model.add_objective(pk.expression(0))

for i in M:
    model.add_constraint(
        (
            x[i]
            + h
            * (
                (1 - t[i]) * sum(t[j] * (x[j] + t[j] + 1.0) ** 3 for j in range(1, i + 1))
                + t[i] * sum((1.0 - t[j]) * (x[j] + t[j] + 1.0) ** 3 for j in range(i + 1, N + 1))
            )
            / 2.0
        )
        == 0
    )
