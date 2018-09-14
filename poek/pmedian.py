from poek.expr import *

import random
random.seed(1000)

N = 640
M = 640
P = 1
#N = 7
#M = 5
#P = 1

x = {}
for n in range(N):
    for m in range(M):
        x[n,m] = variable()

y = {}
for n in range(N):
    y[n] = variable()

d = {}
for n in range(N):
    for m in range(M):
        d[n,m] = random.uniform(0.0,1.0)

pmedian = model()

# objective
pmedian.add( quicksum(d[n,m]*x[n,m] for n in range(N) for m in range(M)) )

# single_x
for m in range(M):
    pmedian.add( quicksum(x[n,m] for n in range(N)) == 1 )

# bound_y
for n in range(N):
    for m in range(M):
        pmedian.add( x[n,m] - y[n] <= 0 )

# num_facilities
pmedian.add( quicksum(y[n] for n in range(N)) == P )



#pmedian.show()
#pmedian.build()
