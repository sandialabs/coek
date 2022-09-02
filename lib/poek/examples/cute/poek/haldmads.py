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
#   J. Hald and K. Madsen,
#   "Combined LP and quasi-Newton methods for minimax optimization",
#   Mathematical Programming 20, pp. 49-62, 1981.

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LOR2-AN-6-42

import poek as pk
from math import exp

model = pk.model()

y = {i: -1.0+0.1*(i-1) for i in range(1,22)}
ey = {i: exp(y[i]) for i in range(1,22)}

x = model.add_variable(index=range(1,6))
for i in range(1,6):
    x[i].value = 0.5 if i==1 else 0.0

u = model.add_variable(value=0.0)

model.add_objective(u)

for i in range(1,22):
    model.add_constraint( (x[1]+y[i]*x[2])/(1.0+x[3]*y[i]+\
        x[4]*y[i]**2+x[5]*y[i]**3)-u <= ey[i] )

for i in range(1,22):
    model.add_constraint( -(x[1]+y[i]*x[2])/(1.0+x[3]*y[i]+x[4]*y[i]**2+\
        x[5]*y[i]**3)-u <= -ey[i] )
