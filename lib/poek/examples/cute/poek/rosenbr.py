# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#Taken from:
#
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

#   Source:  problem 1 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989, Corrected July 1993.

#   classification SUR2-AN-2-0

import poek as pk


model = pk.model()

N = [1,2]
x = model.add_variable(index=N)
x[1].value = -1.2
x[2].value = 1.0

model.add_objective( (x[2]-x[1]**2)**2/0.01 + (x[1]-1)**2 )
