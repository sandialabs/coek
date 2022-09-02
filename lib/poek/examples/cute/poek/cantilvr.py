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
#   an example in a talk by W.K. Zhang and C. Fleury, LLN, 1994.

#   SIF input: Ph. Toint, November 1994

#   classification LOR2-MN-5-1

import poek as pk


model = pk.model()

S = list(range(1,6))
num = {}
num[1] = 61.0
num[2] = 37.0
num[3] = 19.0
num[4] = 7.0
num[5] = 1.0

x = model.add_variable(index=S, lb=0.000001, value=1.0)

se = sum(x[i] for i in S)
model.add_objective( se*0.0624 )

see = sum(num[i]/x[i]**3 for i in S)
model.add_constraint( see - 1.0 <= 0 )
