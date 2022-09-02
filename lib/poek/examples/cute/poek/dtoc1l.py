# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
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

#   classification OLR2-AN-V-V

import poek as pk


model = pk.model()

n = 1000
nx = 5
ny = 10

S1 = list(range(1,ny+1))
S2 = list(range(1,nx+1))
S3 = list(range(1,n))
S4 = list(range(1,n+1))
S5 = list(range(2,ny))

b = {(i,j): (i-j)/(nx+ny) for i in S1 for j in S2}

x = model.add_variable(index=list((i,j) for i in S3 for j in S2))
y = model.add_variable(index=list((i,j) for i in S4 for j in S1))

sum1 = sum((x[t,i] + 0.5)**4 for t in S3 for i in S2)
sum2 = sum((y[t,i] + 0.25)**4 for t in S4 for i in S1)
model.add_objective( sum1 + sum2 )

for t in S3:
    sc1 = sum(b[1,i]*x[t,i] for i in S2)
    model.add_constraint( 0.5*y[t,1] + 0.25*y[t,2] - y[t+1,1] + sc1 == 0 )

for t in S3:
    for j in S5:
        sc2 = sum(b[j,i]*x[t,i] for i in S2)
        model.add_constraint( -y[t+1,j] + 0.5*y[t,j] - 0.25*y[t,j-1] + 0.25*y[t,j+1] + sc2 == 0 )

for t in S3:
    sc3 = sum(b[ny,i]*x[t,i] for i in S2)
    model.add_constraint( 0.5*y[t,ny] - 0.25*y[t,ny-1] - y[t+1,ny] + sc3 == 0 )

for idx in S1:
    y[1,idx].value = 0.0
    y[1,idx].fixed = True
