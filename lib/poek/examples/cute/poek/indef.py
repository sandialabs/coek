# Adapted from Pyomo model by William E. Hart
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

#   classification OUR2-AN-V-0

import poek as pk

cos = pk.cos


model = pk.model()

n = 1000
N = list(range(1, n + 1))
M = list(range(2, n))
alpha = 0.5

x = model.add_variable(index=N)
for i in N:
    x[i].value = i / (n + 1.0)

model.add_objective(sum(x[i] for i in N) + sum(alpha * cos(2 * x[i] - x[n] - x[1]) for i in M))
