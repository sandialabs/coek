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

#   Source: problem 2 in
#   P.T. Boggs and J.W. Tolle,
#   "A strategy for global convergence in a sequential
#    quadratic programming algorithm",
#   SINUM 26(3), pp. 600-623, 1989.

#   SIF input: Ph. Toint, June 1993.

#   classification QQR2-AY-3-1

import poek as pk


model = pk.model()

S = [1,2,3]
x = model.add_variable(index=S, value=10.0)

model.add_objective( (x[1]-1.0)**2 + (x[1]-x[2])**2 + (x[2]-x[3])**4 )

model.add_constraint( x[1]*(1.0+x[2]**2)+x[3]**4 == 8.2426407 )
