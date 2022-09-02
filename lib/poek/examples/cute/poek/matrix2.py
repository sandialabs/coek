# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri

#Taken from:

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

#   classification  QOR2-AY-6-2

import poek as pk


model = pk.model()

x11 = model.add_variable(lb=0, value=1)
x12 = model.add_variable(value=1)
x22 = model.add_variable(lb=0, value=1)
y11 = model.add_variable(ub=0, value=1)
y12 = model.add_variable(value=1)
y22 = model.add_variable(ub=0, value=1)

model.add_objective( (x11-y11)**2 + 2*(x12-y12)**2 + (x22-y22)**2 )

model.add_constraint( 0 <= x11*x22 - x12**2 )
model.add_constraint( y11*y22 - y12**2 <= 0 )
