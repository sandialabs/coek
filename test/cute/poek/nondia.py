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
#   D. Shanno,
#   " On Variable Metric Methods for Sparse Hessians II: the New
#   Method",
#   MIS Tech report 27, University of Arizona (Tucson, UK), 1978.

#   See also Buckley #37 (p. 76) and Toint #15.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10
S = list(range(1, N + 1))
SS = list(range(2, N + 1))

x = model.add_variable(index=S, value=-1.0)

model.add_objective((x[1] - 1) ** 2 + sum(100 * (x[1] - x[i - 1] ** 2) ** 2 for i in SS))
