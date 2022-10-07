# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk

sin = pk.sin
cos = pk.cos
import math

model = pk.model()

c = (48.4 / 50.176) * math.sin(0.25)
d = (48.4 / 50.176) * math.cos(0.25)
N = list(range(1, 10))

x = model.add_variable(index=N)
x[1].value = 0.8
x[2].value = 0.8
x[3].value = 0.2
x[4].value = 0.2
x[5].value = 1.0454
x[6].value = 1.0454
x[7].value = 0.0
x[8].value = 0.0

y1 = pk.sin(x[8])
y2 = pk.cos(x[8])
y3 = pk.sin(x[9])
y4 = pk.cos(x[9])
y5 = pk.sin(x[8] - x[9])
y6 = pk.cos(x[8] - x[9])

model.add_objective(3000 * x[1] + 1000 * x[1] ** 3 + 2000 * x[2] + 666.667 * x[2] ** 3)

model.add_constraint(
    0.4
    - x[1]
    + 2 * c * x[5] ** 2
    - x[5] * x[6] * (d * y1 + c * y2)
    - x[5] * x[7] * (d * y3 + c * y4)
    == 0
)
model.add_constraint(
    0.4
    - x[2]
    + 2 * c * x[6] ** 2
    + x[5] * x[6] * (d * y1 - c * y2)
    + x[6] * x[7] * (d * y5 - c * y6)
    == 0
)
model.add_constraint(
    0.8 + 2 * c * x[7] ** 2 + x[5] * x[7] * (d * y3 - c * y4) - x[6] * x[7] * (d * y5 + c * y6) == 0
)
model.add_constraint(
    0.2
    - x[3]
    + 2 * d * x[5] ** 2
    + x[5] * x[6] * (c * y1 - d * y2)
    + x[5] * x[7] * (c * y3 - d * y4)
    == 0
)
model.add_constraint(
    0.2
    - x[4]
    + 2 * d * x[6] ** 2
    - x[5] * x[6] * (c * y1 + d * y2)
    - x[6] * x[7] * (c * y5 + d * y6)
    == 0
)
model.add_constraint(
    -0.337 + 2 * d * x[7] ** 2 - x[5] * x[7] * (c * y3 + d * y4) + x[6] * x[7] * (c * y5 - d * y6)
    == 0
)
model.add_constraint(x[1] >= 0)
model.add_constraint(x[2] >= 0)
model.add_constraint(x[5] >= 0.90909)
model.add_constraint(x[6] >= 0.90909)
model.add_constraint(x[7] >= 0.90909)
model.add_constraint(x[5] <= 1.0909)
model.add_constraint(x[6] <= 1.0909)
model.add_constraint(x[7] <= 1.0909)
