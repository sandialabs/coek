# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
# Taken from:
#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   A mistake in writing Hock and Schittkowski problem 108.
#   Source:
#   Ph. Toint.
#   classification QQR2-AY-9-13
#   SIF input: Ph. Toint, Apr 1990.
#   Number of variables
#   Parameters
#   Objective Function
#   Constraint function
#   Solution

import poek as pk


model = pk.model()

n = 9
x1 = model.variable(value=1.0)
x2 = model.variable(value=1.0)
x3 = model.variable(value=1.0)
x4 = model.variable(value=1.0)
x5 = model.variable(value=1.0)
x6 = model.variable(value=1.0)
x7 = model.variable(value=1.0)
x8 = model.variable(value=1.0)
x9 = model.variable(lb=0, value=1.0)

model.add( -0.5*x1*x4 + 0.5*x2*x3 - \
    0.5*x3*x9 + 0.5*x5*x9 - 0.5*x5*x8 + 0.5*x6*x7 )

model.add( x3*x3 + x4*x4 - 1.0 <= 0 )
model.add( x5*x5 + x6*x6 - 1.0 <= 0 )
model.add( x9*x9 - 1.0 <= 0)
model.add( x1*x1 + (x2-x9)*(x2-x9) - 1.0 <= 0 )
model.add( (x1-x5)*(x1-x5) + (x2-x6)*(x2-x6) - 1.0 <= 0 )
model.add( (x1-x7)*(x1-x7) + (x2-x8)*(x2-x8) - 1.0 <= 0 )
model.add( (x3-x5)*(x3-x5) + (x4-x6)*(x4-x6) - 1.0 <= 0 )
model.add( (x3-x7)*(x3-x7) + (x4-x8)*(x4-x8) - 1.0 <= 0 )
model.add( x7*x7 + x8*x9 - 1.0 <= 0 )
model.add( 0 <= x8*x9 )
model.add( 0 <= x5*x8 - x6*x7 )
model.add( 0 <= x1*x4 - x2*x3 )
model.add( -x5*x9 <= 0 )
