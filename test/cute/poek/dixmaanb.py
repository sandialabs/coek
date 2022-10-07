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
#   L.C.W. Dixon and Z. Maany,
#   "A family of test problems with sparse Hessians for unconstrained
#   optimization",
#   TR 206, Numerical Optimization Centre, Hatfield Polytechnic, 1988.

#   See also Buckley#221 (p. 49)
#   SIF input: Ph. Toint, Dec 1989.
#              correction by Ph. Shott, January 1995.

#   classification OUR2-AN-V-0

import poek as pk


model = pk.model()

M = 1000
N = 3 * M

alpha = 1.0
beta = 0.0625
gamma = 0.0625
delta = 0.0625

K = {i: 0 for i in [1, 2, 3, 4]}

x = model.add_variable(index=range(1, N + 1), value=2.0)

exp1 = sum(alpha * x[i] ** 2 * (i / N) ** K[1] for i in range(1, N + 1))
exp2 = sum(
    beta * x[i] ** 2 * (x[i + 1] + x[i + 1] ** 2) ** 2 * (i / N) ** K[2] for i in range(1, N)
)
exp3 = sum(gamma * x[i] ** 2 * x[i + M] ** 4 * (i / N) ** K[3] for i in range(1, 2 * M + 1))
exp4 = sum(delta * x[i] * x[i + 2 * M] * (i / N) ** K[4] for i in range(1, M + 1))
model.add_objective(1.0 + exp1 + exp2 + exp3 + exp4)
