# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#
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

#   Source: another very simple box-constrained quadratic

#   classification QBR2-AN-2-0

import poek as pk


model = pk.model()

x1 = model.add_variable(value=10)
x2 = model.add_variable(lb=0, ub=0.5, value=1)

model.add_objective(x2 + (-x1 + x2) ** 2 + (x1 + x2) ** 2)
