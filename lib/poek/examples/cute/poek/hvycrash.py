# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from:

#
# **************************
# SET UP THE INITIAL DATA *
# **************************
#   Problem :
#   *********
#   A problem freely inspired by the heavy spacecraft landing problem by
#   Ivashkevich, as reported by Tyatushkin, Zholudev and Erinchek.  The
#   modifications of the original problem involves dropping the final state
#   constraint on the second variable and the setting of EPS to zero
#   in the second constraint.
#   (No feasible point was found for the original formulation by any of the
#   packages at hand.)
#   This is a badly scaled degenerate nonlinear nonconvex optimal control
#   problem, with lots of negative curvature.
#   Source: inspired by
#   A.I Tyatushkin, A.I. Zholudev and N. M. Erinchek,
#   "The gradient method for solving optimal control problems with phase
#   constraints",
#   in "System Modelling and Optimization", P. Kall, ed., pp. 456--464,
#   Springer Verlag, Lecture Notes in Control and Information Sciences 180, 1992.
#   This reference itself refers to:
#   A.K. Ivashkevich,
#   "Multistage US spacecraft Space Shuttle",
#   Survey of foreign papers (Part 11),  Orbital Craft, Moscow, 1976.
#   SIF input: Ph. Toint, February 1994.
#   classification LOR2-AN-V-V
#   Number of discretized points in (0,TT].
#   The number of variables is    4 * ( N + 1 )
#   The number of constraints is  3 * N
# IE N                   100            $ n =  404, m =  300
# IE N                   500            $ n = 2004, m = 1500
# IE N                   1000           $ n = 4004, m = 3000
#   Problem parameters
#   Mesh
#   Constants
#   State variables
#   Control variables
#   Objective function
#   State constraints
#   State constraints elements
#   State constraints
# ZE C(2,T)    E2(T)                    -H
# ZE C(2,T)    E3(T)                    -H
#   Solution
# LO SOLTN             -0.21850
# LO SOLTN(100)        3.26705331D-8
# LO SOLTN(500)        2.36171208D-8
# LO SOLTN(1000)       8.48265630D-8

import poek as pk

cos = pk.cos


model = pk.model()

n = 50.0
tt = 0.2185
eps = 0.1
alpha = 1.62079
nm1 = -1.0 + (50.0)
rn = 50.0
h = (0.2185) / (50.0)
tm1 = -1.0 + (50.0)

