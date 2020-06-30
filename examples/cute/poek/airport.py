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
#   Contribution from a LANCELOT user.

#   SIF input : Rodrigo de Barros Nabholz & Maria Aparecida Diniz Ehrhardt
#               November 1994, DMA - IMECC- UNICAMP
#   Adaptation for CUTE: Ph. Toint, November 1994.

#   classification SQR2-MN-84-42

import poek as pk


model = pk.model()

N = 42

# LOAD DATA
r = Param(list(range(1,N))
cx = Param(list(range(1,N))
cy = Param(list(range(1,N))

x = model.add_variable(index=range(1,N+1), lb=-10, ub=10)
y = model.add_variable(index=range(1,N+1), lb=-10, ub=10)

model.add_objective( sum((x[i] - x[j])**2 + (y[i] - y[j])**2 
           for i in range(1,N) for j in range(i+1,N+1)) )

for i in x:
    model.add_constraint( (x[i]-cx[i])**2 + (y[i]-cy[i])**2 - r[i] <= 0 )
