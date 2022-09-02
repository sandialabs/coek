# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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
#   H. Maurer and H.D. Mittelman,
#   "The non-linear beam via optimal control with bound state variables",
#   Optimal Control Applications and Methods 12, pp. 19-31, 1991.

#   SIF input: Ph. Toint, Nov 1993.

#   classification  OOR2-MN-V-V

import math
import poek as pk
cos = pk.cos
sin = pk.sin


model = pk.model()

ni = 500
alpha = 350.0
h = 1.0/ni

S = list(range(ni+1))

t = model.add_variable(index=S, lb=-1.0, ub=1.0)
for i in S:
    t[i].value = 0.05*math.cos(i*h)

x = model.add_variable(index=S, lb=-0.05, ub=0.05)
for i in S:
    x[i].value = 0.05*math.cos(i*h)

u = model.add_variable(index=S)

model.add_objective( sum((0.5*h*(u[i+1]**2 + u[i]**2) + 0.5*alpha*h*(cos(t[i+1]) +\
    cos(t[i]))) for i in range(ni)) )

for i in range(ni):
    model.add_constraint( x[i+1] - x[i] - 0.5*h*(sin(t[i+1]) + sin(t[i]))== 0 )

for i in range(ni):
    model.add_constraint( t[i+1] - t[i] - 0.5*h*u[i+1] - 0.5*h*u[i] == 0 )

x[0].value = 0.0
x[0].fixed = True
x[ni].value = 0.0
x[ni].fixed = True
t[0].value = 0.0
t[0].fixed = True
t[ni].value = 0.0
t[ni].fixed = True