x1_0 = model.add_variable(lb=0.0, ub=0.0)
x2_0 = model.add_variable(value=2.19905, lb=2.19905, ub=2.19905)
x3_0 = model.add_variable(lb=0.0, ub=6.2831854)
x1_1 = model.add_variable()
x2_1 = model.add_variable(value=1.5)
x3_1 = model.add_variable(lb=0.0, ub=6.2831854)
x1_2 = model.add_variable()
x2_2 = model.add_variable(value=1.5)
x3_2 = model.add_variable(lb=0.0, ub=6.2831854)
x1_3 = model.add_variable()
x2_3 = model.add_variable(value=1.5)
x3_3 = model.add_variable(lb=0.0, ub=6.2831854)
x1_4 = model.add_variable()
x2_4 = model.add_variable(value=1.5)
x3_4 = model.add_variable(lb=0.0, ub=6.2831854)
x1_5 = model.add_variable()
x2_5 = model.add_variable(value=1.5)
x3_5 = model.add_variable(lb=0.0, ub=6.2831854)
x1_6 = model.add_variable()
x2_6 = model.add_variable(value=1.5)
x3_6 = model.add_variable(lb=0.0, ub=6.2831854)
x1_7 = model.add_variable()
x2_7 = model.add_variable(value=1.5)
x3_7 = model.add_variable(lb=0.0, ub=6.2831854)
x1_8 = model.add_variable()
x2_8 = model.add_variable(value=1.5)
x3_8 = model.add_variable(lb=0.0, ub=6.2831854)
x1_9 = model.add_variable()
x2_9 = model.add_variable(value=1.5)
x3_9 = model.add_variable(lb=0.0, ub=6.2831854)
x1_10 = model.add_variable()
x2_10 = model.add_variable(value=1.5)
x3_10 = model.add_variable(lb=0.0, ub=6.2831854)
x1_11 = model.add_variable()
x2_11 = model.add_variable(value=1.5)
x3_11 = model.add_variable(lb=0.0, ub=6.2831854)
x1_12 = model.add_variable()
x2_12 = model.add_variable(value=1.5)
x3_12 = model.add_variable(lb=0.0, ub=6.2831854)
x1_13 = model.add_variable()
x2_13 = model.add_variable(value=1.5)
x3_13 = model.add_variable(lb=0.0, ub=6.2831854)
x1_14 = model.add_variable()
x2_14 = model.add_variable(value=1.5)
x3_14 = model.add_variable(lb=0.0, ub=6.2831854)
x1_15 = model.add_variable()
x2_15 = model.add_variable(value=1.5)
x3_15 = model.add_variable(lb=0.0, ub=6.2831854)
x1_16 = model.add_variable()
x2_16 = model.add_variable(value=1.5)
x3_16 = model.add_variable(lb=0.0, ub=6.2831854)
x1_17 = model.add_variable()
x2_17 = model.add_variable(value=1.5)
x3_17 = model.add_variable(lb=0.0, ub=6.2831854)
x1_18 = model.add_variable()
x2_18 = model.add_variable(value=1.5)
x3_18 = model.add_variable(lb=0.0, ub=6.2831854)
x1_19 = model.add_variable()
x2_19 = model.add_variable(value=1.5)
x3_19 = model.add_variable(lb=0.0, ub=6.2831854)
x1_20 = model.add_variable()
x2_20 = model.add_variable(value=1.5)
x3_20 = model.add_variable(lb=0.0, ub=6.2831854)
x1_21 = model.add_variable()
x2_21 = model.add_variable(value=1.5)
x3_21 = model.add_variable(lb=0.0, ub=6.2831854)
x1_22 = model.add_variable()
x2_22 = model.add_variable(value=1.5)
x3_22 = model.add_variable(lb=0.0, ub=6.2831854)
x1_23 = model.add_variable()
x2_23 = model.add_variable(value=1.5)
x3_23 = model.add_variable(lb=0.0, ub=6.2831854)
x1_24 = model.add_variable()
x2_24 = model.add_variable(value=1.5)
x3_24 = model.add_variable(lb=0.0, ub=6.2831854)
x1_25 = model.add_variable()
x2_25 = model.add_variable(value=1.5)
x3_25 = model.add_variable(lb=0.0, ub=6.2831854)
x1_26 = model.add_variable()
x2_26 = model.add_variable(value=1.5)
x3_26 = model.add_variable(lb=0.0, ub=6.2831854)
x1_27 = model.add_variable()
x2_27 = model.add_variable(value=1.5)
x3_27 = model.add_variable(lb=0.0, ub=6.2831854)
x1_28 = model.add_variable()
x2_28 = model.add_variable(value=1.5)
x3_28 = model.add_variable(lb=0.0, ub=6.2831854)
x1_29 = model.add_variable()
x2_29 = model.add_variable(value=1.5)
x3_29 = model.add_variable(lb=0.0, ub=6.2831854)
x1_30 = model.add_variable()
x2_30 = model.add_variable(value=1.5)
x3_30 = model.add_variable(lb=0.0, ub=6.2831854)
x1_31 = model.add_variable()
x2_31 = model.add_variable(value=1.5)
x3_31 = model.add_variable(lb=0.0, ub=6.2831854)
x1_32 = model.add_variable()
x2_32 = model.add_variable(value=1.5)
x3_32 = model.add_variable(lb=0.0, ub=6.2831854)
x1_33 = model.add_variable()
x2_33 = model.add_variable(value=1.5)
x3_33 = model.add_variable(lb=0.0, ub=6.2831854)
x1_34 = model.add_variable()
x2_34 = model.add_variable(value=1.5)
x3_34 = model.add_variable(lb=0.0, ub=6.2831854)
x1_35 = model.add_variable()
x2_35 = model.add_variable(value=1.5)
x3_35 = model.add_variable(lb=0.0, ub=6.2831854)
x1_36 = model.add_variable()
x2_36 = model.add_variable(value=1.5)
x3_36 = model.add_variable(lb=0.0, ub=6.2831854)
x1_37 = model.add_variable()
x2_37 = model.add_variable(value=1.5)
x3_37 = model.add_variable(lb=0.0, ub=6.2831854)
x1_38 = model.add_variable()
x2_38 = model.add_variable(value=1.5)
x3_38 = model.add_variable(lb=0.0, ub=6.2831854)
x1_39 = model.add_variable()
x2_39 = model.add_variable(value=1.5)
x3_39 = model.add_variable(lb=0.0, ub=6.2831854)
x1_40 = model.add_variable()
x2_40 = model.add_variable(value=1.5)
x3_40 = model.add_variable(lb=0.0, ub=6.2831854)
x1_41 = model.add_variable()
x2_41 = model.add_variable(value=1.5)
x3_41 = model.add_variable(lb=0.0, ub=6.2831854)
x1_42 = model.add_variable()
x2_42 = model.add_variable(value=1.5)
x3_42 = model.add_variable(lb=0.0, ub=6.2831854)
x1_43 = model.add_variable()
x2_43 = model.add_variable(value=1.5)
x3_43 = model.add_variable(lb=0.0, ub=6.2831854)
x1_44 = model.add_variable()
x2_44 = model.add_variable(value=1.5)
x3_44 = model.add_variable(lb=0.0, ub=6.2831854)
x1_45 = model.add_variable()
x2_45 = model.add_variable(value=1.5)
x3_45 = model.add_variable(lb=0.0, ub=6.2831854)
x1_46 = model.add_variable()
x2_46 = model.add_variable(value=1.5)
x3_46 = model.add_variable(lb=0.0, ub=6.2831854)
x1_47 = model.add_variable()
x2_47 = model.add_variable(value=1.5)
x3_47 = model.add_variable(lb=0.0, ub=6.2831854)
x1_48 = model.add_variable()
x2_48 = model.add_variable(value=1.5)
x3_48 = model.add_variable(lb=0.0, ub=6.2831854)
x1_49 = model.add_variable()
x2_49 = model.add_variable(value=1.5)
x3_49 = model.add_variable(lb=0.0, ub=6.2831854)
x1_50 = model.add_variable()
x2_50 = model.add_variable(value=1.09905)
x3_50 = model.add_variable(lb=0.0, ub=6.2831854)
u0 = model.add_variable(lb=0.08, ub=0.417)
u1 = model.add_variable(lb=0.08, ub=0.417)
u2 = model.add_variable(lb=0.08, ub=0.417)
u3 = model.add_variable(lb=0.08, ub=0.417)
u4 = model.add_variable(lb=0.08, ub=0.417)
u5 = model.add_variable(lb=0.08, ub=0.417)
u6 = model.add_variable(lb=0.08, ub=0.417)
u7 = model.add_variable(lb=0.08, ub=0.417)
u8 = model.add_variable(lb=0.08, ub=0.417)
u9 = model.add_variable(lb=0.08, ub=0.417)
u10 = model.add_variable(lb=0.08, ub=0.417)
u11 = model.add_variable(lb=0.08, ub=0.417)
u12 = model.add_variable(lb=0.08, ub=0.417)
u13 = model.add_variable(lb=0.08, ub=0.417)
u14 = model.add_variable(lb=0.08, ub=0.417)
u15 = model.add_variable(lb=0.08, ub=0.417)
u16 = model.add_variable(lb=0.08, ub=0.417)
u17 = model.add_variable(lb=0.08, ub=0.417)
u18 = model.add_variable(lb=0.08, ub=0.417)
u19 = model.add_variable(lb=0.08, ub=0.417)
u20 = model.add_variable(lb=0.08, ub=0.417)
u21 = model.add_variable(lb=0.08, ub=0.417)
u22 = model.add_variable(lb=0.08, ub=0.417)
u23 = model.add_variable(lb=0.08, ub=0.417)
u24 = model.add_variable(lb=0.08, ub=0.417)
u25 = model.add_variable(lb=0.08, ub=0.417)
u26 = model.add_variable(lb=0.08, ub=0.417)
u27 = model.add_variable(lb=0.08, ub=0.417)
u28 = model.add_variable(lb=0.08, ub=0.417)
u29 = model.add_variable(lb=0.08, ub=0.417)
u30 = model.add_variable(lb=0.08, ub=0.417)
u31 = model.add_variable(lb=0.08, ub=0.417)
u32 = model.add_variable(lb=0.08, ub=0.417)
u33 = model.add_variable(lb=0.08, ub=0.417)
u34 = model.add_variable(lb=0.08, ub=0.417)
u35 = model.add_variable(lb=0.08, ub=0.417)
u36 = model.add_variable(lb=0.08, ub=0.417)
u37 = model.add_variable(lb=0.08, ub=0.417)
u38 = model.add_variable(lb=0.08, ub=0.417)
u39 = model.add_variable(lb=0.08, ub=0.417)
u40 = model.add_variable(lb=0.08, ub=0.417)
u41 = model.add_variable(lb=0.08, ub=0.417)
u42 = model.add_variable(lb=0.08, ub=0.417)
u43 = model.add_variable(lb=0.08, ub=0.417)
u44 = model.add_variable(lb=0.08, ub=0.417)
u45 = model.add_variable(lb=0.08, ub=0.417)
u46 = model.add_variable(lb=0.08, ub=0.417)
u47 = model.add_variable(lb=0.08, ub=0.417)
u48 = model.add_variable(lb=0.08, ub=0.417)
u49 = model.add_variable(lb=0.08, ub=0.417)
u50 = model.add_variable(lb=0.08, ub=0.417)

