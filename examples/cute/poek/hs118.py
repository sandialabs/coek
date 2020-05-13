# TODO
# Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()
N = list(range(1,16))
M = list(range(1,5))
L = list(range(0,5))
P = list(range(4,16))

l = {}
l[1] =  8.0
l[2] = 43.0
l[3] =  3.0
for j in P:
    l[j] = 0.0

u = {}
u[1] = 21.0
u[2] = 57.0
u[3] = 16.0
for k in M:
    u[3*k+1] = 90.0
    u[3*k+2] = 120.0
    u[3*k+3] = 60.0

x = model.variable(index=N)

for i in N:
    x[i].lb = l[i]
    x[i].ub = u[i]

x[1].value = 20
x[2].value = 55
x[3].value = 15
x[4].value = 20
x[5].value = 60
x[6].value = 20
x[7].value = 20
x[8].value = 60
x[9].value = 20
x[10].value = 20
x[11].value = 60
x[12].value = 20
x[13].value = 20
x[14].value = 60
x[15].value = 20

model.add( sum((2.3*x[3*k+1] + 0.0001*x[3*k+1]**2 + 1.7*x[3*k+2] + 0.0001*x[3*k+2]**2 + 2.2*x[3*k+3] + 0.00015*x[3*k+3]**2) for k in L) )

for j in range(1,5):
    model.add( pk.inequality(0, x[3*j+1] - x[3*j-2] + 7, 13) )
for j in range(1,5):
    model.add( pk.inequality(0, x[3*j+2] - x[3*j-1] + 7, 14) )
for j in range(1,5):
    model.add( pk.inequality(0, x[3*j+3] - x[3*j] + 7, 13) )

model.add( x[1] + x[2] + x[3] >= 60 )
model.add( x[4] + x[5] + x[6] >= 50 )
model.add( x[7] + x[8] + x[9] >= 70 )
model.add( x[10] + x[11] + x[12] >= 85 )
model.add( x[13] + x[14] + x[15] >= 100 )
