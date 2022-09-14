# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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
#   N. Dautrebande,
#   private communication, 1994.

#   SIF input: Ph. Toint, Oct 1994.

#   classification SOR2-MY-6907-6900

from itertools import product
import poek as pk


model = pk.model()

NS = 2
NP = 5
NO = 26
H = 1
NT = 3450

data = pk.util.load_data("brainpc1.json")
TO, U, oc_init = data.unpack("TO", "U", "oc_init")

x = model.add_variable(
    index=product(range(1, NS + 1), range(0, NT + 1)), lb=0, value=0.001
)
k = model.add_variable(index=range(1, NP + 1), lb=0, value=0.001)

x[1, 0].value = 0.0
x[1, 0].fixed = True
x[2, 0].value = 0.0
x[2, 0].fixed = True

model.add_objective(
    sum(
        (
            -(x[1, TO[t]] + x[2, TO[t]]) * k[1]
            + x[1, TO[t]]
            + x[2, TO[t]]
            + U[TO[t]] * k[1]
            - oc_init[t]
        )
        ** 2
        for t in range(1, NO + 1)
    )
)

for t in range(0, NT):
    model.add_constraint(
        H * (k[3] + k[4]) * x[1, t]
        - H * k[5] * x[2, t]
        - H * U[t] * k[2]
        + x[1, t + 1]
        - x[1, t]
        == 0
    )

for t in range(0, NT):
    model.add_constraint(
        H * k[5] * x[2, t] - H * k[4] * x[1, t] + x[2, t + 1] - x[2, t] == 0
    )
