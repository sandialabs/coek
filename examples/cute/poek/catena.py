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

#   Source:
#   K. Veselic,
#   "De forma catenarum in campo gravitatis pendentium",
#   Klasicna Gimnazija u Zagrebu, Zagreb, 1987.

#   SIF input: Ph. L. Toint, May 1993.
#              correction by F. Ruediger, Mar 1997.

#   classification LQR2-AY-V-V

import poek as pk


model = pk.model()

N = 10

gamma = 9.81
tmass = 500.0
bl = 1.0
fract = 0.6

length = bl*(N+1)*fract
mass = tmass/(N+1.0)
mg = mass*gamma

Sv = list(range(0,N+2))

x = model.variable(index=Sv, name="x")
for i in Sv:
    x[i].value = i*length/(N+1.0)
y = model.variable(index=Sv, name="y")
for i in Sv:
    y[i].value = -i*length/(N+1.0)
z = model.variable(index=Sv, value=0.0, name="z")

obsum = 0
for i in range(1,N+1):
    obsum += mg*y[i]
obsum += mg*y[N+1]/2.0
expr = mg*y[0]/2.0 + obsum
model.add( expr )

for i in range(1,N+2):
    expr = (x[i]-x[i-1])**2 + (y[i]-y[i-1])**2 + (z[i]-z[i-1])**2
    model.add( expr == bl**2 )

x[0].value = 0.0
x[0].fixed = True
y[0].value = 0.0
y[0].fixed = True
z[0].value = 0.0
z[0].fixed = True
x[N+1].value = length
x[N+1].fixed = True
