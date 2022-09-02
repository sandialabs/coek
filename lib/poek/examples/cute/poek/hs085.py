# TODO
# Formulated in pyomo by Gabe Hackebeil and Logan Barnes.
#
# Taken CUTE models

import poek as pk


model = pk.model()

a = {}
a[ 2] =      17.505
a[ 3] =      11.275
a[ 4] =     214.228
a[ 5] =       7.458
a[ 6] =        .961
a[ 7] =       1.612
a[ 8] =        .146
a[ 9] =     107.99
a[10] =     922.693
a[11] =     926.832
a[12] =      18.766
a[13] =    1072.163
a[14] =    8961.448
a[15] =        .063
a[16] =   71084.33
a[17] = 2802713.0

b = {}
b[2] = 1053.6667
b[ 3] =      35.03
b[ 4] =     665.585
b[ 5] =     584.463
b[ 6] =     265.916
b[ 7] =       7.046
b[ 8] =        .222
b[ 9] =     273.366
b[10] =    1286.105
b[11] =    1444.046
b[12] =     537.141
b[13] =    3247.039
b[14] =   26844.086
b[15] =        .386
b[16] =  140000.0
b[17] = 12146108

c10 = 12.3/752.3

x = model.add_variable(index=list(range(1,6)))
x[1].value = 900.0
x[2].value = 80.0
x[3].value = 115.0
x[4].value = 267.0
x[5].value = 27.0

y1  = x[2]+x[3]+41.6
c1  = .024*x[4]-4.62
y2  = 12.5/c1+12
c2  = .0003535*x[1]**2+.5311*x[1]+.08705*y2*x[1]
c3  = .052*x[1]+78+.002377*y2*x[1]
y3  = c2/c3
y4  = 19.*y3
c4  = .04782*(x[1]-y3)+.1956*(x[1]-y3)**2./x[2]+.6376*y4+1.594*y3
c5  = 100.*x[2]
c6  = x[1]-y3-y4
c7  = .95-c4/c5
y5  = c6*c7
y6  = x[1]-y5-y4-y3
c8  = (y5+y4)*.995
y7  = c8/y1
y8  = c8/3798.
c9  = y7-.0663*y7/y8-.3153
y9  = 96.82/c9+.321*y1
y10 = 1.29*y5+1.258*y4+2.29*y3+1.71*y6
y11 = 1.71*x[1]-.452*y4+.58*y3
c11 = 1.75*y2*.995*x[1]
c12 = .995*y10+1998.
y12 = c10*x[1]+c11/c12
y13 = c12-1.75*y2
y14 = 3623.+64.4*x[2]+58.4*x[3]+146312./(y9+x[5])
c13 = .995*y10+60.8*x[2]+48.*x[4]-.1121*y14-5095.
y15 = y13/c13
y16 = 148000.-331000.*y15+40.*y13-61.*y15*y13
c14 = 2324.*y10-28740000.*y2
y17 = 14130000.-1328.*y10-531.*y11+c14/c12
c15 = y13/y15-y13/.52
c16 = 1.104-.72*y15
c17 = y9+x[5]

model.add_objective(-5.843e-7*y17+1.17e-4*y14+2.358e-5*y13+1.502e-6*y16\
                           +.0321*y12+.004324*y5+1e-4*c15/c16+37.48*y2/c12+.1365)

model.add_constraint(1.5*x[2]-x[3]>=0)
model.add_constraint(y1-213.1>=0)
model.add_constraint(405.23-y1>=0)
model.add_constraint(x[1]>=704.4148)
model.add_constraint(x[1]<=906.3855)
model.add_constraint(x[2]>=68.6)
model.add_constraint(x[2]<=288.88)
model.add_constraint(x[3]>=0)
model.add_constraint(x[3]<=134.75)
model.add_constraint(x[4]>=193)
model.add_constraint(x[4]<=287.0966)
model.add_constraint(x[5]>=25)
model.add_constraint(x[5]<=84.1988)
model.add_constraint(y2-a[2]>=0)
model.add_constraint(y3-a[3]>=0)
model.add_constraint(y4-a[4]>=0)
model.add_constraint(y5-a[5]>=0)
model.add_constraint(y6-a[6]>=0)
model.add_constraint(y7-a[7]>=0)
model.add_constraint(y8-a[8]>=0)
model.add_constraint(y9-a[9]>=0)
model.add_constraint(y10-a[10]>=0)
model.add_constraint(y11-a[11]>=0)
model.add_constraint(y12-a[12]>=0)
model.add_constraint(y13-a[13]>=0)
model.add_constraint(y14-a[14]>=0)
model.add_constraint(y15-a[15]>=0)
model.add_constraint(y16-a[16]>=0)
model.add_constraint(y17-a[17]>=0)
model.add_constraint(b[2]-y2>=0)
model.add_constraint(b[3]-y3>=0)
model.add_constraint(b[4]-y4>=0)
model.add_constraint(b[5]-y5>=0)
model.add_constraint(b[6]-y6>=0)
model.add_constraint(b[7]-y7>=0)
model.add_constraint(b[8]-y8>=0)
model.add_constraint(b[9]-y9>=0)
model.add_constraint(b[10]-y10>=0)
model.add_constraint(b[11]-y11>=0)
model.add_constraint(b[12]-y12>=0)
model.add_constraint(b[13]-y13>=0)
model.add_constraint(b[14]-y14>=0)
model.add_constraint(b[15]-y15>=0)
model.add_constraint(b[16]-y16>=0)
model.add_constraint(b[17]-y17>=0)
model.add_constraint(y4-.28/.72*y5>=0)
model.add_constraint(21-3496.*y2/c12>=0)
model.add_constraint(62212./c17-110.6-y1>=0)
