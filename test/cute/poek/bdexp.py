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

#   Source: Problem 56 in
#   A.R. Conn, N.I.M. Gould, M. Lescrenier and Ph.L. Toint,
#   "Performance of a multifrontal scheme for partially separable
#   optimization",
#   Report 88/4, Dept of Mathematics, FUNDP (Namur, B), 1988.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OBR2-AY-V-0

import poek as pk

exp = pk.exp


model = pk.model()

N = 5000
ngs = N - 2

x = model.add_variable(index=range(1, N + 1), value=1.0)

model.add_objective(
    sum((x[i] + x[i + 1]) * exp((x[i] + x[i + 1]) * (-x[i + 2])) for i in range(1, ngs + 1))
)
