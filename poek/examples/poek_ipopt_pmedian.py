print("START")
import sys
sys.stdout.flush()
import math
from poek import *

print("Imported POEK")
sys.stdout.flush()
import poek_pmedian
print("Created PMEDIAN Model")
sys.stdout.flush()

solver = Solver('ipopt')
sys.stdout.flush()
#sys.exit(0)

print("STARTING SOLVE")
sys.stdout.flush()
solver.solve(poek_pmedian.pmedian)
print("FINISHED SOLVE")
sys.stdout.flush()

j=0
for i in poek_pmedian.x:
    val = poek_pmedian.x[i].value
    if math.fabs(val-1) < 1e-7:
        print((j,i,val))
    j += 1
for i in poek_pmedian.y:
    val = poek_pmedian.y[i].value
    if math.fabs(val-1) < 1e-7:
        print((j,i,val))
    j += 1
