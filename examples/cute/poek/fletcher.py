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
#   R. Fletcher
#   "Practical Methods of Optimization",
#   second edition, Wiley, 1987.

#   SIF input: Ph. Toint, March 1994.

#   classification QOR2-AN-4-4

import poek as pk


model = pk.model()

S1 = list(range(1,5))

x = model.variable(index=S1, value=1)

model.add( x[1]*x[2] )

model.add( (x[1]*x[3]+x[2]*x[4])**2/(x[1]**2+x[2]**2) - x[3]**2 - x[4]**2 + 1 == 0 )
model.add( x[1]-x[3]-1 >= 0 )
model.add( x[2]-x[4]-1 >= 0 )
model.add( x[3]-x[4] >= 0 )
model.add( x[4] >= 1 )
