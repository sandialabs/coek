# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()
N = list(range(1,6))
M = list(range(1,22))

# LOAD DATA
l = Param(N)
u = Param(N)
a = Param(M)

x_init={}
x_init[1] = 2.52
x_init[2] = 2.0
x_init[3] = 37.5
x_init[4] = 9.25
x_init[5] = 6.8

x = model.variable(index=N)
for i in N:
    x[i].value = x_init[i]
    x[i].lb = l[i]
    x[i].ub = u[i]

model.add( -a[1] - a[2]*x[1] - a[3]*x[1]*x[2]\
        - a[4]*x[1]*x[3] - a[5]*x[1]*x[4]\
        - a[6]*x[1]*x[5] )

model.add( pk.inequality(0, a[7]*x[1] + a[8]*x[1]*x[2]\
             + a[9]*x[1]*x[3] + a[10]*x[1]*x[4]\
             + a[11]*x[1]*x[5], 294000.0) )

model.add( pk.inequality(0, a[12]*x[1] + a[13]*x[1]*x[2]\
             + a[14]*x[1]*x[3] + a[15]*x[1]*x[4]\
             + a[16]*x[1]*x[5], 294000.0) )

model.add( pk.inequality(0, a[17]*x[1] + a[18]*x[1]*x[2]\
             + a[19]*x[1]*x[3] + a[20]*x[1]*x[4]\
             + a[21]*x[1]*x[5], 277200.0) )
