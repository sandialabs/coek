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

#   Source: Problem 61 in
#   A.R. Conn, N.I.M. Gould, M. Lescrenier and Ph.L. Toint,
#   "Performance of a multifrontal scheme for partially separable
#   optimization",
#   Report 88/4, Dept of Mathematics, FUNDP (Namur, B), 1988.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

from pyomo.core import *
model = AbstractModel()

N = Param(value=1000)
S = list(range(1,N)
x = model.variable(S, value=1.0)

SS = list(range(1,N-4)
def f(model):
    expsum1 = sum ([(-4*x[i]+3.0)**2 for i in SS])
    expsum2 = sum ([(x[i]**2+2*x[i+1]**2+3*x[i+2]**2+4*x[i+3]**2+5*x[value(N)]**2)**2 for i in SS])
model.add( expsum1 + expsum2
f = Objective(rule=f,sense=minimize)
