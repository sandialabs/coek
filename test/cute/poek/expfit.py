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
#   A.R. Conn, N. Gould and Ph.L. Toint,
#   "LANCELOT, a Fortran package for large-scale nonlinear optimization",
#   Springer Verlag, FUNDP, 1992.

#   SIF input: Ph. Toint, Jan 1991.

#   classification SUR2-AN-2-0

import poek as pk

exp = pk.exp


model = pk.model()

p = 10
h = 0.25

alpha = model.add_variable()
beta = model.add_variable()

model.add_objective(sum((alpha * exp(i * h * beta) - i * h) ** 2 for i in range(1, p + 1)))
