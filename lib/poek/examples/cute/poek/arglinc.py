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

#   Source: Problem 34 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#101 (with different N and M)
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10
M = 20
S = list(range(1,N+1))

x = model.add_variable(index=S, value=1.0)

model.add_objective( 2 + sum( (sum(x[j]*j*(i-1) for j in range(2,N)) - 1.0)**2 
                    for i in range(2,M) ) )

