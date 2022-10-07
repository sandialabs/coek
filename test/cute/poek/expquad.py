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

#   classification OBR2-AN-V-V

import poek as pk

exp = pk.exp


model = pk.model()

n = 120
m = 10

S = list(range(1, n + 1))
x = model.add_variable(index=S, value=0.0)

SS = list(range(1, m + 1))
SSS = list(range(m + 1, n))

sum1 = sum(exp(0.1 * i * m * x[i] * x[i + 1]) for i in SS)
sum2 = sum(4.0 * x[i] * x[i] + 2.0 * x[n] * x[n] + x[i] * x[n] for i in SSS)
sum3 = sum(-10.0 * i * x[i] for i in S)
model.add_objective(sum1 + sum2 + sum3)

for i in SS:
    model.add_constraint(pk.inequality(0.0, x[i], 10.0))
