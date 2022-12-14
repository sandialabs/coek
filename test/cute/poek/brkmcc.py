# TODO
# Formulated in Pyomo by Juan Lopez
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

#   Source: Problem 85 (p.35) in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-2-0

import poek as pk


model = pk.model()

x = model.add_variable(index=[1, 2], value=2.0)

model.add_objective(
    (x[1] - 2) ** 2
    + (x[2] - 1) ** 2
    + (1 / (1 - 0.25 * x[1] ** 2 - x[2] ** 2)) / 25
    + 5 * (x[1] - 2 * x[2] + 1) ** 2
)
