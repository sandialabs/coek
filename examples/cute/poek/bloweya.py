#  _________________________________________________________________________
#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

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

from pyomo.core import *
model = ConcreteModel()

N = 1000.0
A = 1/5.0
B = 2/5.0
C = 3/5.0
D = 4/5.0
INT = N*(1+A+B-C-D)

def v_rule(model,i):
    if (0 <= i) and (i <= N*A):
model.add( 1.0
    elif (N*A+1 <= i) and (i <= N*B):
model.add( 1 - (i-N*A)*2/(N*(B-A))
    elif (N*B+1 <= i) and (i <= N*C):
model.add( -1 
    elif (N*C+1 <= i) and (i <= N*D):
model.add( (-1 + (i-N*C)*2/(N*(D-C)))
    else:
model.add( 1.0
v = Param(list(range(0,N),value=v_rule)

def u_rule(model,i):
model.add( v[i]
u = model.variable(list(range(0,N),bounds=(-1.0,1.0),value=u_rule)
w = model.variable(list(range(0,N),value=0.0)
    
def f_rule(model):
model.add( -2*u[0]*u[1] + u[0]**2 + \
    sum(-2*u[i]*u[i+1] + 2*u[i]**2 for i in range(1,int(N)))+\
    u[N]**2+sum(1/N**2*u[i]*w[i] for i in range(0,int(N)+1))+\
    sum(-1/N**2*v[i]*u[i]-2/N**2*v[i]*w[i] for i in range(0,int(N)+1))+\
    (v[1]-v[0])*u[0]\
    +sum((v[i-1]-2*v[i]+v[i+1])*u[i] for i in range(1,int(N))) + (v[N-1]-v[N])*u[N]
f = Objective(rule=f_rule)
    
def con1_rule(model):
model.add( 0.5*u[0] + sum(u[i] for i in range(1,int(N))) + 0.5*u[N] == 0.2*INT
cons1 = Constraint(rule=con1_rule)

def con2_rule(model):
model.add( u[0] - u[1] - 1/N**2*w[0] == 0
cons2 = Constraint(rule=con2_rule)

def con3_rule(model,i):
model.add( 2*u[i] - u[i+1] - u[i-1] - 1/N**2*w[i] == 0
cons3 = Constraint(list(range(1,N-1),rule=con3_rule)

def con4_rule(model):
model.add( u[N] - u[N-1] - 1/N**2*w[N]== 0
cons4 = Constraint(rule=con4_rule)
