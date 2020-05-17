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

#   Source: a simplification of
#   J.F. Blowey and C.M. Elliott,
#   "The Cahn-Hilliard gradient theory for phase separation with 
#   non-smooth free energy Part II: Numerical analysis",
#   European Journal of Applied Mathematics (3) pp 147-179, 1992.

#   SIF input: Nick Gould, August 1996

#   classification QLR2-MN-V-V

import poek as pk


model = pk.model()

N = 1000
A = 1/5.0
B = 2/5.0
C = 3/5.0
D = 4/5.0
INT = N*(1+A+B-C-D)

v = {}
for i in range(0,N+1):
    if (0 <= i) and (i <= N*A):
        v[i] = 1.0
    elif (N*A+1 <= i) and (i <= N*B):
        v[i] = 1 - (i-N*A)*2/(N*(B-A))
    elif (N*B+1 <= i) and (i <= N*C):
        v[i] = -1 
    elif (N*C+1 <= i) and (i <= N*D):
        v[i] = -1 + (i-N*C)*2/(N*(D-C))
    else:
        v[i] = 1

u = model.variable(N+1, lb=-1.0, ub=1.0)
for i in u:
    u[i].value = v[i]
w = model.variable(N+1, value=0.0)
    
model.add( -2*u[0]*u[1] + u[0]**2 + \
    sum(-2*u[i]*u[i+1] + 2*u[i]**2 for i in range(1,int(N)))+\
    u[N]**2+sum(1/N**2*u[i]*w[i] for i in range(0,int(N)+1))+\
    sum(-1/N**2*v[i]*u[i]-2/N**2*v[i]*w[i] for i in range(0,int(N)+1))+\
    (v[1]-v[0])*u[0]\
    +sum((v[i-1]-2*v[i]+v[i+1])*u[i] for i in range(1,int(N))) + (v[N-1]-v[N])*u[N] )
    
model.add( 0.5*u[0] + sum(u[i] for i in range(1,int(N))) + 0.5*u[N] == 0.2*INT )

model.add( u[0] - u[1] - 1/N**2*w[0] == 0 )

for i in range(1,N):
    model.add( 2*u[i] - u[i+1] - u[i-1] - 1/N**2*w[i] == 0 )

model.add( u[N] - u[N-1] - 1/N**2*w[N]== 0 )
