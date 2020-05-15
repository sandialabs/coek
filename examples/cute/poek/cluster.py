# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
# Taken from:

#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   Source:  problem 207 in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.
#   SIF input: Ph. Toint, Dec 1989.
#   classification NOR2-AN-2-2
#   Solution

import poek as pk
sin = pk.sin
cos = pk.cos


model = pk.model()

x1 = model.variable()
x2 = model.variable()

model.add( pk.expression(0) )

model.add( ((x1-x2*x2) * (x1-sin(x2))) == 0 )

model.add( (((cos(x2))-x1) * (x2-cos(x1))) == 0 )
