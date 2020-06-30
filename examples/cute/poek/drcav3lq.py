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

#   Source:
#   P.N. Brown and Y. Saad,
#   "Hybrid Krylov Methods for Nonlinear Systems of Equations",
#   SIAM J. Sci. Stat. Comput. 11, pp. 450-481, 1990.
#   The boundary conditions have been set according to
#   I.E. Kaporin and O. Axelsson,
#   "On a class of nonlinear equation solvers based on the residual norm
#   reduction over a sequence of affine subspaces",
#   SIAM J, Sci. Comput. 16(1), 1995.

#   SIF input: Ph. Toint, Jan 1995.

#   classification NQR2-MY-V-V

import poek as pk


model = pk.model()

M = 100
H = 1.0/(M+2.0)
RE = 4500.0

S1 = list(range(-1,M+3))
S2 = list(range(1,M+1))

y = model.add_variable(index=list((i,j) for i in S1 for j in S1), value=0.0)

model.add_objective( pk.expression(0) )

for i in S2:
    for j in S2:
        model.add_constraint( (20*y[i,j]-8*y[i-1,j]-8*y[i+1,j]\
            -8*y[i,j-1]-8*y[i,j+1]+2*y[i-1,j+1]+2*y[i+1,j-1]+2*y[i-1,j-1]+2*y[i+1,j+1] +\
            y[i-2,j] + y[i+2,j] + y[i,j-2] + y[i,j+2] + (RE/4.0)*(y[i,j+1]-y[i,j-1])\
            *(y[i-2,j]+y[i-1,j-1]+y[i-1,j+1]-4*y[i-1,j]-4*y[i+1,j]-y[i+1,j-1] \
            -y[i+1,j+1] - y[i+2,j]) - (RE/4.0)*(y[i+1,j]-y[i-1,j])*\
            (y[i,j-2]+y[i-1,j-1]+y[i+1,j-1]-4*y[i,j-1]-4*y[i,j+1]-y[i-1,j+1]-y[i+1,j+1] - y[i,j+2])) == 0 )

for j in S1:
    y[-1,j].value = 0.0
    y[-1,j].fixed = True

    y[0,j].value = 0.0
    y[0,j].fixed = True

for i in S2:
    y[i,-1].value = 0.0
    y[i,-1].fixed = True

    y[i,0].value = 0.0
    y[i,0].fixed = True

    y[i,M+1].value = 0.0
    y[i,M+1].fixed = True

    y[i,M+2].value = 0.0
    y[i,M+2].fixed = True

for j in S1:
    y[M+1,j].value = -H/2.0
    y[M+1,j].fixed = True

    y[M+2,j].value = H/2.0
    y[M+2,j].fixed = True
