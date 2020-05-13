# TODO
# Formulated in pyomo by Logan Barnes.

import poek as pk
log = pk.log


model = pk.model()

N = list(range(1,11))
c = {
  1:-6.089,
  2:-17.164,
  3:-34.054,
  4:-5.914,
  5:-24.721,
  6:-14.986,
  7:-24.100,
  8:-10.708,
  9:-26.662,
 10:-22.179
}

x = model.variable(index=N, lb=.000001, value=0.1)

model.add( sum(x[j]*(c[j] + log(x[j]/sum(x[k] for k in N))) for j in N) )

model.add( x[1] + 2.0*x[2] + 2.0*x[3] + x[6] + x[10] == 2.0 )

model.add( x[4] + 2.0*x[5] + x[6] + x[7] == 1.0 )

model.add( x[3] + x[7] + x[8] + 2.0*x[9] + x[10] == 1.0 )
