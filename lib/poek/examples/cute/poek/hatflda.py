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

#   Source:
#   "The OPTIMA user manual (issue No.8, p. 12)",
#   Numerical Optimization Centre, Hatfield Polytechnic (UK), 1989.

#   SIF input: Ph. Toint, May 1990.

#   classification SBR2-AN-4-0

import poek as pk


model = pk.model()

N=4
x = model.add_variable(index=[1,2,3,4], lb=0.0000001, value=0.1)

model.add_objective( (x[1]-1)**2 + sum((x[i-1]-(x[i])**0.5)**2 for i in range(2,N+1)) )
