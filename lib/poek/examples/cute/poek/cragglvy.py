# TODO
# Formulated in Pyomo by Juan Lopez
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

#   Source:  problem 32 in
#   Ph. L. Toint,
#   "Test problems for partially separable optimization and results
#   for the routine PSPMIN",
#   Report 83/4, Department of Mathematics, FUNDP (Namur, B), 1983.

#   See  also Buckley#18
#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AY-V-0

import poek as pk
exp = pk.exp
tan = pk.tan


model = pk.model()

m = 2499
n = 2*m+2

x = model.add_variable(index=range(1,n+1))
for i in x:
    x[i].value = 1 if i==1 else 2

model.add_objective( sum((exp(x[2*i-1])-x[2*i])**4 +\
    100*(x[2*i]-x[2*i+1])**6 +\
    (tan(x[2*i+1]-x[2*i+2])+x[2*i+1]-x[2*i+2])**4 +\
    (x[2*i-1])**8 +\
    (x[2*i+2]-1.0)**2 for i in range(1,m+1)) )
