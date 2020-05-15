#  _________________________________________________________________________
#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________
#
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
#   Ph. Toint, private communication.

#   SIF input: Ph. Toint, April 1991.

#   classification SOR2-AN-V-V

from pyomo.core import *
model = ConcreteModel()

t = Param(value=1000)
xt = Param(value=10.0)
mass = Param(value=0.37)
tol = Param(value=0.1)

def h_rule(model):
model.add( value(xt)/value(t)
h = Param(value=h_rule)
def w_rule(model):
model.add( value(xt)*(value(t)+1.0)/2.0
w = Param(value=w_rule)

def fmax_rule(model):
model.add( value(xt)/value(t)
fmax = Param(value=fmax_rule)

S = list(range(0,t)
SS = list(range(1,t)

def x_rule(model, i):
model.add( i*value(h)
def x_bound(model, i):
    l = 0.0
    u = value(xt)
model.add( (l,u)
x = model.variable(S, bounds=x_bound, value=x_rule)
y = model.variable(S)
z = model.variable(S)
vx = model.variable(S, value=1.0)
vy = model.variable(S)
vz = model.variable(S)

def u_bound(model, i):
    l = -value(fmax)
    u = value(fmax)
model.add( (l,u)
ux = model.variable(SS, bounds=u_bound)
uy = model.variable(SS, bounds=u_bound)
uz = model.variable(SS, bounds=u_bound)

x[0] = 0.0
x[0].fixed=True
y[0] = 0.0
y[0].fixed=True
z[0] = 1.0
z[0].fixed=True
vx[0] = 0.0
vx[0].fixed=True
vy[0] = 0.0
vy[0].fixed=True
vz[0] = 0.0
vz[0].fixed=True
vx[value(t)] = 0.0
vx[value(t)].fixed=True
vy[value(t)] = 0.0
vy[value(t)].fixed=True
vz[value(t)] = 0.0
vz[value(t)].fixed=True

def f(model):
model.add( sum ([(i*value(h)/value(w))*(x[i] - value(xt))**2 for i in SS])
f = Objective(rule=f,sense=minimize)

def acx(model, i):
model.add( value(mass)*(vx[i]-vx[i-1])/value(h) - ux[i] == 0
acx = Constraint(SS, rule=acx)
def acy(model, i):
model.add( value(mass)*(vy[i]-vy[i-1])/value(h) - uy[i] == 0
acy = Constraint(SS, rule=acy)
def acz(model, i):
model.add( value(mass)*(vz[i]-vz[i-1])/value(h) - uz[i] == 0
acz = Constraint(SS, rule=acz)
def psx(model, i):
model.add( (x[i]-x[i-1])/value(h) - vx[i] == 0
psx = Constraint(SS, rule=psx)
def psy(model, i):
model.add( (y[i]-y[i-1])/value(h) - vy[i] == 0
psy = Constraint(SS, rule=psy)
def psz(model, i):
model.add( (z[i]-z[i-1])/value(h) - vz[i] == 0
psz = Constraint(SS, rule=psz)
def sc(model, i):
model.add( (y[i] - sin(x[i]))**2 + (z[i] - cos(x[i]))**2 - value(tol)**2 <= 0
sc = Constraint(SS, rule=sc)
