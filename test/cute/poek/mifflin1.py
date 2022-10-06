# Adapted from Pyomo model by William E. Hart
#  Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#
#  Taken from:

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
#   M.M. Makela,
#   "Nonsmooth optimization",
#   Ph.D. thesis, Jyvaskyla University, 1990

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LQR2-AN-3-2

import poek as pk


model = pk.model()

N = [1, 2]

x = model.add_variable(index=N)
x[1].value = 0.8
x[2].value = 0.6
u = model.add_variable()

model.add_objective(u)

model.add_constraint(-u - x[1] - 1.0 + x[1] ** 2 + x[2] ** 2 <= 0)
model.add_constraint(-u - x[1] <= 0)
