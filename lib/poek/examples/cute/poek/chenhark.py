# TODO
# Formulated in Pyomo by Juan Lopez
# Taken from:

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
#   B. Chen and P. T. Harker,
#   SIMAX 14 (1993) 1168-1190

#   SDIF input: Nick Gould, November 1993.

#   classification QBR2-AN-V-V

import poek as pk


model = pk.model()

n = 1000
nfree = 500
ndegen = 200

x_p = {i:0 if i<=0 else 0 if i>nfree else 1 for i in range(-1,n+3)}

x = model.add_variable(index=range(1,n+1), value=0.5, lb=0.0)

model.add_objective( sum(0.5*(x[i+1]+x[i-1] - 2*x[i])**2 for i in range(2,n)) +\
    0.5*x[1]**2+0.5*(2*x[1] - x[2])**2 +0.5*(2*x[n] - x[n-1])**2 +\
    0.5*(x[n])**2 +sum(x[i]*(-6*x_p[i] + \
    4*x_p[i+1] + 4*x_p[i-1] -x_p[i+2] - x_p[i-2])for i in range(1,nfree+ndegen+1)) +\
    sum(x[i]*(-6*x_p[i] + 4*x_p[i+1] + 4*x_p[i-1] -\
   x_p[i+2] - x_p[i-2] + 1)for i in range(nfree+ndegen+1,n+1)) )
