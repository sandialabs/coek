# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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
#   Ph.L. Toint,
#   "Some numerical results using a sparse matrix updating formula in
#   unconstrained optimization",
#   Mathematics of Computation, vol. 32(114), pp. 839-852, 1978.

#   See also Buckley#84
#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-V-0

import poek as pk


model = pk.model()

N = 1000

x = model.variable(index=range(1,N+1), value=1.0)

model.add( (abs(-2*x[2]+1+(3-2*x[1])*x[1]))**(7/3.0) +\
    sum((abs(1-x[i-1]-2*x[i+1]+(3-2*x[i])*x[i]))**(7/3.0) for i in range(2,N))+\
    (abs(-x[N-1]+1 +(3-2*x[N])*x[N]))**(7/3.0) +\
    sum((abs(x[i]+x[i+N//2]))**(7/3.0) for i in range(1,N//2+1)) )
