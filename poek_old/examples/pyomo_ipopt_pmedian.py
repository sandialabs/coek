print("START")
import sys
sys.stdout.flush()
import math
from pyomo.environ import *

print("Impored PYOMO")
sys.stdout.flush()
import pyomo_pmedian
print("Created PMEDIAN Model")
sys.stdout.flush()

solver = SolverFactory('ipopt')
sys.stdout.flush()
#sys.exit(0)

print("STARTING SOLVE")
sys.stdout.flush()
solver.solve(pyomo_pmedian.pmedian, tee=True)
print("FINISHED SOLVE")
sys.stdout.flush()

#pyomo_pmedian.pmedian.display()

j=0
for i in pyomo_pmedian.pmedian.x:
    val = pyomo_pmedian.pmedian.x[i].value
    if math.fabs(val-1) < 1e-7:
        print((j,i,val))
    j += 1
for i in pyomo_pmedian.pmedian.y:
    val = pyomo_pmedian.pmedian.y[i].value
    if math.fabs(val-1) < 1e-7:
        print((j,i,val))
    j += 1
