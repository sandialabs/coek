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

#   classification QLR2-AN-V-V

import poek as pk


model = pk.model()

N = 10000
M = 3*N//4

x = model.variable(index=range(1,N+1), lb=0, ub=10, value=0.5)

model.add( sum((x[i]+x[((2*i-1) % N)+1]+x[((3*i-1) % N)+1])**2*i/2.0 for i in x) )

for i in range(1,M+1):
    model.add( x[i]+2*x[((4*i-1) % N)+1] + 3*x[((5*i-1)  % N)+1] - 6.0 == 0 )
