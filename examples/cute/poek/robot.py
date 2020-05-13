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

#   Source: an exercize for L. Watson course on LANCELOT in the Spring 1993.
#   B.Benhabib, R.G.Fenton and A.A.Goldberg,
#   "Analytical trajectory optimization of seven degrees of freedom redundant
#   robot",
#   Transactions of the Canadian Society for Mechanical Engineering,
#   vol.11(4), 1987, pp 197-200.

#   SIF input: Manish Sabu at Virginia Tech., Spring 1993.
#              Minor modifications by Ph. L. Toint, April 1993.

#   classification QOR2-MY-14-2

import poek as pk
cos = pk.cos
sin = pk.sin


model = pk.model()

XPOS = 4
YPOS = 4
HIGH = 2.356194
DOWN = -2.356194
R1 = list(range(1,8))
R2 = list(range(1,7))
THIN = {}
for i in R1:
    THIN[i] = 0.0


TH = model.variable(index=R1)
THI = model.variable(index=R1)

model.add( sum( (TH[i]-THI[i])**2 for i in R1 ) )

model.add( 0 == sum(cos(TH[i]) for i in R2 ) + 0.5*cos(TH[7]) - XPOS )

model.add( 0 == sum(sin(TH[i]) for i in R2) + 0.5*sin(TH[7]) - YPOS )

for i in R1:
    model.add( THI[i] == THIN[i] )
