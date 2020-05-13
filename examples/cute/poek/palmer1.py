# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
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
#   M. Palmer, Edinburgh, private communication.

#   SIF input: Nick Gould, 1990.

#   classification SBR2-RN-4-0

import json
import poek as pk


model = pk.model()

M = list(range(1,32))
with open('palmer1.json', 'r') as INPUT:
    data = json.load(INPUT)
X = {int(key):value for key,value in data["X"].items()}
Y = {int(key):value for key,value in data["Y"].items()}

A = model.variable(value=1.0)
B = model.variable(lb=.00001, value=1.0)
C = model.variable(lb=.00001, value=1.0)
D = model.variable(lb=.00001, value=1.0)

model.add( sum( (Y[m] - (A*(X[m]**2) + B / (C + (X[m]**2)/D)))**2 for m in M ) )
