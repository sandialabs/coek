# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
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

#   Source: K. Oettershagen
#   "Ein superlinear konvergenter algorithmus zur losung
#    semi-infiniter optimierungsproblem",
#    Ph.D thesis, Bonn University, 1982

#   SIF input: Nick Gould, February, 1994.

#   classification LLR2-AN-3-V

import poek as pk
from math import exp
#exp = pk.exp

model = pk.model()

M = 500
lower = 0.0
upper = 2.0
diff = upper-lower
h = diff/M
S = [1,2]
OP = list(range(0,M+1))

u = model.add_variable()
x = model.add_variable(index=S)

model.add_objective( u )

for i in OP:
    model.add_constraint( u - (i*h + lower)*x[1] - exp(i*h + lower)*x[2] - (i*h + lower)**2 >= 0 )

for i in OP:
    model.add_constraint( u + (i*h + lower)*x[1] + exp(i*h + lower)*x[2] + (i*h + lower)**2 >= 0 )
