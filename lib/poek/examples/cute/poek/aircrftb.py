# TODO
# Formulated in Pyomo by Gabe Hackebeil
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

#   Source: problem 9 in
#   J.J. More',"A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer Seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SXR2-RN-8-0

import poek as pk


model = pk.model()

data = pk.util.load_data('aircrftb.json')
n, m, A = data.unpack('n', 'm', 'A')

N = list(range(1,n+1))
M = list(range(1,m+1))

X = model.add_variable(index=N)
X[1].value = 0
X[2].value = 0
X[3].value = 0
X[4].value = 0
X[5].value = 0
X[6].value = -0.05
X[7].value = 0.1
X[8].value = 0.0

AX = {i: sum(A[i,j]*X[j] for j in N) for i in M}

P1 = -0.727*X[2]*X[3]+8.39*X[3]*X[4]-684.4*X[4]*X[5]+63.5*X[4]*X[2]
P2 = 0.949*X[1]*X[3]+0.173*X[1]*X[5]
P3 = -0.716*X[1]*X[2]-1.578*X[1]*X[4]+1.132*X[4]*X[2]
P4 = -1*X[1]*X[5]
P5 = X[1]*X[4]

model.add_objective( (AX[1]+P1)**2 \
          +(AX[2]+P2)**2 \
          +(AX[3]+P3)**2 \
          +(AX[4]+P4)**2 \
          +(AX[5]+P5)**2 )

model.add_constraint( X[6] == -0.05 )

model.add_constraint( X[7] == 0.1 )

model.add_constraint( X[8] == 0.0 )
