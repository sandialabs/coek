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

#   Source:
#   W. Li and J. Swetits,
#   "A Newton method for convex regression, data smoothing and
#   quadratic programming with bounded constraints",
#   SIAM J. Optimization 3 (3) pp 466-488, 1993.

#   SIF input: Nick Gould, August 1994.

#   classification QLR2-AN-V-V

import poek as pk
from math import sin


model = pk.model()

K = 2
N = 10000

S = list(range(0, K + 1))
R = list(range(1, N + K + 1))
Q = list(range(1, N + 1))

B = {}
for i in S:
    B[i] = 1 if i == 0 else B[i - 1] * i

C = {i: 1 if i == 0 else (-1) ** i * B[K] / (B[i] * B[K - i]) for i in S}

T = {i: (i - 1.0) / (N + K - 1.0) for i in R}

x = model.add_variable(index=R)

model.add_objective(
    sum(-(T[i] ** 2 + 0.1 * sin(i)) * x[i] for i in R)
    + sum(0.5 * (T[i] ** 2 + 0.1 * sin(i)) ** 2 for i in R)
    + sum(0.5 * x[i] ** 2 for i in R)
)

for j in Q:
    model.add_constraint(sum(C[i] * x[j + K - i] for i in S) >= 0)
