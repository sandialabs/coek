# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,10))

x = model.add_variable(index=N, value=1.0)

model.add_objective( -.5*(x[1]*x[4]-x[2]*x[3]+x[3]*x[9]-x[5]*x[9]+x[5]*x[8]-x[6]*x[7]) )

model.add_constraint( 1-x[3]**2-x[4]**2>=0)
model.add_constraint( 1-x[5]**2-x[6]**2>=0)
model.add_constraint( 1-x[9]**2>=0)
model.add_constraint( 1-x[1]**2-(x[2]-x[9])**2>=0)
model.add_constraint( 1-(x[1]-x[5])**2-(x[2]-x[6])**2>=0)
model.add_constraint( 1-(x[1]-x[7])**2-(x[2]-x[8])**2>=0)
model.add_constraint( 1-(x[3]-x[7])**2-(x[4]-x[8])**2>=0)
model.add_constraint( 1-(x[3]-x[5])**2-(x[4]-x[6])**2>=0)
model.add_constraint( 1-x[7]**2-(x[8]-x[9])**2>=0)
model.add_constraint( x[1]*x[4]-x[2]*x[3]>=0)
model.add_constraint( x[3]*x[9]>=0)
model.add_constraint( -x[5]*x[9]>=0)
model.add_constraint( x[5]*x[8]-x[6]*x[7]>=0)
model.add_constraint( x[9]>=0)
