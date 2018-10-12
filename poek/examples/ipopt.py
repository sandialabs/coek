from poek import *

x1 = variable('x1', lb=-1, ub=1, initialize=0.5)
x2 = variable('x2', initialize=1.5)

m = model()
m.add( -(x2-2)**2 )
m.add( x1**2 + x2 - 1 == 0 )
m.show(1)
m.build()
m.show(1)

solver = Solver('ipopt')
solver.solve(m)
print("HERE")

m.show(1)
