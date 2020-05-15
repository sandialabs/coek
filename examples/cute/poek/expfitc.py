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
#   M.J.D. Powell,
#   "A tolerant algorithm for linearly constrained optimization
#   calculations"'
#   Mathematical Programming 45(3), pp.561--562, 1989.

#   SDIF input: Ph. Toint and N. Gould, May 1990.

#   classification OLR2-AN-5-502

import poek as pk
from math import exp


model = pk.model()

R = 251

T = {i: 5*(i-1)/(R-1) for i in range(1,R+1)}

ET = {i: exp(T[i]) for i in range(1,R+1)}

P = model.variable(index=[0,1,2])
P[0].value = 1
P[1].value = 1
P[2].value = 6

Q = model.variable(index=[1,2], value=0.0)

model.add( sum((\
    (P[0]+P[1]*T[i]+P[2]*T[i]**2)/\
    (ET[i]*(1+Q[1]*(T[i]-5)+Q[2]*(T[i]-5)**2))\
    -1 )**2 for i in range(1,R+1)) )

for i in range(1,R):
    model.add( P[0]+P[1]*T[i]+P[2]*T[i]**2-(T[i]-5)*ET[i]*Q[1]-\
        (T[i]-5)**2*ET[i]*Q[2]-ET[i]>= 0 )

for i in range(1,R):
    model.add( (T[i]-5)*Q[1] + (T[i]-5)**2*Q[2]+0.99999 >= 0 )
