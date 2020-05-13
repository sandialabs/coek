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

#   classification QLR2-AN-V-V

import poek as pk


model = pk.model()

N = 10
R1 = list(range(N))

x = model.variable(N, lb=-1, ub=1)
y = model.variable(N, lb=-1, ub=1)

model.add( sum( x[i]*y[i] for i in R1 ) )

for i in R1:
    model.add( 1 == x[i]-y[i] )

model.add( N == sum( x[i]+y[i] for i in R1 ) )
