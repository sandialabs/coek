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

#   Source: Hartman problem 6 in
#   L. C. W. Dixon and G. P. Szego (Eds.)
#   Towards Global Optimization
#   North Holland, 1975.
#   Paper 9, page 163.

#   SIF input: A.R. Conn May 1995

#   classification OBR2-AN-6-0

import poek as pk


model = pk.model()

data = pk.util.load_data("hart6.json")
c = data.c
a, p = data.unpack("a", "p", index=("i", "j"))

x = model.add_variable(index=range(1, 7), lb=0, ub=1, value=0.2)

model.add_objective(
    -sum(
        c[i] * pk.exp(-sum(a[i, j] * (x[j] - p[i, j]) ** 2 for j in range(1, 7)))
        for i in range(1, 5)
    )
)
