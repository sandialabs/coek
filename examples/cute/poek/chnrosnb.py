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

#   Source:
#   Ph.L. Toint,
#   "Some numerical results using a sparse matrix updating formula in
#   unconstrained optimization",
#   Mathematics of Computation, vol. 32(114), pp. 839-852, 1978.

#   See also Buckley#46 (n = 25) (p. 45).
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

n = 50
S = list(range(1,n+1))

# LOAD DATA
alph = Param(list(range(1,n))

x = model.variable(index=S, value=-1.0)

model.add( sum((x[i-1]-x[i]**2)**2*16*alph[i]**2 +(x[i]-1.0)**2 for i in range(2,n+1)) )
