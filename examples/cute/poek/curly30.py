# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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

#   Source: Nick Gould

#   SIF input: Nick Gould, September 1997.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10000
K = 30

x = model.variable(index=range(1,N+1), value=0.0001/(N+1))

Q = {i: sum(x[j] for j in range(i,i+K+1)) if i<=N-K else\
        sum(x[j] for j in range(i,N+1)) \
     for i in range(1,N+1)}

model.add( sum(Q[i]*(Q[i]*(Q[i]**2-20)-0.1) for i in range(1,N+1)) )
