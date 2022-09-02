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

#   Source:
#   V.F. Demynanov and V.N. Malozemov
#   "Introduction to Minimax"
#   Wiley, 1974

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LQR2-AN-3-3

import poek as pk


model = pk.model()

S = [1,2]
x = model.add_variable(index=S, value=1)
u = model.add_variable()

model.add_objective( u )

model.add_constraint( -u+5*x[1]+x[2] <= 0 )
model.add_constraint( -u-5*x[1]+x[2] <= 0 )
model.add_constraint( -u+4*x[2]+x[1]**2+x[2]**2 <= 0 )
