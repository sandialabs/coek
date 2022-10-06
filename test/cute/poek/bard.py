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

#   Source: Problem 3 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#16.
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-3-0

import poek as pk


model = pk.model()

N = 3

data = pk.util.load_data("bard.json")
S = data.unpack("S")
y = data.unpack("y", index=("S"))

u = {}
v = {}
w = {}
for i in S:
    u[i] = i
    v[i] = 16 - i
    w[i] = min(u[i], v[i])

x = model.add_variable(index=[1, 2, 3], value=1.0)

model.add_objective(sum((y[i] - (x[1] + u[i] / (v[i] * x[2] + w[i] * x[3]))) ** 2 for i in S))
