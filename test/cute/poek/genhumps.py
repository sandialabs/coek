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

#   Source
#   Ph. Toint, private communication, 1997.

#   SDIF input N. Gould and Ph. Toint, November 1997.

#   classification SUR2-AN-V-0

import poek as pk

sin = pk.sin


model = pk.model()

zeta = 2
N = 5

x = model.add_variable(index=range(1, N + 1))
for i in x:
    x[i].value = -506.0 if i == 1 else 506.2

model.add_objective(
    sum(
        sin(zeta * x[i]) ** 2 * sin(zeta * x[i + 1]) ** 2 + 0.05 * (x[i] ** 2 + x[i + 1] ** 2)
        for i in range(1, N)
    )
)
