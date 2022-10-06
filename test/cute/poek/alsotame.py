# TODO
# Formulated in Pyomo by Gabe Hackebeil
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
#   A.R. Conn, N. Gould and Ph.L. Toint,
#   "The LANCELOT User's Manual",
#   Dept of Maths, FUNDP, 1991.

#   SIF input:  Ph. Toint, Jan 1991.

#   classification OOR2-AN-2-1

import poek as pk

sin = pk.sin
exp = pk.exp


model = pk.model()

x = model.add_variable()
y = model.add_variable()


model.add_objective(exp(x - 2 * y))

model.add_constraint(sin(-x + y - 1) == 0)
model.add_constraint(pk.inequality(-2, x, 2))
model.add_constraint(pk.inequality(-1.5, y, 1.5))
