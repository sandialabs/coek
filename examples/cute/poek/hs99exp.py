# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from:
#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   Source: an expanded form of problem 99 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.
#   SIF input: Ph. Toint, April 1991.
#   classification OOR2-AN-31-21
#   Problem data
#   Constants
#   Increments
#   Solution

import poek as pk
sin = pk.sin
cos = pk.cos

model = pk.model()
t1 = 0.0
t2 = 25.0
t3 = 50.0
t4 = 100.0
t5 = 150.0
t6 = 200.0
t7 = 290.0
t8 = 380.0
a1 = 0.0
a2 = 50.0
a3 = 50.0
a4 = 75.0
a5 = 75.0
a6 = 75.0
a7 = 100.0
a8 = 100.0
b = 32.0
im1 = -1 + (8)
dtisq = ((380.0) - (290.0)) * ((380.0) - (290.0))
dt2 = (25.0) - (0.0)
dtsqd22 = 0.5 * (((25.0) - (0.0)) * ((25.0) - (0.0)))
dt3 = (50.0) - (25.0)
dtsqd23 = 0.5 * (((50.0) - (25.0)) * ((50.0) - (25.0)))
dt4 = (100.0) - (50.0)
dtsqd24 = 0.5 * (((100.0) - (50.0)) * ((100.0) - (50.0)))
dt5 = (150.0) - (100.0)
dtsqd25 = 0.5 * (((150.0) - (100.0)) * ((150.0) - (100.0)))
dt6 = (200.0) - (150.0)
dtsqd26 = 0.5 * (((200.0) - (150.0)) * ((200.0) - (150.0)))
dt7 = (290.0) - (200.0)
dtsqd27 = 0.5 * (((290.0) - (200.0)) * ((290.0) - (200.0)))
dt8 = (380.0) - (290.0)
dtsqd28 = 0.5 * (((380.0) - (290.0)) * ((380.0) - (290.0)))
rhs = ((290.0) - (200.0)) * (32.0)
w = (100.0) * (0.5 * (((380.0) - (290.0)) * ((380.0) - (290.0))))

x1 = model.variable(lb=0.0, ub=1.58, value=0.5)
r1 = model.variable(lb=0.0, ub=0.0)
q1 = model.variable(lb=0.0, ub=0.0)
s1 = model.variable(lb=0.0, ub=0.0)
x2 = model.variable(lb=0.0, ub=1.58, value=0.5)
r2 = model.variable()
q2 = model.variable()
s2 = model.variable()
x3 = model.variable(lb=0.0, ub=1.58, value=0.5)
r3 = model.variable()
q3 = model.variable()
s3 = model.variable()
x4 = model.variable(lb=0.0, ub=1.58, value=0.5)
r4 = model.variable()
q4 = model.variable()
s4 = model.variable()
x5 = model.variable(lb=0.0, ub=1.58, value=0.5)
r5 = model.variable()
q5 = model.variable()
s5 = model.variable()
x6 = model.variable(lb=0.0, ub=1.58, value=0.5)
r6 = model.variable()
q6 = model.variable()
s6 = model.variable()
x7 = model.variable(lb=0.0, ub=1.58, value=0.5)
r7 = model.variable()
q7 = model.variable()
s7 = model.variable()
r8 = model.variable()
q8 = model.variable()
s8 = model.variable()

model.add( -r8*r8 )

model.add(  1250.0*(cos(x1)) - r2 + r1 == 0 )
model.add(  15625.0*(sin(x1)) - q2 + q1 + 25.0*s1 - 10000.0 == 0 )
model.add(  1250.0*(sin(x1)) - s2 + s1 - 800.0 == 0 )
model.add(  1250.0*(cos(x2)) - r3 + r2 == 0 )
model.add(  15625.0*(sin(x2)) - q3 + q2 + 25.0*s2 - 10000.0 == 0 )
model.add(  1250.0*(sin(x2)) - s3 + s2 - 800.0 == 0 )
model.add(  3750.0*(cos(x3)) - r4 + r3 == 0 )
model.add(  93750.0*(sin(x3)) - q4 + q3 + 50.0*s3 - 40000.0 == 0 )
model.add(  3750.0*(sin(x3)) - s4 + s3 - 1600.0 == 0 )
model.add(  3750.0*(cos(x4)) - r5 + r4 == 0 )
model.add(  93750.0*(sin(x4)) - q5 + q4 + 50.0*s4 - 40000.0 == 0 )
model.add(  3750.0*(sin(x4)) - s5 + s4 - 1600.0 == 0 )
model.add(  3750.0*(cos(x5)) - r6 + r5 == 0 )
model.add(  93750.0*(sin(x5)) - q6 + q5 + 50.0*s5 - 40000.0 == 0 )
model.add(  3750.0*(sin(x5)) - s6 + s5 - 1600.0 == 0 )
model.add(  9000.0*(cos(x6)) - r7 + r6 == 0 )
model.add(  405000.0*(sin(x6)) - q7 + q6 + 90.0*s6 - 129600.0 == 0 )
model.add(  9000.0*(sin(x6)) - s7 + s6 - 2880.0 == 0 )
model.add(  9000.0*(cos(x7)) - r8 + r7 == 0 )
model.add(  405000.0*(sin(x7)) - q8 + q7 + 90.0*s7 - 100000.0 == 0 )
model.add(  9000.0*(sin(x7)) - s8 + s7 - 1000.0 == 0 )
