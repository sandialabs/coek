# TODO
# Formulated in Pyomo by Juan Lopez
# Taken from:

#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   A small gas network problem.
#   SIF input: Sybille Schachler, Oxford, August 1992.
#              minor correction by Ph. Shott, Jan 1995.

import poek as pk


model = pk.model()

n = 7
m = 4
demand = -1000.0
pmax1 = 914.73
pmax2 = 904.73
k = -0.597053452

p1 = model.add_variable(ub=914.73,value=965)
p2 = model.add_variable(value=965)
p3 = model.add_variable(ub=904.73,value=965)
p4 = model.add_variable(value=965)
p5 = model.add_variable(ub=904.73,value=965)
p6 = model.add_variable(value=965)
p7 = model.add_variable(ub=914.73,value=965)
q1 = model.add_variable(value=100.0)
f1 = model.add_variable(value=1000.0)
q2 = model.add_variable(value=100.0)
f2 = model.add_variable(value=1000.0)
q3 = model.add_variable(value=-100.0)
f3 = model.add_variable(value=1000.0)
q4 = model.add_variable(value=-100.0)
f4 = model.add_variable(ub=400.0,value=1000.0)

model.add_objective( - p1 - p2 - p3 - p4 - p5 - p6 - p7 )

model.add_constraint( p1 * (abs(p1))-p2 * (abs (p2))  - 0.597053452*q1 * (abs(q1)) **0.8539 == 0 )
model.add_constraint( p3 * (abs(p3)) - p4 * (abs(p4))  - 0.597053452*q2 * (abs(q2))**0.8539 == 0 )
model.add_constraint( p4 * (abs(p4))  - p5 * (abs(p5))  - 0.597053452*q3 * (abs(q3)) **0.8539 == 0 )
model.add_constraint( p6 * (abs(p6))  - p7 * (abs(p7))  - 0.597053452*q4 * (abs(q4)) **0.8539 == 0 )
model.add_constraint( q1 - f3 == 0 )
model.add_constraint( -q1 + f1 == 0 )
model.add_constraint( q2 - f1 == 0 )
model.add_constraint( -q2 + q3 + 1000.0 == 0 )
model.add_constraint( -q3 - f2 == 0 )
model.add_constraint( q4 + f2 == 0 )
model.add_constraint( -q4 - f4 == 0 )
