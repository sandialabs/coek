# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk
exp = pk.exp


model = pk.model()

N = list(range(1,3))

x = model.variable(index=N)
x[1].ub = 75.0
x[2].ub = 65.0

x[1].value = 90.0
x[2].value = 10.0

model.add( -75.196 + 3.8112*x[1] + 0.0020567*x[1]**3 - 1.0345e-5*x[1]**4\
  + 6.8306*x[2] - 0.030234*x[1]*x[2] + 1.28134e-3*x[2]*x[1]**2\
  + 2.266e-7*x[1]**4*x[2] - 0.25645*x[2]**2 + 0.0034604*x[2]**3 - 1.3514e-5*x[2]**4\
  + 28.106/(x[2] + 1.0) + 5.2375e-6*x[1]**2*x[2]**2 + 6.3e-8*x[1]**3*x[2]**2\
  - 7e-10*x[1]**3*x[2]**3 - 3.405e-4*x[1]*x[2]**2 + 1.6638e-6*x[1]*x[2]**3\
  + 2.8673*exp(0.0005*x[1]*x[2]) - 3.5256e-5*x[1]**3*x[2]\
# the last term appears in CUTE but not in H&S
  -0.12694*x[1]**2 )
  
model.add( x[1]*x[2] >= 700 )
model.add( x[2] - (x[1]**2)/125.0 >= 0 )
model.add( (x[2] - 50)**2 - 5*(x[1] - 55) >= 0 )
