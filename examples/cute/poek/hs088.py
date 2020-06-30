# TODO
# Formulated in Pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from CUTE models

import poek as pk


model = pk.model()

n = 2
N = list(range(1,31))
M = list(range(1,n+1))

# LOAD DATA
mu= Param(N)

A = {}
for j in N:
    A[j] = 2.0*sin(mu[j])/(mu[j] + sin(mu[j])*cos(mu[j]))

x = model.add_variable(index=M)
for i in N:
    x[i].value = 0.5 * (-1)**(i+1)

rho = model.add_variable(index=N)
for j in N:
    rho[j].value = -(exp(-mu[j]**2 * sum(x[i]**2 for i in M))\
        + sum(2.0*(-1)**(ii-1)*exp(-mu[j]**2 * sum(x[i]**2 for i in range(ii,n+1))) for ii in range(2,n+1))\
        + (-1.0)**n)/mu[j]**2

model.add_objective( sum(x[i]**2 for i in M) )

model.add_constraint( sum(mu[i]**2 * mu[j]**2 * A[i] * A[j] * rho(model,i) * rho(model,j)\
           *(sin(mu[i]+mu[j])/(mu[i]+mu[j])+sin(mu[i]-mu[j])/(mu[i]-mu[j]))\
           for i in N for j in range(i+1,30+1))\
           + sum(mu[j]**4 * A[j]**2 * rho(model,j)**2 * ((sin(2.0*mu[j])/(2.0*mu[j]) + 1.0)/2.0)\
           for j in N)\
           -sum(mu[j]**2 * A[j] * rho(model,j) * (2.0*sin(mu[j])/(mu[j]**3)\
           - 2.0*cos(mu[j])/(mu[j]**2))\
           for j in N) <= -2.0/15.0 + 0.0001
