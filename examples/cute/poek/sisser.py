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
#   F.S. Sisser,
#   "Elimination of bounds in optimization problems by transforming
#   variables",
#   Mathematical Programming 20:110-121, 1981.

#   See also Buckley#216 (p. 91)

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-2-0

import poek as pk


model = pk.model()

N = [1,2]
xinit = {1:1, 2:0.1}

x = model.variable(index=N)
x[1].value = 1
x[2].value = 0.1

model.add( 3*x[1]**4 - 2*(x[1]*x[2])**2 + 3*x[2]**4 )
