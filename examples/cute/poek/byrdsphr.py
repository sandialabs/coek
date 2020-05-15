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
#   R. Byrd,
#   Private communication, Chicago, 1992.

#   SIF input: Ph. Toint, November 1992.

#   classification LQR2-AN-3-2

import poek as pk


model = pk.model()

S = [1,2,3]
x = model.variable(index=S)
x[1].value = 5.0
x[2].value = 0.0001
x[3].value = -0.0001

model.add( -x[1]-x[2]-x[3] )

model.add( -9.0+x[1]**2+x[2]**2+x[3]**2 == 0 )

model.add( -9.0+(x[1]-1.0)**2+x[2]**2+x[3]**2 == 0 )
