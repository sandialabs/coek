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
#  Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
#  Taken from CUTE models

from pyomo.environ import *
model = AbstractModel()
n = 4
N = list(range(1,30)
M = list(range(1,n)
mu= Param(N)

def A_rule(model,j):
model.add_objective( 2.0*sin(mu[j])/(mu[j] + sin(mu[j])*cos(mu[j]))
A = Param(N,value=A_rule)

def x_init_rule(model,i):
model.add_constraint( 0.5 * (-1)**(i+1)
x = model.add_variable(M,value=x_init_rule)

def rho(model,j):
model.add_constraint(  -(exp(-mu[j]**2 * sum(x[i]**2 for i in M))\
        + sum(2.0*(-1)**(ii-1)*exp(-mu[j]**2 * sum(x[i]**2 for i in range(ii,n+1))) for ii in range(2,n+1))\
        + (-1)**n)/mu[j]**2

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

def obj_rule(model):
model.add_constraint( sum(x[i]**2 for i in M)
obj = Objective(rule=obj_rule,sense=minimize)

def cons1_rule(model):
model.add_constraint( sum(mu[i]**2 * mu[j]**2 * A[i] * A[j] * rho(model,i) * rho(model,j)\
        *(sin(mu[i]+mu[j])/(mu[i]+mu[j])+sin(mu[i]-mu[j])/(mu[i]-mu[j]))\
        for i in N for j in range(i+1,30+1))\
        + sum(mu[j]**4 * A[j]**2 * rho(model,j)**2 *(sin(2.0*mu[j])/(2.0*mu[j]) + 1.0)/2.0\
        for j in N)\
        - sum(mu[j]**2 * A[j] * rho(model,j) * (2.0*sin(mu[j])/mu[j]**3\
        - 2.0*cos(mu[j])/mu[j]**2)\
        for j in N) + 2.0/15.0 <= 0.0001
constr1 = Constraint(rule=cons1_rule)
