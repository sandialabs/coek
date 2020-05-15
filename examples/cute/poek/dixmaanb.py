#  _________________________________________________________________________                                                                                \
#                                                                                                                                                           \
#  Pyomo: Python Optimization Modeling Objects                                                                                                           \
#  Copyright (c) 2010 Sandia Corporation.                                                                                                                   \
#  This software is distributed under the BSD License.                                                                                                      \
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,                                                                                   \
#  the U.S. Government retains certain rights in this software.                                                                                             \
#  For more information, see the Pyomo README.txt file.                                                                                                     \
#  _________________________________________________________________________                                                                                \

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
#   L.C.W. Dixon and Z. Maany,
#   "A family of test problems with sparse Hessians for unconstrained
#   optimization",
#   TR 206, Numerical Optimization Centre, Hatfield Polytechnic, 1988.

#   See also Buckley#221 (p. 49)
#   SIF input: Ph. Toint, Dec 1989.
#              correction by Ph. Shott, January 1995.

#   classification OUR2-AN-V-0

from pyomo.core import *
model = AbstractModel()
M = 1000
M = Param(value=M)
N = Param(value=3*M)

alpha = Param(value=1.0)
beta = Param(value=0.0625)
gamma = Param(value=0.0625)
delta = Param(value=0.0625)

S1 = list(range(1,4)
S2 = list(range(1,N)
K = Param(S1, value=0)
x = model.variable(S2, value=2.0)

S3 = list(range(1,N-1)
S4 = list(range(1,2*M)
S5 = list(range(1,M)

def f(model):
    exp1 = sum ([value(alpha)*x[i]**2*(i/value(N))**value(K[1]) for i in S2])
    exp2 = sum ([value(beta)*x[i]**2*(x[i+1]+x[i+1]**2)**2*(i/value(N))**value(K[2]) for i in S3])
    exp3 = sum ([value(gamma)*x[i]**2*x[i+value(M)]**4*(i/value(N))**value(K[3]) for i in S4])
    exp4 = sum ([value(delta)*x[i]*x[i+2*value(M)]*(i/value(N))**value(K[4]) for i in S5])
model.add( 1.0 + exp1 + exp2 + exp3 + exp4
f = Objective(rule=f,sense=minimize)
