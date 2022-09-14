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

#   Source: problem MINLEN in
#   Vasko and Stott
#   "Optimizing continuous caster product dimensions:
#    an example of a nonlinear design problem in the steel industry"
#   SIAM Review, Vol 37 No, 1 pp.82-84, 1995

#   SIF input: A.R. Conn April 1995

#   classification LOR2-RN-5-4

import poek as pk


model = pk.model()

mintph = 45
minthick = 7
minarea = 200
maxarea = 250
maxaspr = 2
k = 1

thick = model.add_variable(lb=minthick, value=0.5)
wid = model.add_variable(lb=0.0, value=0.5)
Len = model.add_variable(lb=0.0, value=0.5)
tph = model.add_variable(lb=mintph, value=0.5)
ipm = model.add_variable(lb=0.0, value=0.5)

model.add_objective(Len)

model.add_constraint(117.370892 * tph / (wid * thick) - ipm == 0.0)
model.add_constraint(thick**2 * ipm / 48.0 - Len == 0.0)
model.add_constraint(wid / thick <= maxaspr)
model.add_constraint(pk.inequality(0.0, thick * wid - minarea, maxarea - minarea))
