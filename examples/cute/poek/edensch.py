# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
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

#   Source:
#   G. Li,
#   "The secant/finite difference algorithm for solving sparse
#   nonlinear systems of equations",
#   SIAM Journal on Optimization, (to appear), 1990.

#   SIF input: Ph. Toint, Apr 1990.
#              minor correction by Ph. Shott, January 1995.

#   classification OUR2-AN-V-0

import poek as pk


model = pk.model()

N = 2000
x = model.variable(index=range(1,N+1), value=0.0)

model.add( sum((x[i]-2)**4 + (x[i]*x[i+1]-2*x[i+1])**2 +\
    (x[i+1]+1)**2 for i in range(1,N)) + 16 )
