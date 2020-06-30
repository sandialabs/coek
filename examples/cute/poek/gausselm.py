# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
# Taken from:

# AMPL Model by Hande Y. Benson
#
# Copyright (C) 2001 Princeton University
# All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice appear in all
# copies and that the copyright notice and this
# permission notice appear in all supporting documentation.                     

#   classification LOR2-AN-V-V

import poek as pk


model = pk.model()

n=16

x = model.add_variable(index=[(k,i,j) for k in range(1,n+1) for i in range(k,n+1) for j in range(k,n+1)])
for k,i,j in x:
    x[k,i,j].value = 1 if i==j else 0.01

model.add_objective( -x[n,n,n] )

for k in range(1,n):
    for i in range(k+1,n+1):
        for j in range(k+1,n+1):
            model.add_constraint( x[k,i,k]*x[k,k,j]/x[k,k,k] + x[k+1,i,j] - x[k,i,j] == 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        model.add_constraint( x[k,i,k] - x[k,k,k] <= 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        model.add_constraint( x[k,k,i] - x[k,k,k] <= 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        for j in range(k+1,n+1):
            model.add_constraint( x[k,i,j] - x[k,k,k] <= 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        model.add_constraint( x[k,i,k] + x[k,k,k] >= 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        model.add_constraint( x[k,k,i] + x[k,k,k] >= 0 )

for k in range(2,n):
    for i in range(k+1,n+1):
        for j in range(k+1,n+1):
            model.add_constraint( x[k,i,j] + x[k,k,k] >= 0 )

for i in range(1,n+1):
    for j in range(1,n+1):
        model.add_constraint( pk.inequality(-1.0, x[1,i,j], 1.0) )

for k in range(1,n+1):
    model.add_constraint( x[k,k,k] >= 0.0 )

x[1,1,1].value = 1.0
x[1,1,1].fixed = True