model.add_objective(x1_50)

model.add_constraint(
    0.00437 * (cos(x3_1)) / (1.62079 * (0.01 + 0.3 * u1 * u1) * x2_1 * x2_1)
    + x1_0
    - x1_1
    == 0
)
model.add_constraint(
    -1.0 / x2_1
    - (cos(x3_1)) / (1.62079 * (0.01 + 0.3 * u1 * u1) * (x2_1 * x2_1) * x2_1)
    == 0
)
model.add_constraint(
    0.00437 * u1 / ((0.01 + 0.3 * u1 * u1) * (x2_1 * x2_1))
    - 0.00437
    * (cos(x3_1))
    / (1.62079 * (0.01 + 0.3 * u1 * u1) * (x2_1 * x2_1) * x2_1 * x2_1)
    + 0.1 * x3_0
    - 0.1 * x3_1
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_2)) / (1.62079 * (0.01 + 0.3 * u2 * u2) * x2_2 * x2_2)
    + x1_1
    - x1_2
    == 0
)
model.add_constraint(
    -1.0 / x2_2
    - (cos(x3_2)) / (1.62079 * (0.01 + 0.3 * u2 * u2) * (x2_2 * x2_2) * x2_2)
    == 0
)
model.add_constraint(
    0.00437 * u2 / ((0.01 + 0.3 * u2 * u2) * (x2_2 * x2_2))
    - 0.00437
    * (cos(x3_2))
    / (1.62079 * (0.01 + 0.3 * u2 * u2) * (x2_2 * x2_2) * x2_2 * x2_2)
    + 0.1 * x3_1
    - 0.1 * x3_2
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_3)) / (1.62079 * (0.01 + 0.3 * u3 * u3) * x2_3 * x2_3)
    + x1_2
    - x1_3
    == 0
)
model.add_constraint(
    -1.0 / x2_3
    - (cos(x3_3)) / (1.62079 * (0.01 + 0.3 * u3 * u3) * (x2_3 * x2_3) * x2_3)
    == 0
)
model.add_constraint(
    0.00437 * u3 / ((0.01 + 0.3 * u3 * u3) * (x2_3 * x2_3))
    - 0.00437
    * (cos(x3_3))
    / (1.62079 * (0.01 + 0.3 * u3 * u3) * (x2_3 * x2_3) * x2_3 * x2_3)
    + 0.1 * x3_2
    - 0.1 * x3_3
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_4)) / (1.62079 * (0.01 + 0.3 * u4 * u4) * x2_4 * x2_4)
    + x1_3
    - x1_4
    == 0
)
model.add_constraint(
    -1.0 / x2_4
    - (cos(x3_4)) / (1.62079 * (0.01 + 0.3 * u4 * u4) * (x2_4 * x2_4) * x2_4)
    == 0
)
model.add_constraint(
    0.00437 * u4 / ((0.01 + 0.3 * u4 * u4) * (x2_4 * x2_4))
    - 0.00437
    * (cos(x3_4))
    / (1.62079 * (0.01 + 0.3 * u4 * u4) * (x2_4 * x2_4) * x2_4 * x2_4)
    + 0.1 * x3_3
    - 0.1 * x3_4
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_5)) / (1.62079 * (0.01 + 0.3 * u5 * u5) * x2_5 * x2_5)
    + x1_4
    - x1_5
    == 0
)
model.add_constraint(
    -1.0 / x2_5
    - (cos(x3_5)) / (1.62079 * (0.01 + 0.3 * u5 * u5) * (x2_5 * x2_5) * x2_5)
    == 0
)
model.add_constraint(
    0.00437 * u5 / ((0.01 + 0.3 * u5 * u5) * (x2_5 * x2_5))
    - 0.00437
    * (cos(x3_5))
    / (1.62079 * (0.01 + 0.3 * u5 * u5) * (x2_5 * x2_5) * x2_5 * x2_5)
    + 0.1 * x3_4
    - 0.1 * x3_5
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_6)) / (1.62079 * (0.01 + 0.3 * u6 * u6) * x2_6 * x2_6)
    + x1_5
    - x1_6
    == 0
)
model.add_constraint(
    -1.0 / x2_6
    - (cos(x3_6)) / (1.62079 * (0.01 + 0.3 * u6 * u6) * (x2_6 * x2_6) * x2_6)
    == 0
)
model.add_constraint(
    0.00437 * u6 / ((0.01 + 0.3 * u6 * u6) * (x2_6 * x2_6))
    - 0.00437
    * (cos(x3_6))
    / (1.62079 * (0.01 + 0.3 * u6 * u6) * (x2_6 * x2_6) * x2_6 * x2_6)
    + 0.1 * x3_5
    - 0.1 * x3_6
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_7)) / (1.62079 * (0.01 + 0.3 * u7 * u7) * x2_7 * x2_7)
    + x1_6
    - x1_7
    == 0
)
model.add_constraint(
    -1.0 / x2_7
    - (cos(x3_7)) / (1.62079 * (0.01 + 0.3 * u7 * u7) * (x2_7 * x2_7) * x2_7)
    == 0
)
model.add_constraint(
    0.00437 * u7 / ((0.01 + 0.3 * u7 * u7) * (x2_7 * x2_7))
    - 0.00437
    * (cos(x3_7))
    / (1.62079 * (0.01 + 0.3 * u7 * u7) * (x2_7 * x2_7) * x2_7 * x2_7)
    + 0.1 * x3_6
    - 0.1 * x3_7
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_8)) / (1.62079 * (0.01 + 0.3 * u8 * u8) * x2_8 * x2_8)
    + x1_7
    - x1_8
    == 0
)
model.add_constraint(
    -1.0 / x2_8
    - (cos(x3_8)) / (1.62079 * (0.01 + 0.3 * u8 * u8) * (x2_8 * x2_8) * x2_8)
    == 0
)
model.add_constraint(
    0.00437 * u8 / ((0.01 + 0.3 * u8 * u8) * (x2_8 * x2_8))
    - 0.00437
    * (cos(x3_8))
    / (1.62079 * (0.01 + 0.3 * u8 * u8) * (x2_8 * x2_8) * x2_8 * x2_8)
    + 0.1 * x3_7
    - 0.1 * x3_8
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_9)) / (1.62079 * (0.01 + 0.3 * u9 * u9) * x2_9 * x2_9)
    + x1_8
    - x1_9
    == 0
)
model.add_constraint(
    -1.0 / x2_9
    - (cos(x3_9)) / (1.62079 * (0.01 + 0.3 * u9 * u9) * (x2_9 * x2_9) * x2_9)
    == 0
)
model.add_constraint(
    0.00437 * u9 / ((0.01 + 0.3 * u9 * u9) * (x2_9 * x2_9))
    - 0.00437
    * (cos(x3_9))
    / (1.62079 * (0.01 + 0.3 * u9 * u9) * (x2_9 * x2_9) * x2_9 * x2_9)
    + 0.1 * x3_8
    - 0.1 * x3_9
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_10)) / (1.62079 * (0.01 + 0.3 * u10 * u10) * x2_10 * x2_10)
    + x1_9
    - x1_10
    == 0
)
model.add_constraint(
    -1.0 / x2_10
    - (cos(x3_10)) / (1.62079 * (0.01 + 0.3 * u10 * u10) * (x2_10 * x2_10) * x2_10)
    == 0
)
model.add_constraint(
    0.00437 * u10 / ((0.01 + 0.3 * u10 * u10) * (x2_10 * x2_10))
    - 0.00437
    * (cos(x3_10))
    / (1.62079 * (0.01 + 0.3 * u10 * u10) * (x2_10 * x2_10) * x2_10 * x2_10)
    + 0.1 * x3_9
    - 0.1 * x3_10
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_11)) / (1.62079 * (0.01 + 0.3 * u11 * u11) * x2_11 * x2_11)
    + x1_10
    - x1_11
    == 0
)
model.add_constraint(
    -1.0 / x2_11
    - (cos(x3_11)) / (1.62079 * (0.01 + 0.3 * u11 * u11) * (x2_11 * x2_11) * x2_11)
    == 0
)
model.add_constraint(
    0.00437 * u11 / ((0.01 + 0.3 * u11 * u11) * (x2_11 * x2_11))
    - 0.00437
    * (cos(x3_11))
    / (1.62079 * (0.01 + 0.3 * u11 * u11) * (x2_11 * x2_11) * x2_11 * x2_11)
    + 0.1 * x3_10
    - 0.1 * x3_11
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_12)) / (1.62079 * (0.01 + 0.3 * u12 * u12) * x2_12 * x2_12)
    + x1_11
    - x1_12
    == 0
)
model.add_constraint(
    -1.0 / x2_12
    - (cos(x3_12)) / (1.62079 * (0.01 + 0.3 * u12 * u12) * (x2_12 * x2_12) * x2_12)
    == 0
)
model.add_constraint(
    0.00437 * u12 / ((0.01 + 0.3 * u12 * u12) * (x2_12 * x2_12))
    - 0.00437
    * (cos(x3_12))
    / (1.62079 * (0.01 + 0.3 * u12 * u12) * (x2_12 * x2_12) * x2_12 * x2_12)
    + 0.1 * x3_11
    - 0.1 * x3_12
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_13)) / (1.62079 * (0.01 + 0.3 * u13 * u13) * x2_13 * x2_13)
    + x1_12
    - x1_13
    == 0
)
model.add_constraint(
    -1.0 / x2_13
    - (cos(x3_13)) / (1.62079 * (0.01 + 0.3 * u13 * u13) * (x2_13 * x2_13) * x2_13)
    == 0
)
model.add_constraint(
    0.00437 * u13 / ((0.01 + 0.3 * u13 * u13) * (x2_13 * x2_13))
    - 0.00437
    * (cos(x3_13))
    / (1.62079 * (0.01 + 0.3 * u13 * u13) * (x2_13 * x2_13) * x2_13 * x2_13)
    + 0.1 * x3_12
    - 0.1 * x3_13
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_14)) / (1.62079 * (0.01 + 0.3 * u14 * u14) * x2_14 * x2_14)
    + x1_13
    - x1_14
    == 0
)
model.add_constraint(
    -1.0 / x2_14
    - (cos(x3_14)) / (1.62079 * (0.01 + 0.3 * u14 * u14) * (x2_14 * x2_14) * x2_14)
    == 0
)
model.add_constraint(
    0.00437 * u14 / ((0.01 + 0.3 * u14 * u14) * (x2_14 * x2_14))
    - 0.00437
    * (cos(x3_14))
    / (1.62079 * (0.01 + 0.3 * u14 * u14) * (x2_14 * x2_14) * x2_14 * x2_14)
    + 0.1 * x3_13
    - 0.1 * x3_14
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_15)) / (1.62079 * (0.01 + 0.3 * u15 * u15) * x2_15 * x2_15)
    + x1_14
    - x1_15
    == 0
)
model.add_constraint(
    -1.0 / x2_15
    - (cos(x3_15)) / (1.62079 * (0.01 + 0.3 * u15 * u15) * (x2_15 * x2_15) * x2_15)
    == 0
)
model.add_constraint(
    0.00437 * u15 / ((0.01 + 0.3 * u15 * u15) * (x2_15 * x2_15))
    - 0.00437
    * (cos(x3_15))
    / (1.62079 * (0.01 + 0.3 * u15 * u15) * (x2_15 * x2_15) * x2_15 * x2_15)
    + 0.1 * x3_14
    - 0.1 * x3_15
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_16)) / (1.62079 * (0.01 + 0.3 * u16 * u16) * x2_16 * x2_16)
    + x1_15
    - x1_16
    == 0
)
model.add_constraint(
    -1.0 / x2_16
    - (cos(x3_16)) / (1.62079 * (0.01 + 0.3 * u16 * u16) * (x2_16 * x2_16) * x2_16)
    == 0
)
model.add_constraint(
    0.00437 * u16 / ((0.01 + 0.3 * u16 * u16) * (x2_16 * x2_16))
    - 0.00437
    * (cos(x3_16))
    / (1.62079 * (0.01 + 0.3 * u16 * u16) * (x2_16 * x2_16) * x2_16 * x2_16)
    + 0.1 * x3_15
    - 0.1 * x3_16
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_17)) / (1.62079 * (0.01 + 0.3 * u17 * u17) * x2_17 * x2_17)
    + x1_16
    - x1_17
    == 0
)
model.add_constraint(
    -1.0 / x2_17
    - (cos(x3_17)) / (1.62079 * (0.01 + 0.3 * u17 * u17) * (x2_17 * x2_17) * x2_17)
    == 0
)
model.add_constraint(
    0.00437 * u17 / ((0.01 + 0.3 * u17 * u17) * (x2_17 * x2_17))
    - 0.00437
    * (cos(x3_17))
    / (1.62079 * (0.01 + 0.3 * u17 * u17) * (x2_17 * x2_17) * x2_17 * x2_17)
    + 0.1 * x3_16
    - 0.1 * x3_17
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_18)) / (1.62079 * (0.01 + 0.3 * u18 * u18) * x2_18 * x2_18)
    + x1_17
    - x1_18
    == 0
)
model.add_constraint(
    -1.0 / x2_18
    - (cos(x3_18)) / (1.62079 * (0.01 + 0.3 * u18 * u18) * (x2_18 * x2_18) * x2_18)
    == 0
)
model.add_constraint(
    0.00437 * u18 / ((0.01 + 0.3 * u18 * u18) * (x2_18 * x2_18))
    - 0.00437
    * (cos(x3_18))
    / (1.62079 * (0.01 + 0.3 * u18 * u18) * (x2_18 * x2_18) * x2_18 * x2_18)
    + 0.1 * x3_17
    - 0.1 * x3_18
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_19)) / (1.62079 * (0.01 + 0.3 * u19 * u19) * x2_19 * x2_19)
    + x1_18
    - x1_19
    == 0
)
model.add_constraint(
    -1.0 / x2_19
    - (cos(x3_19)) / (1.62079 * (0.01 + 0.3 * u19 * u19) * (x2_19 * x2_19) * x2_19)
    == 0
)
model.add_constraint(
    0.00437 * u19 / ((0.01 + 0.3 * u19 * u19) * (x2_19 * x2_19))
    - 0.00437
    * (cos(x3_19))
    / (1.62079 * (0.01 + 0.3 * u19 * u19) * (x2_19 * x2_19) * x2_19 * x2_19)
    + 0.1 * x3_18
    - 0.1 * x3_19
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_20)) / (1.62079 * (0.01 + 0.3 * u20 * u20) * x2_20 * x2_20)
    + x1_19
    - x1_20
    == 0
)
model.add_constraint(
    -1.0 / x2_20
    - (cos(x3_20)) / (1.62079 * (0.01 + 0.3 * u20 * u20) * (x2_20 * x2_20) * x2_20)
    == 0
)
model.add_constraint(
    0.00437 * u20 / ((0.01 + 0.3 * u20 * u20) * (x2_20 * x2_20))
    - 0.00437
    * (cos(x3_20))
    / (1.62079 * (0.01 + 0.3 * u20 * u20) * (x2_20 * x2_20) * x2_20 * x2_20)
    + 0.1 * x3_19
    - 0.1 * x3_20
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_21)) / (1.62079 * (0.01 + 0.3 * u21 * u21) * x2_21 * x2_21)
    + x1_20
    - x1_21
    == 0
)
model.add_constraint(
    -1.0 / x2_21
    - (cos(x3_21)) / (1.62079 * (0.01 + 0.3 * u21 * u21) * (x2_21 * x2_21) * x2_21)
    == 0
)
model.add_constraint(
    0.00437 * u21 / ((0.01 + 0.3 * u21 * u21) * (x2_21 * x2_21))
    - 0.00437
    * (cos(x3_21))
    / (1.62079 * (0.01 + 0.3 * u21 * u21) * (x2_21 * x2_21) * x2_21 * x2_21)
    + 0.1 * x3_20
    - 0.1 * x3_21
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_22)) / (1.62079 * (0.01 + 0.3 * u22 * u22) * x2_22 * x2_22)
    + x1_21
    - x1_22
    == 0
)
model.add_constraint(
    -1.0 / x2_22
    - (cos(x3_22)) / (1.62079 * (0.01 + 0.3 * u22 * u22) * (x2_22 * x2_22) * x2_22)
    == 0
)
model.add_constraint(
    0.00437 * u22 / ((0.01 + 0.3 * u22 * u22) * (x2_22 * x2_22))
    - 0.00437
    * (cos(x3_22))
    / (1.62079 * (0.01 + 0.3 * u22 * u22) * (x2_22 * x2_22) * x2_22 * x2_22)
    + 0.1 * x3_21
    - 0.1 * x3_22
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_23)) / (1.62079 * (0.01 + 0.3 * u23 * u23) * x2_23 * x2_23)
    + x1_22
    - x1_23
    == 0
)
model.add_constraint(
    -1.0 / x2_23
    - (cos(x3_23)) / (1.62079 * (0.01 + 0.3 * u23 * u23) * (x2_23 * x2_23) * x2_23)
    == 0
)
model.add_constraint(
    0.00437 * u23 / ((0.01 + 0.3 * u23 * u23) * (x2_23 * x2_23))
    - 0.00437
    * (cos(x3_23))
    / (1.62079 * (0.01 + 0.3 * u23 * u23) * (x2_23 * x2_23) * x2_23 * x2_23)
    + 0.1 * x3_22
    - 0.1 * x3_23
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_24)) / (1.62079 * (0.01 + 0.3 * u24 * u24) * x2_24 * x2_24)
    + x1_23
    - x1_24
    == 0
)
model.add_constraint(
    -1.0 / x2_24
    - (cos(x3_24)) / (1.62079 * (0.01 + 0.3 * u24 * u24) * (x2_24 * x2_24) * x2_24)
    == 0
)
model.add_constraint(
    0.00437 * u24 / ((0.01 + 0.3 * u24 * u24) * (x2_24 * x2_24))
    - 0.00437
    * (cos(x3_24))
    / (1.62079 * (0.01 + 0.3 * u24 * u24) * (x2_24 * x2_24) * x2_24 * x2_24)
    + 0.1 * x3_23
    - 0.1 * x3_24
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_25)) / (1.62079 * (0.01 + 0.3 * u25 * u25) * x2_25 * x2_25)
    + x1_24
    - x1_25
    == 0
)
model.add_constraint(
    -1.0 / x2_25
    - (cos(x3_25)) / (1.62079 * (0.01 + 0.3 * u25 * u25) * (x2_25 * x2_25) * x2_25)
    == 0
)
model.add_constraint(
    0.00437 * u25 / ((0.01 + 0.3 * u25 * u25) * (x2_25 * x2_25))
    - 0.00437
    * (cos(x3_25))
    / (1.62079 * (0.01 + 0.3 * u25 * u25) * (x2_25 * x2_25) * x2_25 * x2_25)
    + 0.1 * x3_24
    - 0.1 * x3_25
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_26)) / (1.62079 * (0.01 + 0.3 * u26 * u26) * x2_26 * x2_26)
    + x1_25
    - x1_26
    == 0
)
model.add_constraint(
    -1.0 / x2_26
    - (cos(x3_26)) / (1.62079 * (0.01 + 0.3 * u26 * u26) * (x2_26 * x2_26) * x2_26)
    == 0
)
model.add_constraint(
    0.00437 * u26 / ((0.01 + 0.3 * u26 * u26) * (x2_26 * x2_26))
    - 0.00437
    * (cos(x3_26))
    / (1.62079 * (0.01 + 0.3 * u26 * u26) * (x2_26 * x2_26) * x2_26 * x2_26)
    + 0.1 * x3_25
    - 0.1 * x3_26
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_27)) / (1.62079 * (0.01 + 0.3 * u27 * u27) * x2_27 * x2_27)
    + x1_26
    - x1_27
    == 0
)
model.add_constraint(
    -1.0 / x2_27
    - (cos(x3_27)) / (1.62079 * (0.01 + 0.3 * u27 * u27) * (x2_27 * x2_27) * x2_27)
    == 0
)
model.add_constraint(
    0.00437 * u27 / ((0.01 + 0.3 * u27 * u27) * (x2_27 * x2_27))
    - 0.00437
    * (cos(x3_27))
    / (1.62079 * (0.01 + 0.3 * u27 * u27) * (x2_27 * x2_27) * x2_27 * x2_27)
    + 0.1 * x3_26
    - 0.1 * x3_27
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_28)) / (1.62079 * (0.01 + 0.3 * u28 * u28) * x2_28 * x2_28)
    + x1_27
    - x1_28
    == 0
)
model.add_constraint(
    -1.0 / x2_28
    - (cos(x3_28)) / (1.62079 * (0.01 + 0.3 * u28 * u28) * (x2_28 * x2_28) * x2_28)
    == 0
)
model.add_constraint(
    0.00437 * u28 / ((0.01 + 0.3 * u28 * u28) * (x2_28 * x2_28))
    - 0.00437
    * (cos(x3_28))
    / (1.62079 * (0.01 + 0.3 * u28 * u28) * (x2_28 * x2_28) * x2_28 * x2_28)
    + 0.1 * x3_27
    - 0.1 * x3_28
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_29)) / (1.62079 * (0.01 + 0.3 * u29 * u29) * x2_29 * x2_29)
    + x1_28
    - x1_29
    == 0
)
model.add_constraint(
    -1.0 / x2_29
    - (cos(x3_29)) / (1.62079 * (0.01 + 0.3 * u29 * u29) * (x2_29 * x2_29) * x2_29)
    == 0
)
model.add_constraint(
    0.00437 * u29 / ((0.01 + 0.3 * u29 * u29) * (x2_29 * x2_29))
    - 0.00437
    * (cos(x3_29))
    / (1.62079 * (0.01 + 0.3 * u29 * u29) * (x2_29 * x2_29) * x2_29 * x2_29)
    + 0.1 * x3_28
    - 0.1 * x3_29
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_30)) / (1.62079 * (0.01 + 0.3 * u30 * u30) * x2_30 * x2_30)
    + x1_29
    - x1_30
    == 0
)
model.add_constraint(
    -1.0 / x2_30
    - (cos(x3_30)) / (1.62079 * (0.01 + 0.3 * u30 * u30) * (x2_30 * x2_30) * x2_30)
    == 0
)
model.add_constraint(
    0.00437 * u30 / ((0.01 + 0.3 * u30 * u30) * (x2_30 * x2_30))
    - 0.00437
    * (cos(x3_30))
    / (1.62079 * (0.01 + 0.3 * u30 * u30) * (x2_30 * x2_30) * x2_30 * x2_30)
    + 0.1 * x3_29
    - 0.1 * x3_30
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_31)) / (1.62079 * (0.01 + 0.3 * u31 * u31) * x2_31 * x2_31)
    + x1_30
    - x1_31
    == 0
)
model.add_constraint(
    -1.0 / x2_31
    - (cos(x3_31)) / (1.62079 * (0.01 + 0.3 * u31 * u31) * (x2_31 * x2_31) * x2_31)
    == 0
)
model.add_constraint(
    0.00437 * u31 / ((0.01 + 0.3 * u31 * u31) * (x2_31 * x2_31))
    - 0.00437
    * (cos(x3_31))
    / (1.62079 * (0.01 + 0.3 * u31 * u31) * (x2_31 * x2_31) * x2_31 * x2_31)
    + 0.1 * x3_30
    - 0.1 * x3_31
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_32)) / (1.62079 * (0.01 + 0.3 * u32 * u32) * x2_32 * x2_32)
    + x1_31
    - x1_32
    == 0
)
model.add_constraint(
    -1.0 / x2_32
    - (cos(x3_32)) / (1.62079 * (0.01 + 0.3 * u32 * u32) * (x2_32 * x2_32) * x2_32)
    == 0
)
model.add_constraint(
    0.00437 * u32 / ((0.01 + 0.3 * u32 * u32) * (x2_32 * x2_32))
    - 0.00437
    * (cos(x3_32))
    / (1.62079 * (0.01 + 0.3 * u32 * u32) * (x2_32 * x2_32) * x2_32 * x2_32)
    + 0.1 * x3_31
    - 0.1 * x3_32
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_33)) / (1.62079 * (0.01 + 0.3 * u33 * u33) * x2_33 * x2_33)
    + x1_32
    - x1_33
    == 0
)
model.add_constraint(
    -1.0 / x2_33
    - (cos(x3_33)) / (1.62079 * (0.01 + 0.3 * u33 * u33) * (x2_33 * x2_33) * x2_33)
    == 0
)
model.add_constraint(
    0.00437 * u33 / ((0.01 + 0.3 * u33 * u33) * (x2_33 * x2_33))
    - 0.00437
    * (cos(x3_33))
    / (1.62079 * (0.01 + 0.3 * u33 * u33) * (x2_33 * x2_33) * x2_33 * x2_33)
    + 0.1 * x3_32
    - 0.1 * x3_33
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_34)) / (1.62079 * (0.01 + 0.3 * u34 * u34) * x2_34 * x2_34)
    + x1_33
    - x1_34
    == 0
)
model.add_constraint(
    -1.0 / x2_34
    - (cos(x3_34)) / (1.62079 * (0.01 + 0.3 * u34 * u34) * (x2_34 * x2_34) * x2_34)
    == 0
)
model.add_constraint(
    0.00437 * u34 / ((0.01 + 0.3 * u34 * u34) * (x2_34 * x2_34))
    - 0.00437
    * (cos(x3_34))
    / (1.62079 * (0.01 + 0.3 * u34 * u34) * (x2_34 * x2_34) * x2_34 * x2_34)
    + 0.1 * x3_33
    - 0.1 * x3_34
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_35)) / (1.62079 * (0.01 + 0.3 * u35 * u35) * x2_35 * x2_35)
    + x1_34
    - x1_35
    == 0
)
model.add_constraint(
    -1.0 / x2_35
    - (cos(x3_35)) / (1.62079 * (0.01 + 0.3 * u35 * u35) * (x2_35 * x2_35) * x2_35)
    == 0
)
model.add_constraint(
    0.00437 * u35 / ((0.01 + 0.3 * u35 * u35) * (x2_35 * x2_35))
    - 0.00437
    * (cos(x3_35))
    / (1.62079 * (0.01 + 0.3 * u35 * u35) * (x2_35 * x2_35) * x2_35 * x2_35)
    + 0.1 * x3_34
    - 0.1 * x3_35
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_36)) / (1.62079 * (0.01 + 0.3 * u36 * u36) * x2_36 * x2_36)
    + x1_35
    - x1_36
    == 0
)
model.add_constraint(
    -1.0 / x2_36
    - (cos(x3_36)) / (1.62079 * (0.01 + 0.3 * u36 * u36) * (x2_36 * x2_36) * x2_36)
    == 0
)
model.add_constraint(
    0.00437 * u36 / ((0.01 + 0.3 * u36 * u36) * (x2_36 * x2_36))
    - 0.00437
    * (cos(x3_36))
    / (1.62079 * (0.01 + 0.3 * u36 * u36) * (x2_36 * x2_36) * x2_36 * x2_36)
    + 0.1 * x3_35
    - 0.1 * x3_36
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_37)) / (1.62079 * (0.01 + 0.3 * u37 * u37) * x2_37 * x2_37)
    + x1_36
    - x1_37
    == 0
)
model.add_constraint(
    -1.0 / x2_37
    - (cos(x3_37)) / (1.62079 * (0.01 + 0.3 * u37 * u37) * (x2_37 * x2_37) * x2_37)
    == 0
)
model.add_constraint(
    0.00437 * u37 / ((0.01 + 0.3 * u37 * u37) * (x2_37 * x2_37))
    - 0.00437
    * (cos(x3_37))
    / (1.62079 * (0.01 + 0.3 * u37 * u37) * (x2_37 * x2_37) * x2_37 * x2_37)
    + 0.1 * x3_36
    - 0.1 * x3_37
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_38)) / (1.62079 * (0.01 + 0.3 * u38 * u38) * x2_38 * x2_38)
    + x1_37
    - x1_38
    == 0
)
model.add_constraint(
    -1.0 / x2_38
    - (cos(x3_38)) / (1.62079 * (0.01 + 0.3 * u38 * u38) * (x2_38 * x2_38) * x2_38)
    == 0
)
model.add_constraint(
    0.00437 * u38 / ((0.01 + 0.3 * u38 * u38) * (x2_38 * x2_38))
    - 0.00437
    * (cos(x3_38))
    / (1.62079 * (0.01 + 0.3 * u38 * u38) * (x2_38 * x2_38) * x2_38 * x2_38)
    + 0.1 * x3_37
    - 0.1 * x3_38
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_39)) / (1.62079 * (0.01 + 0.3 * u39 * u39) * x2_39 * x2_39)
    + x1_38
    - x1_39
    == 0
)
model.add_constraint(
    -1.0 / x2_39
    - (cos(x3_39)) / (1.62079 * (0.01 + 0.3 * u39 * u39) * (x2_39 * x2_39) * x2_39)
    == 0
)
model.add_constraint(
    0.00437 * u39 / ((0.01 + 0.3 * u39 * u39) * (x2_39 * x2_39))
    - 0.00437
    * (cos(x3_39))
    / (1.62079 * (0.01 + 0.3 * u39 * u39) * (x2_39 * x2_39) * x2_39 * x2_39)
    + 0.1 * x3_38
    - 0.1 * x3_39
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_40)) / (1.62079 * (0.01 + 0.3 * u40 * u40) * x2_40 * x2_40)
    + x1_39
    - x1_40
    == 0
)
model.add_constraint(
    -1.0 / x2_40
    - (cos(x3_40)) / (1.62079 * (0.01 + 0.3 * u40 * u40) * (x2_40 * x2_40) * x2_40)
    == 0
)
model.add_constraint(
    0.00437 * u40 / ((0.01 + 0.3 * u40 * u40) * (x2_40 * x2_40))
    - 0.00437
    * (cos(x3_40))
    / (1.62079 * (0.01 + 0.3 * u40 * u40) * (x2_40 * x2_40) * x2_40 * x2_40)
    + 0.1 * x3_39
    - 0.1 * x3_40
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_41)) / (1.62079 * (0.01 + 0.3 * u41 * u41) * x2_41 * x2_41)
    + x1_40
    - x1_41
    == 0
)
model.add_constraint(
    -1.0 / x2_41
    - (cos(x3_41)) / (1.62079 * (0.01 + 0.3 * u41 * u41) * (x2_41 * x2_41) * x2_41)
    == 0
)
model.add_constraint(
    0.00437 * u41 / ((0.01 + 0.3 * u41 * u41) * (x2_41 * x2_41))
    - 0.00437
    * (cos(x3_41))
    / (1.62079 * (0.01 + 0.3 * u41 * u41) * (x2_41 * x2_41) * x2_41 * x2_41)
    + 0.1 * x3_40
    - 0.1 * x3_41
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_42)) / (1.62079 * (0.01 + 0.3 * u42 * u42) * x2_42 * x2_42)
    + x1_41
    - x1_42
    == 0
)
model.add_constraint(
    -1.0 / x2_42
    - (cos(x3_42)) / (1.62079 * (0.01 + 0.3 * u42 * u42) * (x2_42 * x2_42) * x2_42)
    == 0
)
model.add_constraint(
    0.00437 * u42 / ((0.01 + 0.3 * u42 * u42) * (x2_42 * x2_42))
    - 0.00437
    * (cos(x3_42))
    / (1.62079 * (0.01 + 0.3 * u42 * u42) * (x2_42 * x2_42) * x2_42 * x2_42)
    + 0.1 * x3_41
    - 0.1 * x3_42
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_43)) / (1.62079 * (0.01 + 0.3 * u43 * u43) * x2_43 * x2_43)
    + x1_42
    - x1_43
    == 0
)
model.add_constraint(
    -1.0 / x2_43
    - (cos(x3_43)) / (1.62079 * (0.01 + 0.3 * u43 * u43) * (x2_43 * x2_43) * x2_43)
    == 0
)
model.add_constraint(
    0.00437 * u43 / ((0.01 + 0.3 * u43 * u43) * (x2_43 * x2_43))
    - 0.00437
    * (cos(x3_43))
    / (1.62079 * (0.01 + 0.3 * u43 * u43) * (x2_43 * x2_43) * x2_43 * x2_43)
    + 0.1 * x3_42
    - 0.1 * x3_43
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_44)) / (1.62079 * (0.01 + 0.3 * u44 * u44) * x2_44 * x2_44)
    + x1_43
    - x1_44
    == 0
)
model.add_constraint(
    -1.0 / x2_44
    - (cos(x3_44)) / (1.62079 * (0.01 + 0.3 * u44 * u44) * (x2_44 * x2_44) * x2_44)
    == 0
)
model.add_constraint(
    0.00437 * u44 / ((0.01 + 0.3 * u44 * u44) * (x2_44 * x2_44))
    - 0.00437
    * (cos(x3_44))
    / (1.62079 * (0.01 + 0.3 * u44 * u44) * (x2_44 * x2_44) * x2_44 * x2_44)
    + 0.1 * x3_43
    - 0.1 * x3_44
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_45)) / (1.62079 * (0.01 + 0.3 * u45 * u45) * x2_45 * x2_45)
    + x1_44
    - x1_45
    == 0
)
model.add_constraint(
    -1.0 / x2_45
    - (cos(x3_45)) / (1.62079 * (0.01 + 0.3 * u45 * u45) * (x2_45 * x2_45) * x2_45)
    == 0
)
model.add_constraint(
    0.00437 * u45 / ((0.01 + 0.3 * u45 * u45) * (x2_45 * x2_45))
    - 0.00437
    * (cos(x3_45))
    / (1.62079 * (0.01 + 0.3 * u45 * u45) * (x2_45 * x2_45) * x2_45 * x2_45)
    + 0.1 * x3_44
    - 0.1 * x3_45
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_46)) / (1.62079 * (0.01 + 0.3 * u46 * u46) * x2_46 * x2_46)
    + x1_45
    - x1_46
    == 0
)
model.add_constraint(
    -1.0 / x2_46
    - (cos(x3_46)) / (1.62079 * (0.01 + 0.3 * u46 * u46) * (x2_46 * x2_46) * x2_46)
    == 0
)
model.add_constraint(
    0.00437 * u46 / ((0.01 + 0.3 * u46 * u46) * (x2_46 * x2_46))
    - 0.00437
    * (cos(x3_46))
    / (1.62079 * (0.01 + 0.3 * u46 * u46) * (x2_46 * x2_46) * x2_46 * x2_46)
    + 0.1 * x3_45
    - 0.1 * x3_46
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_47)) / (1.62079 * (0.01 + 0.3 * u47 * u47) * x2_47 * x2_47)
    + x1_46
    - x1_47
    == 0
)
model.add_constraint(
    -1.0 / x2_47
    - (cos(x3_47)) / (1.62079 * (0.01 + 0.3 * u47 * u47) * (x2_47 * x2_47) * x2_47)
    == 0
)
model.add_constraint(
    0.00437 * u47 / ((0.01 + 0.3 * u47 * u47) * (x2_47 * x2_47))
    - 0.00437
    * (cos(x3_47))
    / (1.62079 * (0.01 + 0.3 * u47 * u47) * (x2_47 * x2_47) * x2_47 * x2_47)
    + 0.1 * x3_46
    - 0.1 * x3_47
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_48)) / (1.62079 * (0.01 + 0.3 * u48 * u48) * x2_48 * x2_48)
    + x1_47
    - x1_48
    == 0
)
model.add_constraint(
    -1.0 / x2_48
    - (cos(x3_48)) / (1.62079 * (0.01 + 0.3 * u48 * u48) * (x2_48 * x2_48) * x2_48)
    == 0
)
model.add_constraint(
    0.00437 * u48 / ((0.01 + 0.3 * u48 * u48) * (x2_48 * x2_48))
    - 0.00437
    * (cos(x3_48))
    / (1.62079 * (0.01 + 0.3 * u48 * u48) * (x2_48 * x2_48) * x2_48 * x2_48)
    + 0.1 * x3_47
    - 0.1 * x3_48
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_49)) / (1.62079 * (0.01 + 0.3 * u49 * u49) * x2_49 * x2_49)
    + x1_48
    - x1_49
    == 0
)
model.add_constraint(
    -1.0 / x2_49
    - (cos(x3_49)) / (1.62079 * (0.01 + 0.3 * u49 * u49) * (x2_49 * x2_49) * x2_49)
    == 0
)
model.add_constraint(
    0.00437 * u49 / ((0.01 + 0.3 * u49 * u49) * (x2_49 * x2_49))
    - 0.00437
    * (cos(x3_49))
    / (1.62079 * (0.01 + 0.3 * u49 * u49) * (x2_49 * x2_49) * x2_49 * x2_49)
    + 0.1 * x3_48
    - 0.1 * x3_49
    == 0
)
model.add_constraint(
    0.00437 * (cos(x3_50)) / (1.62079 * (0.01 + 0.3 * u50 * u50) * x2_50 * x2_50)
    + x1_49
    - x1_50
    == 0
)
model.add_constraint(
    -1.0 / x2_50
    - (cos(x3_50)) / (1.62079 * (0.01 + 0.3 * u50 * u50) * (x2_50 * x2_50) * x2_50)
    == 0
)
model.add_constraint(
    0.00437 * u50 / ((0.01 + 0.3 * u50 * u50) * (x2_50 * x2_50))
    - 0.00437
    * (cos(x3_50))
    / (1.62079 * (0.01 + 0.3 * u50 * u50) * (x2_50 * x2_50) * x2_50 * x2_50)
    + 0.1 * x3_49
    - 0.1 * x3_50
    == 0
)
