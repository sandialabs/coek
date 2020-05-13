# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil. Taken from:

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
#   A. Lewis, private communication.

#   SIF input: A.R. Conn and Ph. Toint, March 1990.

#   classification QOR2-AN-6-9

import poek as pk


model = pk.model()

N = 6
DEG = 3
PEN = 1e4

range1 = list(range(0,N))
range2 = list(range(0,DEG))

c = {}
for i in range(DEG):
    for j in range(i,N):
        c[i,j] = 1 if i==0 else c[i-1,j]*j
ct = {}
for i in range(DEG):
    ct[i] = -1 if i==0 else ct[i-1]*(N-i+1)

a = model.variable(index=range1, lb=-10, ub=10)
a[0].value = -1.0
a[1].value = 1.0
a[2].value = 1.0
a[3].value = 0.0
a[4].value = 1.0
a[5].value = -1.0

model.add( sum(a[j]**2 for j in range1) )

for j in range1:
    model.add( (a[j]**3-a[j])/PEN == 0 )

model.add( sum(a[j]*c[0,j] for j in range1) - ct[0] == 0 )

for i in range(1,DEG):
    model.add( sum(a[j]*c[i,j] for j in range(i,N)) - ct[i] == 0 )
