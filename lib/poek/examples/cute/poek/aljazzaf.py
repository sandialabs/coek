# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
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

#   Source:
#   M. Aljazzaf,
#   "Multiplier methods with partial elimination of constraints for
#   nonlinear programming",
#   PhD Thesis, North Carolina State University, Raleigh, 1990.

#   SDIF input: Ph. Toint, May 1990.

#   classification QQR2-AN-3-1

import poek as pk


model = pk.model()

N = 3
N1 = 2
Biga = 100.0

F = (Biga**2 - 1.0) / (N - 1)
F2 = (Biga**2 - 1.0) / (Biga * (N - 1))

S = list(range(1, N + 1))
SS1 = list(range(2, N1 + 1))
SS2 = list(range(N1 + 1, N + 1))
A = {i: Biga - (i - 1) * F2 for i in S}
B = {i: (i - 1) * F + 1.0 for i in S}

x = model.add_variable(index=S, lb=0, value=0.0)

model.add_objective(
    A[1] * (x[1] - 0.5) ** 2
    + sum(A[i] * (x[i] + 1.0) ** 2 for i in SS1)
    + sum(A[i] * (x[i] - 1.0) ** 2 for i in SS2)
)

model.add_constraint(
    -B[1] * x[1]
    + B[1]
    + sum(B[i] * (x[i] - 0.0) ** 2 for i in SS1)
    + sum(B[i] * (x[i] - 1.0) ** 2 for i in SS2)
    == 0
)
