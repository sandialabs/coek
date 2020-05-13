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

#   Source: problem 21 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.

#   SIF input: A.R. Conn, April 1990

#   classification SLR2-AN-7-1

import poek as pk


model = pk.model()

N = list(range(1,8))

x = model.variable(index=N)
for i in N:
    if i <= 2:
        x[i].value = -1.0
    else:
        x[i].value = 0.0

model.add( -100+0.01*(x[1]**2+x[3]**2+x[5]**2+x[6]**2) + (x[2]**2+x[4]**2+x[7]**2) )

model.add( 10.0*x[1]-x[2]-10 >= 0 )
model.add( pk.inequality(2, x[1], 50) )
model.add( pk.inequality(-50, x[2], 50) )
model.add( x[3] <= 50 )
model.add( 2 <= x[4] )
model.add( x[6] <= 0 )
model.add( 0 <= x[7] )
