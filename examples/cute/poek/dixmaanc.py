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

#   classification LQI2-RN-157-134

import poek as pk


model = pk.model()

M =1000
N =3*M

alpha = 1.0
beta = 0.125
gamma = 0.125
delta = 0.125

K = {i:0 for i in [1,2,3,4]}

x = model.variable(index=range(1,N+1), value=2.0)

model.add( 1.0 + sum(alpha*x[i]**2*(i/N)**K[1] for i in range(1,N+1))+\
    sum(beta*x[i]**2*(x[i+1]+x[i+1]**2)**2*(i/N)**K[2] for i in range(1,N))+\
    sum(gamma*x[i]**2*x[i+M]**4*(i/N)**K[3] for i in range(1,2*M+1)) +\
    sum(delta*x[i]*x[i+2*M]*(i/N)**K[4] for i in range(1,M+1)) )
