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
#   R.  Wommersley and R. Fletcher,
#   "An algorithm for composite nonsmooth optimization problems"
#   JOTA, vol.48, pp.493-523, 1986

#   SIF input: Ph. Toint, April 1992.

#   classification LOR2-AN-3-3

import poek as pk
exp = pk.exp


model = pk.model()

S = [1,2]
x = model.variable(index=S, value=2.0)
u = model.variable(value=1.0)

model.add( u )

model.add( u-x[1]**2-x[2]**4 >= 0 )

model.add( u-(2.0-x[1])**2-(2.0-x[2])**2 >= 0 )

model.add( u-2*exp(x[2]-x[1]) >= 0 )
