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
#   C. Charalambous and A.R. Conn,
#   "An efficient method to solve the minmax problem directly",
#   SINUM 15, pp. 162-187, 1978.

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LOR2-AY-3-3

import poek as pk
exp = pk.exp


model = pk.model()

S = [1,2]

x = model.add_variable(index=S)
x[1].value =  1.0
x[2].value = -0.1
u = model.add_variable()

model.add_objective( u )

model.add_constraint( -u+x[1]**2+x[2]**4 <= 0 )

model.add_constraint( -u+(2-x[1])**2+(2-x[2])**2 <= 0 )

model.add_constraint( -u+2*exp(x[2]-x[1]) <= 0 )
