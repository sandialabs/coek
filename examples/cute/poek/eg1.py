# TODO
# Formulated in Pyomo by Carl D. Laird and Daniel P. Word

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
#   A.R. Conn, N. Gould and Ph.L. Toint,
#   "LANCELOT, A Fortran Package for Large-Scale Nonlinear Optimization
#   (Release A)"
#   Springer Verlag, 1992.

#   SIF input: N. Gould and Ph. Toint, June 1994.

#   classification OBR2-AY-3-0

import poek as pk
sin = pk.sin


model = pk.model()

x1 = model.variable()
x2 = model.variable(lb=-1, ub=1)
x3 = model.variable(lb=1, ub=2)

model.add( x1**2 + (x2*x3)**4 + x1*x3 + x2*sin(x1+x3) + x2 )
