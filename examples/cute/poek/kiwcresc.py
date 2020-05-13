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

#   Source: 
#   K.C. Kiwiel,
#   "Methods of Descent for Nondifferentiable Optimization"
#   Lectures Notes in Mathematics 1133, Springer Verlag, 1985.

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LQR2-AN-3-2

import poek as pk


model = pk.model()

N = [1,2]

x = model.variable(index=N)
x[1].value = -1.5
x[2].value =  2.0
u = model.variable()

model.add( u )

model.add( -u+x[2]-1+x[1]**2+(x[2]-1.0)**2 <= 0.0 )
model.add( -u+x[2]+1-x[1]**2-(x[2]-1.0)**2 <= 0.0 )
