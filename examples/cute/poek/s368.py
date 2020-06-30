# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#
#Taken from:

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
#   P. Wolfe,
#   "Explicit solution of an optimization problem",
#   Mathematical Programming 2, 258-260, 1972.

#   SIF input: Nick Gould, Oct 1992.

#   See also Schittkowski #368 (for N = 8)

#   classification OBR2-MN-V-0

import poek as pk


model = pk.model()

n = 10
Rx = list(range(n))

x = model.add_variable(n, lb=0.0, ub=1.0)

model.add_objective( sum( -x[i]**2*x[j]**4 for i in Rx for j in Rx ) +\
           sum(  x[i]**3*x[j]**3 for i in Rx for j in Rx ) )

model.write("s368.nl")
