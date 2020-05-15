#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

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

#   classification LQI2-RN-157-134

from pyomo.core import *
model = ConcreteModel()

M =1000
N =3*M

alpha = 1.0
beta = 0.26
gamma = 0.26
delta = 0.26

K = Param(list(range(1,4),value=0)
x = model.variable(list(range(1,N),value=2.0)

def f_rule(model):
model.add( 1.0 + sum(alpha*x[i]**2*(i/N)**K[1] for i in range(1,N+1))+\
    sum(beta*x[i]**2*(x[i+1]+x[i+1]**2)**2*(i/N)**K[2] for i in range(1,N))+\
    sum(gamma*x[i]**2*x[i+M]**4*(i/N)**K[3] for i in range(1,2*M+1)) +\
    sum(delta*x[i]*x[i+2*M]*(i/N)**K[4] for i in range(1,M+1))
f = Objective(rule=f_rule)
