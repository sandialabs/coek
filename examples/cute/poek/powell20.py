# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
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

#   Source: a generalization of problem 20 in
#   M. J. D. Powell
#   "On the quadratic progamming algorithm of Goldfarb and Idnani",
#   Mathematical Programmimg Study 25 (1985) 46-61.

#   SIF input: Nick Gould, August 1994.

#   classification QLR2-AN-V-V

import poek as pk


model = pk.model()

N = 10
Rx = list(range(1,N+1))

x = model.variable(index=Rx)
for i in Rx:
    if i%2 == 1:
        x[i].value = 0
    else:
        x[i].value = -0.5-i

model.add( 0.5*sum( x[i]**2 for i in Rx ) )

Rc1 = list(range(1,N))

for k in Rc1:
    model.add( x[k+1]-x[k] >= -0.5+(-1)**k*k )

model.add( x[1]-x[N] >= N - 0.5 )
