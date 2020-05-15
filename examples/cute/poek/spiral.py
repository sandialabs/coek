# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#Taken from:
##
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   A nonlinear minmax problem.
#   Source:
#   E. Polak, J.E. Higgins and D. Mayne,
#   "A barrier function for minmax problems",
#   Mathematical Programming, vol.54(2), pp. 155-176, 1992.
#   SIF input: Ph. Toint, April 1992.
#   classification LOR2-AN-3-2
#   Solution

import poek as pk
sqrt = pk.sqrt
cos = pk.cos
sin = pk.sin

model = pk.model()

x1 = model.variable(value=1.41831)
x2 = model.variable(value=-4.79462)
u = model.variable(value = 1.0)

model.add( u )

model.add( 0 <= -(x1-(sqrt((x1*x1)+(x2*x2)))*cos((sqrt((x1*x1)+(x2*x2))))) * (x1\
    -(sqrt((x1*x1)+(x2*x2)))*cos((sqrt((x1*x1)+(x2*x2))))) - 0.0050*x1 * x1 - \
    0.0050*x2 * x2 + u )

model.add( 0 <= -(x2-(sqrt((x1*x1)+(x2*x2)))*sin((sqrt((x1*x1)+(x2*x2))))) * \
    (x2-(sqrt((x1*x1)+(x2*x2)))*sin((sqrt((x1*x1)+(x2*x2))))) - 0.0050*x1 * x1 - \
    0.0050*x2 * x2 + u )
