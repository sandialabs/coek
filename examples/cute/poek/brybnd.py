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

#   Source: problem 31 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#73 (p. 41) and Toint#18

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 5000
ml = 5
mu = 1

x = model.variable(index=range(1,N+1), value=-1)


def J(i):
    for j in range(max(1,i-ml), min(N+1,i+mu+1)):
        if j != i:
            yield j

model.add( sum((x[i]*(2+5*x[i]**2) + 1 -\
           sum(x[j]*(1+x[j]) for j in J(i)))**2 for i in range(1,N+1)) )
