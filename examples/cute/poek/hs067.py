# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes and Gabriel Hackebeil.

import poek as pk


model = pk.model()

N = list(range(1,9))
M = list(range(1,15))

a  = {}
a[1]=    0.0
a[2]=    0.0
a[3]=   85.0
a[4]=   90.0
a[5]=    3.0
a[6]=    0.01
a[7]=  145.0
a[8]= 5000.0
a[9]= 2000.0
a[10]=  93.0
a[11]=  95.0
a[12]=  12.0
a[13]=   4.0
a[14]= 162.0

x1 = model.variable(lb=1.0e-5, ub=2.0e+3)
x2 = model.variable(lb=1.0e-5, ub=1.6e+4)
x3 = model.variable(lb=1.0e-5, ub=1.2e+2)
y  = model.variable(index=N, value=0.0)
x1.value =  1745.0
x2.value = 12000.0
x3.value =   110.0

y[2].value = 1.6*x1.value
while (1):
    y2old = y[2].value
    y[3].value = 1.22 * y[2].value - x1.value
    y[6].value = (x2.value + y[3].value) / x1.value
    y[2].value = 0.01 * x1.value * \
                 (112.0 + \
                  13.167 * y[6].value - \
                  0.6667 * y[6].value**2)
    if abs(y2old - y[2].value) < 0.001:
        break

y[4].value = 93.0
while (1):
    y4old = y[4].value
    y[5].value = 86.35 + \
                 1.098 * y[6].value - \
                 0.038 * y[6].value**2 + \
                 0.325 * (y[4].value - 89)
    y[8].value = 3 * y[5].value - 133
    y[7].value = 35.82 - 0.222 * y[8].value
    y[4].value = 98000.0 * x3.value / \
                 (y[2].value * y[7].value + 1000 * x3.value)
    if abs(y4old - y[4].value) < 0.001:
        break

model.add( -(0.063*y[2]*y[5] - 5.04*x1 - 3.36*y[3] - 0.035*x2 - 10*x3) )

for i in range(1,8):
    model.add( y[i+1] >= a[i] )
for i in range(8,15):
    model.add( a[i] >= y[i-6] )
model.add( y[3] == 1.22*y[2] - x1)
model.add( y[6] == (x2+y[3])/x1)
model.add( y[2] == 0.01*x1*(112 + 13.167*y[6] - 0.6667*y[6]**2))
model.add( y[5] == 86.35 + 1.098*y[6] - 0.038*y[6]**2 + 0.325*(y[4]-89))
model.add( y[8] == 3.0*y[5] - 133.0)
model.add( y[7] == 35.82 - 0.222*y[8])
model.add( y[4] == 98000.0*x3/(y[2]*y[7] + 1000.0*x3))
