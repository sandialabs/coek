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

#   Source:  problem 28 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-MN-V-V

import poek as pk


model = pk.model()

ndp = 5002
h = 1.0/(ndp-1)

x = model.variable(index=range(1,ndp+1))
for i in x:
    x[i].value = ((i-1)*h)*((i-1)*h-1)
x[1].value = 0
x[1].fixed = True
x[ndp].fixed = True

model.add( pk.expression(0) )

for i in range(2,ndp):
    model.add( (-x[i-1]+2*x[i]-x[i+1]+0.5*h**2*(x[i]+i*h+1)**3 ) == 0 )
