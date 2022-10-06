# Adapted from Pyomo model by William E. Hart
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

#   Source:  Problem 6 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-2-0

import poek as pk

exp = pk.exp


model = pk.model()

N = list(range(1, 11))

x1 = model.add_variable(value=0.3)
x2 = model.add_variable(value=0.4)

model.add_objective(sum((2 + 2 * i - (exp(i * x1) + exp(i * x2))) ** 2 for i in N))
