# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
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
#   Ph. Toint, private communication,

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AY-2-0

import poek as pk

sin = pk.sin
cos = pk.cos
sqrt = pk.sqrt

model = pk.model()

hlength = 30
cslope = 100

x1 = model.add_variable(value=-5)
x2 = model.add_variable(value=-7)


model.add_objective(
    sin(7 * x1) ** 2 * cos(7 * x2) ** 2 * hlength
    + cslope * sqrt(0.01 + (x1 - x2) ** 2)
    + cslope * sqrt(0.01 + x1**2)
)
