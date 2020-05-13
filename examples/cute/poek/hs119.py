# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from CUTE models
#

import poek as pk


model = pk.model()

N = list(range(1,17))
M = list(range(1,9))

# LOAD DATA
a = Param(N,N,default=0)
b = Param(M,N,default=0)
c = Param(M,default=0)

x = model.variable(index=N, lb=0, ub=5, value=10.0)


model.add( sum(a[i,j]*(x[i]**2+x[i]+1)*(x[j]**2+x[j]+1) for i in N for j in N) )

for i in M:
    model.add( sum(b[i,j]*x[j] for j in N) == c[i] )
