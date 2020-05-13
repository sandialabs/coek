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

#   Source:
#   E. Polak, D.H. Mayne and J.E. Higgins,
#   "Superlinearly convergent algorithm for min-max problems"
#   JOTA 69, pp. 407-439, 1991.

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LOR2-AN-3-2

import poek as pk
exp = pk.exp


model = pk.model()

N = [1,2]

x = model.variable(index=N)
x[1].value = 50
x[2].value = 0.05
u = model.variable()

model.add( u )

model.add( -u + exp(0.001*x[1]**2 + (x[2] - 1)**2) <= 0 )

model.add( -u + exp(0.001*x[1]**2 + (x[2] + 1)**2) <= 0 )
