# TODO
# Formulated in Pyomo by Juan Lopez
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

#   Source: Problem 33 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#93 (with different N and M)
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10
M = 20
S = list(range(1,N+1))

x = model.variable(index=S, value=1.0)

model.add( sum((sum(x[j]*i*j for j in range(1,N+1))-1.0)**2 for i in range(1,M+1)) )
