# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes. Taken from:

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
#   Ph. Toint, private communication, 1997.

#   SDIF input: Ph. Toint, May 1997.

#   classification SUR2-AN-2-0

import poek as pk
sin = pk.sin


model = pk.model()
zeta = 20.0

x = model.variable(value=-506.0)
y = model.variable(value=-506.2)

model.add( 0.05*(x**2+y**2)+(sin(zeta*x)*sin(zeta*y))**2 )
