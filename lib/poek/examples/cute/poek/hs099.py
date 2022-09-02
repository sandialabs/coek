# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk
cos = pk.cos
sin = pk.sin

model = pk.model()

N = list(range(1,9))
M = list(range(1,8))
L = list(range(2,9))

a = {
  1:0,
  2:50,
  3:50,
  4:75,
  5:75,
  6:75,
  7:100,
  8:100
}
t = {
  1:0,
  2:25,
  3:50,
  4:100,
  5:150,
  6:200,
  7:290,
  8:380
}
b = 32

x = model.add_variable(index=M, lb=0, ub=1.58, value=0.5)
q = model.add_variable(index=N)
s = model.add_variable(index=N)

model.add_objective( -(sum(a[j+1]*(t[j+1]-t[j])*cos(x[j]) for j in M)**2) )

model.add_constraint( q[8] == 1.0e+5 )
model.add_constraint( s[8] == 1.0e+3 )
model.add_constraint( q[1] == 0.0 )
model.add_constraint( s[1] == 0.0 )
for i in L:
    model.add_constraint( q[i] == 0.5*(t[i]-t[i-1])**2*(a[i]*sin(x[i-1]) - b)\
     + (t[i]-t[i-1])*s[i-1] + q[i-1] )
for i in L:
    model.add_constraint( s[i] == (t[i]-t[i-1])*(a[i]*sin(x[i-1]) - b) + s[i-1] )

#model.add_constraint( r[1] == 0 )

#model.add_constraint( r[i] == a[i]*(t[i]-t[i-1])*cos(x[i-1]) + r[i-1] )
