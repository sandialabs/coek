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

#   Source:  problem 206 (p. 46) in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-2-0

import poek as pk
exp = pk.exp


model = pk.model()

x = model.variable(index=[1,2])
x[1].value =  0.0
x[2].value = -1.0

model.add( (0.01*x[1]-0.03)**2 -x[1]+x[2]+exp(20*(x[1]-x[2])) )
