# TODO
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

#   Source: problem 2 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Toint#33, Buckley#24
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

n = 5000
ngs = n-1

x = model.variable(index=range(1,n+1))
x[1].value = 0.5
x[2].value = -2.0
for i in range(3,n+1):
    x[i].value = 0.0

model.add( sum(((5.0-x[i+1])*x[i+1]**2+x[i]-2*x[i+1]-13.0)**2 for i in range(1,ngs+1))+\
    sum(((1.0+x[i+1])*x[i+1]**2+x[i]-14*x[i+1]-29.0)**2 for i in range(1,ngs+1)) )
