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

#   Source: a variant on a problem of optimal knot placement in a
#   scheme for ordinary differential equations with boundary values
#   suggested by J. R. Kightley, see N. I. M. Gould, "An algorithm for
#   large-scale quadratic programming", IMA J. Num. Anal (1991),
#   11, 299-324, problem class 3. Note that the optimal solution values
#   given in that paper are incorrect.

#   SIF input: Nick Gould, December 1991

#   classification QLR2-MN-V-V

import poek as pk


model = pk.model()

K = 350

alpha = {i: 1.0+1.01**i if i>1 else 2.0 for i in range(1,K+2)}

knot = model.variable(index=range(1,K+1))
for i in range(1,K+1):
    knot[i].value = alpha[i]
    knot[i].lb = alpha[i]
    knot[i].ub = alpha[i+1]

space = model.variable(index=range(1,K))
for i in range(1,K):
    space[i].value =   alpha[i+1] - alpha[i]
    space[i].lb = 0.4*(alpha[i+2] - alpha[i])
    space[i].ub = 0.6*(alpha[i+2] - alpha[i])


model.add( sum(0.5*(space[i+1]-space[i])**2 for i in range(1,K-1))+\
           sum(0.5*(knot[K-i]+space[i]-alpha[K+1-i])**2 for i in range(1,K)) )

for i in range(1,K):
    model.add( space[i]-knot[i+1]+knot[i] == 0 )
