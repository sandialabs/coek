# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#Taken from:

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

#   classification QBR2-AN-V-0

import poek as pk


model = pk.model()

N = 100
M = N//2
Nplus = N//4
Rx = list(range(1,N+1))
Rsum1 = list(range(1,Nplus+1))
Rsum2 = list(range(Nplus+1,N+1))

x = model.variable(index=Rx, lb=0.1, ub=10.0, value=0.5)

model.add(\
    sum( 0.5*i*(x[i]+x[ ((2*i-1) % N) + 1] + x[ ((3*i-1) % N) + 1 ] )**2 for i in Rsum1 ) -\
    sum( 0.5*i*(x[i]+x[ ((2*i-1) % N) + 1] + x[ ((3*i-1) % N) + 1 ] )**2 for i in Rsum2 ) )
