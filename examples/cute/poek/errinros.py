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
#   An error in specifying problem CHNROSNB.
#   SIF input: Ph. Toint, Sept 1990.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 50
S = list(range(1,N+1))
alpha = Param(S)

# LOAD DATA
SS = RangeSet(2,N)

x = model.variable(index=S, value=-1.0)

sum1 = sum((x[i-1]-16*alpha[i]**2*x[i]**2)**2 for i in SS)
sum2 = sum((x[i]-1.0)**2 for i in SS)
model.add( sum1 + sum2 )
