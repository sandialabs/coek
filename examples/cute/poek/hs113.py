# TODO
# Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,11))

x = model.variable(index=N)
x[1].value = 2.0
x[2].value = 3.0
x[3].value = 5.0
x[4].value = 5.0
x[5].value = 1.0
x[6].value = 2.0
x[7].value = 7.0
x[8].value = 3.0
x[9].value = 6.0
x[10].value=10.0

model.add( x[1]**2+x[2]**2+x[1]*x[2]-14*x[1]-16*x[2]+(x[3]-10)**2+4*(x[4]-5)**2 +(x[5]-3)**2+2*(x[6]-1)**2+5*x[7]**2+7*(x[8]-11)**2+2*(x[9]-10)**2+(x[10]-7)**2+45 )

model.add( 105-4*x[1]-5*x[2]+3*x[7]-9*x[8]>=0 )
model.add( -10*x[1]+8*x[2]+17*x[7]-2*x[8]>=0 )
model.add( 8*x[1]-2*x[2]-5*x[9]+2*x[10]+12>=0 )
model.add( -3*(x[1]-2)**2-4*(x[2]-3)**2-2*x[3]**2+7*x[4]+120>=0 )
model.add( -5*x[1]**2-8*x[2]-(x[3]-6)**2+2*x[4]+40>=0 )
model.add( -.5*(x[1]-8)**2-2*(x[2]-4)**2-3*x[5]**2+x[6]+30>=0 )
model.add( -x[1]**2-2*(x[2]-2)**2+2*x[1]*x[2]-14*x[5]+6*x[6]>=0 )
model.add( 3*x[1]-6*x[2]-12*(x[9]-8)**2+7*x[10]>=0 )
