# TODO
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

#   Source:  problem 8, eqs (8.10)--(8.11) in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-MN-V-V

import poek as pk
exp = pk.exp


model = pk.model()

n = 1000

pe = 5.0
d = 0.135
b = 0.5
gamma = 25.0

h = 1/(n-1)
ct1 = -h*pe
cti1 = 1/h + 1/((h**2)*pe)
cti = -1/h-2/((h**2)*pe)

SS = list(range(2,n))

t = model.variable(index=range(1,n+1), lb=0.0000001, value=1.0)


model.add( pk.expression(0) )

model.add( (ct1*t[2]-t[1]+h*pe) == 0 )

for i in SS:
    model.add( (d*(b+1-t[i])*exp(gamma-gamma/t[i])+cti1*t[i-1]\
                +cti*t[i]+t[i+1]/(h**2*pe)) == 0 )

model.add( (t[n]-t[n-1]) == 0 )
