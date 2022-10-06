# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
# Taken from:

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
#   S. Ceria, private communication, 1995.

#   SIF input: Ph. Toint, Feb 1995.

#   classification NQR2-RN-9-9

from itertools import product
import poek as pk


model = pk.model()

N = [1, 2, 3]

data = pk.util.load_data("coolhans.json")
A, B, C = data.unpack("A", "B", "C", index=("i", "j"))

X = model.add_variable(index=product(N, N), value=0.0)

AXX = {}
for i in N:
    for j in N:
        AXX[i, j] = sum(sum(A[i, k] * X[k, m] for k in N) * X[m, j] for m in N)

BX = {}
for i in N:
    for j in N:
        BX[i, j] = sum(B[i, k] * X[k, j] for k in N)

model.add_objective(pk.expression(0))

for i, j in AXX:
    model.add_constraint(AXX[i, j] + BX[i, j] + C[i, j] == 0)
