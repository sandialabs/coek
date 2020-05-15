#  _________________________________________________________________________
#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

# Formulated in Pyomo by Gabe Hackebeil
# Taken from:

#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   An expanded system formulation of a 3-D PDE system.
#   A nine-point discretization of Laplace's equation in a 
#   rectangular domain may be expressed in the form 
#         - M v = b, 
#   where M = sum a_i a_i^T. Letting A = (a_1 .... a_m), 
#   this system may be expanded as
#          ( I   A^T ) (x) = (0),
#          ( A    0  ) (v)   (b)
#   which is then equivalentto solving the EQP 
#   minimize 1/2 || x ||_2^2   s.t.    A x = b
#   In this variant, we replace the leading I block in the
#   above formulation with a zero-one diagonal matrix D.
#   This corresponds to certain boundary conditions.
#   The resulting QP is thus convex but not strictly convex.
#   SIF input: Nick Gould, February 1994
#   classification QLR2-AN-V-V
#   Number of nodes in x direction
#IE NX                  3
#   Number of nodes in y direction
#IE NY                  3
#   Number of nodes in z direction
#IE NZ                  3
#   Other useful parameters
#  It is easier to describe this problem by columns.
# objective function
# constraints
# objective function terms
# constraints : central constraints

from pyomo.core import *
model = ConcreteModel()

nx = Param(value=10.0)
ny = Param(value=10.0)
nz = Param(value=10.0)
xp = Param(value=1.0 + (10.0))
xm = Param(value=-1.0 + (10.0))
yp = Param(value=1.0 + (10.0))
ym = Param(value=-1.0 + (10.0))
zp = Param(value=1.0 + (10.0))
zm = Param(value=-1.0 + (10.0))
m = Param(value=10.0)
n = Param(value=10.0)
p = Param(value=10.0)
kp = Param(value=1.0 + (9.0))
jp = Param(value=1.0 + (9.0))
ip = Param(value=1.0 + (9.0))

x1_1_1 = model.variable()
y1_1_1 = model.variable()
z1_1_1 = model.variable()
x2_1_1 = model.variable()
y2_1_1 = model.variable()
z2_1_1 = model.variable()
x3_1_1 = model.variable()
y3_1_1 = model.variable()
z3_1_1 = model.variable()
x4_1_1 = model.variable()
y4_1_1 = model.variable()
z4_1_1 = model.variable()
x5_1_1 = model.variable()
y5_1_1 = model.variable()
z5_1_1 = model.variable()
x6_1_1 = model.variable()
y6_1_1 = model.variable()
z6_1_1 = model.variable()
x7_1_1 = model.variable()
y7_1_1 = model.variable()
z7_1_1 = model.variable()
x8_1_1 = model.variable()
y8_1_1 = model.variable()
z8_1_1 = model.variable()
x9_1_1 = model.variable()
y9_1_1 = model.variable()
z9_1_1 = model.variable()
x1_2_1 = model.variable()
y1_2_1 = model.variable()
z1_2_1 = model.variable()
x2_2_1 = model.variable()
y2_2_1 = model.variable()
z2_2_1 = model.variable()
x3_2_1 = model.variable()
y3_2_1 = model.variable()
z3_2_1 = model.variable()
x4_2_1 = model.variable()
y4_2_1 = model.variable()
z4_2_1 = model.variable()
x5_2_1 = model.variable()
y5_2_1 = model.variable()
z5_2_1 = model.variable()
x6_2_1 = model.variable()
y6_2_1 = model.variable()
z6_2_1 = model.variable()
x7_2_1 = model.variable()
y7_2_1 = model.variable()
z7_2_1 = model.variable()
x8_2_1 = model.variable()
y8_2_1 = model.variable()
z8_2_1 = model.variable()
x9_2_1 = model.variable()
y9_2_1 = model.variable()
z9_2_1 = model.variable()
x1_3_1 = model.variable()
y1_3_1 = model.variable()
z1_3_1 = model.variable()
x2_3_1 = model.variable()
y2_3_1 = model.variable()
z2_3_1 = model.variable()
x3_3_1 = model.variable()
y3_3_1 = model.variable()
z3_3_1 = model.variable()
x4_3_1 = model.variable()
y4_3_1 = model.variable()
z4_3_1 = model.variable()
x5_3_1 = model.variable()
y5_3_1 = model.variable()
z5_3_1 = model.variable()
x6_3_1 = model.variable()
y6_3_1 = model.variable()
z6_3_1 = model.variable()
x7_3_1 = model.variable()
y7_3_1 = model.variable()
z7_3_1 = model.variable()
x8_3_1 = model.variable()
y8_3_1 = model.variable()
z8_3_1 = model.variable()
x9_3_1 = model.variable()
y9_3_1 = model.variable()
z9_3_1 = model.variable()
x1_4_1 = model.variable()
y1_4_1 = model.variable()
z1_4_1 = model.variable()
x2_4_1 = model.variable()
y2_4_1 = model.variable()
z2_4_1 = model.variable()
x3_4_1 = model.variable()
y3_4_1 = model.variable()
z3_4_1 = model.variable()
x4_4_1 = model.variable()
y4_4_1 = model.variable()
z4_4_1 = model.variable()
x5_4_1 = model.variable()
y5_4_1 = model.variable()
z5_4_1 = model.variable()
x6_4_1 = model.variable()
y6_4_1 = model.variable()
z6_4_1 = model.variable()
x7_4_1 = model.variable()
y7_4_1 = model.variable()
z7_4_1 = model.variable()
x8_4_1 = model.variable()
y8_4_1 = model.variable()
z8_4_1 = model.variable()
x9_4_1 = model.variable()
y9_4_1 = model.variable()
z9_4_1 = model.variable()
x1_5_1 = model.variable()
y1_5_1 = model.variable()
z1_5_1 = model.variable()
x2_5_1 = model.variable()
y2_5_1 = model.variable()
z2_5_1 = model.variable()
x3_5_1 = model.variable()
y3_5_1 = model.variable()
z3_5_1 = model.variable()
x4_5_1 = model.variable()
y4_5_1 = model.variable()
z4_5_1 = model.variable()
x5_5_1 = model.variable()
y5_5_1 = model.variable()
z5_5_1 = model.variable()
x6_5_1 = model.variable()
y6_5_1 = model.variable()
z6_5_1 = model.variable()
x7_5_1 = model.variable()
y7_5_1 = model.variable()
z7_5_1 = model.variable()
x8_5_1 = model.variable()
y8_5_1 = model.variable()
z8_5_1 = model.variable()
x9_5_1 = model.variable()
y9_5_1 = model.variable()
z9_5_1 = model.variable()
x1_6_1 = model.variable()
y1_6_1 = model.variable()
z1_6_1 = model.variable()
x2_6_1 = model.variable()
y2_6_1 = model.variable()
z2_6_1 = model.variable()
x3_6_1 = model.variable()
y3_6_1 = model.variable()
z3_6_1 = model.variable()
x4_6_1 = model.variable()
y4_6_1 = model.variable()
z4_6_1 = model.variable()
x5_6_1 = model.variable()
y5_6_1 = model.variable()
z5_6_1 = model.variable()
x6_6_1 = model.variable()
y6_6_1 = model.variable()
z6_6_1 = model.variable()
x7_6_1 = model.variable()
y7_6_1 = model.variable()
z7_6_1 = model.variable()
x8_6_1 = model.variable()
y8_6_1 = model.variable()
z8_6_1 = model.variable()
x9_6_1 = model.variable()
y9_6_1 = model.variable()
z9_6_1 = model.variable()
x1_7_1 = model.variable()
y1_7_1 = model.variable()
z1_7_1 = model.variable()
x2_7_1 = model.variable()
y2_7_1 = model.variable()
z2_7_1 = model.variable()
x3_7_1 = model.variable()
y3_7_1 = model.variable()
z3_7_1 = model.variable()
x4_7_1 = model.variable()
y4_7_1 = model.variable()
z4_7_1 = model.variable()
x5_7_1 = model.variable()
y5_7_1 = model.variable()
z5_7_1 = model.variable()
x6_7_1 = model.variable()
y6_7_1 = model.variable()
z6_7_1 = model.variable()
x7_7_1 = model.variable()
y7_7_1 = model.variable()
z7_7_1 = model.variable()
x8_7_1 = model.variable()
y8_7_1 = model.variable()
z8_7_1 = model.variable()
x9_7_1 = model.variable()
y9_7_1 = model.variable()
z9_7_1 = model.variable()
x1_8_1 = model.variable()
y1_8_1 = model.variable()
z1_8_1 = model.variable()
x2_8_1 = model.variable()
y2_8_1 = model.variable()
z2_8_1 = model.variable()
x3_8_1 = model.variable()
y3_8_1 = model.variable()
z3_8_1 = model.variable()
x4_8_1 = model.variable()
y4_8_1 = model.variable()
z4_8_1 = model.variable()
x5_8_1 = model.variable()
y5_8_1 = model.variable()
z5_8_1 = model.variable()
x6_8_1 = model.variable()
y6_8_1 = model.variable()
z6_8_1 = model.variable()
x7_8_1 = model.variable()
y7_8_1 = model.variable()
z7_8_1 = model.variable()
x8_8_1 = model.variable()
y8_8_1 = model.variable()
z8_8_1 = model.variable()
x9_8_1 = model.variable()
y9_8_1 = model.variable()
z9_8_1 = model.variable()
x1_9_1 = model.variable()
y1_9_1 = model.variable()
z1_9_1 = model.variable()
x2_9_1 = model.variable()
y2_9_1 = model.variable()
z2_9_1 = model.variable()
x3_9_1 = model.variable()
y3_9_1 = model.variable()
z3_9_1 = model.variable()
x4_9_1 = model.variable()
y4_9_1 = model.variable()
z4_9_1 = model.variable()
x5_9_1 = model.variable()
y5_9_1 = model.variable()
z5_9_1 = model.variable()
x6_9_1 = model.variable()
y6_9_1 = model.variable()
z6_9_1 = model.variable()
x7_9_1 = model.variable()
y7_9_1 = model.variable()
z7_9_1 = model.variable()
x8_9_1 = model.variable()
y8_9_1 = model.variable()
z8_9_1 = model.variable()
x9_9_1 = model.variable()
y9_9_1 = model.variable()
z9_9_1 = model.variable()
x1_1_2 = model.variable()
y1_1_2 = model.variable()
z1_1_2 = model.variable()
x2_1_2 = model.variable()
y2_1_2 = model.variable()
z2_1_2 = model.variable()
x3_1_2 = model.variable()
y3_1_2 = model.variable()
z3_1_2 = model.variable()
x4_1_2 = model.variable()
y4_1_2 = model.variable()
z4_1_2 = model.variable()
x5_1_2 = model.variable()
y5_1_2 = model.variable()
z5_1_2 = model.variable()
x6_1_2 = model.variable()
y6_1_2 = model.variable()
z6_1_2 = model.variable()
x7_1_2 = model.variable()
y7_1_2 = model.variable()
z7_1_2 = model.variable()
x8_1_2 = model.variable()
y8_1_2 = model.variable()
z8_1_2 = model.variable()
x9_1_2 = model.variable()
y9_1_2 = model.variable()
z9_1_2 = model.variable()
x1_2_2 = model.variable()
y1_2_2 = model.variable()
z1_2_2 = model.variable()
x2_2_2 = model.variable()
y2_2_2 = model.variable()
z2_2_2 = model.variable()
x3_2_2 = model.variable()
y3_2_2 = model.variable()
z3_2_2 = model.variable()
x4_2_2 = model.variable()
y4_2_2 = model.variable()
z4_2_2 = model.variable()
x5_2_2 = model.variable()
y5_2_2 = model.variable()
z5_2_2 = model.variable()
x6_2_2 = model.variable()
y6_2_2 = model.variable()
z6_2_2 = model.variable()
x7_2_2 = model.variable()
y7_2_2 = model.variable()
z7_2_2 = model.variable()
x8_2_2 = model.variable()
y8_2_2 = model.variable()
z8_2_2 = model.variable()
x9_2_2 = model.variable()
y9_2_2 = model.variable()
z9_2_2 = model.variable()
x1_3_2 = model.variable()
y1_3_2 = model.variable()
z1_3_2 = model.variable()
x2_3_2 = model.variable()
y2_3_2 = model.variable()
z2_3_2 = model.variable()
x3_3_2 = model.variable()
y3_3_2 = model.variable()
z3_3_2 = model.variable()
x4_3_2 = model.variable()
y4_3_2 = model.variable()
z4_3_2 = model.variable()
x5_3_2 = model.variable()
y5_3_2 = model.variable()
z5_3_2 = model.variable()
x6_3_2 = model.variable()
y6_3_2 = model.variable()
z6_3_2 = model.variable()
x7_3_2 = model.variable()
y7_3_2 = model.variable()
z7_3_2 = model.variable()
x8_3_2 = model.variable()
y8_3_2 = model.variable()
z8_3_2 = model.variable()
x9_3_2 = model.variable()
y9_3_2 = model.variable()
z9_3_2 = model.variable()
x1_4_2 = model.variable()
y1_4_2 = model.variable()
z1_4_2 = model.variable()
x2_4_2 = model.variable()
y2_4_2 = model.variable()
z2_4_2 = model.variable()
x3_4_2 = model.variable()
y3_4_2 = model.variable()
z3_4_2 = model.variable()
x4_4_2 = model.variable()
y4_4_2 = model.variable()
z4_4_2 = model.variable()
x5_4_2 = model.variable()
y5_4_2 = model.variable()
z5_4_2 = model.variable()
x6_4_2 = model.variable()
y6_4_2 = model.variable()
z6_4_2 = model.variable()
x7_4_2 = model.variable()
y7_4_2 = model.variable()
z7_4_2 = model.variable()
x8_4_2 = model.variable()
y8_4_2 = model.variable()
z8_4_2 = model.variable()
x9_4_2 = model.variable()
y9_4_2 = model.variable()
z9_4_2 = model.variable()
x1_5_2 = model.variable()
y1_5_2 = model.variable()
z1_5_2 = model.variable()
x2_5_2 = model.variable()
y2_5_2 = model.variable()
z2_5_2 = model.variable()
x3_5_2 = model.variable()
y3_5_2 = model.variable()
z3_5_2 = model.variable()
x4_5_2 = model.variable()
y4_5_2 = model.variable()
z4_5_2 = model.variable()
x5_5_2 = model.variable()
y5_5_2 = model.variable()
z5_5_2 = model.variable()
x6_5_2 = model.variable()
y6_5_2 = model.variable()
z6_5_2 = model.variable()
x7_5_2 = model.variable()
y7_5_2 = model.variable()
z7_5_2 = model.variable()
x8_5_2 = model.variable()
y8_5_2 = model.variable()
z8_5_2 = model.variable()
x9_5_2 = model.variable()
y9_5_2 = model.variable()
z9_5_2 = model.variable()
x1_6_2 = model.variable()
y1_6_2 = model.variable()
z1_6_2 = model.variable()
x2_6_2 = model.variable()
y2_6_2 = model.variable()
z2_6_2 = model.variable()
x3_6_2 = model.variable()
y3_6_2 = model.variable()
z3_6_2 = model.variable()
x4_6_2 = model.variable()
y4_6_2 = model.variable()
z4_6_2 = model.variable()
x5_6_2 = model.variable()
y5_6_2 = model.variable()
z5_6_2 = model.variable()
x6_6_2 = model.variable()
y6_6_2 = model.variable()
z6_6_2 = model.variable()
x7_6_2 = model.variable()
y7_6_2 = model.variable()
z7_6_2 = model.variable()
x8_6_2 = model.variable()
y8_6_2 = model.variable()
z8_6_2 = model.variable()
x9_6_2 = model.variable()
y9_6_2 = model.variable()
z9_6_2 = model.variable()
x1_7_2 = model.variable()
y1_7_2 = model.variable()
z1_7_2 = model.variable()
x2_7_2 = model.variable()
y2_7_2 = model.variable()
z2_7_2 = model.variable()
x3_7_2 = model.variable()
y3_7_2 = model.variable()
z3_7_2 = model.variable()
x4_7_2 = model.variable()
y4_7_2 = model.variable()
z4_7_2 = model.variable()
x5_7_2 = model.variable()
y5_7_2 = model.variable()
z5_7_2 = model.variable()
x6_7_2 = model.variable()
y6_7_2 = model.variable()
z6_7_2 = model.variable()
x7_7_2 = model.variable()
y7_7_2 = model.variable()
z7_7_2 = model.variable()
x8_7_2 = model.variable()
y8_7_2 = model.variable()
z8_7_2 = model.variable()
x9_7_2 = model.variable()
y9_7_2 = model.variable()
z9_7_2 = model.variable()
x1_8_2 = model.variable()
y1_8_2 = model.variable()
z1_8_2 = model.variable()
x2_8_2 = model.variable()
y2_8_2 = model.variable()
z2_8_2 = model.variable()
x3_8_2 = model.variable()
y3_8_2 = model.variable()
z3_8_2 = model.variable()
x4_8_2 = model.variable()
y4_8_2 = model.variable()
z4_8_2 = model.variable()
x5_8_2 = model.variable()
y5_8_2 = model.variable()
z5_8_2 = model.variable()
x6_8_2 = model.variable()
y6_8_2 = model.variable()
z6_8_2 = model.variable()
x7_8_2 = model.variable()
y7_8_2 = model.variable()
z7_8_2 = model.variable()
x8_8_2 = model.variable()
y8_8_2 = model.variable()
z8_8_2 = model.variable()
x9_8_2 = model.variable()
y9_8_2 = model.variable()
z9_8_2 = model.variable()
x1_9_2 = model.variable()
y1_9_2 = model.variable()
z1_9_2 = model.variable()
x2_9_2 = model.variable()
y2_9_2 = model.variable()
z2_9_2 = model.variable()
x3_9_2 = model.variable()
y3_9_2 = model.variable()
z3_9_2 = model.variable()
x4_9_2 = model.variable()
y4_9_2 = model.variable()
z4_9_2 = model.variable()
x5_9_2 = model.variable()
y5_9_2 = model.variable()
z5_9_2 = model.variable()
x6_9_2 = model.variable()
y6_9_2 = model.variable()
z6_9_2 = model.variable()
x7_9_2 = model.variable()
y7_9_2 = model.variable()
z7_9_2 = model.variable()
x8_9_2 = model.variable()
y8_9_2 = model.variable()
z8_9_2 = model.variable()
x9_9_2 = model.variable()
y9_9_2 = model.variable()
z9_9_2 = model.variable()
x1_1_3 = model.variable()
y1_1_3 = model.variable()
z1_1_3 = model.variable()
x2_1_3 = model.variable()
y2_1_3 = model.variable()
z2_1_3 = model.variable()
x3_1_3 = model.variable()
y3_1_3 = model.variable()
z3_1_3 = model.variable()
x4_1_3 = model.variable()
y4_1_3 = model.variable()
z4_1_3 = model.variable()
x5_1_3 = model.variable()
y5_1_3 = model.variable()
z5_1_3 = model.variable()
x6_1_3 = model.variable()
y6_1_3 = model.variable()
z6_1_3 = model.variable()
x7_1_3 = model.variable()
y7_1_3 = model.variable()
z7_1_3 = model.variable()
x8_1_3 = model.variable()
y8_1_3 = model.variable()
z8_1_3 = model.variable()
x9_1_3 = model.variable()
y9_1_3 = model.variable()
z9_1_3 = model.variable()
x1_2_3 = model.variable()
y1_2_3 = model.variable()
z1_2_3 = model.variable()
x2_2_3 = model.variable()
y2_2_3 = model.variable()
z2_2_3 = model.variable()
x3_2_3 = model.variable()
y3_2_3 = model.variable()
z3_2_3 = model.variable()
x4_2_3 = model.variable()
y4_2_3 = model.variable()
z4_2_3 = model.variable()
x5_2_3 = model.variable()
y5_2_3 = model.variable()
z5_2_3 = model.variable()
x6_2_3 = model.variable()
y6_2_3 = model.variable()
z6_2_3 = model.variable()
x7_2_3 = model.variable()
y7_2_3 = model.variable()
z7_2_3 = model.variable()
x8_2_3 = model.variable()
y8_2_3 = model.variable()
z8_2_3 = model.variable()
x9_2_3 = model.variable()
y9_2_3 = model.variable()
z9_2_3 = model.variable()
x1_3_3 = model.variable()
y1_3_3 = model.variable()
z1_3_3 = model.variable()
x2_3_3 = model.variable()
y2_3_3 = model.variable()
z2_3_3 = model.variable()
x3_3_3 = model.variable()
y3_3_3 = model.variable()
z3_3_3 = model.variable()
x4_3_3 = model.variable()
y4_3_3 = model.variable()
z4_3_3 = model.variable()
x5_3_3 = model.variable()
y5_3_3 = model.variable()
z5_3_3 = model.variable()
x6_3_3 = model.variable()
y6_3_3 = model.variable()
z6_3_3 = model.variable()
x7_3_3 = model.variable()
y7_3_3 = model.variable()
z7_3_3 = model.variable()
x8_3_3 = model.variable()
y8_3_3 = model.variable()
z8_3_3 = model.variable()
x9_3_3 = model.variable()
y9_3_3 = model.variable()
z9_3_3 = model.variable()
x1_4_3 = model.variable()
y1_4_3 = model.variable()
z1_4_3 = model.variable()
x2_4_3 = model.variable()
y2_4_3 = model.variable()
z2_4_3 = model.variable()
x3_4_3 = model.variable()
y3_4_3 = model.variable()
z3_4_3 = model.variable()
x4_4_3 = model.variable()
y4_4_3 = model.variable()
z4_4_3 = model.variable()
x5_4_3 = model.variable()
y5_4_3 = model.variable()
z5_4_3 = model.variable()
x6_4_3 = model.variable()
y6_4_3 = model.variable()
z6_4_3 = model.variable()
x7_4_3 = model.variable()
y7_4_3 = model.variable()
z7_4_3 = model.variable()
x8_4_3 = model.variable()
y8_4_3 = model.variable()
z8_4_3 = model.variable()
x9_4_3 = model.variable()
y9_4_3 = model.variable()
z9_4_3 = model.variable()
x1_5_3 = model.variable()
y1_5_3 = model.variable()
z1_5_3 = model.variable()
x2_5_3 = model.variable()
y2_5_3 = model.variable()
z2_5_3 = model.variable()
x3_5_3 = model.variable()
y3_5_3 = model.variable()
z3_5_3 = model.variable()
x4_5_3 = model.variable()
y4_5_3 = model.variable()
z4_5_3 = model.variable()
x5_5_3 = model.variable()
y5_5_3 = model.variable()
z5_5_3 = model.variable()
x6_5_3 = model.variable()
y6_5_3 = model.variable()
z6_5_3 = model.variable()
x7_5_3 = model.variable()
y7_5_3 = model.variable()
z7_5_3 = model.variable()
x8_5_3 = model.variable()
y8_5_3 = model.variable()
z8_5_3 = model.variable()
x9_5_3 = model.variable()
y9_5_3 = model.variable()
z9_5_3 = model.variable()
x1_6_3 = model.variable()
y1_6_3 = model.variable()
z1_6_3 = model.variable()
x2_6_3 = model.variable()
y2_6_3 = model.variable()
z2_6_3 = model.variable()
x3_6_3 = model.variable()
y3_6_3 = model.variable()
z3_6_3 = model.variable()
x4_6_3 = model.variable()
y4_6_3 = model.variable()
z4_6_3 = model.variable()
x5_6_3 = model.variable()
y5_6_3 = model.variable()
z5_6_3 = model.variable()
x6_6_3 = model.variable()
y6_6_3 = model.variable()
z6_6_3 = model.variable()
x7_6_3 = model.variable()
y7_6_3 = model.variable()
z7_6_3 = model.variable()
x8_6_3 = model.variable()
y8_6_3 = model.variable()
z8_6_3 = model.variable()
x9_6_3 = model.variable()
y9_6_3 = model.variable()
z9_6_3 = model.variable()
x1_7_3 = model.variable()
y1_7_3 = model.variable()
z1_7_3 = model.variable()
x2_7_3 = model.variable()
y2_7_3 = model.variable()
z2_7_3 = model.variable()
x3_7_3 = model.variable()
y3_7_3 = model.variable()
z3_7_3 = model.variable()
x4_7_3 = model.variable()
y4_7_3 = model.variable()
z4_7_3 = model.variable()
x5_7_3 = model.variable()
y5_7_3 = model.variable()
z5_7_3 = model.variable()
x6_7_3 = model.variable()
y6_7_3 = model.variable()
z6_7_3 = model.variable()
x7_7_3 = model.variable()
y7_7_3 = model.variable()
z7_7_3 = model.variable()
x8_7_3 = model.variable()
y8_7_3 = model.variable()
z8_7_3 = model.variable()
x9_7_3 = model.variable()
y9_7_3 = model.variable()
z9_7_3 = model.variable()
x1_8_3 = model.variable()
y1_8_3 = model.variable()
z1_8_3 = model.variable()
x2_8_3 = model.variable()
y2_8_3 = model.variable()
z2_8_3 = model.variable()
x3_8_3 = model.variable()
y3_8_3 = model.variable()
z3_8_3 = model.variable()
x4_8_3 = model.variable()
y4_8_3 = model.variable()
z4_8_3 = model.variable()
x5_8_3 = model.variable()
y5_8_3 = model.variable()
z5_8_3 = model.variable()
x6_8_3 = model.variable()
y6_8_3 = model.variable()
z6_8_3 = model.variable()
x7_8_3 = model.variable()
y7_8_3 = model.variable()
z7_8_3 = model.variable()
x8_8_3 = model.variable()
y8_8_3 = model.variable()
z8_8_3 = model.variable()
x9_8_3 = model.variable()
y9_8_3 = model.variable()
z9_8_3 = model.variable()
x1_9_3 = model.variable()
y1_9_3 = model.variable()
z1_9_3 = model.variable()
x2_9_3 = model.variable()
y2_9_3 = model.variable()
z2_9_3 = model.variable()
x3_9_3 = model.variable()
y3_9_3 = model.variable()
z3_9_3 = model.variable()
x4_9_3 = model.variable()
y4_9_3 = model.variable()
z4_9_3 = model.variable()
x5_9_3 = model.variable()
y5_9_3 = model.variable()
z5_9_3 = model.variable()
x6_9_3 = model.variable()
y6_9_3 = model.variable()
z6_9_3 = model.variable()
x7_9_3 = model.variable()
y7_9_3 = model.variable()
z7_9_3 = model.variable()
x8_9_3 = model.variable()
y8_9_3 = model.variable()
z8_9_3 = model.variable()
x9_9_3 = model.variable()
y9_9_3 = model.variable()
z9_9_3 = model.variable()
x1_1_4 = model.variable()
y1_1_4 = model.variable()
z1_1_4 = model.variable()
x2_1_4 = model.variable()
y2_1_4 = model.variable()
z2_1_4 = model.variable()
x3_1_4 = model.variable()
y3_1_4 = model.variable()
z3_1_4 = model.variable()
x4_1_4 = model.variable()
y4_1_4 = model.variable()
z4_1_4 = model.variable()
x5_1_4 = model.variable()
y5_1_4 = model.variable()
z5_1_4 = model.variable()
x6_1_4 = model.variable()
y6_1_4 = model.variable()
z6_1_4 = model.variable()
x7_1_4 = model.variable()
y7_1_4 = model.variable()
z7_1_4 = model.variable()
x8_1_4 = model.variable()
y8_1_4 = model.variable()
z8_1_4 = model.variable()
x9_1_4 = model.variable()
y9_1_4 = model.variable()
z9_1_4 = model.variable()
x1_2_4 = model.variable()
y1_2_4 = model.variable()
z1_2_4 = model.variable()
x2_2_4 = model.variable()
y2_2_4 = model.variable()
z2_2_4 = model.variable()
x3_2_4 = model.variable()
y3_2_4 = model.variable()
z3_2_4 = model.variable()
x4_2_4 = model.variable()
y4_2_4 = model.variable()
z4_2_4 = model.variable()
x5_2_4 = model.variable()
y5_2_4 = model.variable()
z5_2_4 = model.variable()
x6_2_4 = model.variable()
y6_2_4 = model.variable()
z6_2_4 = model.variable()
x7_2_4 = model.variable()
y7_2_4 = model.variable()
z7_2_4 = model.variable()
x8_2_4 = model.variable()
y8_2_4 = model.variable()
z8_2_4 = model.variable()
x9_2_4 = model.variable()
y9_2_4 = model.variable()
z9_2_4 = model.variable()
x1_3_4 = model.variable()
y1_3_4 = model.variable()
z1_3_4 = model.variable()
x2_3_4 = model.variable()
y2_3_4 = model.variable()
z2_3_4 = model.variable()
x3_3_4 = model.variable()
y3_3_4 = model.variable()
z3_3_4 = model.variable()
x4_3_4 = model.variable()
y4_3_4 = model.variable()
z4_3_4 = model.variable()
x5_3_4 = model.variable()
y5_3_4 = model.variable()
z5_3_4 = model.variable()
x6_3_4 = model.variable()
y6_3_4 = model.variable()
z6_3_4 = model.variable()
x7_3_4 = model.variable()
y7_3_4 = model.variable()
z7_3_4 = model.variable()
x8_3_4 = model.variable()
y8_3_4 = model.variable()
z8_3_4 = model.variable()
x9_3_4 = model.variable()
y9_3_4 = model.variable()
z9_3_4 = model.variable()
x1_4_4 = model.variable()
y1_4_4 = model.variable()
z1_4_4 = model.variable()
x2_4_4 = model.variable()
y2_4_4 = model.variable()
z2_4_4 = model.variable()
x3_4_4 = model.variable()
y3_4_4 = model.variable()
z3_4_4 = model.variable()
x4_4_4 = model.variable()
y4_4_4 = model.variable()
z4_4_4 = model.variable()
x5_4_4 = model.variable()
y5_4_4 = model.variable()
z5_4_4 = model.variable()
x6_4_4 = model.variable()
y6_4_4 = model.variable()
z6_4_4 = model.variable()
x7_4_4 = model.variable()
y7_4_4 = model.variable()
z7_4_4 = model.variable()
x8_4_4 = model.variable()
y8_4_4 = model.variable()
z8_4_4 = model.variable()
x9_4_4 = model.variable()
y9_4_4 = model.variable()
z9_4_4 = model.variable()
x1_5_4 = model.variable()
y1_5_4 = model.variable()
z1_5_4 = model.variable()
x2_5_4 = model.variable()
y2_5_4 = model.variable()
z2_5_4 = model.variable()
x3_5_4 = model.variable()
y3_5_4 = model.variable()
z3_5_4 = model.variable()
x4_5_4 = model.variable()
y4_5_4 = model.variable()
z4_5_4 = model.variable()
x5_5_4 = model.variable()
y5_5_4 = model.variable()
z5_5_4 = model.variable()
x6_5_4 = model.variable()
y6_5_4 = model.variable()
z6_5_4 = model.variable()
x7_5_4 = model.variable()
y7_5_4 = model.variable()
z7_5_4 = model.variable()
x8_5_4 = model.variable()
y8_5_4 = model.variable()
z8_5_4 = model.variable()
x9_5_4 = model.variable()
y9_5_4 = model.variable()
z9_5_4 = model.variable()
x1_6_4 = model.variable()
y1_6_4 = model.variable()
z1_6_4 = model.variable()
x2_6_4 = model.variable()
y2_6_4 = model.variable()
z2_6_4 = model.variable()
x3_6_4 = model.variable()
y3_6_4 = model.variable()
z3_6_4 = model.variable()
x4_6_4 = model.variable()
y4_6_4 = model.variable()
z4_6_4 = model.variable()
x5_6_4 = model.variable()
y5_6_4 = model.variable()
z5_6_4 = model.variable()
x6_6_4 = model.variable()
y6_6_4 = model.variable()
z6_6_4 = model.variable()
x7_6_4 = model.variable()
y7_6_4 = model.variable()
z7_6_4 = model.variable()
x8_6_4 = model.variable()
y8_6_4 = model.variable()
z8_6_4 = model.variable()
x9_6_4 = model.variable()
y9_6_4 = model.variable()
z9_6_4 = model.variable()
x1_7_4 = model.variable()
y1_7_4 = model.variable()
z1_7_4 = model.variable()
x2_7_4 = model.variable()
y2_7_4 = model.variable()
z2_7_4 = model.variable()
x3_7_4 = model.variable()
y3_7_4 = model.variable()
z3_7_4 = model.variable()
x4_7_4 = model.variable()
y4_7_4 = model.variable()
z4_7_4 = model.variable()
x5_7_4 = model.variable()
y5_7_4 = model.variable()
z5_7_4 = model.variable()
x6_7_4 = model.variable()
y6_7_4 = model.variable()
z6_7_4 = model.variable()
x7_7_4 = model.variable()
y7_7_4 = model.variable()
z7_7_4 = model.variable()
x8_7_4 = model.variable()
y8_7_4 = model.variable()
z8_7_4 = model.variable()
x9_7_4 = model.variable()
y9_7_4 = model.variable()
z9_7_4 = model.variable()
x1_8_4 = model.variable()
y1_8_4 = model.variable()
z1_8_4 = model.variable()
x2_8_4 = model.variable()
y2_8_4 = model.variable()
z2_8_4 = model.variable()
x3_8_4 = model.variable()
y3_8_4 = model.variable()
z3_8_4 = model.variable()
x4_8_4 = model.variable()
y4_8_4 = model.variable()
z4_8_4 = model.variable()
x5_8_4 = model.variable()
y5_8_4 = model.variable()
z5_8_4 = model.variable()
x6_8_4 = model.variable()
y6_8_4 = model.variable()
z6_8_4 = model.variable()
x7_8_4 = model.variable()
y7_8_4 = model.variable()
z7_8_4 = model.variable()
x8_8_4 = model.variable()
y8_8_4 = model.variable()
z8_8_4 = model.variable()
x9_8_4 = model.variable()
y9_8_4 = model.variable()
z9_8_4 = model.variable()
x1_9_4 = model.variable()
y1_9_4 = model.variable()
z1_9_4 = model.variable()
x2_9_4 = model.variable()
y2_9_4 = model.variable()
z2_9_4 = model.variable()
x3_9_4 = model.variable()
y3_9_4 = model.variable()
z3_9_4 = model.variable()
x4_9_4 = model.variable()
y4_9_4 = model.variable()
z4_9_4 = model.variable()
x5_9_4 = model.variable()
y5_9_4 = model.variable()
z5_9_4 = model.variable()
x6_9_4 = model.variable()
y6_9_4 = model.variable()
z6_9_4 = model.variable()
x7_9_4 = model.variable()
y7_9_4 = model.variable()
z7_9_4 = model.variable()
x8_9_4 = model.variable()
y8_9_4 = model.variable()
z8_9_4 = model.variable()
x9_9_4 = model.variable()
y9_9_4 = model.variable()
z9_9_4 = model.variable()
x1_1_5 = model.variable()
y1_1_5 = model.variable()
z1_1_5 = model.variable()
x2_1_5 = model.variable()
y2_1_5 = model.variable()
z2_1_5 = model.variable()
x3_1_5 = model.variable()
y3_1_5 = model.variable()
z3_1_5 = model.variable()
x4_1_5 = model.variable()
y4_1_5 = model.variable()
z4_1_5 = model.variable()
x5_1_5 = model.variable()
y5_1_5 = model.variable()
z5_1_5 = model.variable()
x6_1_5 = model.variable()
y6_1_5 = model.variable()
z6_1_5 = model.variable()
x7_1_5 = model.variable()
y7_1_5 = model.variable()
z7_1_5 = model.variable()
x8_1_5 = model.variable()
y8_1_5 = model.variable()
z8_1_5 = model.variable()
x9_1_5 = model.variable()
y9_1_5 = model.variable()
z9_1_5 = model.variable()
x1_2_5 = model.variable()
y1_2_5 = model.variable()
z1_2_5 = model.variable()
x2_2_5 = model.variable()
y2_2_5 = model.variable()
z2_2_5 = model.variable()
x3_2_5 = model.variable()
y3_2_5 = model.variable()
z3_2_5 = model.variable()
x4_2_5 = model.variable()
y4_2_5 = model.variable()
z4_2_5 = model.variable()
x5_2_5 = model.variable()
y5_2_5 = model.variable()
z5_2_5 = model.variable()
x6_2_5 = model.variable()
y6_2_5 = model.variable()
z6_2_5 = model.variable()
x7_2_5 = model.variable()
y7_2_5 = model.variable()
z7_2_5 = model.variable()
x8_2_5 = model.variable()
y8_2_5 = model.variable()
z8_2_5 = model.variable()
x9_2_5 = model.variable()
y9_2_5 = model.variable()
z9_2_5 = model.variable()
x1_3_5 = model.variable()
y1_3_5 = model.variable()
z1_3_5 = model.variable()
x2_3_5 = model.variable()
y2_3_5 = model.variable()
z2_3_5 = model.variable()
x3_3_5 = model.variable()
y3_3_5 = model.variable()
z3_3_5 = model.variable()
x4_3_5 = model.variable()
y4_3_5 = model.variable()
z4_3_5 = model.variable()
x5_3_5 = model.variable()
y5_3_5 = model.variable()
z5_3_5 = model.variable()
x6_3_5 = model.variable()
y6_3_5 = model.variable()
z6_3_5 = model.variable()
x7_3_5 = model.variable()
y7_3_5 = model.variable()
z7_3_5 = model.variable()
x8_3_5 = model.variable()
y8_3_5 = model.variable()
z8_3_5 = model.variable()
x9_3_5 = model.variable()
y9_3_5 = model.variable()
z9_3_5 = model.variable()
x1_4_5 = model.variable()
y1_4_5 = model.variable()
z1_4_5 = model.variable()
x2_4_5 = model.variable()
y2_4_5 = model.variable()
z2_4_5 = model.variable()
x3_4_5 = model.variable()
y3_4_5 = model.variable()
z3_4_5 = model.variable()
x4_4_5 = model.variable()
y4_4_5 = model.variable()
z4_4_5 = model.variable()
x5_4_5 = model.variable()
y5_4_5 = model.variable()
z5_4_5 = model.variable()
x6_4_5 = model.variable()
y6_4_5 = model.variable()
z6_4_5 = model.variable()
x7_4_5 = model.variable()
y7_4_5 = model.variable()
z7_4_5 = model.variable()
x8_4_5 = model.variable()
y8_4_5 = model.variable()
z8_4_5 = model.variable()
x9_4_5 = model.variable()
y9_4_5 = model.variable()
z9_4_5 = model.variable()
x1_5_5 = model.variable()
y1_5_5 = model.variable()
z1_5_5 = model.variable()
x2_5_5 = model.variable()
y2_5_5 = model.variable()
z2_5_5 = model.variable()
x3_5_5 = model.variable()
y3_5_5 = model.variable()
z3_5_5 = model.variable()
x4_5_5 = model.variable()
y4_5_5 = model.variable()
z4_5_5 = model.variable()
x5_5_5 = model.variable()
y5_5_5 = model.variable()
z5_5_5 = model.variable()
x6_5_5 = model.variable()
y6_5_5 = model.variable()
z6_5_5 = model.variable()
x7_5_5 = model.variable()
y7_5_5 = model.variable()
z7_5_5 = model.variable()
x8_5_5 = model.variable()
y8_5_5 = model.variable()
z8_5_5 = model.variable()
x9_5_5 = model.variable()
y9_5_5 = model.variable()
z9_5_5 = model.variable()
x1_6_5 = model.variable()
y1_6_5 = model.variable()
z1_6_5 = model.variable()
x2_6_5 = model.variable()
y2_6_5 = model.variable()
z2_6_5 = model.variable()
x3_6_5 = model.variable()
y3_6_5 = model.variable()
z3_6_5 = model.variable()
x4_6_5 = model.variable()
y4_6_5 = model.variable()
z4_6_5 = model.variable()
x5_6_5 = model.variable()
y5_6_5 = model.variable()
z5_6_5 = model.variable()
x6_6_5 = model.variable()
y6_6_5 = model.variable()
z6_6_5 = model.variable()
x7_6_5 = model.variable()
y7_6_5 = model.variable()
z7_6_5 = model.variable()
x8_6_5 = model.variable()
y8_6_5 = model.variable()
z8_6_5 = model.variable()
x9_6_5 = model.variable()
y9_6_5 = model.variable()
z9_6_5 = model.variable()
x1_7_5 = model.variable()
y1_7_5 = model.variable()
z1_7_5 = model.variable()
x2_7_5 = model.variable()
y2_7_5 = model.variable()
z2_7_5 = model.variable()
x3_7_5 = model.variable()
y3_7_5 = model.variable()
z3_7_5 = model.variable()
x4_7_5 = model.variable()
y4_7_5 = model.variable()
z4_7_5 = model.variable()
x5_7_5 = model.variable()
y5_7_5 = model.variable()
z5_7_5 = model.variable()
x6_7_5 = model.variable()
y6_7_5 = model.variable()
z6_7_5 = model.variable()
x7_7_5 = model.variable()
y7_7_5 = model.variable()
z7_7_5 = model.variable()
x8_7_5 = model.variable()
y8_7_5 = model.variable()
z8_7_5 = model.variable()
x9_7_5 = model.variable()
y9_7_5 = model.variable()
z9_7_5 = model.variable()
x1_8_5 = model.variable()
y1_8_5 = model.variable()
z1_8_5 = model.variable()
x2_8_5 = model.variable()
y2_8_5 = model.variable()
z2_8_5 = model.variable()
x3_8_5 = model.variable()
y3_8_5 = model.variable()
z3_8_5 = model.variable()
x4_8_5 = model.variable()
y4_8_5 = model.variable()
z4_8_5 = model.variable()
x5_8_5 = model.variable()
y5_8_5 = model.variable()
z5_8_5 = model.variable()
x6_8_5 = model.variable()
y6_8_5 = model.variable()
z6_8_5 = model.variable()
x7_8_5 = model.variable()
y7_8_5 = model.variable()
z7_8_5 = model.variable()
x8_8_5 = model.variable()
y8_8_5 = model.variable()
z8_8_5 = model.variable()
x9_8_5 = model.variable()
y9_8_5 = model.variable()
z9_8_5 = model.variable()
x1_9_5 = model.variable()
y1_9_5 = model.variable()
z1_9_5 = model.variable()
x2_9_5 = model.variable()
y2_9_5 = model.variable()
z2_9_5 = model.variable()
x3_9_5 = model.variable()
y3_9_5 = model.variable()
z3_9_5 = model.variable()
x4_9_5 = model.variable()
y4_9_5 = model.variable()
z4_9_5 = model.variable()
x5_9_5 = model.variable()
y5_9_5 = model.variable()
z5_9_5 = model.variable()
x6_9_5 = model.variable()
y6_9_5 = model.variable()
z6_9_5 = model.variable()
x7_9_5 = model.variable()
y7_9_5 = model.variable()
z7_9_5 = model.variable()
x8_9_5 = model.variable()
y8_9_5 = model.variable()
z8_9_5 = model.variable()
x9_9_5 = model.variable()
y9_9_5 = model.variable()
z9_9_5 = model.variable()
x1_1_6 = model.variable()
y1_1_6 = model.variable()
z1_1_6 = model.variable()
x2_1_6 = model.variable()
y2_1_6 = model.variable()
z2_1_6 = model.variable()
x3_1_6 = model.variable()
y3_1_6 = model.variable()
z3_1_6 = model.variable()
x4_1_6 = model.variable()
y4_1_6 = model.variable()
z4_1_6 = model.variable()
x5_1_6 = model.variable()
y5_1_6 = model.variable()
z5_1_6 = model.variable()
x6_1_6 = model.variable()
y6_1_6 = model.variable()
z6_1_6 = model.variable()
x7_1_6 = model.variable()
y7_1_6 = model.variable()
z7_1_6 = model.variable()
x8_1_6 = model.variable()
y8_1_6 = model.variable()
z8_1_6 = model.variable()
x9_1_6 = model.variable()
y9_1_6 = model.variable()
z9_1_6 = model.variable()
x1_2_6 = model.variable()
y1_2_6 = model.variable()
z1_2_6 = model.variable()
x2_2_6 = model.variable()
y2_2_6 = model.variable()
z2_2_6 = model.variable()
x3_2_6 = model.variable()
y3_2_6 = model.variable()
z3_2_6 = model.variable()
x4_2_6 = model.variable()
y4_2_6 = model.variable()
z4_2_6 = model.variable()
x5_2_6 = model.variable()
y5_2_6 = model.variable()
z5_2_6 = model.variable()
x6_2_6 = model.variable()
y6_2_6 = model.variable()
z6_2_6 = model.variable()
x7_2_6 = model.variable()
y7_2_6 = model.variable()
z7_2_6 = model.variable()
x8_2_6 = model.variable()
y8_2_6 = model.variable()
z8_2_6 = model.variable()
x9_2_6 = model.variable()
y9_2_6 = model.variable()
z9_2_6 = model.variable()
x1_3_6 = model.variable()
y1_3_6 = model.variable()
z1_3_6 = model.variable()
x2_3_6 = model.variable()
y2_3_6 = model.variable()
z2_3_6 = model.variable()
x3_3_6 = model.variable()
y3_3_6 = model.variable()
z3_3_6 = model.variable()
x4_3_6 = model.variable()
y4_3_6 = model.variable()
z4_3_6 = model.variable()
x5_3_6 = model.variable()
y5_3_6 = model.variable()
z5_3_6 = model.variable()
x6_3_6 = model.variable()
y6_3_6 = model.variable()
z6_3_6 = model.variable()
x7_3_6 = model.variable()
y7_3_6 = model.variable()
z7_3_6 = model.variable()
x8_3_6 = model.variable()
y8_3_6 = model.variable()
z8_3_6 = model.variable()
x9_3_6 = model.variable()
y9_3_6 = model.variable()
z9_3_6 = model.variable()
x1_4_6 = model.variable()
y1_4_6 = model.variable()
z1_4_6 = model.variable()
x2_4_6 = model.variable()
y2_4_6 = model.variable()
z2_4_6 = model.variable()
x3_4_6 = model.variable()
y3_4_6 = model.variable()
z3_4_6 = model.variable()
x4_4_6 = model.variable()
y4_4_6 = model.variable()
z4_4_6 = model.variable()
x5_4_6 = model.variable()
y5_4_6 = model.variable()
z5_4_6 = model.variable()
x6_4_6 = model.variable()
y6_4_6 = model.variable()
z6_4_6 = model.variable()
x7_4_6 = model.variable()
y7_4_6 = model.variable()
z7_4_6 = model.variable()
x8_4_6 = model.variable()
y8_4_6 = model.variable()
z8_4_6 = model.variable()
x9_4_6 = model.variable()
y9_4_6 = model.variable()
z9_4_6 = model.variable()
x1_5_6 = model.variable()
y1_5_6 = model.variable()
z1_5_6 = model.variable()
x2_5_6 = model.variable()
y2_5_6 = model.variable()
z2_5_6 = model.variable()
x3_5_6 = model.variable()
y3_5_6 = model.variable()
z3_5_6 = model.variable()
x4_5_6 = model.variable()
y4_5_6 = model.variable()
z4_5_6 = model.variable()
x5_5_6 = model.variable()
y5_5_6 = model.variable()
z5_5_6 = model.variable()
x6_5_6 = model.variable()
y6_5_6 = model.variable()
z6_5_6 = model.variable()
x7_5_6 = model.variable()
y7_5_6 = model.variable()
z7_5_6 = model.variable()
x8_5_6 = model.variable()
y8_5_6 = model.variable()
z8_5_6 = model.variable()
x9_5_6 = model.variable()
y9_5_6 = model.variable()
z9_5_6 = model.variable()
x1_6_6 = model.variable()
y1_6_6 = model.variable()
z1_6_6 = model.variable()
x2_6_6 = model.variable()
y2_6_6 = model.variable()
z2_6_6 = model.variable()
x3_6_6 = model.variable()
y3_6_6 = model.variable()
z3_6_6 = model.variable()
x4_6_6 = model.variable()
y4_6_6 = model.variable()
z4_6_6 = model.variable()
x5_6_6 = model.variable()
y5_6_6 = model.variable()
z5_6_6 = model.variable()
x6_6_6 = model.variable()
y6_6_6 = model.variable()
z6_6_6 = model.variable()
x7_6_6 = model.variable()
y7_6_6 = model.variable()
z7_6_6 = model.variable()
x8_6_6 = model.variable()
y8_6_6 = model.variable()
z8_6_6 = model.variable()
x9_6_6 = model.variable()
y9_6_6 = model.variable()
z9_6_6 = model.variable()
x1_7_6 = model.variable()
y1_7_6 = model.variable()
z1_7_6 = model.variable()
x2_7_6 = model.variable()
y2_7_6 = model.variable()
z2_7_6 = model.variable()
x3_7_6 = model.variable()
y3_7_6 = model.variable()
z3_7_6 = model.variable()
x4_7_6 = model.variable()
y4_7_6 = model.variable()
z4_7_6 = model.variable()
x5_7_6 = model.variable()
y5_7_6 = model.variable()
z5_7_6 = model.variable()
x6_7_6 = model.variable()
y6_7_6 = model.variable()
z6_7_6 = model.variable()
x7_7_6 = model.variable()
y7_7_6 = model.variable()
z7_7_6 = model.variable()
x8_7_6 = model.variable()
y8_7_6 = model.variable()
z8_7_6 = model.variable()
x9_7_6 = model.variable()
y9_7_6 = model.variable()
z9_7_6 = model.variable()
x1_8_6 = model.variable()
y1_8_6 = model.variable()
z1_8_6 = model.variable()
x2_8_6 = model.variable()
y2_8_6 = model.variable()
z2_8_6 = model.variable()
x3_8_6 = model.variable()
y3_8_6 = model.variable()
z3_8_6 = model.variable()
x4_8_6 = model.variable()
y4_8_6 = model.variable()
z4_8_6 = model.variable()
x5_8_6 = model.variable()
y5_8_6 = model.variable()
z5_8_6 = model.variable()
x6_8_6 = model.variable()
y6_8_6 = model.variable()
z6_8_6 = model.variable()
x7_8_6 = model.variable()
y7_8_6 = model.variable()
z7_8_6 = model.variable()
x8_8_6 = model.variable()
y8_8_6 = model.variable()
z8_8_6 = model.variable()
x9_8_6 = model.variable()
y9_8_6 = model.variable()
z9_8_6 = model.variable()
x1_9_6 = model.variable()
y1_9_6 = model.variable()
z1_9_6 = model.variable()
x2_9_6 = model.variable()
y2_9_6 = model.variable()
z2_9_6 = model.variable()
x3_9_6 = model.variable()
y3_9_6 = model.variable()
z3_9_6 = model.variable()
x4_9_6 = model.variable()
y4_9_6 = model.variable()
z4_9_6 = model.variable()
x5_9_6 = model.variable()
y5_9_6 = model.variable()
z5_9_6 = model.variable()
x6_9_6 = model.variable()
y6_9_6 = model.variable()
z6_9_6 = model.variable()
x7_9_6 = model.variable()
y7_9_6 = model.variable()
z7_9_6 = model.variable()
x8_9_6 = model.variable()
y8_9_6 = model.variable()
z8_9_6 = model.variable()
x9_9_6 = model.variable()
y9_9_6 = model.variable()
z9_9_6 = model.variable()
x1_1_7 = model.variable()
y1_1_7 = model.variable()
z1_1_7 = model.variable()
x2_1_7 = model.variable()
y2_1_7 = model.variable()
z2_1_7 = model.variable()
x3_1_7 = model.variable()
y3_1_7 = model.variable()
z3_1_7 = model.variable()
x4_1_7 = model.variable()
y4_1_7 = model.variable()
z4_1_7 = model.variable()
x5_1_7 = model.variable()
y5_1_7 = model.variable()
z5_1_7 = model.variable()
x6_1_7 = model.variable()
y6_1_7 = model.variable()
z6_1_7 = model.variable()
x7_1_7 = model.variable()
y7_1_7 = model.variable()
z7_1_7 = model.variable()
x8_1_7 = model.variable()
y8_1_7 = model.variable()
z8_1_7 = model.variable()
x9_1_7 = model.variable()
y9_1_7 = model.variable()
z9_1_7 = model.variable()
x1_2_7 = model.variable()
y1_2_7 = model.variable()
z1_2_7 = model.variable()
x2_2_7 = model.variable()
y2_2_7 = model.variable()
z2_2_7 = model.variable()
x3_2_7 = model.variable()
y3_2_7 = model.variable()
z3_2_7 = model.variable()
x4_2_7 = model.variable()
y4_2_7 = model.variable()
z4_2_7 = model.variable()
x5_2_7 = model.variable()
y5_2_7 = model.variable()
z5_2_7 = model.variable()
x6_2_7 = model.variable()
y6_2_7 = model.variable()
z6_2_7 = model.variable()
x7_2_7 = model.variable()
y7_2_7 = model.variable()
z7_2_7 = model.variable()
x8_2_7 = model.variable()
y8_2_7 = model.variable()
z8_2_7 = model.variable()
x9_2_7 = model.variable()
y9_2_7 = model.variable()
z9_2_7 = model.variable()
x1_3_7 = model.variable()
y1_3_7 = model.variable()
z1_3_7 = model.variable()
x2_3_7 = model.variable()
y2_3_7 = model.variable()
z2_3_7 = model.variable()
x3_3_7 = model.variable()
y3_3_7 = model.variable()
z3_3_7 = model.variable()
x4_3_7 = model.variable()
y4_3_7 = model.variable()
z4_3_7 = model.variable()
x5_3_7 = model.variable()
y5_3_7 = model.variable()
z5_3_7 = model.variable()
x6_3_7 = model.variable()
y6_3_7 = model.variable()
z6_3_7 = model.variable()
x7_3_7 = model.variable()
y7_3_7 = model.variable()
z7_3_7 = model.variable()
x8_3_7 = model.variable()
y8_3_7 = model.variable()
z8_3_7 = model.variable()
x9_3_7 = model.variable()
y9_3_7 = model.variable()
z9_3_7 = model.variable()
x1_4_7 = model.variable()
y1_4_7 = model.variable()
z1_4_7 = model.variable()
x2_4_7 = model.variable()
y2_4_7 = model.variable()
z2_4_7 = model.variable()
x3_4_7 = model.variable()
y3_4_7 = model.variable()
z3_4_7 = model.variable()
x4_4_7 = model.variable()
y4_4_7 = model.variable()
z4_4_7 = model.variable()
x5_4_7 = model.variable()
y5_4_7 = model.variable()
z5_4_7 = model.variable()
x6_4_7 = model.variable()
y6_4_7 = model.variable()
z6_4_7 = model.variable()
x7_4_7 = model.variable()
y7_4_7 = model.variable()
z7_4_7 = model.variable()
x8_4_7 = model.variable()
y8_4_7 = model.variable()
z8_4_7 = model.variable()
x9_4_7 = model.variable()
y9_4_7 = model.variable()
z9_4_7 = model.variable()
x1_5_7 = model.variable()
y1_5_7 = model.variable()
z1_5_7 = model.variable()
x2_5_7 = model.variable()
y2_5_7 = model.variable()
z2_5_7 = model.variable()
x3_5_7 = model.variable()
y3_5_7 = model.variable()
z3_5_7 = model.variable()
x4_5_7 = model.variable()
y4_5_7 = model.variable()
z4_5_7 = model.variable()
x5_5_7 = model.variable()
y5_5_7 = model.variable()
z5_5_7 = model.variable()
x6_5_7 = model.variable()
y6_5_7 = model.variable()
z6_5_7 = model.variable()
x7_5_7 = model.variable()
y7_5_7 = model.variable()
z7_5_7 = model.variable()
x8_5_7 = model.variable()
y8_5_7 = model.variable()
z8_5_7 = model.variable()
x9_5_7 = model.variable()
y9_5_7 = model.variable()
z9_5_7 = model.variable()
x1_6_7 = model.variable()
y1_6_7 = model.variable()
z1_6_7 = model.variable()
x2_6_7 = model.variable()
y2_6_7 = model.variable()
z2_6_7 = model.variable()
x3_6_7 = model.variable()
y3_6_7 = model.variable()
z3_6_7 = model.variable()
x4_6_7 = model.variable()
y4_6_7 = model.variable()
z4_6_7 = model.variable()
x5_6_7 = model.variable()
y5_6_7 = model.variable()
z5_6_7 = model.variable()
x6_6_7 = model.variable()
y6_6_7 = model.variable()
z6_6_7 = model.variable()
x7_6_7 = model.variable()
y7_6_7 = model.variable()
z7_6_7 = model.variable()
x8_6_7 = model.variable()
y8_6_7 = model.variable()
z8_6_7 = model.variable()
x9_6_7 = model.variable()
y9_6_7 = model.variable()
z9_6_7 = model.variable()
x1_7_7 = model.variable()
y1_7_7 = model.variable()
z1_7_7 = model.variable()
x2_7_7 = model.variable()
y2_7_7 = model.variable()
z2_7_7 = model.variable()
x3_7_7 = model.variable()
y3_7_7 = model.variable()
z3_7_7 = model.variable()
x4_7_7 = model.variable()
y4_7_7 = model.variable()
z4_7_7 = model.variable()
x5_7_7 = model.variable()
y5_7_7 = model.variable()
z5_7_7 = model.variable()
x6_7_7 = model.variable()
y6_7_7 = model.variable()
z6_7_7 = model.variable()
x7_7_7 = model.variable()
y7_7_7 = model.variable()
z7_7_7 = model.variable()
x8_7_7 = model.variable()
y8_7_7 = model.variable()
z8_7_7 = model.variable()
x9_7_7 = model.variable()
y9_7_7 = model.variable()
z9_7_7 = model.variable()
x1_8_7 = model.variable()
y1_8_7 = model.variable()
z1_8_7 = model.variable()
x2_8_7 = model.variable()
y2_8_7 = model.variable()
z2_8_7 = model.variable()
x3_8_7 = model.variable()
y3_8_7 = model.variable()
z3_8_7 = model.variable()
x4_8_7 = model.variable()
y4_8_7 = model.variable()
z4_8_7 = model.variable()
x5_8_7 = model.variable()
y5_8_7 = model.variable()
z5_8_7 = model.variable()
x6_8_7 = model.variable()
y6_8_7 = model.variable()
z6_8_7 = model.variable()
x7_8_7 = model.variable()
y7_8_7 = model.variable()
z7_8_7 = model.variable()
x8_8_7 = model.variable()
y8_8_7 = model.variable()
z8_8_7 = model.variable()
x9_8_7 = model.variable()
y9_8_7 = model.variable()
z9_8_7 = model.variable()
x1_9_7 = model.variable()
y1_9_7 = model.variable()
z1_9_7 = model.variable()
x2_9_7 = model.variable()
y2_9_7 = model.variable()
z2_9_7 = model.variable()
x3_9_7 = model.variable()
y3_9_7 = model.variable()
z3_9_7 = model.variable()
x4_9_7 = model.variable()
y4_9_7 = model.variable()
z4_9_7 = model.variable()
x5_9_7 = model.variable()
y5_9_7 = model.variable()
z5_9_7 = model.variable()
x6_9_7 = model.variable()
y6_9_7 = model.variable()
z6_9_7 = model.variable()
x7_9_7 = model.variable()
y7_9_7 = model.variable()
z7_9_7 = model.variable()
x8_9_7 = model.variable()
y8_9_7 = model.variable()
z8_9_7 = model.variable()
x9_9_7 = model.variable()
y9_9_7 = model.variable()
z9_9_7 = model.variable()
x1_1_8 = model.variable()
y1_1_8 = model.variable()
z1_1_8 = model.variable()
x2_1_8 = model.variable()
y2_1_8 = model.variable()
z2_1_8 = model.variable()
x3_1_8 = model.variable()
y3_1_8 = model.variable()
z3_1_8 = model.variable()
x4_1_8 = model.variable()
y4_1_8 = model.variable()
z4_1_8 = model.variable()
x5_1_8 = model.variable()
y5_1_8 = model.variable()
z5_1_8 = model.variable()
x6_1_8 = model.variable()
y6_1_8 = model.variable()
z6_1_8 = model.variable()
x7_1_8 = model.variable()
y7_1_8 = model.variable()
z7_1_8 = model.variable()
x8_1_8 = model.variable()
y8_1_8 = model.variable()
z8_1_8 = model.variable()
x9_1_8 = model.variable()
y9_1_8 = model.variable()
z9_1_8 = model.variable()
x1_2_8 = model.variable()
y1_2_8 = model.variable()
z1_2_8 = model.variable()
x2_2_8 = model.variable()
y2_2_8 = model.variable()
z2_2_8 = model.variable()
x3_2_8 = model.variable()
y3_2_8 = model.variable()
z3_2_8 = model.variable()
x4_2_8 = model.variable()
y4_2_8 = model.variable()
z4_2_8 = model.variable()
x5_2_8 = model.variable()
y5_2_8 = model.variable()
z5_2_8 = model.variable()
x6_2_8 = model.variable()
y6_2_8 = model.variable()
z6_2_8 = model.variable()
x7_2_8 = model.variable()
y7_2_8 = model.variable()
z7_2_8 = model.variable()
x8_2_8 = model.variable()
y8_2_8 = model.variable()
z8_2_8 = model.variable()
x9_2_8 = model.variable()
y9_2_8 = model.variable()
z9_2_8 = model.variable()
x1_3_8 = model.variable()
y1_3_8 = model.variable()
z1_3_8 = model.variable()
x2_3_8 = model.variable()
y2_3_8 = model.variable()
z2_3_8 = model.variable()
x3_3_8 = model.variable()
y3_3_8 = model.variable()
z3_3_8 = model.variable()
x4_3_8 = model.variable()
y4_3_8 = model.variable()
z4_3_8 = model.variable()
x5_3_8 = model.variable()
y5_3_8 = model.variable()
z5_3_8 = model.variable()
x6_3_8 = model.variable()
y6_3_8 = model.variable()
z6_3_8 = model.variable()
x7_3_8 = model.variable()
y7_3_8 = model.variable()
z7_3_8 = model.variable()
x8_3_8 = model.variable()
y8_3_8 = model.variable()
z8_3_8 = model.variable()
x9_3_8 = model.variable()
y9_3_8 = model.variable()
z9_3_8 = model.variable()
x1_4_8 = model.variable()
y1_4_8 = model.variable()
z1_4_8 = model.variable()
x2_4_8 = model.variable()
y2_4_8 = model.variable()
z2_4_8 = model.variable()
x3_4_8 = model.variable()
y3_4_8 = model.variable()
z3_4_8 = model.variable()
x4_4_8 = model.variable()
y4_4_8 = model.variable()
z4_4_8 = model.variable()
x5_4_8 = model.variable()
y5_4_8 = model.variable()
z5_4_8 = model.variable()
x6_4_8 = model.variable()
y6_4_8 = model.variable()
z6_4_8 = model.variable()
x7_4_8 = model.variable()
y7_4_8 = model.variable()
z7_4_8 = model.variable()
x8_4_8 = model.variable()
y8_4_8 = model.variable()
z8_4_8 = model.variable()
x9_4_8 = model.variable()
y9_4_8 = model.variable()
z9_4_8 = model.variable()
x1_5_8 = model.variable()
y1_5_8 = model.variable()
z1_5_8 = model.variable()
x2_5_8 = model.variable()
y2_5_8 = model.variable()
z2_5_8 = model.variable()
x3_5_8 = model.variable()
y3_5_8 = model.variable()
z3_5_8 = model.variable()
x4_5_8 = model.variable()
y4_5_8 = model.variable()
z4_5_8 = model.variable()
x5_5_8 = model.variable()
y5_5_8 = model.variable()
z5_5_8 = model.variable()
x6_5_8 = model.variable()
y6_5_8 = model.variable()
z6_5_8 = model.variable()
x7_5_8 = model.variable()
y7_5_8 = model.variable()
z7_5_8 = model.variable()
x8_5_8 = model.variable()
y8_5_8 = model.variable()
z8_5_8 = model.variable()
x9_5_8 = model.variable()
y9_5_8 = model.variable()
z9_5_8 = model.variable()
x1_6_8 = model.variable()
y1_6_8 = model.variable()
z1_6_8 = model.variable()
x2_6_8 = model.variable()
y2_6_8 = model.variable()
z2_6_8 = model.variable()
x3_6_8 = model.variable()
y3_6_8 = model.variable()
z3_6_8 = model.variable()
x4_6_8 = model.variable()
y4_6_8 = model.variable()
z4_6_8 = model.variable()
x5_6_8 = model.variable()
y5_6_8 = model.variable()
z5_6_8 = model.variable()
x6_6_8 = model.variable()
y6_6_8 = model.variable()
z6_6_8 = model.variable()
x7_6_8 = model.variable()
y7_6_8 = model.variable()
z7_6_8 = model.variable()
x8_6_8 = model.variable()
y8_6_8 = model.variable()
z8_6_8 = model.variable()
x9_6_8 = model.variable()
y9_6_8 = model.variable()
z9_6_8 = model.variable()
x1_7_8 = model.variable()
y1_7_8 = model.variable()
z1_7_8 = model.variable()
x2_7_8 = model.variable()
y2_7_8 = model.variable()
z2_7_8 = model.variable()
x3_7_8 = model.variable()
y3_7_8 = model.variable()
z3_7_8 = model.variable()
x4_7_8 = model.variable()
y4_7_8 = model.variable()
z4_7_8 = model.variable()
x5_7_8 = model.variable()
y5_7_8 = model.variable()
z5_7_8 = model.variable()
x6_7_8 = model.variable()
y6_7_8 = model.variable()
z6_7_8 = model.variable()
x7_7_8 = model.variable()
y7_7_8 = model.variable()
z7_7_8 = model.variable()
x8_7_8 = model.variable()
y8_7_8 = model.variable()
z8_7_8 = model.variable()
x9_7_8 = model.variable()
y9_7_8 = model.variable()
z9_7_8 = model.variable()
x1_8_8 = model.variable()
y1_8_8 = model.variable()
z1_8_8 = model.variable()
x2_8_8 = model.variable()
y2_8_8 = model.variable()
z2_8_8 = model.variable()
x3_8_8 = model.variable()
y3_8_8 = model.variable()
z3_8_8 = model.variable()
x4_8_8 = model.variable()
y4_8_8 = model.variable()
z4_8_8 = model.variable()
x5_8_8 = model.variable()
y5_8_8 = model.variable()
z5_8_8 = model.variable()
x6_8_8 = model.variable()
y6_8_8 = model.variable()
z6_8_8 = model.variable()
x7_8_8 = model.variable()
y7_8_8 = model.variable()
z7_8_8 = model.variable()
x8_8_8 = model.variable()
y8_8_8 = model.variable()
z8_8_8 = model.variable()
x9_8_8 = model.variable()
y9_8_8 = model.variable()
z9_8_8 = model.variable()
x1_9_8 = model.variable()
y1_9_8 = model.variable()
z1_9_8 = model.variable()
x2_9_8 = model.variable()
y2_9_8 = model.variable()
z2_9_8 = model.variable()
x3_9_8 = model.variable()
y3_9_8 = model.variable()
z3_9_8 = model.variable()
x4_9_8 = model.variable()
y4_9_8 = model.variable()
z4_9_8 = model.variable()
x5_9_8 = model.variable()
y5_9_8 = model.variable()
z5_9_8 = model.variable()
x6_9_8 = model.variable()
y6_9_8 = model.variable()
z6_9_8 = model.variable()
x7_9_8 = model.variable()
y7_9_8 = model.variable()
z7_9_8 = model.variable()
x8_9_8 = model.variable()
y8_9_8 = model.variable()
z8_9_8 = model.variable()
x9_9_8 = model.variable()
y9_9_8 = model.variable()
z9_9_8 = model.variable()
x1_1_9 = model.variable()
y1_1_9 = model.variable()
z1_1_9 = model.variable()
x2_1_9 = model.variable()
y2_1_9 = model.variable()
z2_1_9 = model.variable()
x3_1_9 = model.variable()
y3_1_9 = model.variable()
z3_1_9 = model.variable()
x4_1_9 = model.variable()
y4_1_9 = model.variable()
z4_1_9 = model.variable()
x5_1_9 = model.variable()
y5_1_9 = model.variable()
z5_1_9 = model.variable()
x6_1_9 = model.variable()
y6_1_9 = model.variable()
z6_1_9 = model.variable()
x7_1_9 = model.variable()
y7_1_9 = model.variable()
z7_1_9 = model.variable()
x8_1_9 = model.variable()
y8_1_9 = model.variable()
z8_1_9 = model.variable()
x9_1_9 = model.variable()
y9_1_9 = model.variable()
z9_1_9 = model.variable()
x1_2_9 = model.variable()
y1_2_9 = model.variable()
z1_2_9 = model.variable()
x2_2_9 = model.variable()
y2_2_9 = model.variable()
z2_2_9 = model.variable()
x3_2_9 = model.variable()
y3_2_9 = model.variable()
z3_2_9 = model.variable()
x4_2_9 = model.variable()
y4_2_9 = model.variable()
z4_2_9 = model.variable()
x5_2_9 = model.variable()
y5_2_9 = model.variable()
z5_2_9 = model.variable()
x6_2_9 = model.variable()
y6_2_9 = model.variable()
z6_2_9 = model.variable()
x7_2_9 = model.variable()
y7_2_9 = model.variable()
z7_2_9 = model.variable()
x8_2_9 = model.variable()
y8_2_9 = model.variable()
z8_2_9 = model.variable()
x9_2_9 = model.variable()
y9_2_9 = model.variable()
z9_2_9 = model.variable()
x1_3_9 = model.variable()
y1_3_9 = model.variable()
z1_3_9 = model.variable()
x2_3_9 = model.variable()
y2_3_9 = model.variable()
z2_3_9 = model.variable()
x3_3_9 = model.variable()
y3_3_9 = model.variable()
z3_3_9 = model.variable()
x4_3_9 = model.variable()
y4_3_9 = model.variable()
z4_3_9 = model.variable()
x5_3_9 = model.variable()
y5_3_9 = model.variable()
z5_3_9 = model.variable()
x6_3_9 = model.variable()
y6_3_9 = model.variable()
z6_3_9 = model.variable()
x7_3_9 = model.variable()
y7_3_9 = model.variable()
z7_3_9 = model.variable()
x8_3_9 = model.variable()
y8_3_9 = model.variable()
z8_3_9 = model.variable()
x9_3_9 = model.variable()
y9_3_9 = model.variable()
z9_3_9 = model.variable()
x1_4_9 = model.variable()
y1_4_9 = model.variable()
z1_4_9 = model.variable()
x2_4_9 = model.variable()
y2_4_9 = model.variable()
z2_4_9 = model.variable()
x3_4_9 = model.variable()
y3_4_9 = model.variable()
z3_4_9 = model.variable()
x4_4_9 = model.variable()
y4_4_9 = model.variable()
z4_4_9 = model.variable()
x5_4_9 = model.variable()
y5_4_9 = model.variable()
z5_4_9 = model.variable()
x6_4_9 = model.variable()
y6_4_9 = model.variable()
z6_4_9 = model.variable()
x7_4_9 = model.variable()
y7_4_9 = model.variable()
z7_4_9 = model.variable()
x8_4_9 = model.variable()
y8_4_9 = model.variable()
z8_4_9 = model.variable()
x9_4_9 = model.variable()
y9_4_9 = model.variable()
z9_4_9 = model.variable()
x1_5_9 = model.variable()
y1_5_9 = model.variable()
z1_5_9 = model.variable()
x2_5_9 = model.variable()
y2_5_9 = model.variable()
z2_5_9 = model.variable()
x3_5_9 = model.variable()
y3_5_9 = model.variable()
z3_5_9 = model.variable()
x4_5_9 = model.variable()
y4_5_9 = model.variable()
z4_5_9 = model.variable()
x5_5_9 = model.variable()
y5_5_9 = model.variable()
z5_5_9 = model.variable()
x6_5_9 = model.variable()
y6_5_9 = model.variable()
z6_5_9 = model.variable()
x7_5_9 = model.variable()
y7_5_9 = model.variable()
z7_5_9 = model.variable()
x8_5_9 = model.variable()
y8_5_9 = model.variable()
z8_5_9 = model.variable()
x9_5_9 = model.variable()
y9_5_9 = model.variable()
z9_5_9 = model.variable()
x1_6_9 = model.variable()
y1_6_9 = model.variable()
z1_6_9 = model.variable()
x2_6_9 = model.variable()
y2_6_9 = model.variable()
z2_6_9 = model.variable()
x3_6_9 = model.variable()
y3_6_9 = model.variable()
z3_6_9 = model.variable()
x4_6_9 = model.variable()
y4_6_9 = model.variable()
z4_6_9 = model.variable()
x5_6_9 = model.variable()
y5_6_9 = model.variable()
z5_6_9 = model.variable()
x6_6_9 = model.variable()
y6_6_9 = model.variable()
z6_6_9 = model.variable()
x7_6_9 = model.variable()
y7_6_9 = model.variable()
z7_6_9 = model.variable()
x8_6_9 = model.variable()
y8_6_9 = model.variable()
z8_6_9 = model.variable()
x9_6_9 = model.variable()
y9_6_9 = model.variable()
z9_6_9 = model.variable()
x1_7_9 = model.variable()
y1_7_9 = model.variable()
z1_7_9 = model.variable()
x2_7_9 = model.variable()
y2_7_9 = model.variable()
z2_7_9 = model.variable()
x3_7_9 = model.variable()
y3_7_9 = model.variable()
z3_7_9 = model.variable()
x4_7_9 = model.variable()
y4_7_9 = model.variable()
z4_7_9 = model.variable()
x5_7_9 = model.variable()
y5_7_9 = model.variable()
z5_7_9 = model.variable()
x6_7_9 = model.variable()
y6_7_9 = model.variable()
z6_7_9 = model.variable()
x7_7_9 = model.variable()
y7_7_9 = model.variable()
z7_7_9 = model.variable()
x8_7_9 = model.variable()
y8_7_9 = model.variable()
z8_7_9 = model.variable()
x9_7_9 = model.variable()
y9_7_9 = model.variable()
z9_7_9 = model.variable()
x1_8_9 = model.variable()
y1_8_9 = model.variable()
z1_8_9 = model.variable()
x2_8_9 = model.variable()
y2_8_9 = model.variable()
z2_8_9 = model.variable()
x3_8_9 = model.variable()
y3_8_9 = model.variable()
z3_8_9 = model.variable()
x4_8_9 = model.variable()
y4_8_9 = model.variable()
z4_8_9 = model.variable()
x5_8_9 = model.variable()
y5_8_9 = model.variable()
z5_8_9 = model.variable()
x6_8_9 = model.variable()
y6_8_9 = model.variable()
z6_8_9 = model.variable()
x7_8_9 = model.variable()
y7_8_9 = model.variable()
z7_8_9 = model.variable()
x8_8_9 = model.variable()
y8_8_9 = model.variable()
z8_8_9 = model.variable()
x9_8_9 = model.variable()
y9_8_9 = model.variable()
z9_8_9 = model.variable()
x1_9_9 = model.variable()
y1_9_9 = model.variable()
z1_9_9 = model.variable()
x2_9_9 = model.variable()
y2_9_9 = model.variable()
z2_9_9 = model.variable()
x3_9_9 = model.variable()
y3_9_9 = model.variable()
z3_9_9 = model.variable()
x4_9_9 = model.variable()
y4_9_9 = model.variable()
z4_9_9 = model.variable()
x5_9_9 = model.variable()
y5_9_9 = model.variable()
z5_9_9 = model.variable()
x6_9_9 = model.variable()
y6_9_9 = model.variable()
z6_9_9 = model.variable()
x7_9_9 = model.variable()
y7_9_9 = model.variable()
z7_9_9 = model.variable()
x8_9_9 = model.variable()
y8_9_9 = model.variable()
z8_9_9 = model.variable()
x9_9_9 = model.variable()
y9_9_9 = model.variable()
z9_9_9 = model.variable()
y10_1_1 = model.variable()
z10_1_1 = model.variable()
y10_2_1 = model.variable()
z10_2_1 = model.variable()
y10_3_1 = model.variable()
z10_3_1 = model.variable()
y10_4_1 = model.variable()
z10_4_1 = model.variable()
y10_5_1 = model.variable()
z10_5_1 = model.variable()
y10_6_1 = model.variable()
z10_6_1 = model.variable()
y10_7_1 = model.variable()
z10_7_1 = model.variable()
y10_8_1 = model.variable()
z10_8_1 = model.variable()
y10_9_1 = model.variable()
z10_9_1 = model.variable()
y10_1_2 = model.variable()
z10_1_2 = model.variable()
y10_2_2 = model.variable()
z10_2_2 = model.variable()
y10_3_2 = model.variable()
z10_3_2 = model.variable()
y10_4_2 = model.variable()
z10_4_2 = model.variable()
y10_5_2 = model.variable()
z10_5_2 = model.variable()
y10_6_2 = model.variable()
z10_6_2 = model.variable()
y10_7_2 = model.variable()
z10_7_2 = model.variable()
y10_8_2 = model.variable()
z10_8_2 = model.variable()
y10_9_2 = model.variable()
z10_9_2 = model.variable()
y10_1_3 = model.variable()
z10_1_3 = model.variable()
y10_2_3 = model.variable()
z10_2_3 = model.variable()
y10_3_3 = model.variable()
z10_3_3 = model.variable()
y10_4_3 = model.variable()
z10_4_3 = model.variable()
y10_5_3 = model.variable()
z10_5_3 = model.variable()
y10_6_3 = model.variable()
z10_6_3 = model.variable()
y10_7_3 = model.variable()
z10_7_3 = model.variable()
y10_8_3 = model.variable()
z10_8_3 = model.variable()
y10_9_3 = model.variable()
z10_9_3 = model.variable()
y10_1_4 = model.variable()
z10_1_4 = model.variable()
y10_2_4 = model.variable()
z10_2_4 = model.variable()
y10_3_4 = model.variable()
z10_3_4 = model.variable()
y10_4_4 = model.variable()
z10_4_4 = model.variable()
y10_5_4 = model.variable()
z10_5_4 = model.variable()
y10_6_4 = model.variable()
z10_6_4 = model.variable()
y10_7_4 = model.variable()
z10_7_4 = model.variable()
y10_8_4 = model.variable()
z10_8_4 = model.variable()
y10_9_4 = model.variable()
z10_9_4 = model.variable()
y10_1_5 = model.variable()
z10_1_5 = model.variable()
y10_2_5 = model.variable()
z10_2_5 = model.variable()
y10_3_5 = model.variable()
z10_3_5 = model.variable()
y10_4_5 = model.variable()
z10_4_5 = model.variable()
y10_5_5 = model.variable()
z10_5_5 = model.variable()
y10_6_5 = model.variable()
z10_6_5 = model.variable()
y10_7_5 = model.variable()
z10_7_5 = model.variable()
y10_8_5 = model.variable()
z10_8_5 = model.variable()
y10_9_5 = model.variable()
z10_9_5 = model.variable()
y10_1_6 = model.variable()
z10_1_6 = model.variable()
y10_2_6 = model.variable()
z10_2_6 = model.variable()
y10_3_6 = model.variable()
z10_3_6 = model.variable()
y10_4_6 = model.variable()
z10_4_6 = model.variable()
y10_5_6 = model.variable()
z10_5_6 = model.variable()
y10_6_6 = model.variable()
z10_6_6 = model.variable()
y10_7_6 = model.variable()
z10_7_6 = model.variable()
y10_8_6 = model.variable()
z10_8_6 = model.variable()
y10_9_6 = model.variable()
z10_9_6 = model.variable()
y10_1_7 = model.variable()
z10_1_7 = model.variable()
y10_2_7 = model.variable()
z10_2_7 = model.variable()
y10_3_7 = model.variable()
z10_3_7 = model.variable()
y10_4_7 = model.variable()
z10_4_7 = model.variable()
y10_5_7 = model.variable()
z10_5_7 = model.variable()
y10_6_7 = model.variable()
z10_6_7 = model.variable()
y10_7_7 = model.variable()
z10_7_7 = model.variable()
y10_8_7 = model.variable()
z10_8_7 = model.variable()
y10_9_7 = model.variable()
z10_9_7 = model.variable()
y10_1_8 = model.variable()
z10_1_8 = model.variable()
y10_2_8 = model.variable()
z10_2_8 = model.variable()
y10_3_8 = model.variable()
z10_3_8 = model.variable()
y10_4_8 = model.variable()
z10_4_8 = model.variable()
y10_5_8 = model.variable()
z10_5_8 = model.variable()
y10_6_8 = model.variable()
z10_6_8 = model.variable()
y10_7_8 = model.variable()
z10_7_8 = model.variable()
y10_8_8 = model.variable()
z10_8_8 = model.variable()
y10_9_8 = model.variable()
z10_9_8 = model.variable()
y10_1_9 = model.variable()
z10_1_9 = model.variable()
y10_2_9 = model.variable()
z10_2_9 = model.variable()
y10_3_9 = model.variable()
z10_3_9 = model.variable()
y10_4_9 = model.variable()
z10_4_9 = model.variable()
y10_5_9 = model.variable()
z10_5_9 = model.variable()
y10_6_9 = model.variable()
z10_6_9 = model.variable()
y10_7_9 = model.variable()
z10_7_9 = model.variable()
y10_8_9 = model.variable()
z10_8_9 = model.variable()
y10_9_9 = model.variable()
z10_9_9 = model.variable()
x1_10_1 = model.variable()
z1_10_1 = model.variable()
x2_10_1 = model.variable()
z2_10_1 = model.variable()
x3_10_1 = model.variable()
z3_10_1 = model.variable()
x4_10_1 = model.variable()
z4_10_1 = model.variable()
x5_10_1 = model.variable()
z5_10_1 = model.variable()
x6_10_1 = model.variable()
z6_10_1 = model.variable()
x7_10_1 = model.variable()
z7_10_1 = model.variable()
x8_10_1 = model.variable()
z8_10_1 = model.variable()
x9_10_1 = model.variable()
z9_10_1 = model.variable()
x1_10_2 = model.variable()
z1_10_2 = model.variable()
x2_10_2 = model.variable()
z2_10_2 = model.variable()
x3_10_2 = model.variable()
z3_10_2 = model.variable()
x4_10_2 = model.variable()
z4_10_2 = model.variable()
x5_10_2 = model.variable()
z5_10_2 = model.variable()
x6_10_2 = model.variable()
z6_10_2 = model.variable()
x7_10_2 = model.variable()
z7_10_2 = model.variable()
x8_10_2 = model.variable()
z8_10_2 = model.variable()
x9_10_2 = model.variable()
z9_10_2 = model.variable()
x1_10_3 = model.variable()
z1_10_3 = model.variable()
x2_10_3 = model.variable()
z2_10_3 = model.variable()
x3_10_3 = model.variable()
z3_10_3 = model.variable()
x4_10_3 = model.variable()
z4_10_3 = model.variable()
x5_10_3 = model.variable()
z5_10_3 = model.variable()
x6_10_3 = model.variable()
z6_10_3 = model.variable()
x7_10_3 = model.variable()
z7_10_3 = model.variable()
x8_10_3 = model.variable()
z8_10_3 = model.variable()
x9_10_3 = model.variable()
z9_10_3 = model.variable()
x1_10_4 = model.variable()
z1_10_4 = model.variable()
x2_10_4 = model.variable()
z2_10_4 = model.variable()
x3_10_4 = model.variable()
z3_10_4 = model.variable()
x4_10_4 = model.variable()
z4_10_4 = model.variable()
x5_10_4 = model.variable()
z5_10_4 = model.variable()
x6_10_4 = model.variable()
z6_10_4 = model.variable()
x7_10_4 = model.variable()
z7_10_4 = model.variable()
x8_10_4 = model.variable()
z8_10_4 = model.variable()
x9_10_4 = model.variable()
z9_10_4 = model.variable()
x1_10_5 = model.variable()
z1_10_5 = model.variable()
x2_10_5 = model.variable()
z2_10_5 = model.variable()
x3_10_5 = model.variable()
z3_10_5 = model.variable()
x4_10_5 = model.variable()
z4_10_5 = model.variable()
x5_10_5 = model.variable()
z5_10_5 = model.variable()
x6_10_5 = model.variable()
z6_10_5 = model.variable()
x7_10_5 = model.variable()
z7_10_5 = model.variable()
x8_10_5 = model.variable()
z8_10_5 = model.variable()
x9_10_5 = model.variable()
z9_10_5 = model.variable()
x1_10_6 = model.variable()
z1_10_6 = model.variable()
x2_10_6 = model.variable()
z2_10_6 = model.variable()
x3_10_6 = model.variable()
z3_10_6 = model.variable()
x4_10_6 = model.variable()
z4_10_6 = model.variable()
x5_10_6 = model.variable()
z5_10_6 = model.variable()
x6_10_6 = model.variable()
z6_10_6 = model.variable()
x7_10_6 = model.variable()
z7_10_6 = model.variable()
x8_10_6 = model.variable()
z8_10_6 = model.variable()
x9_10_6 = model.variable()
z9_10_6 = model.variable()
x1_10_7 = model.variable()
z1_10_7 = model.variable()
x2_10_7 = model.variable()
z2_10_7 = model.variable()
x3_10_7 = model.variable()
z3_10_7 = model.variable()
x4_10_7 = model.variable()
z4_10_7 = model.variable()
x5_10_7 = model.variable()
z5_10_7 = model.variable()
x6_10_7 = model.variable()
z6_10_7 = model.variable()
x7_10_7 = model.variable()
z7_10_7 = model.variable()
x8_10_7 = model.variable()
z8_10_7 = model.variable()
x9_10_7 = model.variable()
z9_10_7 = model.variable()
x1_10_8 = model.variable()
z1_10_8 = model.variable()
x2_10_8 = model.variable()
z2_10_8 = model.variable()
x3_10_8 = model.variable()
z3_10_8 = model.variable()
x4_10_8 = model.variable()
z4_10_8 = model.variable()
x5_10_8 = model.variable()
z5_10_8 = model.variable()
x6_10_8 = model.variable()
z6_10_8 = model.variable()
x7_10_8 = model.variable()
z7_10_8 = model.variable()
x8_10_8 = model.variable()
z8_10_8 = model.variable()
x9_10_8 = model.variable()
z9_10_8 = model.variable()
x1_10_9 = model.variable()
z1_10_9 = model.variable()
x2_10_9 = model.variable()
z2_10_9 = model.variable()
x3_10_9 = model.variable()
z3_10_9 = model.variable()
x4_10_9 = model.variable()
z4_10_9 = model.variable()
x5_10_9 = model.variable()
z5_10_9 = model.variable()
x6_10_9 = model.variable()
z6_10_9 = model.variable()
x7_10_9 = model.variable()
z7_10_9 = model.variable()
x8_10_9 = model.variable()
z8_10_9 = model.variable()
x9_10_9 = model.variable()
z9_10_9 = model.variable()
x1_1_10 = model.variable()
y1_1_10 = model.variable()
x2_1_10 = model.variable()
y2_1_10 = model.variable()
x3_1_10 = model.variable()
y3_1_10 = model.variable()
x4_1_10 = model.variable()
y4_1_10 = model.variable()
x5_1_10 = model.variable()
y5_1_10 = model.variable()
x6_1_10 = model.variable()
y6_1_10 = model.variable()
x7_1_10 = model.variable()
y7_1_10 = model.variable()
x8_1_10 = model.variable()
y8_1_10 = model.variable()
x9_1_10 = model.variable()
y9_1_10 = model.variable()
x1_2_10 = model.variable()
y1_2_10 = model.variable()
x2_2_10 = model.variable()
y2_2_10 = model.variable()
x3_2_10 = model.variable()
y3_2_10 = model.variable()
x4_2_10 = model.variable()
y4_2_10 = model.variable()
x5_2_10 = model.variable()
y5_2_10 = model.variable()
x6_2_10 = model.variable()
y6_2_10 = model.variable()
x7_2_10 = model.variable()
y7_2_10 = model.variable()
x8_2_10 = model.variable()
y8_2_10 = model.variable()
x9_2_10 = model.variable()
y9_2_10 = model.variable()
x1_3_10 = model.variable()
y1_3_10 = model.variable()
x2_3_10 = model.variable()
y2_3_10 = model.variable()
x3_3_10 = model.variable()
y3_3_10 = model.variable()
x4_3_10 = model.variable()
y4_3_10 = model.variable()
x5_3_10 = model.variable()
y5_3_10 = model.variable()
x6_3_10 = model.variable()
y6_3_10 = model.variable()
x7_3_10 = model.variable()
y7_3_10 = model.variable()
x8_3_10 = model.variable()
y8_3_10 = model.variable()
x9_3_10 = model.variable()
y9_3_10 = model.variable()
x1_4_10 = model.variable()
y1_4_10 = model.variable()
x2_4_10 = model.variable()
y2_4_10 = model.variable()
x3_4_10 = model.variable()
y3_4_10 = model.variable()
x4_4_10 = model.variable()
y4_4_10 = model.variable()
x5_4_10 = model.variable()
y5_4_10 = model.variable()
x6_4_10 = model.variable()
y6_4_10 = model.variable()
x7_4_10 = model.variable()
y7_4_10 = model.variable()
x8_4_10 = model.variable()
y8_4_10 = model.variable()
x9_4_10 = model.variable()
y9_4_10 = model.variable()
x1_5_10 = model.variable()
y1_5_10 = model.variable()
x2_5_10 = model.variable()
y2_5_10 = model.variable()
x3_5_10 = model.variable()
y3_5_10 = model.variable()
x4_5_10 = model.variable()
y4_5_10 = model.variable()
x5_5_10 = model.variable()
y5_5_10 = model.variable()
x6_5_10 = model.variable()
y6_5_10 = model.variable()
x7_5_10 = model.variable()
y7_5_10 = model.variable()
x8_5_10 = model.variable()
y8_5_10 = model.variable()
x9_5_10 = model.variable()
y9_5_10 = model.variable()
x1_6_10 = model.variable()
y1_6_10 = model.variable()
x2_6_10 = model.variable()
y2_6_10 = model.variable()
x3_6_10 = model.variable()
y3_6_10 = model.variable()
x4_6_10 = model.variable()
y4_6_10 = model.variable()
x5_6_10 = model.variable()
y5_6_10 = model.variable()
x6_6_10 = model.variable()
y6_6_10 = model.variable()
x7_6_10 = model.variable()
y7_6_10 = model.variable()
x8_6_10 = model.variable()
y8_6_10 = model.variable()
x9_6_10 = model.variable()
y9_6_10 = model.variable()
x1_7_10 = model.variable()
y1_7_10 = model.variable()
x2_7_10 = model.variable()
y2_7_10 = model.variable()
x3_7_10 = model.variable()
y3_7_10 = model.variable()
x4_7_10 = model.variable()
y4_7_10 = model.variable()
x5_7_10 = model.variable()
y5_7_10 = model.variable()
x6_7_10 = model.variable()
y6_7_10 = model.variable()
x7_7_10 = model.variable()
y7_7_10 = model.variable()
x8_7_10 = model.variable()
y8_7_10 = model.variable()
x9_7_10 = model.variable()
y9_7_10 = model.variable()
x1_8_10 = model.variable()
y1_8_10 = model.variable()
x2_8_10 = model.variable()
y2_8_10 = model.variable()
x3_8_10 = model.variable()
y3_8_10 = model.variable()
x4_8_10 = model.variable()
y4_8_10 = model.variable()
x5_8_10 = model.variable()
y5_8_10 = model.variable()
x6_8_10 = model.variable()
y6_8_10 = model.variable()
x7_8_10 = model.variable()
y7_8_10 = model.variable()
x8_8_10 = model.variable()
y8_8_10 = model.variable()
x9_8_10 = model.variable()
y9_8_10 = model.variable()
x1_9_10 = model.variable()
y1_9_10 = model.variable()
x2_9_10 = model.variable()
y2_9_10 = model.variable()
x3_9_10 = model.variable()
y3_9_10 = model.variable()
x4_9_10 = model.variable()
y4_9_10 = model.variable()
x5_9_10 = model.variable()
y5_9_10 = model.variable()
x6_9_10 = model.variable()
y6_9_10 = model.variable()
x7_9_10 = model.variable()
y7_9_10 = model.variable()
x8_9_10 = model.variable()
y8_9_10 = model.variable()
x9_9_10 = model.variable()
y9_9_10 = model.variable()
y0_1_1 = model.variable()
z0_1_1 = model.variable()
y11_1_1 = model.variable()
z11_1_1 = model.variable()
y0_2_1 = model.variable()
z0_2_1 = model.variable()
y11_2_1 = model.variable()
z11_2_1 = model.variable()
y0_3_1 = model.variable()
z0_3_1 = model.variable()
y11_3_1 = model.variable()
z11_3_1 = model.variable()
y0_4_1 = model.variable()
z0_4_1 = model.variable()
y11_4_1 = model.variable()
z11_4_1 = model.variable()
y0_5_1 = model.variable()
z0_5_1 = model.variable()
y11_5_1 = model.variable()
z11_5_1 = model.variable()
y0_6_1 = model.variable()
z0_6_1 = model.variable()
y11_6_1 = model.variable()
z11_6_1 = model.variable()
y0_7_1 = model.variable()
z0_7_1 = model.variable()
y11_7_1 = model.variable()
z11_7_1 = model.variable()
y0_8_1 = model.variable()
z0_8_1 = model.variable()
y11_8_1 = model.variable()
z11_8_1 = model.variable()
y0_9_1 = model.variable()
z0_9_1 = model.variable()
y11_9_1 = model.variable()
z11_9_1 = model.variable()
y0_10_1 = model.variable()
z0_10_1 = model.variable()
y11_10_1 = model.variable()
z11_10_1 = model.variable()
y0_1_2 = model.variable()
z0_1_2 = model.variable()
y11_1_2 = model.variable()
z11_1_2 = model.variable()
y0_2_2 = model.variable()
z0_2_2 = model.variable()
y11_2_2 = model.variable()
z11_2_2 = model.variable()
y0_3_2 = model.variable()
z0_3_2 = model.variable()
y11_3_2 = model.variable()
z11_3_2 = model.variable()
y0_4_2 = model.variable()
z0_4_2 = model.variable()
y11_4_2 = model.variable()
z11_4_2 = model.variable()
y0_5_2 = model.variable()
z0_5_2 = model.variable()
y11_5_2 = model.variable()
z11_5_2 = model.variable()
y0_6_2 = model.variable()
z0_6_2 = model.variable()
y11_6_2 = model.variable()
z11_6_2 = model.variable()
y0_7_2 = model.variable()
z0_7_2 = model.variable()
y11_7_2 = model.variable()
z11_7_2 = model.variable()
y0_8_2 = model.variable()
z0_8_2 = model.variable()
y11_8_2 = model.variable()
z11_8_2 = model.variable()
y0_9_2 = model.variable()
z0_9_2 = model.variable()
y11_9_2 = model.variable()
z11_9_2 = model.variable()
y0_10_2 = model.variable()
z0_10_2 = model.variable()
y11_10_2 = model.variable()
z11_10_2 = model.variable()
y0_1_3 = model.variable()
z0_1_3 = model.variable()
y11_1_3 = model.variable()
z11_1_3 = model.variable()
y0_2_3 = model.variable()
z0_2_3 = model.variable()
y11_2_3 = model.variable()
z11_2_3 = model.variable()
y0_3_3 = model.variable()
z0_3_3 = model.variable()
y11_3_3 = model.variable()
z11_3_3 = model.variable()
y0_4_3 = model.variable()
z0_4_3 = model.variable()
y11_4_3 = model.variable()
z11_4_3 = model.variable()
y0_5_3 = model.variable()
z0_5_3 = model.variable()
y11_5_3 = model.variable()
z11_5_3 = model.variable()
y0_6_3 = model.variable()
z0_6_3 = model.variable()
y11_6_3 = model.variable()
z11_6_3 = model.variable()
y0_7_3 = model.variable()
z0_7_3 = model.variable()
y11_7_3 = model.variable()
z11_7_3 = model.variable()
y0_8_3 = model.variable()
z0_8_3 = model.variable()
y11_8_3 = model.variable()
z11_8_3 = model.variable()
y0_9_3 = model.variable()
z0_9_3 = model.variable()
y11_9_3 = model.variable()
z11_9_3 = model.variable()
y0_10_3 = model.variable()
z0_10_3 = model.variable()
y11_10_3 = model.variable()
z11_10_3 = model.variable()
y0_1_4 = model.variable()
z0_1_4 = model.variable()
y11_1_4 = model.variable()
z11_1_4 = model.variable()
y0_2_4 = model.variable()
z0_2_4 = model.variable()
y11_2_4 = model.variable()
z11_2_4 = model.variable()
y0_3_4 = model.variable()
z0_3_4 = model.variable()
y11_3_4 = model.variable()
z11_3_4 = model.variable()
y0_4_4 = model.variable()
z0_4_4 = model.variable()
y11_4_4 = model.variable()
z11_4_4 = model.variable()
y0_5_4 = model.variable()
z0_5_4 = model.variable()
y11_5_4 = model.variable()
z11_5_4 = model.variable()
y0_6_4 = model.variable()
z0_6_4 = model.variable()
y11_6_4 = model.variable()
z11_6_4 = model.variable()
y0_7_4 = model.variable()
z0_7_4 = model.variable()
y11_7_4 = model.variable()
z11_7_4 = model.variable()
y0_8_4 = model.variable()
z0_8_4 = model.variable()
y11_8_4 = model.variable()
z11_8_4 = model.variable()
y0_9_4 = model.variable()
z0_9_4 = model.variable()
y11_9_4 = model.variable()
z11_9_4 = model.variable()
y0_10_4 = model.variable()
z0_10_4 = model.variable()
y11_10_4 = model.variable()
z11_10_4 = model.variable()
y0_1_5 = model.variable()
z0_1_5 = model.variable()
y11_1_5 = model.variable()
z11_1_5 = model.variable()
y0_2_5 = model.variable()
z0_2_5 = model.variable()
y11_2_5 = model.variable()
z11_2_5 = model.variable()
y0_3_5 = model.variable()
z0_3_5 = model.variable()
y11_3_5 = model.variable()
z11_3_5 = model.variable()
y0_4_5 = model.variable()
z0_4_5 = model.variable()
y11_4_5 = model.variable()
z11_4_5 = model.variable()
y0_5_5 = model.variable()
z0_5_5 = model.variable()
y11_5_5 = model.variable()
z11_5_5 = model.variable()
y0_6_5 = model.variable()
z0_6_5 = model.variable()
y11_6_5 = model.variable()
z11_6_5 = model.variable()
y0_7_5 = model.variable()
z0_7_5 = model.variable()
y11_7_5 = model.variable()
z11_7_5 = model.variable()
y0_8_5 = model.variable()
z0_8_5 = model.variable()
y11_8_5 = model.variable()
z11_8_5 = model.variable()
y0_9_5 = model.variable()
z0_9_5 = model.variable()
y11_9_5 = model.variable()
z11_9_5 = model.variable()
y0_10_5 = model.variable()
z0_10_5 = model.variable()
y11_10_5 = model.variable()
z11_10_5 = model.variable()
y0_1_6 = model.variable()
z0_1_6 = model.variable()
y11_1_6 = model.variable()
z11_1_6 = model.variable()
y0_2_6 = model.variable()
z0_2_6 = model.variable()
y11_2_6 = model.variable()
z11_2_6 = model.variable()
y0_3_6 = model.variable()
z0_3_6 = model.variable()
y11_3_6 = model.variable()
z11_3_6 = model.variable()
y0_4_6 = model.variable()
z0_4_6 = model.variable()
y11_4_6 = model.variable()
z11_4_6 = model.variable()
y0_5_6 = model.variable()
z0_5_6 = model.variable()
y11_5_6 = model.variable()
z11_5_6 = model.variable()
y0_6_6 = model.variable()
z0_6_6 = model.variable()
y11_6_6 = model.variable()
z11_6_6 = model.variable()
y0_7_6 = model.variable()
z0_7_6 = model.variable()
y11_7_6 = model.variable()
z11_7_6 = model.variable()
y0_8_6 = model.variable()
z0_8_6 = model.variable()
y11_8_6 = model.variable()
z11_8_6 = model.variable()
y0_9_6 = model.variable()
z0_9_6 = model.variable()
y11_9_6 = model.variable()
z11_9_6 = model.variable()
y0_10_6 = model.variable()
z0_10_6 = model.variable()
y11_10_6 = model.variable()
z11_10_6 = model.variable()
y0_1_7 = model.variable()
z0_1_7 = model.variable()
y11_1_7 = model.variable()
z11_1_7 = model.variable()
y0_2_7 = model.variable()
z0_2_7 = model.variable()
y11_2_7 = model.variable()
z11_2_7 = model.variable()
y0_3_7 = model.variable()
z0_3_7 = model.variable()
y11_3_7 = model.variable()
z11_3_7 = model.variable()
y0_4_7 = model.variable()
z0_4_7 = model.variable()
y11_4_7 = model.variable()
z11_4_7 = model.variable()
y0_5_7 = model.variable()
z0_5_7 = model.variable()
y11_5_7 = model.variable()
z11_5_7 = model.variable()
y0_6_7 = model.variable()
z0_6_7 = model.variable()
y11_6_7 = model.variable()
z11_6_7 = model.variable()
y0_7_7 = model.variable()
z0_7_7 = model.variable()
y11_7_7 = model.variable()
z11_7_7 = model.variable()
y0_8_7 = model.variable()
z0_8_7 = model.variable()
y11_8_7 = model.variable()
z11_8_7 = model.variable()
y0_9_7 = model.variable()
z0_9_7 = model.variable()
y11_9_7 = model.variable()
z11_9_7 = model.variable()
y0_10_7 = model.variable()
z0_10_7 = model.variable()
y11_10_7 = model.variable()
z11_10_7 = model.variable()
y0_1_8 = model.variable()
z0_1_8 = model.variable()
y11_1_8 = model.variable()
z11_1_8 = model.variable()
y0_2_8 = model.variable()
z0_2_8 = model.variable()
y11_2_8 = model.variable()
z11_2_8 = model.variable()
y0_3_8 = model.variable()
z0_3_8 = model.variable()
y11_3_8 = model.variable()
z11_3_8 = model.variable()
y0_4_8 = model.variable()
z0_4_8 = model.variable()
y11_4_8 = model.variable()
z11_4_8 = model.variable()
y0_5_8 = model.variable()
z0_5_8 = model.variable()
y11_5_8 = model.variable()
z11_5_8 = model.variable()
y0_6_8 = model.variable()
z0_6_8 = model.variable()
y11_6_8 = model.variable()
z11_6_8 = model.variable()
y0_7_8 = model.variable()
z0_7_8 = model.variable()
y11_7_8 = model.variable()
z11_7_8 = model.variable()
y0_8_8 = model.variable()
z0_8_8 = model.variable()
y11_8_8 = model.variable()
z11_8_8 = model.variable()
y0_9_8 = model.variable()
z0_9_8 = model.variable()
y11_9_8 = model.variable()
z11_9_8 = model.variable()
y0_10_8 = model.variable()
z0_10_8 = model.variable()
y11_10_8 = model.variable()
z11_10_8 = model.variable()
y0_1_9 = model.variable()
z0_1_9 = model.variable()
y11_1_9 = model.variable()
z11_1_9 = model.variable()
y0_2_9 = model.variable()
z0_2_9 = model.variable()
y11_2_9 = model.variable()
z11_2_9 = model.variable()
y0_3_9 = model.variable()
z0_3_9 = model.variable()
y11_3_9 = model.variable()
z11_3_9 = model.variable()
y0_4_9 = model.variable()
z0_4_9 = model.variable()
y11_4_9 = model.variable()
z11_4_9 = model.variable()
y0_5_9 = model.variable()
z0_5_9 = model.variable()
y11_5_9 = model.variable()
z11_5_9 = model.variable()
y0_6_9 = model.variable()
z0_6_9 = model.variable()
y11_6_9 = model.variable()
z11_6_9 = model.variable()
y0_7_9 = model.variable()
z0_7_9 = model.variable()
y11_7_9 = model.variable()
z11_7_9 = model.variable()
y0_8_9 = model.variable()
z0_8_9 = model.variable()
y11_8_9 = model.variable()
z11_8_9 = model.variable()
y0_9_9 = model.variable()
z0_9_9 = model.variable()
y11_9_9 = model.variable()
z11_9_9 = model.variable()
y0_10_9 = model.variable()
z0_10_9 = model.variable()
y11_10_9 = model.variable()
z11_10_9 = model.variable()
y0_1_10 = model.variable()
z0_1_10 = model.variable()
y11_1_10 = model.variable()
z11_1_10 = model.variable()
y0_2_10 = model.variable()
z0_2_10 = model.variable()
y11_2_10 = model.variable()
z11_2_10 = model.variable()
y0_3_10 = model.variable()
z0_3_10 = model.variable()
y11_3_10 = model.variable()
z11_3_10 = model.variable()
y0_4_10 = model.variable()
z0_4_10 = model.variable()
y11_4_10 = model.variable()
z11_4_10 = model.variable()
y0_5_10 = model.variable()
z0_5_10 = model.variable()
y11_5_10 = model.variable()
z11_5_10 = model.variable()
y0_6_10 = model.variable()
z0_6_10 = model.variable()
y11_6_10 = model.variable()
z11_6_10 = model.variable()
y0_7_10 = model.variable()
z0_7_10 = model.variable()
y11_7_10 = model.variable()
z11_7_10 = model.variable()
y0_8_10 = model.variable()
z0_8_10 = model.variable()
y11_8_10 = model.variable()
z11_8_10 = model.variable()
y0_9_10 = model.variable()
z0_9_10 = model.variable()
y11_9_10 = model.variable()
z11_9_10 = model.variable()
y0_10_10 = model.variable()
z0_10_10 = model.variable()
y11_10_10 = model.variable()
z11_10_10 = model.variable()
x1_0_1 = model.variable()
z1_0_1 = model.variable()
x1_11_1 = model.variable()
z1_11_1 = model.variable()
x2_0_1 = model.variable()
z2_0_1 = model.variable()
x2_11_1 = model.variable()
z2_11_1 = model.variable()
x3_0_1 = model.variable()
z3_0_1 = model.variable()
x3_11_1 = model.variable()
z3_11_1 = model.variable()
x4_0_1 = model.variable()
z4_0_1 = model.variable()
x4_11_1 = model.variable()
z4_11_1 = model.variable()
x5_0_1 = model.variable()
z5_0_1 = model.variable()
x5_11_1 = model.variable()
z5_11_1 = model.variable()
x6_0_1 = model.variable()
z6_0_1 = model.variable()
x6_11_1 = model.variable()
z6_11_1 = model.variable()
x7_0_1 = model.variable()
z7_0_1 = model.variable()
x7_11_1 = model.variable()
z7_11_1 = model.variable()
x8_0_1 = model.variable()
z8_0_1 = model.variable()
x8_11_1 = model.variable()
z8_11_1 = model.variable()
x9_0_1 = model.variable()
z9_0_1 = model.variable()
x9_11_1 = model.variable()
z9_11_1 = model.variable()
x10_0_1 = model.variable()
z10_0_1 = model.variable()
x10_11_1 = model.variable()
z10_11_1 = model.variable()
x1_0_2 = model.variable()
z1_0_2 = model.variable()
x1_11_2 = model.variable()
z1_11_2 = model.variable()
x2_0_2 = model.variable()
z2_0_2 = model.variable()
x2_11_2 = model.variable()
z2_11_2 = model.variable()
x3_0_2 = model.variable()
z3_0_2 = model.variable()
x3_11_2 = model.variable()
z3_11_2 = model.variable()
x4_0_2 = model.variable()
z4_0_2 = model.variable()
x4_11_2 = model.variable()
z4_11_2 = model.variable()
x5_0_2 = model.variable()
z5_0_2 = model.variable()
x5_11_2 = model.variable()
z5_11_2 = model.variable()
x6_0_2 = model.variable()
z6_0_2 = model.variable()
x6_11_2 = model.variable()
z6_11_2 = model.variable()
x7_0_2 = model.variable()
z7_0_2 = model.variable()
x7_11_2 = model.variable()
z7_11_2 = model.variable()
x8_0_2 = model.variable()
z8_0_2 = model.variable()
x8_11_2 = model.variable()
z8_11_2 = model.variable()
x9_0_2 = model.variable()
z9_0_2 = model.variable()
x9_11_2 = model.variable()
z9_11_2 = model.variable()
x10_0_2 = model.variable()
z10_0_2 = model.variable()
x10_11_2 = model.variable()
z10_11_2 = model.variable()
x1_0_3 = model.variable()
z1_0_3 = model.variable()
x1_11_3 = model.variable()
z1_11_3 = model.variable()
x2_0_3 = model.variable()
z2_0_3 = model.variable()
x2_11_3 = model.variable()
z2_11_3 = model.variable()
x3_0_3 = model.variable()
z3_0_3 = model.variable()
x3_11_3 = model.variable()
z3_11_3 = model.variable()
x4_0_3 = model.variable()
z4_0_3 = model.variable()
x4_11_3 = model.variable()
z4_11_3 = model.variable()
x5_0_3 = model.variable()
z5_0_3 = model.variable()
x5_11_3 = model.variable()
z5_11_3 = model.variable()
x6_0_3 = model.variable()
z6_0_3 = model.variable()
x6_11_3 = model.variable()
z6_11_3 = model.variable()
x7_0_3 = model.variable()
z7_0_3 = model.variable()
x7_11_3 = model.variable()
z7_11_3 = model.variable()
x8_0_3 = model.variable()
z8_0_3 = model.variable()
x8_11_3 = model.variable()
z8_11_3 = model.variable()
x9_0_3 = model.variable()
z9_0_3 = model.variable()
x9_11_3 = model.variable()
z9_11_3 = model.variable()
x10_0_3 = model.variable()
z10_0_3 = model.variable()
x10_11_3 = model.variable()
z10_11_3 = model.variable()
x1_0_4 = model.variable()
z1_0_4 = model.variable()
x1_11_4 = model.variable()
z1_11_4 = model.variable()
x2_0_4 = model.variable()
z2_0_4 = model.variable()
x2_11_4 = model.variable()
z2_11_4 = model.variable()
x3_0_4 = model.variable()
z3_0_4 = model.variable()
x3_11_4 = model.variable()
z3_11_4 = model.variable()
x4_0_4 = model.variable()
z4_0_4 = model.variable()
x4_11_4 = model.variable()
z4_11_4 = model.variable()
x5_0_4 = model.variable()
z5_0_4 = model.variable()
x5_11_4 = model.variable()
z5_11_4 = model.variable()
x6_0_4 = model.variable()
z6_0_4 = model.variable()
x6_11_4 = model.variable()
z6_11_4 = model.variable()
x7_0_4 = model.variable()
z7_0_4 = model.variable()
x7_11_4 = model.variable()
z7_11_4 = model.variable()
x8_0_4 = model.variable()
z8_0_4 = model.variable()
x8_11_4 = model.variable()
z8_11_4 = model.variable()
x9_0_4 = model.variable()
z9_0_4 = model.variable()
x9_11_4 = model.variable()
z9_11_4 = model.variable()
x10_0_4 = model.variable()
z10_0_4 = model.variable()
x10_11_4 = model.variable()
z10_11_4 = model.variable()
x1_0_5 = model.variable()
z1_0_5 = model.variable()
x1_11_5 = model.variable()
z1_11_5 = model.variable()
x2_0_5 = model.variable()
z2_0_5 = model.variable()
x2_11_5 = model.variable()
z2_11_5 = model.variable()
x3_0_5 = model.variable()
z3_0_5 = model.variable()
x3_11_5 = model.variable()
z3_11_5 = model.variable()
x4_0_5 = model.variable()
z4_0_5 = model.variable()
x4_11_5 = model.variable()
z4_11_5 = model.variable()
x5_0_5 = model.variable()
z5_0_5 = model.variable()
x5_11_5 = model.variable()
z5_11_5 = model.variable()
x6_0_5 = model.variable()
z6_0_5 = model.variable()
x6_11_5 = model.variable()
z6_11_5 = model.variable()
x7_0_5 = model.variable()
z7_0_5 = model.variable()
x7_11_5 = model.variable()
z7_11_5 = model.variable()
x8_0_5 = model.variable()
z8_0_5 = model.variable()
x8_11_5 = model.variable()
z8_11_5 = model.variable()
x9_0_5 = model.variable()
z9_0_5 = model.variable()
x9_11_5 = model.variable()
z9_11_5 = model.variable()
x10_0_5 = model.variable()
z10_0_5 = model.variable()
x10_11_5 = model.variable()
z10_11_5 = model.variable()
x1_0_6 = model.variable()
z1_0_6 = model.variable()
x1_11_6 = model.variable()
z1_11_6 = model.variable()
x2_0_6 = model.variable()
z2_0_6 = model.variable()
x2_11_6 = model.variable()
z2_11_6 = model.variable()
x3_0_6 = model.variable()
z3_0_6 = model.variable()
x3_11_6 = model.variable()
z3_11_6 = model.variable()
x4_0_6 = model.variable()
z4_0_6 = model.variable()
x4_11_6 = model.variable()
z4_11_6 = model.variable()
x5_0_6 = model.variable()
z5_0_6 = model.variable()
x5_11_6 = model.variable()
z5_11_6 = model.variable()
x6_0_6 = model.variable()
z6_0_6 = model.variable()
x6_11_6 = model.variable()
z6_11_6 = model.variable()
x7_0_6 = model.variable()
z7_0_6 = model.variable()
x7_11_6 = model.variable()
z7_11_6 = model.variable()
x8_0_6 = model.variable()
z8_0_6 = model.variable()
x8_11_6 = model.variable()
z8_11_6 = model.variable()
x9_0_6 = model.variable()
z9_0_6 = model.variable()
x9_11_6 = model.variable()
z9_11_6 = model.variable()
x10_0_6 = model.variable()
z10_0_6 = model.variable()
x10_11_6 = model.variable()
z10_11_6 = model.variable()
x1_0_7 = model.variable()
z1_0_7 = model.variable()
x1_11_7 = model.variable()
z1_11_7 = model.variable()
x2_0_7 = model.variable()
z2_0_7 = model.variable()
x2_11_7 = model.variable()
z2_11_7 = model.variable()
x3_0_7 = model.variable()
z3_0_7 = model.variable()
x3_11_7 = model.variable()
z3_11_7 = model.variable()
x4_0_7 = model.variable()
z4_0_7 = model.variable()
x4_11_7 = model.variable()
z4_11_7 = model.variable()
x5_0_7 = model.variable()
z5_0_7 = model.variable()
x5_11_7 = model.variable()
z5_11_7 = model.variable()
x6_0_7 = model.variable()
z6_0_7 = model.variable()
x6_11_7 = model.variable()
z6_11_7 = model.variable()
x7_0_7 = model.variable()
z7_0_7 = model.variable()
x7_11_7 = model.variable()
z7_11_7 = model.variable()
x8_0_7 = model.variable()
z8_0_7 = model.variable()
x8_11_7 = model.variable()
z8_11_7 = model.variable()
x9_0_7 = model.variable()
z9_0_7 = model.variable()
x9_11_7 = model.variable()
z9_11_7 = model.variable()
x10_0_7 = model.variable()
z10_0_7 = model.variable()
x10_11_7 = model.variable()
z10_11_7 = model.variable()
x1_0_8 = model.variable()
z1_0_8 = model.variable()
x1_11_8 = model.variable()
z1_11_8 = model.variable()
x2_0_8 = model.variable()
z2_0_8 = model.variable()
x2_11_8 = model.variable()
z2_11_8 = model.variable()
x3_0_8 = model.variable()
z3_0_8 = model.variable()
x3_11_8 = model.variable()
z3_11_8 = model.variable()
x4_0_8 = model.variable()
z4_0_8 = model.variable()
x4_11_8 = model.variable()
z4_11_8 = model.variable()
x5_0_8 = model.variable()
z5_0_8 = model.variable()
x5_11_8 = model.variable()
z5_11_8 = model.variable()
x6_0_8 = model.variable()
z6_0_8 = model.variable()
x6_11_8 = model.variable()
z6_11_8 = model.variable()
x7_0_8 = model.variable()
z7_0_8 = model.variable()
x7_11_8 = model.variable()
z7_11_8 = model.variable()
x8_0_8 = model.variable()
z8_0_8 = model.variable()
x8_11_8 = model.variable()
z8_11_8 = model.variable()
x9_0_8 = model.variable()
z9_0_8 = model.variable()
x9_11_8 = model.variable()
z9_11_8 = model.variable()
x10_0_8 = model.variable()
z10_0_8 = model.variable()
x10_11_8 = model.variable()
z10_11_8 = model.variable()
x1_0_9 = model.variable()
z1_0_9 = model.variable()
x1_11_9 = model.variable()
z1_11_9 = model.variable()
x2_0_9 = model.variable()
z2_0_9 = model.variable()
x2_11_9 = model.variable()
z2_11_9 = model.variable()
x3_0_9 = model.variable()
z3_0_9 = model.variable()
x3_11_9 = model.variable()
z3_11_9 = model.variable()
x4_0_9 = model.variable()
z4_0_9 = model.variable()
x4_11_9 = model.variable()
z4_11_9 = model.variable()
x5_0_9 = model.variable()
z5_0_9 = model.variable()
x5_11_9 = model.variable()
z5_11_9 = model.variable()
x6_0_9 = model.variable()
z6_0_9 = model.variable()
x6_11_9 = model.variable()
z6_11_9 = model.variable()
x7_0_9 = model.variable()
z7_0_9 = model.variable()
x7_11_9 = model.variable()
z7_11_9 = model.variable()
x8_0_9 = model.variable()
z8_0_9 = model.variable()
x8_11_9 = model.variable()
z8_11_9 = model.variable()
x9_0_9 = model.variable()
z9_0_9 = model.variable()
x9_11_9 = model.variable()
z9_11_9 = model.variable()
x10_0_9 = model.variable()
z10_0_9 = model.variable()
x10_11_9 = model.variable()
z10_11_9 = model.variable()
x1_0_10 = model.variable()
z1_0_10 = model.variable()
x1_11_10 = model.variable()
z1_11_10 = model.variable()
x2_0_10 = model.variable()
z2_0_10 = model.variable()
x2_11_10 = model.variable()
z2_11_10 = model.variable()
x3_0_10 = model.variable()
z3_0_10 = model.variable()
x3_11_10 = model.variable()
z3_11_10 = model.variable()
x4_0_10 = model.variable()
z4_0_10 = model.variable()
x4_11_10 = model.variable()
z4_11_10 = model.variable()
x5_0_10 = model.variable()
z5_0_10 = model.variable()
x5_11_10 = model.variable()
z5_11_10 = model.variable()
x6_0_10 = model.variable()
z6_0_10 = model.variable()
x6_11_10 = model.variable()
z6_11_10 = model.variable()
x7_0_10 = model.variable()
z7_0_10 = model.variable()
x7_11_10 = model.variable()
z7_11_10 = model.variable()
x8_0_10 = model.variable()
z8_0_10 = model.variable()
x8_11_10 = model.variable()
z8_11_10 = model.variable()
x9_0_10 = model.variable()
z9_0_10 = model.variable()
x9_11_10 = model.variable()
z9_11_10 = model.variable()
x10_0_10 = model.variable()
z10_0_10 = model.variable()
x10_11_10 = model.variable()
z10_11_10 = model.variable()
x1_1_0 = model.variable()
y1_1_0 = model.variable()
x1_1_11 = model.variable()
y1_1_11 = model.variable()
x2_1_0 = model.variable()
y2_1_0 = model.variable()
x2_1_11 = model.variable()
y2_1_11 = model.variable()
x3_1_0 = model.variable()
y3_1_0 = model.variable()
x3_1_11 = model.variable()
y3_1_11 = model.variable()
x4_1_0 = model.variable()
y4_1_0 = model.variable()
x4_1_11 = model.variable()
y4_1_11 = model.variable()
x5_1_0 = model.variable()
y5_1_0 = model.variable()
x5_1_11 = model.variable()
y5_1_11 = model.variable()
x6_1_0 = model.variable()
y6_1_0 = model.variable()
x6_1_11 = model.variable()
y6_1_11 = model.variable()
x7_1_0 = model.variable()
y7_1_0 = model.variable()
x7_1_11 = model.variable()
y7_1_11 = model.variable()
x8_1_0 = model.variable()
y8_1_0 = model.variable()
x8_1_11 = model.variable()
y8_1_11 = model.variable()
x9_1_0 = model.variable()
y9_1_0 = model.variable()
x9_1_11 = model.variable()
y9_1_11 = model.variable()
x10_1_0 = model.variable()
y10_1_0 = model.variable()
x10_1_11 = model.variable()
y10_1_11 = model.variable()
x1_2_0 = model.variable()
y1_2_0 = model.variable()
x1_2_11 = model.variable()
y1_2_11 = model.variable()
x2_2_0 = model.variable()
y2_2_0 = model.variable()
x2_2_11 = model.variable()
y2_2_11 = model.variable()
x3_2_0 = model.variable()
y3_2_0 = model.variable()
x3_2_11 = model.variable()
y3_2_11 = model.variable()
x4_2_0 = model.variable()
y4_2_0 = model.variable()
x4_2_11 = model.variable()
y4_2_11 = model.variable()
x5_2_0 = model.variable()
y5_2_0 = model.variable()
x5_2_11 = model.variable()
y5_2_11 = model.variable()
x6_2_0 = model.variable()
y6_2_0 = model.variable()
x6_2_11 = model.variable()
y6_2_11 = model.variable()
x7_2_0 = model.variable()
y7_2_0 = model.variable()
x7_2_11 = model.variable()
y7_2_11 = model.variable()
x8_2_0 = model.variable()
y8_2_0 = model.variable()
x8_2_11 = model.variable()
y8_2_11 = model.variable()
x9_2_0 = model.variable()
y9_2_0 = model.variable()
x9_2_11 = model.variable()
y9_2_11 = model.variable()
x10_2_0 = model.variable()
y10_2_0 = model.variable()
x10_2_11 = model.variable()
y10_2_11 = model.variable()
x1_3_0 = model.variable()
y1_3_0 = model.variable()
x1_3_11 = model.variable()
y1_3_11 = model.variable()
x2_3_0 = model.variable()
y2_3_0 = model.variable()
x2_3_11 = model.variable()
y2_3_11 = model.variable()
x3_3_0 = model.variable()
y3_3_0 = model.variable()
x3_3_11 = model.variable()
y3_3_11 = model.variable()
x4_3_0 = model.variable()
y4_3_0 = model.variable()
x4_3_11 = model.variable()
y4_3_11 = model.variable()
x5_3_0 = model.variable()
y5_3_0 = model.variable()
x5_3_11 = model.variable()
y5_3_11 = model.variable()
x6_3_0 = model.variable()
y6_3_0 = model.variable()
x6_3_11 = model.variable()
y6_3_11 = model.variable()
x7_3_0 = model.variable()
y7_3_0 = model.variable()
x7_3_11 = model.variable()
y7_3_11 = model.variable()
x8_3_0 = model.variable()
y8_3_0 = model.variable()
x8_3_11 = model.variable()
y8_3_11 = model.variable()
x9_3_0 = model.variable()
y9_3_0 = model.variable()
x9_3_11 = model.variable()
y9_3_11 = model.variable()
x10_3_0 = model.variable()
y10_3_0 = model.variable()
x10_3_11 = model.variable()
y10_3_11 = model.variable()
x1_4_0 = model.variable()
y1_4_0 = model.variable()
x1_4_11 = model.variable()
y1_4_11 = model.variable()
x2_4_0 = model.variable()
y2_4_0 = model.variable()
x2_4_11 = model.variable()
y2_4_11 = model.variable()
x3_4_0 = model.variable()
y3_4_0 = model.variable()
x3_4_11 = model.variable()
y3_4_11 = model.variable()
x4_4_0 = model.variable()
y4_4_0 = model.variable()
x4_4_11 = model.variable()
y4_4_11 = model.variable()
x5_4_0 = model.variable()
y5_4_0 = model.variable()
x5_4_11 = model.variable()
y5_4_11 = model.variable()
x6_4_0 = model.variable()
y6_4_0 = model.variable()
x6_4_11 = model.variable()
y6_4_11 = model.variable()
x7_4_0 = model.variable()
y7_4_0 = model.variable()
x7_4_11 = model.variable()
y7_4_11 = model.variable()
x8_4_0 = model.variable()
y8_4_0 = model.variable()
x8_4_11 = model.variable()
y8_4_11 = model.variable()
x9_4_0 = model.variable()
y9_4_0 = model.variable()
x9_4_11 = model.variable()
y9_4_11 = model.variable()
x10_4_0 = model.variable()
y10_4_0 = model.variable()
x10_4_11 = model.variable()
y10_4_11 = model.variable()
x1_5_0 = model.variable()
y1_5_0 = model.variable()
x1_5_11 = model.variable()
y1_5_11 = model.variable()
x2_5_0 = model.variable()
y2_5_0 = model.variable()
x2_5_11 = model.variable()
y2_5_11 = model.variable()
x3_5_0 = model.variable()
y3_5_0 = model.variable()
x3_5_11 = model.variable()
y3_5_11 = model.variable()
x4_5_0 = model.variable()
y4_5_0 = model.variable()
x4_5_11 = model.variable()
y4_5_11 = model.variable()
x5_5_0 = model.variable()
y5_5_0 = model.variable()
x5_5_11 = model.variable()
y5_5_11 = model.variable()
x6_5_0 = model.variable()
y6_5_0 = model.variable()
x6_5_11 = model.variable()
y6_5_11 = model.variable()
x7_5_0 = model.variable()
y7_5_0 = model.variable()
x7_5_11 = model.variable()
y7_5_11 = model.variable()
x8_5_0 = model.variable()
y8_5_0 = model.variable()
x8_5_11 = model.variable()
y8_5_11 = model.variable()
x9_5_0 = model.variable()
y9_5_0 = model.variable()
x9_5_11 = model.variable()
y9_5_11 = model.variable()
x10_5_0 = model.variable()
y10_5_0 = model.variable()
x10_5_11 = model.variable()
y10_5_11 = model.variable()
x1_6_0 = model.variable()
y1_6_0 = model.variable()
x1_6_11 = model.variable()
y1_6_11 = model.variable()
x2_6_0 = model.variable()
y2_6_0 = model.variable()
x2_6_11 = model.variable()
y2_6_11 = model.variable()
x3_6_0 = model.variable()
y3_6_0 = model.variable()
x3_6_11 = model.variable()
y3_6_11 = model.variable()
x4_6_0 = model.variable()
y4_6_0 = model.variable()
x4_6_11 = model.variable()
y4_6_11 = model.variable()
x5_6_0 = model.variable()
y5_6_0 = model.variable()
x5_6_11 = model.variable()
y5_6_11 = model.variable()
x6_6_0 = model.variable()
y6_6_0 = model.variable()
x6_6_11 = model.variable()
y6_6_11 = model.variable()
x7_6_0 = model.variable()
y7_6_0 = model.variable()
x7_6_11 = model.variable()
y7_6_11 = model.variable()
x8_6_0 = model.variable()
y8_6_0 = model.variable()
x8_6_11 = model.variable()
y8_6_11 = model.variable()
x9_6_0 = model.variable()
y9_6_0 = model.variable()
x9_6_11 = model.variable()
y9_6_11 = model.variable()
x10_6_0 = model.variable()
y10_6_0 = model.variable()
x10_6_11 = model.variable()
y10_6_11 = model.variable()
x1_7_0 = model.variable()
y1_7_0 = model.variable()
x1_7_11 = model.variable()
y1_7_11 = model.variable()
x2_7_0 = model.variable()
y2_7_0 = model.variable()
x2_7_11 = model.variable()
y2_7_11 = model.variable()
x3_7_0 = model.variable()
y3_7_0 = model.variable()
x3_7_11 = model.variable()
y3_7_11 = model.variable()
x4_7_0 = model.variable()
y4_7_0 = model.variable()
x4_7_11 = model.variable()
y4_7_11 = model.variable()
x5_7_0 = model.variable()
y5_7_0 = model.variable()
x5_7_11 = model.variable()
y5_7_11 = model.variable()
x6_7_0 = model.variable()
y6_7_0 = model.variable()
x6_7_11 = model.variable()
y6_7_11 = model.variable()
x7_7_0 = model.variable()
y7_7_0 = model.variable()
x7_7_11 = model.variable()
y7_7_11 = model.variable()
x8_7_0 = model.variable()
y8_7_0 = model.variable()
x8_7_11 = model.variable()
y8_7_11 = model.variable()
x9_7_0 = model.variable()
y9_7_0 = model.variable()
x9_7_11 = model.variable()
y9_7_11 = model.variable()
x10_7_0 = model.variable()
y10_7_0 = model.variable()
x10_7_11 = model.variable()
y10_7_11 = model.variable()
x1_8_0 = model.variable()
y1_8_0 = model.variable()
x1_8_11 = model.variable()
y1_8_11 = model.variable()
x2_8_0 = model.variable()
y2_8_0 = model.variable()
x2_8_11 = model.variable()
y2_8_11 = model.variable()
x3_8_0 = model.variable()
y3_8_0 = model.variable()
x3_8_11 = model.variable()
y3_8_11 = model.variable()
x4_8_0 = model.variable()
y4_8_0 = model.variable()
x4_8_11 = model.variable()
y4_8_11 = model.variable()
x5_8_0 = model.variable()
y5_8_0 = model.variable()
x5_8_11 = model.variable()
y5_8_11 = model.variable()
x6_8_0 = model.variable()
y6_8_0 = model.variable()
x6_8_11 = model.variable()
y6_8_11 = model.variable()
x7_8_0 = model.variable()
y7_8_0 = model.variable()
x7_8_11 = model.variable()
y7_8_11 = model.variable()
x8_8_0 = model.variable()
y8_8_0 = model.variable()
x8_8_11 = model.variable()
y8_8_11 = model.variable()
x9_8_0 = model.variable()
y9_8_0 = model.variable()
x9_8_11 = model.variable()
y9_8_11 = model.variable()
x10_8_0 = model.variable()
y10_8_0 = model.variable()
x10_8_11 = model.variable()
y10_8_11 = model.variable()
x1_9_0 = model.variable()
y1_9_0 = model.variable()
x1_9_11 = model.variable()
y1_9_11 = model.variable()
x2_9_0 = model.variable()
y2_9_0 = model.variable()
x2_9_11 = model.variable()
y2_9_11 = model.variable()
x3_9_0 = model.variable()
y3_9_0 = model.variable()
x3_9_11 = model.variable()
y3_9_11 = model.variable()
x4_9_0 = model.variable()
y4_9_0 = model.variable()
x4_9_11 = model.variable()
y4_9_11 = model.variable()
x5_9_0 = model.variable()
y5_9_0 = model.variable()
x5_9_11 = model.variable()
y5_9_11 = model.variable()
x6_9_0 = model.variable()
y6_9_0 = model.variable()
x6_9_11 = model.variable()
y6_9_11 = model.variable()
x7_9_0 = model.variable()
y7_9_0 = model.variable()
x7_9_11 = model.variable()
y7_9_11 = model.variable()
x8_9_0 = model.variable()
y8_9_0 = model.variable()
x8_9_11 = model.variable()
y8_9_11 = model.variable()
x9_9_0 = model.variable()
y9_9_0 = model.variable()
x9_9_11 = model.variable()
y9_9_11 = model.variable()
x10_9_0 = model.variable()
y10_9_0 = model.variable()
x10_9_11 = model.variable()
y10_9_11 = model.variable()
x1_10_0 = model.variable()
y1_10_0 = model.variable()
x1_10_11 = model.variable()
y1_10_11 = model.variable()
x2_10_0 = model.variable()
y2_10_0 = model.variable()
x2_10_11 = model.variable()
y2_10_11 = model.variable()
x3_10_0 = model.variable()
y3_10_0 = model.variable()
x3_10_11 = model.variable()
y3_10_11 = model.variable()
x4_10_0 = model.variable()
y4_10_0 = model.variable()
x4_10_11 = model.variable()
y4_10_11 = model.variable()
x5_10_0 = model.variable()
y5_10_0 = model.variable()
x5_10_11 = model.variable()
y5_10_11 = model.variable()
x6_10_0 = model.variable()
y6_10_0 = model.variable()
x6_10_11 = model.variable()
y6_10_11 = model.variable()
x7_10_0 = model.variable()
y7_10_0 = model.variable()
x7_10_11 = model.variable()
y7_10_11 = model.variable()
x8_10_0 = model.variable()
y8_10_0 = model.variable()
x8_10_11 = model.variable()
y8_10_11 = model.variable()
x9_10_0 = model.variable()
y9_10_0 = model.variable()
x9_10_11 = model.variable()
y9_10_11 = model.variable()
x10_10_0 = model.variable()
y10_10_0 = model.variable()
x10_10_11 = model.variable()
y10_10_11 = model.variable()

model.add(\
    0.5*(x1_1_1 - 1.0)*(x1_1_1 - 1.0) + 0.5*(y1_1_1 - 1.0)*(y1_1_1 - 1.0) + \
    0.5*(z1_1_1 - 1.0)*(z1_1_1 - 1.0) + 0.5*(x2_1_1 - 1.0)*(x2_1_1 - 1.0) + \
    0.5*(y2_1_1 - 1.0)*(y2_1_1 - 1.0) + 0.5*(z2_1_1 - 1.0)*(z2_1_1 - 1.0) + \
    0.5*(x3_1_1 - 1.0)*(x3_1_1 - 1.0) + 0.5*(y3_1_1 - 1.0)*(y3_1_1 - 1.0) + \
    0.5*(z3_1_1 - 1.0)*(z3_1_1 - 1.0) + 0.5*(x4_1_1 - 1.0)*(x4_1_1 - 1.0) + \
    0.5*(y4_1_1 - 1.0)*(y4_1_1 - 1.0) + 0.5*(z4_1_1 - 1.0)*(z4_1_1 - 1.0) + \
    0.5*(x5_1_1 - 1.0)*(x5_1_1 - 1.0) + 0.5*(y5_1_1 - 1.0)*(y5_1_1 - 1.0) + \
    0.5*(z5_1_1 - 1.0)*(z5_1_1 - 1.0) + 0.5*(x6_1_1 - 1.0)*(x6_1_1 - 1.0) + \
    0.5*(y6_1_1 - 1.0)*(y6_1_1 - 1.0) + 0.5*(z6_1_1 - 1.0)*(z6_1_1 - 1.0) + \
    0.5*(x7_1_1 - 1.0)*(x7_1_1 - 1.0) + 0.5*(y7_1_1 - 1.0)*(y7_1_1 - 1.0) + \
    0.5*(z7_1_1 - 1.0)*(z7_1_1 - 1.0) + 0.5*(x8_1_1 - 1.0)*(x8_1_1 - 1.0) + \
    0.5*(y8_1_1 - 1.0)*(y8_1_1 - 1.0) + 0.5*(z8_1_1 - 1.0)*(z8_1_1 - 1.0) + \
    0.5*(x9_1_1 - 1.0)*(x9_1_1 - 1.0) + 0.5*(y9_1_1 - 1.0)*(y9_1_1 - 1.0) + \
    0.5*(z9_1_1 - 1.0)*(z9_1_1 - 1.0) + 0.5*(x1_2_1 - 1.0)*(x1_2_1 - 1.0) + \
    0.5*(y1_2_1 - 1.0)*(y1_2_1 - 1.0) + 0.5*(z1_2_1 - 1.0)*(z1_2_1 - 1.0) + \
    0.5*(x2_2_1 - 1.0)*(x2_2_1 - 1.0) + 0.5*(y2_2_1 - 1.0)*(y2_2_1 - 1.0) + \
    0.5*(z2_2_1 - 1.0)*(z2_2_1 - 1.0) + 0.5*(x3_2_1 - 1.0)*(x3_2_1 - 1.0) + \
    0.5*(y3_2_1 - 1.0)*(y3_2_1 - 1.0) + 0.5*(z3_2_1 - 1.0)*(z3_2_1 - 1.0) + \
    0.5*(x4_2_1 - 1.0)*(x4_2_1 - 1.0) + 0.5*(y4_2_1 - 1.0)*(y4_2_1 - 1.0) + \
    0.5*(z4_2_1 - 1.0)*(z4_2_1 - 1.0) + 0.5*(x5_2_1 - 1.0)*(x5_2_1 - 1.0) + \
    0.5*(y5_2_1 - 1.0)*(y5_2_1 - 1.0) + 0.5*(z5_2_1 - 1.0)*(z5_2_1 - 1.0) + \
    0.5*(x6_2_1 - 1.0)*(x6_2_1 - 1.0) + 0.5*(y6_2_1 - 1.0)*(y6_2_1 - 1.0) + \
    0.5*(z6_2_1 - 1.0)*(z6_2_1 - 1.0) + 0.5*(x7_2_1 - 1.0)*(x7_2_1 - 1.0) + \
    0.5*(y7_2_1 - 1.0)*(y7_2_1 - 1.0) + 0.5*(z7_2_1 - 1.0)*(z7_2_1 - 1.0) + \
    0.5*(x8_2_1 - 1.0)*(x8_2_1 - 1.0) + 0.5*(y8_2_1 - 1.0)*(y8_2_1 - 1.0) + \
    0.5*(z8_2_1 - 1.0)*(z8_2_1 - 1.0) + 0.5*(x9_2_1 - 1.0)*(x9_2_1 - 1.0) + \
    0.5*(y9_2_1 - 1.0)*(y9_2_1 - 1.0) + 0.5*(z9_2_1 - 1.0)*(z9_2_1 - 1.0) + \
    0.5*(x1_3_1 - 1.0)*(x1_3_1 - 1.0) + 0.5*(y1_3_1 - 1.0)*(y1_3_1 - 1.0) + \
    0.5*(z1_3_1 - 1.0)*(z1_3_1 - 1.0) + 0.5*(x2_3_1 - 1.0)*(x2_3_1 - 1.0) + \
    0.5*(y2_3_1 - 1.0)*(y2_3_1 - 1.0) + 0.5*(z2_3_1 - 1.0)*(z2_3_1 - 1.0) + \
    0.5*(x3_3_1 - 1.0)*(x3_3_1 - 1.0) + 0.5*(y3_3_1 - 1.0)*(y3_3_1 - 1.0) + \
    0.5*(z3_3_1 - 1.0)*(z3_3_1 - 1.0) + 0.5*(x4_3_1 - 1.0)*(x4_3_1 - 1.0) + \
    0.5*(y4_3_1 - 1.0)*(y4_3_1 - 1.0) + 0.5*(z4_3_1 - 1.0)*(z4_3_1 - 1.0) + \
    0.5*(x5_3_1 - 1.0)*(x5_3_1 - 1.0) + 0.5*(y5_3_1 - 1.0)*(y5_3_1 - 1.0) + \
    0.5*(z5_3_1 - 1.0)*(z5_3_1 - 1.0) + 0.5*(x6_3_1 - 1.0)*(x6_3_1 - 1.0) + \
    0.5*(y6_3_1 - 1.0)*(y6_3_1 - 1.0) + 0.5*(z6_3_1 - 1.0)*(z6_3_1 - 1.0) + \
    0.5*(x7_3_1 - 1.0)*(x7_3_1 - 1.0) + 0.5*(y7_3_1 - 1.0)*(y7_3_1 - 1.0) + \
    0.5*(z7_3_1 - 1.0)*(z7_3_1 - 1.0) + 0.5*(x8_3_1 - 1.0)*(x8_3_1 - 1.0) + \
    0.5*(y8_3_1 - 1.0)*(y8_3_1 - 1.0) + 0.5*(z8_3_1 - 1.0)*(z8_3_1 - 1.0) + \
    0.5*(x9_3_1 - 1.0)*(x9_3_1 - 1.0) + 0.5*(y9_3_1 - 1.0)*(y9_3_1 - 1.0) + \
    0.5*(z9_3_1 - 1.0)*(z9_3_1 - 1.0) + 0.5*(x1_4_1 - 1.0)*(x1_4_1 - 1.0) + \
    0.5*(y1_4_1 - 1.0)*(y1_4_1 - 1.0) + 0.5*(z1_4_1 - 1.0)*(z1_4_1 - 1.0) + \
    0.5*(x2_4_1 - 1.0)*(x2_4_1 - 1.0) + 0.5*(y2_4_1 - 1.0)*(y2_4_1 - 1.0) + \
    0.5*(z2_4_1 - 1.0)*(z2_4_1 - 1.0) + 0.5*(x3_4_1 - 1.0)*(x3_4_1 - 1.0) + \
    0.5*(y3_4_1 - 1.0)*(y3_4_1 - 1.0) + 0.5*(z3_4_1 - 1.0)*(z3_4_1 - 1.0) + \
    0.5*(x4_4_1 - 1.0)*(x4_4_1 - 1.0) + 0.5*(y4_4_1 - 1.0)*(y4_4_1 - 1.0) + \
    0.5*(z4_4_1 - 1.0)*(z4_4_1 - 1.0) + 0.5*(x5_4_1 - 1.0)*(x5_4_1 - 1.0) + \
    0.5*(y5_4_1 - 1.0)*(y5_4_1 - 1.0) + 0.5*(z5_4_1 - 1.0)*(z5_4_1 - 1.0) + \
    0.5*(x6_4_1 - 1.0)*(x6_4_1 - 1.0) + 0.5*(y6_4_1 - 1.0)*(y6_4_1 - 1.0) + \
    0.5*(z6_4_1 - 1.0)*(z6_4_1 - 1.0) + 0.5*(x7_4_1 - 1.0)*(x7_4_1 - 1.0) + \
    0.5*(y7_4_1 - 1.0)*(y7_4_1 - 1.0) + 0.5*(z7_4_1 - 1.0)*(z7_4_1 - 1.0) + \
    0.5*(x8_4_1 - 1.0)*(x8_4_1 - 1.0) + 0.5*(y8_4_1 - 1.0)*(y8_4_1 - 1.0) + \
    0.5*(z8_4_1 - 1.0)*(z8_4_1 - 1.0) + 0.5*(x9_4_1 - 1.0)*(x9_4_1 - 1.0) + \
    0.5*(y9_4_1 - 1.0)*(y9_4_1 - 1.0) + 0.5*(z9_4_1 - 1.0)*(z9_4_1 - 1.0) + \
    0.5*(x1_5_1 - 1.0)*(x1_5_1 - 1.0) + 0.5*(y1_5_1 - 1.0)*(y1_5_1 - 1.0) + \
    0.5*(z1_5_1 - 1.0)*(z1_5_1 - 1.0) + 0.5*(x2_5_1 - 1.0)*(x2_5_1 - 1.0) + \
    0.5*(y2_5_1 - 1.0)*(y2_5_1 - 1.0) + 0.5*(z2_5_1 - 1.0)*(z2_5_1 - 1.0) + \
    0.5*(x3_5_1 - 1.0)*(x3_5_1 - 1.0) + 0.5*(y3_5_1 - 1.0)*(y3_5_1 - 1.0) + \
    0.5*(z3_5_1 - 1.0)*(z3_5_1 - 1.0) + 0.5*(x4_5_1 - 1.0)*(x4_5_1 - 1.0) + \
    0.5*(y4_5_1 - 1.0)*(y4_5_1 - 1.0) + 0.5*(z4_5_1 - 1.0)*(z4_5_1 - 1.0) + \
    0.5*(x5_5_1 - 1.0)*(x5_5_1 - 1.0) + 0.5*(y5_5_1 - 1.0)*(y5_5_1 - 1.0) + \
    0.5*(z5_5_1 - 1.0)*(z5_5_1 - 1.0) + 0.5*(x6_5_1 - 1.0)*(x6_5_1 - 1.0) + \
    0.5*(y6_5_1 - 1.0)*(y6_5_1 - 1.0) + 0.5*(z6_5_1 - 1.0)*(z6_5_1 - 1.0) + \
    0.5*(x7_5_1 - 1.0)*(x7_5_1 - 1.0) + 0.5*(y7_5_1 - 1.0)*(y7_5_1 - 1.0) + \
    0.5*(z7_5_1 - 1.0)*(z7_5_1 - 1.0) + 0.5*(x8_5_1 - 1.0)*(x8_5_1 - 1.0) + \
    0.5*(y8_5_1 - 1.0)*(y8_5_1 - 1.0) + 0.5*(z8_5_1 - 1.0)*(z8_5_1 - 1.0) + \
    0.5*(x9_5_1 - 1.0)*(x9_5_1 - 1.0) + 0.5*(y9_5_1 - 1.0)*(y9_5_1 - 1.0) + \
    0.5*(z9_5_1 - 1.0)*(z9_5_1 - 1.0) + 0.5*(x1_6_1 - 1.0)*(x1_6_1 - 1.0) + \
    0.5*(y1_6_1 - 1.0)*(y1_6_1 - 1.0) + 0.5*(z1_6_1 - 1.0)*(z1_6_1 - 1.0) + \
    0.5*(x2_6_1 - 1.0)*(x2_6_1 - 1.0) + 0.5*(y2_6_1 - 1.0)*(y2_6_1 - 1.0) + \
    0.5*(z2_6_1 - 1.0)*(z2_6_1 - 1.0) + 0.5*(x3_6_1 - 1.0)*(x3_6_1 - 1.0) + \
    0.5*(y3_6_1 - 1.0)*(y3_6_1 - 1.0) + 0.5*(z3_6_1 - 1.0)*(z3_6_1 - 1.0) + \
    0.5*(x4_6_1 - 1.0)*(x4_6_1 - 1.0) + 0.5*(y4_6_1 - 1.0)*(y4_6_1 - 1.0) + \
    0.5*(z4_6_1 - 1.0)*(z4_6_1 - 1.0) + 0.5*(x5_6_1 - 1.0)*(x5_6_1 - 1.0) + \
    0.5*(y5_6_1 - 1.0)*(y5_6_1 - 1.0) + 0.5*(z5_6_1 - 1.0)*(z5_6_1 - 1.0) + \
    0.5*(x6_6_1 - 1.0)*(x6_6_1 - 1.0) + 0.5*(y6_6_1 - 1.0)*(y6_6_1 - 1.0) + \
    0.5*(z6_6_1 - 1.0)*(z6_6_1 - 1.0) + 0.5*(x7_6_1 - 1.0)*(x7_6_1 - 1.0) + \
    0.5*(y7_6_1 - 1.0)*(y7_6_1 - 1.0) + 0.5*(z7_6_1 - 1.0)*(z7_6_1 - 1.0) + \
    0.5*(x8_6_1 - 1.0)*(x8_6_1 - 1.0) + 0.5*(y8_6_1 - 1.0)*(y8_6_1 - 1.0) + \
    0.5*(z8_6_1 - 1.0)*(z8_6_1 - 1.0) + 0.5*(x9_6_1 - 1.0)*(x9_6_1 - 1.0) + \
    0.5*(y9_6_1 - 1.0)*(y9_6_1 - 1.0) + 0.5*(z9_6_1 - 1.0)*(z9_6_1 - 1.0) + \
    0.5*(x1_7_1 - 1.0)*(x1_7_1 - 1.0) + 0.5*(y1_7_1 - 1.0)*(y1_7_1 - 1.0) + \
    0.5*(z1_7_1 - 1.0)*(z1_7_1 - 1.0) + 0.5*(x2_7_1 - 1.0)*(x2_7_1 - 1.0) + \
    0.5*(y2_7_1 - 1.0)*(y2_7_1 - 1.0) + 0.5*(z2_7_1 - 1.0)*(z2_7_1 - 1.0) + \
    0.5*(x3_7_1 - 1.0)*(x3_7_1 - 1.0) + 0.5*(y3_7_1 - 1.0)*(y3_7_1 - 1.0) + \
    0.5*(z3_7_1 - 1.0)*(z3_7_1 - 1.0) + 0.5*(x4_7_1 - 1.0)*(x4_7_1 - 1.0) + \
    0.5*(y4_7_1 - 1.0)*(y4_7_1 - 1.0) + 0.5*(z4_7_1 - 1.0)*(z4_7_1 - 1.0) + \
    0.5*(x5_7_1 - 1.0)*(x5_7_1 - 1.0) + 0.5*(y5_7_1 - 1.0)*(y5_7_1 - 1.0) + \
    0.5*(z5_7_1 - 1.0)*(z5_7_1 - 1.0) + 0.5*(x6_7_1 - 1.0)*(x6_7_1 - 1.0) + \
    0.5*(y6_7_1 - 1.0)*(y6_7_1 - 1.0) + 0.5*(z6_7_1 - 1.0)*(z6_7_1 - 1.0) + \
    0.5*(x7_7_1 - 1.0)*(x7_7_1 - 1.0) + 0.5*(y7_7_1 - 1.0)*(y7_7_1 - 1.0) + \
    0.5*(z7_7_1 - 1.0)*(z7_7_1 - 1.0) + 0.5*(x8_7_1 - 1.0)*(x8_7_1 - 1.0) + \
    0.5*(y8_7_1 - 1.0)*(y8_7_1 - 1.0) + 0.5*(z8_7_1 - 1.0)*(z8_7_1 - 1.0) + \
    0.5*(x9_7_1 - 1.0)*(x9_7_1 - 1.0) + 0.5*(y9_7_1 - 1.0)*(y9_7_1 - 1.0) + \
    0.5*(z9_7_1 - 1.0)*(z9_7_1 - 1.0) + 0.5*(x1_8_1 - 1.0)*(x1_8_1 - 1.0) + \
    0.5*(y1_8_1 - 1.0)*(y1_8_1 - 1.0) + 0.5*(z1_8_1 - 1.0)*(z1_8_1 - 1.0) + \
    0.5*(x2_8_1 - 1.0)*(x2_8_1 - 1.0) + 0.5*(y2_8_1 - 1.0)*(y2_8_1 - 1.0) + \
    0.5*(z2_8_1 - 1.0)*(z2_8_1 - 1.0) + 0.5*(x3_8_1 - 1.0)*(x3_8_1 - 1.0) + \
    0.5*(y3_8_1 - 1.0)*(y3_8_1 - 1.0) + 0.5*(z3_8_1 - 1.0)*(z3_8_1 - 1.0) + \
    0.5*(x4_8_1 - 1.0)*(x4_8_1 - 1.0) + 0.5*(y4_8_1 - 1.0)*(y4_8_1 - 1.0) + \
    0.5*(z4_8_1 - 1.0)*(z4_8_1 - 1.0) + 0.5*(x5_8_1 - 1.0)*(x5_8_1 - 1.0) + \
    0.5*(y5_8_1 - 1.0)*(y5_8_1 - 1.0) + 0.5*(z5_8_1 - 1.0)*(z5_8_1 - 1.0) + \
    0.5*(x6_8_1 - 1.0)*(x6_8_1 - 1.0) + 0.5*(y6_8_1 - 1.0)*(y6_8_1 - 1.0) + \
    0.5*(z6_8_1 - 1.0)*(z6_8_1 - 1.0) + 0.5*(x7_8_1 - 1.0)*(x7_8_1 - 1.0) + \
    0.5*(y7_8_1 - 1.0)*(y7_8_1 - 1.0) + 0.5*(z7_8_1 - 1.0)*(z7_8_1 - 1.0) + \
    0.5*(x8_8_1 - 1.0)*(x8_8_1 - 1.0) + 0.5*(y8_8_1 - 1.0)*(y8_8_1 - 1.0) + \
    0.5*(z8_8_1 - 1.0)*(z8_8_1 - 1.0) + 0.5*(x9_8_1 - 1.0)*(x9_8_1 - 1.0) + \
    0.5*(y9_8_1 - 1.0)*(y9_8_1 - 1.0) + 0.5*(z9_8_1 - 1.0)*(z9_8_1 - 1.0) + \
    0.5*(x1_9_1 - 1.0)*(x1_9_1 - 1.0) + 0.5*(y1_9_1 - 1.0)*(y1_9_1 - 1.0) + \
    0.5*(z1_9_1 - 1.0)*(z1_9_1 - 1.0) + 0.5*(x2_9_1 - 1.0)*(x2_9_1 - 1.0) + \
    0.5*(y2_9_1 - 1.0)*(y2_9_1 - 1.0) + 0.5*(z2_9_1 - 1.0)*(z2_9_1 - 1.0) + \
    0.5*(x3_9_1 - 1.0)*(x3_9_1 - 1.0) + 0.5*(y3_9_1 - 1.0)*(y3_9_1 - 1.0) + \
    0.5*(z3_9_1 - 1.0)*(z3_9_1 - 1.0) + 0.5*(x4_9_1 - 1.0)*(x4_9_1 - 1.0) + \
    0.5*(y4_9_1 - 1.0)*(y4_9_1 - 1.0) + 0.5*(z4_9_1 - 1.0)*(z4_9_1 - 1.0) + \
    0.5*(x5_9_1 - 1.0)*(x5_9_1 - 1.0) + 0.5*(y5_9_1 - 1.0)*(y5_9_1 - 1.0) + \
    0.5*(z5_9_1 - 1.0)*(z5_9_1 - 1.0) + 0.5*(x6_9_1 - 1.0)*(x6_9_1 - 1.0) + \
    0.5*(y6_9_1 - 1.0)*(y6_9_1 - 1.0) + 0.5*(z6_9_1 - 1.0)*(z6_9_1 - 1.0) + \
    0.5*(x7_9_1 - 1.0)*(x7_9_1 - 1.0) + 0.5*(y7_9_1 - 1.0)*(y7_9_1 - 1.0) + \
    0.5*(z7_9_1 - 1.0)*(z7_9_1 - 1.0) + 0.5*(x8_9_1 - 1.0)*(x8_9_1 - 1.0) + \
    0.5*(y8_9_1 - 1.0)*(y8_9_1 - 1.0) + 0.5*(z8_9_1 - 1.0)*(z8_9_1 - 1.0) + \
    0.5*(x9_9_1 - 1.0)*(x9_9_1 - 1.0) + 0.5*(y9_9_1 - 1.0)*(y9_9_1 - 1.0) + \
    0.5*(z9_9_1 - 1.0)*(z9_9_1 - 1.0) + 0.5*(x1_1_2 - 1.0)*(x1_1_2 - 1.0) + \
    0.5*(y1_1_2 - 1.0)*(y1_1_2 - 1.0) + 0.5*(z1_1_2 - 1.0)*(z1_1_2 - 1.0) + \
    0.5*(x2_1_2 - 1.0)*(x2_1_2 - 1.0) + 0.5*(y2_1_2 - 1.0)*(y2_1_2 - 1.0) + \
    0.5*(z2_1_2 - 1.0)*(z2_1_2 - 1.0) + 0.5*(x3_1_2 - 1.0)*(x3_1_2 - 1.0) + \
    0.5*(y3_1_2 - 1.0)*(y3_1_2 - 1.0) + 0.5*(z3_1_2 - 1.0)*(z3_1_2 - 1.0) + \
    0.5*(x4_1_2 - 1.0)*(x4_1_2 - 1.0) + 0.5*(y4_1_2 - 1.0)*(y4_1_2 - 1.0) + \
    0.5*(z4_1_2 - 1.0)*(z4_1_2 - 1.0) + 0.5*(x5_1_2 - 1.0)*(x5_1_2 - 1.0) + \
    0.5*(y5_1_2 - 1.0)*(y5_1_2 - 1.0) + 0.5*(z5_1_2 - 1.0)*(z5_1_2 - 1.0) + \
    0.5*(x6_1_2 - 1.0)*(x6_1_2 - 1.0) + 0.5*(y6_1_2 - 1.0)*(y6_1_2 - 1.0) + \
    0.5*(z6_1_2 - 1.0)*(z6_1_2 - 1.0) + 0.5*(x7_1_2 - 1.0)*(x7_1_2 - 1.0) + \
    0.5*(y7_1_2 - 1.0)*(y7_1_2 - 1.0) + 0.5*(z7_1_2 - 1.0)*(z7_1_2 - 1.0) + \
    0.5*(x8_1_2 - 1.0)*(x8_1_2 - 1.0) + 0.5*(y8_1_2 - 1.0)*(y8_1_2 - 1.0) + \
    0.5*(z8_1_2 - 1.0)*(z8_1_2 - 1.0) + 0.5*(x9_1_2 - 1.0)*(x9_1_2 - 1.0) + \
    0.5*(y9_1_2 - 1.0)*(y9_1_2 - 1.0) + 0.5*(z9_1_2 - 1.0)*(z9_1_2 - 1.0) + \
    0.5*(x1_2_2 - 1.0)*(x1_2_2 - 1.0) + 0.5*(y1_2_2 - 1.0)*(y1_2_2 - 1.0) + \
    0.5*(z1_2_2 - 1.0)*(z1_2_2 - 1.0) + 0.5*(x2_2_2 - 1.0)*(x2_2_2 - 1.0) + \
    0.5*(y2_2_2 - 1.0)*(y2_2_2 - 1.0) + 0.5*(z2_2_2 - 1.0)*(z2_2_2 - 1.0) + \
    0.5*(x3_2_2 - 1.0)*(x3_2_2 - 1.0) + 0.5*(y3_2_2 - 1.0)*(y3_2_2 - 1.0) + \
    0.5*(z3_2_2 - 1.0)*(z3_2_2 - 1.0) + 0.5*(x4_2_2 - 1.0)*(x4_2_2 - 1.0) + \
    0.5*(y4_2_2 - 1.0)*(y4_2_2 - 1.0) + 0.5*(z4_2_2 - 1.0)*(z4_2_2 - 1.0) + \
    0.5*(x5_2_2 - 1.0)*(x5_2_2 - 1.0) + 0.5*(y5_2_2 - 1.0)*(y5_2_2 - 1.0) + \
    0.5*(z5_2_2 - 1.0)*(z5_2_2 - 1.0) + 0.5*(x6_2_2 - 1.0)*(x6_2_2 - 1.0) + \
    0.5*(y6_2_2 - 1.0)*(y6_2_2 - 1.0) + 0.5*(z6_2_2 - 1.0)*(z6_2_2 - 1.0) + \
    0.5*(x7_2_2 - 1.0)*(x7_2_2 - 1.0) + 0.5*(y7_2_2 - 1.0)*(y7_2_2 - 1.0) + \
    0.5*(z7_2_2 - 1.0)*(z7_2_2 - 1.0) + 0.5*(x8_2_2 - 1.0)*(x8_2_2 - 1.0) + \
    0.5*(y8_2_2 - 1.0)*(y8_2_2 - 1.0) + 0.5*(z8_2_2 - 1.0)*(z8_2_2 - 1.0) + \
    0.5*(x9_2_2 - 1.0)*(x9_2_2 - 1.0) + 0.5*(y9_2_2 - 1.0)*(y9_2_2 - 1.0) + \
    0.5*(z9_2_2 - 1.0)*(z9_2_2 - 1.0) + 0.5*(x1_3_2 - 1.0)*(x1_3_2 - 1.0) + \
    0.5*(y1_3_2 - 1.0)*(y1_3_2 - 1.0) + 0.5*(z1_3_2 - 1.0)*(z1_3_2 - 1.0) + \
    0.5*(x2_3_2 - 1.0)*(x2_3_2 - 1.0) + 0.5*(y2_3_2 - 1.0)*(y2_3_2 - 1.0) + \
    0.5*(z2_3_2 - 1.0)*(z2_3_2 - 1.0) + 0.5*(x3_3_2 - 1.0)*(x3_3_2 - 1.0) + \
    0.5*(y3_3_2 - 1.0)*(y3_3_2 - 1.0) + 0.5*(z3_3_2 - 1.0)*(z3_3_2 - 1.0) + \
    0.5*(x4_3_2 - 1.0)*(x4_3_2 - 1.0) + 0.5*(y4_3_2 - 1.0)*(y4_3_2 - 1.0) + \
    0.5*(z4_3_2 - 1.0)*(z4_3_2 - 1.0) + 0.5*(x5_3_2 - 1.0)*(x5_3_2 - 1.0) + \
    0.5*(y5_3_2 - 1.0)*(y5_3_2 - 1.0) + 0.5*(z5_3_2 - 1.0)*(z5_3_2 - 1.0) + \
    0.5*(x6_3_2 - 1.0)*(x6_3_2 - 1.0) + 0.5*(y6_3_2 - 1.0)*(y6_3_2 - 1.0) + \
    0.5*(z6_3_2 - 1.0)*(z6_3_2 - 1.0) + 0.5*(x7_3_2 - 1.0)*(x7_3_2 - 1.0) + \
    0.5*(y7_3_2 - 1.0)*(y7_3_2 - 1.0) + 0.5*(z7_3_2 - 1.0)*(z7_3_2 - 1.0) + \
    0.5*(x8_3_2 - 1.0)*(x8_3_2 - 1.0) + 0.5*(y8_3_2 - 1.0)*(y8_3_2 - 1.0) + \
    0.5*(z8_3_2 - 1.0)*(z8_3_2 - 1.0) + 0.5*(x9_3_2 - 1.0)*(x9_3_2 - 1.0) + \
    0.5*(y9_3_2 - 1.0)*(y9_3_2 - 1.0) + 0.5*(z9_3_2 - 1.0)*(z9_3_2 - 1.0) + \
    0.5*(x1_4_2 - 1.0)*(x1_4_2 - 1.0) + 0.5*(y1_4_2 - 1.0)*(y1_4_2 - 1.0) + \
    0.5*(z1_4_2 - 1.0)*(z1_4_2 - 1.0) + 0.5*(x2_4_2 - 1.0)*(x2_4_2 - 1.0) + \
    0.5*(y2_4_2 - 1.0)*(y2_4_2 - 1.0) + 0.5*(z2_4_2 - 1.0)*(z2_4_2 - 1.0) + \
    0.5*(x3_4_2 - 1.0)*(x3_4_2 - 1.0) + 0.5*(y3_4_2 - 1.0)*(y3_4_2 - 1.0) + \
    0.5*(z3_4_2 - 1.0)*(z3_4_2 - 1.0) + 0.5*(x4_4_2 - 1.0)*(x4_4_2 - 1.0) + \
    0.5*(y4_4_2 - 1.0)*(y4_4_2 - 1.0) + 0.5*(z4_4_2 - 1.0)*(z4_4_2 - 1.0) + \
    0.5*(x5_4_2 - 1.0)*(x5_4_2 - 1.0) + 0.5*(y5_4_2 - 1.0)*(y5_4_2 - 1.0) + \
    0.5*(z5_4_2 - 1.0)*(z5_4_2 - 1.0) + 0.5*(x6_4_2 - 1.0)*(x6_4_2 - 1.0) + \
    0.5*(y6_4_2 - 1.0)*(y6_4_2 - 1.0) + 0.5*(z6_4_2 - 1.0)*(z6_4_2 - 1.0) + \
    0.5*(x7_4_2 - 1.0)*(x7_4_2 - 1.0) + 0.5*(y7_4_2 - 1.0)*(y7_4_2 - 1.0) + \
    0.5*(z7_4_2 - 1.0)*(z7_4_2 - 1.0) + 0.5*(x8_4_2 - 1.0)*(x8_4_2 - 1.0) + \
    0.5*(y8_4_2 - 1.0)*(y8_4_2 - 1.0) + 0.5*(z8_4_2 - 1.0)*(z8_4_2 - 1.0) + \
    0.5*(x9_4_2 - 1.0)*(x9_4_2 - 1.0) + 0.5*(y9_4_2 - 1.0)*(y9_4_2 - 1.0) + \
    0.5*(z9_4_2 - 1.0)*(z9_4_2 - 1.0) + 0.5*(x1_5_2 - 1.0)*(x1_5_2 - 1.0) + \
    0.5*(y1_5_2 - 1.0)*(y1_5_2 - 1.0) + 0.5*(z1_5_2 - 1.0)*(z1_5_2 - 1.0) + \
    0.5*(x2_5_2 - 1.0)*(x2_5_2 - 1.0) + 0.5*(y2_5_2 - 1.0)*(y2_5_2 - 1.0) + \
    0.5*(z2_5_2 - 1.0)*(z2_5_2 - 1.0) + 0.5*(x3_5_2 - 1.0)*(x3_5_2 - 1.0) + \
    0.5*(y3_5_2 - 1.0)*(y3_5_2 - 1.0) + 0.5*(z3_5_2 - 1.0)*(z3_5_2 - 1.0) + \
    0.5*(x4_5_2 - 1.0)*(x4_5_2 - 1.0) + 0.5*(y4_5_2 - 1.0)*(y4_5_2 - 1.0) + \
    0.5*(z4_5_2 - 1.0)*(z4_5_2 - 1.0) + 0.5*(x5_5_2 - 1.0)*(x5_5_2 - 1.0) + \
    0.5*(y5_5_2 - 1.0)*(y5_5_2 - 1.0) + 0.5*(z5_5_2 - 1.0)*(z5_5_2 - 1.0) + \
    0.5*(x6_5_2 - 1.0)*(x6_5_2 - 1.0) + 0.5*(y6_5_2 - 1.0)*(y6_5_2 - 1.0) + \
    0.5*(z6_5_2 - 1.0)*(z6_5_2 - 1.0) + 0.5*(x7_5_2 - 1.0)*(x7_5_2 - 1.0) + \
    0.5*(y7_5_2 - 1.0)*(y7_5_2 - 1.0) + 0.5*(z7_5_2 - 1.0)*(z7_5_2 - 1.0) + \
    0.5*(x8_5_2 - 1.0)*(x8_5_2 - 1.0) + 0.5*(y8_5_2 - 1.0)*(y8_5_2 - 1.0) + \
    0.5*(z8_5_2 - 1.0)*(z8_5_2 - 1.0) + 0.5*(x9_5_2 - 1.0)*(x9_5_2 - 1.0) + \
    0.5*(y9_5_2 - 1.0)*(y9_5_2 - 1.0) + 0.5*(z9_5_2 - 1.0)*(z9_5_2 - 1.0) + \
    0.5*(x1_6_2 - 1.0)*(x1_6_2 - 1.0) + 0.5*(y1_6_2 - 1.0)*(y1_6_2 - 1.0) + \
    0.5*(z1_6_2 - 1.0)*(z1_6_2 - 1.0) + 0.5*(x2_6_2 - 1.0)*(x2_6_2 - 1.0) + \
    0.5*(y2_6_2 - 1.0)*(y2_6_2 - 1.0) + 0.5*(z2_6_2 - 1.0)*(z2_6_2 - 1.0) + \
    0.5*(x3_6_2 - 1.0)*(x3_6_2 - 1.0) + 0.5*(y3_6_2 - 1.0)*(y3_6_2 - 1.0) + \
    0.5*(z3_6_2 - 1.0)*(z3_6_2 - 1.0) + 0.5*(x4_6_2 - 1.0)*(x4_6_2 - 1.0) + \
    0.5*(y4_6_2 - 1.0)*(y4_6_2 - 1.0) + 0.5*(z4_6_2 - 1.0)*(z4_6_2 - 1.0) + \
    0.5*(x5_6_2 - 1.0)*(x5_6_2 - 1.0) + 0.5*(y5_6_2 - 1.0)*(y5_6_2 - 1.0) + \
    0.5*(z5_6_2 - 1.0)*(z5_6_2 - 1.0) + 0.5*(x6_6_2 - 1.0)*(x6_6_2 - 1.0) + \
    0.5*(y6_6_2 - 1.0)*(y6_6_2 - 1.0) + 0.5*(z6_6_2 - 1.0)*(z6_6_2 - 1.0) + \
    0.5*(x7_6_2 - 1.0)*(x7_6_2 - 1.0) + 0.5*(y7_6_2 - 1.0)*(y7_6_2 - 1.0) + \
    0.5*(z7_6_2 - 1.0)*(z7_6_2 - 1.0) + 0.5*(x8_6_2 - 1.0)*(x8_6_2 - 1.0) + \
    0.5*(y8_6_2 - 1.0)*(y8_6_2 - 1.0) + 0.5*(z8_6_2 - 1.0)*(z8_6_2 - 1.0) + \
    0.5*(x9_6_2 - 1.0)*(x9_6_2 - 1.0) + 0.5*(y9_6_2 - 1.0)*(y9_6_2 - 1.0) + \
    0.5*(z9_6_2 - 1.0)*(z9_6_2 - 1.0) + 0.5*(x1_7_2 - 1.0)*(x1_7_2 - 1.0) + \
    0.5*(y1_7_2 - 1.0)*(y1_7_2 - 1.0) + 0.5*(z1_7_2 - 1.0)*(z1_7_2 - 1.0) + \
    0.5*(x2_7_2 - 1.0)*(x2_7_2 - 1.0) + 0.5*(y2_7_2 - 1.0)*(y2_7_2 - 1.0) + \
    0.5*(z2_7_2 - 1.0)*(z2_7_2 - 1.0) + 0.5*(x3_7_2 - 1.0)*(x3_7_2 - 1.0) + \
    0.5*(y3_7_2 - 1.0)*(y3_7_2 - 1.0) + 0.5*(z3_7_2 - 1.0)*(z3_7_2 - 1.0) + \
    0.5*(x4_7_2 - 1.0)*(x4_7_2 - 1.0) + 0.5*(y4_7_2 - 1.0)*(y4_7_2 - 1.0) + \
    0.5*(z4_7_2 - 1.0)*(z4_7_2 - 1.0) + 0.5*(x5_7_2 - 1.0)*(x5_7_2 - 1.0) + \
    0.5*(y5_7_2 - 1.0)*(y5_7_2 - 1.0) + 0.5*(z5_7_2 - 1.0)*(z5_7_2 - 1.0) + \
    0.5*(x6_7_2 - 1.0)*(x6_7_2 - 1.0) + 0.5*(y6_7_2 - 1.0)*(y6_7_2 - 1.0) + \
    0.5*(z6_7_2 - 1.0)*(z6_7_2 - 1.0) + 0.5*(x7_7_2 - 1.0)*(x7_7_2 - 1.0) + \
    0.5*(y7_7_2 - 1.0)*(y7_7_2 - 1.0) + 0.5*(z7_7_2 - 1.0)*(z7_7_2 - 1.0) + \
    0.5*(x8_7_2 - 1.0)*(x8_7_2 - 1.0) + 0.5*(y8_7_2 - 1.0)*(y8_7_2 - 1.0) + \
    0.5*(z8_7_2 - 1.0)*(z8_7_2 - 1.0) + 0.5*(x9_7_2 - 1.0)*(x9_7_2 - 1.0) + \
    0.5*(y9_7_2 - 1.0)*(y9_7_2 - 1.0) + 0.5*(z9_7_2 - 1.0)*(z9_7_2 - 1.0) + \
    0.5*(x1_8_2 - 1.0)*(x1_8_2 - 1.0) + 0.5*(y1_8_2 - 1.0)*(y1_8_2 - 1.0) + \
    0.5*(z1_8_2 - 1.0)*(z1_8_2 - 1.0) + 0.5*(x2_8_2 - 1.0)*(x2_8_2 - 1.0) + \
    0.5*(y2_8_2 - 1.0)*(y2_8_2 - 1.0) + 0.5*(z2_8_2 - 1.0)*(z2_8_2 - 1.0) + \
    0.5*(x3_8_2 - 1.0)*(x3_8_2 - 1.0) + 0.5*(y3_8_2 - 1.0)*(y3_8_2 - 1.0) + \
    0.5*(z3_8_2 - 1.0)*(z3_8_2 - 1.0) + 0.5*(x4_8_2 - 1.0)*(x4_8_2 - 1.0) + \
    0.5*(y4_8_2 - 1.0)*(y4_8_2 - 1.0) + 0.5*(z4_8_2 - 1.0)*(z4_8_2 - 1.0) + \
    0.5*(x5_8_2 - 1.0)*(x5_8_2 - 1.0) + 0.5*(y5_8_2 - 1.0)*(y5_8_2 - 1.0) + \
    0.5*(z5_8_2 - 1.0)*(z5_8_2 - 1.0) + 0.5*(x6_8_2 - 1.0)*(x6_8_2 - 1.0) + \
    0.5*(y6_8_2 - 1.0)*(y6_8_2 - 1.0) + 0.5*(z6_8_2 - 1.0)*(z6_8_2 - 1.0) + \
    0.5*(x7_8_2 - 1.0)*(x7_8_2 - 1.0) + 0.5*(y7_8_2 - 1.0)*(y7_8_2 - 1.0) + \
    0.5*(z7_8_2 - 1.0)*(z7_8_2 - 1.0) + 0.5*(x8_8_2 - 1.0)*(x8_8_2 - 1.0) + \
    0.5*(y8_8_2 - 1.0)*(y8_8_2 - 1.0) + 0.5*(z8_8_2 - 1.0)*(z8_8_2 - 1.0) + \
    0.5*(x9_8_2 - 1.0)*(x9_8_2 - 1.0) + 0.5*(y9_8_2 - 1.0)*(y9_8_2 - 1.0) + \
    0.5*(z9_8_2 - 1.0)*(z9_8_2 - 1.0) + 0.5*(x1_9_2 - 1.0)*(x1_9_2 - 1.0) + \
    0.5*(y1_9_2 - 1.0)*(y1_9_2 - 1.0) + 0.5*(z1_9_2 - 1.0)*(z1_9_2 - 1.0) + \
    0.5*(x2_9_2 - 1.0)*(x2_9_2 - 1.0) + 0.5*(y2_9_2 - 1.0)*(y2_9_2 - 1.0) + \
    0.5*(z2_9_2 - 1.0)*(z2_9_2 - 1.0) + 0.5*(x3_9_2 - 1.0)*(x3_9_2 - 1.0) + \
    0.5*(y3_9_2 - 1.0)*(y3_9_2 - 1.0) + 0.5*(z3_9_2 - 1.0)*(z3_9_2 - 1.0) + \
    0.5*(x4_9_2 - 1.0)*(x4_9_2 - 1.0) + 0.5*(y4_9_2 - 1.0)*(y4_9_2 - 1.0) + \
    0.5*(z4_9_2 - 1.0)*(z4_9_2 - 1.0) + 0.5*(x5_9_2 - 1.0)*(x5_9_2 - 1.0) + \
    0.5*(y5_9_2 - 1.0)*(y5_9_2 - 1.0) + 0.5*(z5_9_2 - 1.0)*(z5_9_2 - 1.0) + \
    0.5*(x6_9_2 - 1.0)*(x6_9_2 - 1.0) + 0.5*(y6_9_2 - 1.0)*(y6_9_2 - 1.0) + \
    0.5*(z6_9_2 - 1.0)*(z6_9_2 - 1.0) + 0.5*(x7_9_2 - 1.0)*(x7_9_2 - 1.0) + \
    0.5*(y7_9_2 - 1.0)*(y7_9_2 - 1.0) + 0.5*(z7_9_2 - 1.0)*(z7_9_2 - 1.0) + \
    0.5*(x8_9_2 - 1.0)*(x8_9_2 - 1.0) + 0.5*(y8_9_2 - 1.0)*(y8_9_2 - 1.0) + \
    0.5*(z8_9_2 - 1.0)*(z8_9_2 - 1.0) + 0.5*(x9_9_2 - 1.0)*(x9_9_2 - 1.0) + \
    0.5*(y9_9_2 - 1.0)*(y9_9_2 - 1.0) + 0.5*(z9_9_2 - 1.0)*(z9_9_2 - 1.0) + \
    0.5*(x1_1_3 - 1.0)*(x1_1_3 - 1.0) + 0.5*(y1_1_3 - 1.0)*(y1_1_3 - 1.0) + \
    0.5*(z1_1_3 - 1.0)*(z1_1_3 - 1.0) + 0.5*(x2_1_3 - 1.0)*(x2_1_3 - 1.0) + \
    0.5*(y2_1_3 - 1.0)*(y2_1_3 - 1.0) + 0.5*(z2_1_3 - 1.0)*(z2_1_3 - 1.0) + \
    0.5*(x3_1_3 - 1.0)*(x3_1_3 - 1.0) + 0.5*(y3_1_3 - 1.0)*(y3_1_3 - 1.0) + \
    0.5*(z3_1_3 - 1.0)*(z3_1_3 - 1.0) + 0.5*(x4_1_3 - 1.0)*(x4_1_3 - 1.0) + \
    0.5*(y4_1_3 - 1.0)*(y4_1_3 - 1.0) + 0.5*(z4_1_3 - 1.0)*(z4_1_3 - 1.0) + \
    0.5*(x5_1_3 - 1.0)*(x5_1_3 - 1.0) + 0.5*(y5_1_3 - 1.0)*(y5_1_3 - 1.0) + \
    0.5*(z5_1_3 - 1.0)*(z5_1_3 - 1.0) + 0.5*(x6_1_3 - 1.0)*(x6_1_3 - 1.0) + \
    0.5*(y6_1_3 - 1.0)*(y6_1_3 - 1.0) + 0.5*(z6_1_3 - 1.0)*(z6_1_3 - 1.0) + \
    0.5*(x7_1_3 - 1.0)*(x7_1_3 - 1.0) + 0.5*(y7_1_3 - 1.0)*(y7_1_3 - 1.0) + \
    0.5*(z7_1_3 - 1.0)*(z7_1_3 - 1.0) + 0.5*(x8_1_3 - 1.0)*(x8_1_3 - 1.0) + \
    0.5*(y8_1_3 - 1.0)*(y8_1_3 - 1.0) + 0.5*(z8_1_3 - 1.0)*(z8_1_3 - 1.0) + \
    0.5*(x9_1_3 - 1.0)*(x9_1_3 - 1.0) + 0.5*(y9_1_3 - 1.0)*(y9_1_3 - 1.0) + \
    0.5*(z9_1_3 - 1.0)*(z9_1_3 - 1.0) + 0.5*(x1_2_3 - 1.0)*(x1_2_3 - 1.0) + \
    0.5*(y1_2_3 - 1.0)*(y1_2_3 - 1.0) + 0.5*(z1_2_3 - 1.0)*(z1_2_3 - 1.0) + \
    0.5*(x2_2_3 - 1.0)*(x2_2_3 - 1.0) + 0.5*(y2_2_3 - 1.0)*(y2_2_3 - 1.0) + \
    0.5*(z2_2_3 - 1.0)*(z2_2_3 - 1.0) + 0.5*(x3_2_3 - 1.0)*(x3_2_3 - 1.0) + \
    0.5*(y3_2_3 - 1.0)*(y3_2_3 - 1.0) + 0.5*(z3_2_3 - 1.0)*(z3_2_3 - 1.0) + \
    0.5*(x4_2_3 - 1.0)*(x4_2_3 - 1.0) + 0.5*(y4_2_3 - 1.0)*(y4_2_3 - 1.0) + \
    0.5*(z4_2_3 - 1.0)*(z4_2_3 - 1.0) + 0.5*(x5_2_3 - 1.0)*(x5_2_3 - 1.0) + \
    0.5*(y5_2_3 - 1.0)*(y5_2_3 - 1.0) + 0.5*(z5_2_3 - 1.0)*(z5_2_3 - 1.0) + \
    0.5*(x6_2_3 - 1.0)*(x6_2_3 - 1.0) + 0.5*(y6_2_3 - 1.0)*(y6_2_3 - 1.0) + \
    0.5*(z6_2_3 - 1.0)*(z6_2_3 - 1.0) + 0.5*(x7_2_3 - 1.0)*(x7_2_3 - 1.0) + \
    0.5*(y7_2_3 - 1.0)*(y7_2_3 - 1.0) + 0.5*(z7_2_3 - 1.0)*(z7_2_3 - 1.0) + \
    0.5*(x8_2_3 - 1.0)*(x8_2_3 - 1.0) + 0.5*(y8_2_3 - 1.0)*(y8_2_3 - 1.0) + \
    0.5*(z8_2_3 - 1.0)*(z8_2_3 - 1.0) + 0.5*(x9_2_3 - 1.0)*(x9_2_3 - 1.0) + \
    0.5*(y9_2_3 - 1.0)*(y9_2_3 - 1.0) + 0.5*(z9_2_3 - 1.0)*(z9_2_3 - 1.0) + \
    0.5*(x1_3_3 - 1.0)*(x1_3_3 - 1.0) + 0.5*(y1_3_3 - 1.0)*(y1_3_3 - 1.0) + \
    0.5*(z1_3_3 - 1.0)*(z1_3_3 - 1.0) + 0.5*(x2_3_3 - 1.0)*(x2_3_3 - 1.0) + \
    0.5*(y2_3_3 - 1.0)*(y2_3_3 - 1.0) + 0.5*(z2_3_3 - 1.0)*(z2_3_3 - 1.0) + \
    0.5*(x3_3_3 - 1.0)*(x3_3_3 - 1.0) + 0.5*(y3_3_3 - 1.0)*(y3_3_3 - 1.0) + \
    0.5*(z3_3_3 - 1.0)*(z3_3_3 - 1.0) + 0.5*(x4_3_3 - 1.0)*(x4_3_3 - 1.0) + \
    0.5*(y4_3_3 - 1.0)*(y4_3_3 - 1.0) + 0.5*(z4_3_3 - 1.0)*(z4_3_3 - 1.0) + \
    0.5*(x5_3_3 - 1.0)*(x5_3_3 - 1.0) + 0.5*(y5_3_3 - 1.0)*(y5_3_3 - 1.0) + \
    0.5*(z5_3_3 - 1.0)*(z5_3_3 - 1.0) + 0.5*(x6_3_3 - 1.0)*(x6_3_3 - 1.0) + \
    0.5*(y6_3_3 - 1.0)*(y6_3_3 - 1.0) + 0.5*(z6_3_3 - 1.0)*(z6_3_3 - 1.0) + \
    0.5*(x7_3_3 - 1.0)*(x7_3_3 - 1.0) + 0.5*(y7_3_3 - 1.0)*(y7_3_3 - 1.0) + \
    0.5*(z7_3_3 - 1.0)*(z7_3_3 - 1.0) + 0.5*(x8_3_3 - 1.0)*(x8_3_3 - 1.0) + \
    0.5*(y8_3_3 - 1.0)*(y8_3_3 - 1.0) + 0.5*(z8_3_3 - 1.0)*(z8_3_3 - 1.0) + \
    0.5*(x9_3_3 - 1.0)*(x9_3_3 - 1.0) + 0.5*(y9_3_3 - 1.0)*(y9_3_3 - 1.0) + \
    0.5*(z9_3_3 - 1.0)*(z9_3_3 - 1.0) + 0.5*(x1_4_3 - 1.0)*(x1_4_3 - 1.0) + \
    0.5*(y1_4_3 - 1.0)*(y1_4_3 - 1.0) + 0.5*(z1_4_3 - 1.0)*(z1_4_3 - 1.0) + \
    0.5*(x2_4_3 - 1.0)*(x2_4_3 - 1.0) + 0.5*(y2_4_3 - 1.0)*(y2_4_3 - 1.0) + \
    0.5*(z2_4_3 - 1.0)*(z2_4_3 - 1.0) + 0.5*(x3_4_3 - 1.0)*(x3_4_3 - 1.0) + \
    0.5*(y3_4_3 - 1.0)*(y3_4_3 - 1.0) + 0.5*(z3_4_3 - 1.0)*(z3_4_3 - 1.0) + \
    0.5*(x4_4_3 - 1.0)*(x4_4_3 - 1.0) + 0.5*(y4_4_3 - 1.0)*(y4_4_3 - 1.0) + \
    0.5*(z4_4_3 - 1.0)*(z4_4_3 - 1.0) + 0.5*(x5_4_3 - 1.0)*(x5_4_3 - 1.0) + \
    0.5*(y5_4_3 - 1.0)*(y5_4_3 - 1.0) + 0.5*(z5_4_3 - 1.0)*(z5_4_3 - 1.0) + \
    0.5*(x6_4_3 - 1.0)*(x6_4_3 - 1.0) + 0.5*(y6_4_3 - 1.0)*(y6_4_3 - 1.0) + \
    0.5*(z6_4_3 - 1.0)*(z6_4_3 - 1.0) + 0.5*(x7_4_3 - 1.0)*(x7_4_3 - 1.0) + \
    0.5*(y7_4_3 - 1.0)*(y7_4_3 - 1.0) + 0.5*(z7_4_3 - 1.0)*(z7_4_3 - 1.0) + \
    0.5*(x8_4_3 - 1.0)*(x8_4_3 - 1.0) + 0.5*(y8_4_3 - 1.0)*(y8_4_3 - 1.0) + \
    0.5*(z8_4_3 - 1.0)*(z8_4_3 - 1.0) + 0.5*(x9_4_3 - 1.0)*(x9_4_3 - 1.0) + \
    0.5*(y9_4_3 - 1.0)*(y9_4_3 - 1.0) + 0.5*(z9_4_3 - 1.0)*(z9_4_3 - 1.0) + \
    0.5*(x1_5_3 - 1.0)*(x1_5_3 - 1.0) + 0.5*(y1_5_3 - 1.0)*(y1_5_3 - 1.0) + \
    0.5*(z1_5_3 - 1.0)*(z1_5_3 - 1.0) + 0.5*(x2_5_3 - 1.0)*(x2_5_3 - 1.0) + \
    0.5*(y2_5_3 - 1.0)*(y2_5_3 - 1.0) + 0.5*(z2_5_3 - 1.0)*(z2_5_3 - 1.0) + \
    0.5*(x3_5_3 - 1.0)*(x3_5_3 - 1.0) + 0.5*(y3_5_3 - 1.0)*(y3_5_3 - 1.0) + \
    0.5*(z3_5_3 - 1.0)*(z3_5_3 - 1.0) + 0.5*(x4_5_3 - 1.0)*(x4_5_3 - 1.0) + \
    0.5*(y4_5_3 - 1.0)*(y4_5_3 - 1.0) + 0.5*(z4_5_3 - 1.0)*(z4_5_3 - 1.0) + \
    0.5*(x5_5_3 - 1.0)*(x5_5_3 - 1.0) + 0.5*(y5_5_3 - 1.0)*(y5_5_3 - 1.0) + \
    0.5*(z5_5_3 - 1.0)*(z5_5_3 - 1.0) + 0.5*(x6_5_3 - 1.0)*(x6_5_3 - 1.0) + \
    0.5*(y6_5_3 - 1.0)*(y6_5_3 - 1.0) + 0.5*(z6_5_3 - 1.0)*(z6_5_3 - 1.0) + \
    0.5*(x7_5_3 - 1.0)*(x7_5_3 - 1.0) + 0.5*(y7_5_3 - 1.0)*(y7_5_3 - 1.0) + \
    0.5*(z7_5_3 - 1.0)*(z7_5_3 - 1.0) + 0.5*(x8_5_3 - 1.0)*(x8_5_3 - 1.0) + \
    0.5*(y8_5_3 - 1.0)*(y8_5_3 - 1.0) + 0.5*(z8_5_3 - 1.0)*(z8_5_3 - 1.0) + \
    0.5*(x9_5_3 - 1.0)*(x9_5_3 - 1.0) + 0.5*(y9_5_3 - 1.0)*(y9_5_3 - 1.0) + \
    0.5*(z9_5_3 - 1.0)*(z9_5_3 - 1.0) + 0.5*(x1_6_3 - 1.0)*(x1_6_3 - 1.0) + \
    0.5*(y1_6_3 - 1.0)*(y1_6_3 - 1.0) + 0.5*(z1_6_3 - 1.0)*(z1_6_3 - 1.0) + \
    0.5*(x2_6_3 - 1.0)*(x2_6_3 - 1.0) + 0.5*(y2_6_3 - 1.0)*(y2_6_3 - 1.0) + \
    0.5*(z2_6_3 - 1.0)*(z2_6_3 - 1.0) + 0.5*(x3_6_3 - 1.0)*(x3_6_3 - 1.0) + \
    0.5*(y3_6_3 - 1.0)*(y3_6_3 - 1.0) + 0.5*(z3_6_3 - 1.0)*(z3_6_3 - 1.0) + \
    0.5*(x4_6_3 - 1.0)*(x4_6_3 - 1.0) + 0.5*(y4_6_3 - 1.0)*(y4_6_3 - 1.0) + \
    0.5*(z4_6_3 - 1.0)*(z4_6_3 - 1.0) + 0.5*(x5_6_3 - 1.0)*(x5_6_3 - 1.0) + \
    0.5*(y5_6_3 - 1.0)*(y5_6_3 - 1.0) + 0.5*(z5_6_3 - 1.0)*(z5_6_3 - 1.0) + \
    0.5*(x6_6_3 - 1.0)*(x6_6_3 - 1.0) + 0.5*(y6_6_3 - 1.0)*(y6_6_3 - 1.0) + \
    0.5*(z6_6_3 - 1.0)*(z6_6_3 - 1.0) + 0.5*(x7_6_3 - 1.0)*(x7_6_3 - 1.0) + \
    0.5*(y7_6_3 - 1.0)*(y7_6_3 - 1.0) + 0.5*(z7_6_3 - 1.0)*(z7_6_3 - 1.0) + \
    0.5*(x8_6_3 - 1.0)*(x8_6_3 - 1.0) + 0.5*(y8_6_3 - 1.0)*(y8_6_3 - 1.0) + \
    0.5*(z8_6_3 - 1.0)*(z8_6_3 - 1.0) + 0.5*(x9_6_3 - 1.0)*(x9_6_3 - 1.0) + \
    0.5*(y9_6_3 - 1.0)*(y9_6_3 - 1.0) + 0.5*(z9_6_3 - 1.0)*(z9_6_3 - 1.0) + \
    0.5*(x1_7_3 - 1.0)*(x1_7_3 - 1.0) + 0.5*(y1_7_3 - 1.0)*(y1_7_3 - 1.0) + \
    0.5*(z1_7_3 - 1.0)*(z1_7_3 - 1.0) + 0.5*(x2_7_3 - 1.0)*(x2_7_3 - 1.0) + \
    0.5*(y2_7_3 - 1.0)*(y2_7_3 - 1.0) + 0.5*(z2_7_3 - 1.0)*(z2_7_3 - 1.0) + \
    0.5*(x3_7_3 - 1.0)*(x3_7_3 - 1.0) + 0.5*(y3_7_3 - 1.0)*(y3_7_3 - 1.0) + \
    0.5*(z3_7_3 - 1.0)*(z3_7_3 - 1.0) + 0.5*(x4_7_3 - 1.0)*(x4_7_3 - 1.0) + \
    0.5*(y4_7_3 - 1.0)*(y4_7_3 - 1.0) + 0.5*(z4_7_3 - 1.0)*(z4_7_3 - 1.0) + \
    0.5*(x5_7_3 - 1.0)*(x5_7_3 - 1.0) + 0.5*(y5_7_3 - 1.0)*(y5_7_3 - 1.0) + \
    0.5*(z5_7_3 - 1.0)*(z5_7_3 - 1.0) + 0.5*(x6_7_3 - 1.0)*(x6_7_3 - 1.0) + \
    0.5*(y6_7_3 - 1.0)*(y6_7_3 - 1.0) + 0.5*(z6_7_3 - 1.0)*(z6_7_3 - 1.0) + \
    0.5*(x7_7_3 - 1.0)*(x7_7_3 - 1.0) + 0.5*(y7_7_3 - 1.0)*(y7_7_3 - 1.0) + \
    0.5*(z7_7_3 - 1.0)*(z7_7_3 - 1.0) + 0.5*(x8_7_3 - 1.0)*(x8_7_3 - 1.0) + \
    0.5*(y8_7_3 - 1.0)*(y8_7_3 - 1.0) + 0.5*(z8_7_3 - 1.0)*(z8_7_3 - 1.0) + \
    0.5*(x9_7_3 - 1.0)*(x9_7_3 - 1.0) + 0.5*(y9_7_3 - 1.0)*(y9_7_3 - 1.0) + \
    0.5*(z9_7_3 - 1.0)*(z9_7_3 - 1.0) + 0.5*(x1_8_3 - 1.0)*(x1_8_3 - 1.0) + \
    0.5*(y1_8_3 - 1.0)*(y1_8_3 - 1.0) + 0.5*(z1_8_3 - 1.0)*(z1_8_3 - 1.0) + \
    0.5*(x2_8_3 - 1.0)*(x2_8_3 - 1.0) + 0.5*(y2_8_3 - 1.0)*(y2_8_3 - 1.0) + \
    0.5*(z2_8_3 - 1.0)*(z2_8_3 - 1.0) + 0.5*(x3_8_3 - 1.0)*(x3_8_3 - 1.0) + \
    0.5*(y3_8_3 - 1.0)*(y3_8_3 - 1.0) + 0.5*(z3_8_3 - 1.0)*(z3_8_3 - 1.0) + \
    0.5*(x4_8_3 - 1.0)*(x4_8_3 - 1.0) + 0.5*(y4_8_3 - 1.0)*(y4_8_3 - 1.0) + \
    0.5*(z4_8_3 - 1.0)*(z4_8_3 - 1.0) + 0.5*(x5_8_3 - 1.0)*(x5_8_3 - 1.0) + \
    0.5*(y5_8_3 - 1.0)*(y5_8_3 - 1.0) + 0.5*(z5_8_3 - 1.0)*(z5_8_3 - 1.0) + \
    0.5*(x6_8_3 - 1.0)*(x6_8_3 - 1.0) + 0.5*(y6_8_3 - 1.0)*(y6_8_3 - 1.0) + \
    0.5*(z6_8_3 - 1.0)*(z6_8_3 - 1.0) + 0.5*(x7_8_3 - 1.0)*(x7_8_3 - 1.0) + \
    0.5*(y7_8_3 - 1.0)*(y7_8_3 - 1.0) + 0.5*(z7_8_3 - 1.0)*(z7_8_3 - 1.0) + \
    0.5*(x8_8_3 - 1.0)*(x8_8_3 - 1.0) + 0.5*(y8_8_3 - 1.0)*(y8_8_3 - 1.0) + \
    0.5*(z8_8_3 - 1.0)*(z8_8_3 - 1.0) + 0.5*(x9_8_3 - 1.0)*(x9_8_3 - 1.0) + \
    0.5*(y9_8_3 - 1.0)*(y9_8_3 - 1.0) + 0.5*(z9_8_3 - 1.0)*(z9_8_3 - 1.0) + \
    0.5*(x1_9_3 - 1.0)*(x1_9_3 - 1.0) + 0.5*(y1_9_3 - 1.0)*(y1_9_3 - 1.0) + \
    0.5*(z1_9_3 - 1.0)*(z1_9_3 - 1.0) + 0.5*(x2_9_3 - 1.0)*(x2_9_3 - 1.0) + \
    0.5*(y2_9_3 - 1.0)*(y2_9_3 - 1.0) + 0.5*(z2_9_3 - 1.0)*(z2_9_3 - 1.0) + \
    0.5*(x3_9_3 - 1.0)*(x3_9_3 - 1.0) + 0.5*(y3_9_3 - 1.0)*(y3_9_3 - 1.0) + \
    0.5*(z3_9_3 - 1.0)*(z3_9_3 - 1.0) + 0.5*(x4_9_3 - 1.0)*(x4_9_3 - 1.0) + \
    0.5*(y4_9_3 - 1.0)*(y4_9_3 - 1.0) + 0.5*(z4_9_3 - 1.0)*(z4_9_3 - 1.0) + \
    0.5*(x5_9_3 - 1.0)*(x5_9_3 - 1.0) + 0.5*(y5_9_3 - 1.0)*(y5_9_3 - 1.0) + \
    0.5*(z5_9_3 - 1.0)*(z5_9_3 - 1.0) + 0.5*(x6_9_3 - 1.0)*(x6_9_3 - 1.0) + \
    0.5*(y6_9_3 - 1.0)*(y6_9_3 - 1.0) + 0.5*(z6_9_3 - 1.0)*(z6_9_3 - 1.0) + \
    0.5*(x7_9_3 - 1.0)*(x7_9_3 - 1.0) + 0.5*(y7_9_3 - 1.0)*(y7_9_3 - 1.0) + \
    0.5*(z7_9_3 - 1.0)*(z7_9_3 - 1.0) + 0.5*(x8_9_3 - 1.0)*(x8_9_3 - 1.0) + \
    0.5*(y8_9_3 - 1.0)*(y8_9_3 - 1.0) + 0.5*(z8_9_3 - 1.0)*(z8_9_3 - 1.0) + \
    0.5*(x9_9_3 - 1.0)*(x9_9_3 - 1.0) + 0.5*(y9_9_3 - 1.0)*(y9_9_3 - 1.0) + \
    0.5*(z9_9_3 - 1.0)*(z9_9_3 - 1.0) + 0.5*(x1_1_4 - 1.0)*(x1_1_4 - 1.0) + \
    0.5*(y1_1_4 - 1.0)*(y1_1_4 - 1.0) + 0.5*(z1_1_4 - 1.0)*(z1_1_4 - 1.0) + \
    0.5*(x2_1_4 - 1.0)*(x2_1_4 - 1.0) + 0.5*(y2_1_4 - 1.0)*(y2_1_4 - 1.0) + \
    0.5*(z2_1_4 - 1.0)*(z2_1_4 - 1.0) + 0.5*(x3_1_4 - 1.0)*(x3_1_4 - 1.0) + \
    0.5*(y3_1_4 - 1.0)*(y3_1_4 - 1.0) + 0.5*(z3_1_4 - 1.0)*(z3_1_4 - 1.0) + \
    0.5*(x4_1_4 - 1.0)*(x4_1_4 - 1.0) + 0.5*(y4_1_4 - 1.0)*(y4_1_4 - 1.0) + \
    0.5*(z4_1_4 - 1.0)*(z4_1_4 - 1.0) + 0.5*(x5_1_4 - 1.0)*(x5_1_4 - 1.0) + \
    0.5*(y5_1_4 - 1.0)*(y5_1_4 - 1.0) + 0.5*(z5_1_4 - 1.0)*(z5_1_4 - 1.0) + \
    0.5*(x6_1_4 - 1.0)*(x6_1_4 - 1.0) + 0.5*(y6_1_4 - 1.0)*(y6_1_4 - 1.0) + \
    0.5*(z6_1_4 - 1.0)*(z6_1_4 - 1.0) + 0.5*(x7_1_4 - 1.0)*(x7_1_4 - 1.0) + \
    0.5*(y7_1_4 - 1.0)*(y7_1_4 - 1.0) + 0.5*(z7_1_4 - 1.0)*(z7_1_4 - 1.0) + \
    0.5*(x8_1_4 - 1.0)*(x8_1_4 - 1.0) + 0.5*(y8_1_4 - 1.0)*(y8_1_4 - 1.0) + \
    0.5*(z8_1_4 - 1.0)*(z8_1_4 - 1.0) + 0.5*(x9_1_4 - 1.0)*(x9_1_4 - 1.0) + \
    0.5*(y9_1_4 - 1.0)*(y9_1_4 - 1.0) + 0.5*(z9_1_4 - 1.0)*(z9_1_4 - 1.0) + \
    0.5*(x1_2_4 - 1.0)*(x1_2_4 - 1.0) + 0.5*(y1_2_4 - 1.0)*(y1_2_4 - 1.0) + \
    0.5*(z1_2_4 - 1.0)*(z1_2_4 - 1.0) + 0.5*(x2_2_4 - 1.0)*(x2_2_4 - 1.0) + \
    0.5*(y2_2_4 - 1.0)*(y2_2_4 - 1.0) + 0.5*(z2_2_4 - 1.0)*(z2_2_4 - 1.0) + \
    0.5*(x3_2_4 - 1.0)*(x3_2_4 - 1.0) + 0.5*(y3_2_4 - 1.0)*(y3_2_4 - 1.0) + \
    0.5*(z3_2_4 - 1.0)*(z3_2_4 - 1.0) + 0.5*(x4_2_4 - 1.0)*(x4_2_4 - 1.0) + \
    0.5*(y4_2_4 - 1.0)*(y4_2_4 - 1.0) + 0.5*(z4_2_4 - 1.0)*(z4_2_4 - 1.0) + \
    0.5*(x5_2_4 - 1.0)*(x5_2_4 - 1.0) + 0.5*(y5_2_4 - 1.0)*(y5_2_4 - 1.0) + \
    0.5*(z5_2_4 - 1.0)*(z5_2_4 - 1.0) + 0.5*(x6_2_4 - 1.0)*(x6_2_4 - 1.0) + \
    0.5*(y6_2_4 - 1.0)*(y6_2_4 - 1.0) + 0.5*(z6_2_4 - 1.0)*(z6_2_4 - 1.0) + \
    0.5*(x7_2_4 - 1.0)*(x7_2_4 - 1.0) + 0.5*(y7_2_4 - 1.0)*(y7_2_4 - 1.0) + \
    0.5*(z7_2_4 - 1.0)*(z7_2_4 - 1.0) + 0.5*(x8_2_4 - 1.0)*(x8_2_4 - 1.0) + \
    0.5*(y8_2_4 - 1.0)*(y8_2_4 - 1.0) + 0.5*(z8_2_4 - 1.0)*(z8_2_4 - 1.0) + \
    0.5*(x9_2_4 - 1.0)*(x9_2_4 - 1.0) + 0.5*(y9_2_4 - 1.0)*(y9_2_4 - 1.0) + \
    0.5*(z9_2_4 - 1.0)*(z9_2_4 - 1.0) + 0.5*(x1_3_4 - 1.0)*(x1_3_4 - 1.0) + \
    0.5*(y1_3_4 - 1.0)*(y1_3_4 - 1.0) + 0.5*(z1_3_4 - 1.0)*(z1_3_4 - 1.0) + \
    0.5*(x2_3_4 - 1.0)*(x2_3_4 - 1.0) + 0.5*(y2_3_4 - 1.0)*(y2_3_4 - 1.0) + \
    0.5*(z2_3_4 - 1.0)*(z2_3_4 - 1.0) + 0.5*(x3_3_4 - 1.0)*(x3_3_4 - 1.0) + \
    0.5*(y3_3_4 - 1.0)*(y3_3_4 - 1.0) + 0.5*(z3_3_4 - 1.0)*(z3_3_4 - 1.0) + \
    0.5*(x4_3_4 - 1.0)*(x4_3_4 - 1.0) + 0.5*(y4_3_4 - 1.0)*(y4_3_4 - 1.0) + \
    0.5*(z4_3_4 - 1.0)*(z4_3_4 - 1.0) + 0.5*(x5_3_4 - 1.0)*(x5_3_4 - 1.0) + \
    0.5*(y5_3_4 - 1.0)*(y5_3_4 - 1.0) + 0.5*(z5_3_4 - 1.0)*(z5_3_4 - 1.0) + \
    0.5*(x6_3_4 - 1.0)*(x6_3_4 - 1.0) + 0.5*(y6_3_4 - 1.0)*(y6_3_4 - 1.0) + \
    0.5*(z6_3_4 - 1.0)*(z6_3_4 - 1.0) + 0.5*(x7_3_4 - 1.0)*(x7_3_4 - 1.0) + \
    0.5*(y7_3_4 - 1.0)*(y7_3_4 - 1.0) + 0.5*(z7_3_4 - 1.0)*(z7_3_4 - 1.0) + \
    0.5*(x8_3_4 - 1.0)*(x8_3_4 - 1.0) + 0.5*(y8_3_4 - 1.0)*(y8_3_4 - 1.0) + \
    0.5*(z8_3_4 - 1.0)*(z8_3_4 - 1.0) + 0.5*(x9_3_4 - 1.0)*(x9_3_4 - 1.0) + \
    0.5*(y9_3_4 - 1.0)*(y9_3_4 - 1.0) + 0.5*(z9_3_4 - 1.0)*(z9_3_4 - 1.0) + \
    0.5*(x1_4_4 - 1.0)*(x1_4_4 - 1.0) + 0.5*(y1_4_4 - 1.0)*(y1_4_4 - 1.0) + \
    0.5*(z1_4_4 - 1.0)*(z1_4_4 - 1.0) + 0.5*(x2_4_4 - 1.0)*(x2_4_4 - 1.0) + \
    0.5*(y2_4_4 - 1.0)*(y2_4_4 - 1.0) + 0.5*(z2_4_4 - 1.0)*(z2_4_4 - 1.0) + \
    0.5*(x3_4_4 - 1.0)*(x3_4_4 - 1.0) + 0.5*(y3_4_4 - 1.0)*(y3_4_4 - 1.0) + \
    0.5*(z3_4_4 - 1.0)*(z3_4_4 - 1.0) + 0.5*(x4_4_4 - 1.0)*(x4_4_4 - 1.0) + \
    0.5*(y4_4_4 - 1.0)*(y4_4_4 - 1.0) + 0.5*(z4_4_4 - 1.0)*(z4_4_4 - 1.0) + \
    0.5*(x5_4_4 - 1.0)*(x5_4_4 - 1.0) + 0.5*(y5_4_4 - 1.0)*(y5_4_4 - 1.0) + \
    0.5*(z5_4_4 - 1.0)*(z5_4_4 - 1.0) + 0.5*(x6_4_4 - 1.0)*(x6_4_4 - 1.0) + \
    0.5*(y6_4_4 - 1.0)*(y6_4_4 - 1.0) + 0.5*(z6_4_4 - 1.0)*(z6_4_4 - 1.0) + \
    0.5*(x7_4_4 - 1.0)*(x7_4_4 - 1.0) + 0.5*(y7_4_4 - 1.0)*(y7_4_4 - 1.0) + \
    0.5*(z7_4_4 - 1.0)*(z7_4_4 - 1.0) + 0.5*(x8_4_4 - 1.0)*(x8_4_4 - 1.0) + \
    0.5*(y8_4_4 - 1.0)*(y8_4_4 - 1.0) + 0.5*(z8_4_4 - 1.0)*(z8_4_4 - 1.0) + \
    0.5*(x9_4_4 - 1.0)*(x9_4_4 - 1.0) + 0.5*(y9_4_4 - 1.0)*(y9_4_4 - 1.0) + \
    0.5*(z9_4_4 - 1.0)*(z9_4_4 - 1.0) + 0.5*(x1_5_4 - 1.0)*(x1_5_4 - 1.0) + \
    0.5*(y1_5_4 - 1.0)*(y1_5_4 - 1.0) + 0.5*(z1_5_4 - 1.0)*(z1_5_4 - 1.0) + \
    0.5*(x2_5_4 - 1.0)*(x2_5_4 - 1.0) + 0.5*(y2_5_4 - 1.0)*(y2_5_4 - 1.0) + \
    0.5*(z2_5_4 - 1.0)*(z2_5_4 - 1.0) + 0.5*(x3_5_4 - 1.0)*(x3_5_4 - 1.0) + \
    0.5*(y3_5_4 - 1.0)*(y3_5_4 - 1.0) + 0.5*(z3_5_4 - 1.0)*(z3_5_4 - 1.0) + \
    0.5*(x4_5_4 - 1.0)*(x4_5_4 - 1.0) + 0.5*(y4_5_4 - 1.0)*(y4_5_4 - 1.0) + \
    0.5*(z4_5_4 - 1.0)*(z4_5_4 - 1.0) + 0.5*(x5_5_4 - 1.0)*(x5_5_4 - 1.0) + \
    0.5*(y5_5_4 - 1.0)*(y5_5_4 - 1.0) + 0.5*(z5_5_4 - 1.0)*(z5_5_4 - 1.0) + \
    0.5*(x6_5_4 - 1.0)*(x6_5_4 - 1.0) + 0.5*(y6_5_4 - 1.0)*(y6_5_4 - 1.0) + \
    0.5*(z6_5_4 - 1.0)*(z6_5_4 - 1.0) + 0.5*(x7_5_4 - 1.0)*(x7_5_4 - 1.0) + \
    0.5*(y7_5_4 - 1.0)*(y7_5_4 - 1.0) + 0.5*(z7_5_4 - 1.0)*(z7_5_4 - 1.0) + \
    0.5*(x8_5_4 - 1.0)*(x8_5_4 - 1.0) + 0.5*(y8_5_4 - 1.0)*(y8_5_4 - 1.0) + \
    0.5*(z8_5_4 - 1.0)*(z8_5_4 - 1.0) + 0.5*(x9_5_4 - 1.0)*(x9_5_4 - 1.0) + \
    0.5*(y9_5_4 - 1.0)*(y9_5_4 - 1.0) + 0.5*(z9_5_4 - 1.0)*(z9_5_4 - 1.0) + \
    0.5*(x1_6_4 - 1.0)*(x1_6_4 - 1.0) + 0.5*(y1_6_4 - 1.0)*(y1_6_4 - 1.0) + \
    0.5*(z1_6_4 - 1.0)*(z1_6_4 - 1.0) + 0.5*(x2_6_4 - 1.0)*(x2_6_4 - 1.0) + \
    0.5*(y2_6_4 - 1.0)*(y2_6_4 - 1.0) + 0.5*(z2_6_4 - 1.0)*(z2_6_4 - 1.0) + \
    0.5*(x3_6_4 - 1.0)*(x3_6_4 - 1.0) + 0.5*(y3_6_4 - 1.0)*(y3_6_4 - 1.0) + \
    0.5*(z3_6_4 - 1.0)*(z3_6_4 - 1.0) + 0.5*(x4_6_4 - 1.0)*(x4_6_4 - 1.0) + \
    0.5*(y4_6_4 - 1.0)*(y4_6_4 - 1.0) + 0.5*(z4_6_4 - 1.0)*(z4_6_4 - 1.0) + \
    0.5*(x5_6_4 - 1.0)*(x5_6_4 - 1.0) + 0.5*(y5_6_4 - 1.0)*(y5_6_4 - 1.0) + \
    0.5*(z5_6_4 - 1.0)*(z5_6_4 - 1.0) + 0.5*(x6_6_4 - 1.0)*(x6_6_4 - 1.0) + \
    0.5*(y6_6_4 - 1.0)*(y6_6_4 - 1.0) + 0.5*(z6_6_4 - 1.0)*(z6_6_4 - 1.0) + \
    0.5*(x7_6_4 - 1.0)*(x7_6_4 - 1.0) + 0.5*(y7_6_4 - 1.0)*(y7_6_4 - 1.0) + \
    0.5*(z7_6_4 - 1.0)*(z7_6_4 - 1.0) + 0.5*(x8_6_4 - 1.0)*(x8_6_4 - 1.0) + \
    0.5*(y8_6_4 - 1.0)*(y8_6_4 - 1.0) + 0.5*(z8_6_4 - 1.0)*(z8_6_4 - 1.0) + \
    0.5*(x9_6_4 - 1.0)*(x9_6_4 - 1.0) + 0.5*(y9_6_4 - 1.0)*(y9_6_4 - 1.0) + \
    0.5*(z9_6_4 - 1.0)*(z9_6_4 - 1.0) + 0.5*(x1_7_4 - 1.0)*(x1_7_4 - 1.0) + \
    0.5*(y1_7_4 - 1.0)*(y1_7_4 - 1.0) + 0.5*(z1_7_4 - 1.0)*(z1_7_4 - 1.0) + \
    0.5*(x2_7_4 - 1.0)*(x2_7_4 - 1.0) + 0.5*(y2_7_4 - 1.0)*(y2_7_4 - 1.0) + \
    0.5*(z2_7_4 - 1.0)*(z2_7_4 - 1.0) + 0.5*(x3_7_4 - 1.0)*(x3_7_4 - 1.0) + \
    0.5*(y3_7_4 - 1.0)*(y3_7_4 - 1.0) + 0.5*(z3_7_4 - 1.0)*(z3_7_4 - 1.0) + \
    0.5*(x4_7_4 - 1.0)*(x4_7_4 - 1.0) + 0.5*(y4_7_4 - 1.0)*(y4_7_4 - 1.0) + \
    0.5*(z4_7_4 - 1.0)*(z4_7_4 - 1.0) + 0.5*(x5_7_4 - 1.0)*(x5_7_4 - 1.0) + \
    0.5*(y5_7_4 - 1.0)*(y5_7_4 - 1.0) + 0.5*(z5_7_4 - 1.0)*(z5_7_4 - 1.0) + \
    0.5*(x6_7_4 - 1.0)*(x6_7_4 - 1.0) + 0.5*(y6_7_4 - 1.0)*(y6_7_4 - 1.0) + \
    0.5*(z6_7_4 - 1.0)*(z6_7_4 - 1.0) + 0.5*(x7_7_4 - 1.0)*(x7_7_4 - 1.0) + \
    0.5*(y7_7_4 - 1.0)*(y7_7_4 - 1.0) + 0.5*(z7_7_4 - 1.0)*(z7_7_4 - 1.0) + \
    0.5*(x8_7_4 - 1.0)*(x8_7_4 - 1.0) + 0.5*(y8_7_4 - 1.0)*(y8_7_4 - 1.0) + \
    0.5*(z8_7_4 - 1.0)*(z8_7_4 - 1.0) + 0.5*(x9_7_4 - 1.0)*(x9_7_4 - 1.0) + \
    0.5*(y9_7_4 - 1.0)*(y9_7_4 - 1.0) + 0.5*(z9_7_4 - 1.0)*(z9_7_4 - 1.0) + \
    0.5*(x1_8_4 - 1.0)*(x1_8_4 - 1.0) + 0.5*(y1_8_4 - 1.0)*(y1_8_4 - 1.0) + \
    0.5*(z1_8_4 - 1.0)*(z1_8_4 - 1.0) + 0.5*(x2_8_4 - 1.0)*(x2_8_4 - 1.0) + \
    0.5*(y2_8_4 - 1.0)*(y2_8_4 - 1.0) + 0.5*(z2_8_4 - 1.0)*(z2_8_4 - 1.0) + \
    0.5*(x3_8_4 - 1.0)*(x3_8_4 - 1.0) + 0.5*(y3_8_4 - 1.0)*(y3_8_4 - 1.0) + \
    0.5*(z3_8_4 - 1.0)*(z3_8_4 - 1.0) + 0.5*(x4_8_4 - 1.0)*(x4_8_4 - 1.0) + \
    0.5*(y4_8_4 - 1.0)*(y4_8_4 - 1.0) + 0.5*(z4_8_4 - 1.0)*(z4_8_4 - 1.0) + \
    0.5*(x5_8_4 - 1.0)*(x5_8_4 - 1.0) + 0.5*(y5_8_4 - 1.0)*(y5_8_4 - 1.0) + \
    0.5*(z5_8_4 - 1.0)*(z5_8_4 - 1.0) + 0.5*(x6_8_4 - 1.0)*(x6_8_4 - 1.0) + \
    0.5*(y6_8_4 - 1.0)*(y6_8_4 - 1.0) + 0.5*(z6_8_4 - 1.0)*(z6_8_4 - 1.0) + \
    0.5*(x7_8_4 - 1.0)*(x7_8_4 - 1.0) + 0.5*(y7_8_4 - 1.0)*(y7_8_4 - 1.0) + \
    0.5*(z7_8_4 - 1.0)*(z7_8_4 - 1.0) + 0.5*(x8_8_4 - 1.0)*(x8_8_4 - 1.0) + \
    0.5*(y8_8_4 - 1.0)*(y8_8_4 - 1.0) + 0.5*(z8_8_4 - 1.0)*(z8_8_4 - 1.0) + \
    0.5*(x9_8_4 - 1.0)*(x9_8_4 - 1.0) + 0.5*(y9_8_4 - 1.0)*(y9_8_4 - 1.0) + \
    0.5*(z9_8_4 - 1.0)*(z9_8_4 - 1.0) + 0.5*(x1_9_4 - 1.0)*(x1_9_4 - 1.0) + \
    0.5*(y1_9_4 - 1.0)*(y1_9_4 - 1.0) + 0.5*(z1_9_4 - 1.0)*(z1_9_4 - 1.0) + \
    0.5*(x2_9_4 - 1.0)*(x2_9_4 - 1.0) + 0.5*(y2_9_4 - 1.0)*(y2_9_4 - 1.0) + \
    0.5*(z2_9_4 - 1.0)*(z2_9_4 - 1.0) + 0.5*(x3_9_4 - 1.0)*(x3_9_4 - 1.0) + \
    0.5*(y3_9_4 - 1.0)*(y3_9_4 - 1.0) + 0.5*(z3_9_4 - 1.0)*(z3_9_4 - 1.0) + \
    0.5*(x4_9_4 - 1.0)*(x4_9_4 - 1.0) + 0.5*(y4_9_4 - 1.0)*(y4_9_4 - 1.0) + \
    0.5*(z4_9_4 - 1.0)*(z4_9_4 - 1.0) + 0.5*(x5_9_4 - 1.0)*(x5_9_4 - 1.0) + \
    0.5*(y5_9_4 - 1.0)*(y5_9_4 - 1.0) + 0.5*(z5_9_4 - 1.0)*(z5_9_4 - 1.0) + \
    0.5*(x6_9_4 - 1.0)*(x6_9_4 - 1.0) + 0.5*(y6_9_4 - 1.0)*(y6_9_4 - 1.0) + \
    0.5*(z6_9_4 - 1.0)*(z6_9_4 - 1.0) + 0.5*(x7_9_4 - 1.0)*(x7_9_4 - 1.0) + \
    0.5*(y7_9_4 - 1.0)*(y7_9_4 - 1.0) + 0.5*(z7_9_4 - 1.0)*(z7_9_4 - 1.0) + \
    0.5*(x8_9_4 - 1.0)*(x8_9_4 - 1.0) + 0.5*(y8_9_4 - 1.0)*(y8_9_4 - 1.0) + \
    0.5*(z8_9_4 - 1.0)*(z8_9_4 - 1.0) + 0.5*(x9_9_4 - 1.0)*(x9_9_4 - 1.0) + \
    0.5*(y9_9_4 - 1.0)*(y9_9_4 - 1.0) + 0.5*(z9_9_4 - 1.0)*(z9_9_4 - 1.0) + \
    0.5*(x1_1_5 - 1.0)*(x1_1_5 - 1.0) + 0.5*(y1_1_5 - 1.0)*(y1_1_5 - 1.0) + \
    0.5*(z1_1_5 - 1.0)*(z1_1_5 - 1.0) + 0.5*(x2_1_5 - 1.0)*(x2_1_5 - 1.0) + \
    0.5*(y2_1_5 - 1.0)*(y2_1_5 - 1.0) + 0.5*(z2_1_5 - 1.0)*(z2_1_5 - 1.0) + \
    0.5*(x3_1_5 - 1.0)*(x3_1_5 - 1.0) + 0.5*(y3_1_5 - 1.0)*(y3_1_5 - 1.0) + \
    0.5*(z3_1_5 - 1.0)*(z3_1_5 - 1.0) + 0.5*(x4_1_5 - 1.0)*(x4_1_5 - 1.0) + \
    0.5*(y4_1_5 - 1.0)*(y4_1_5 - 1.0) + 0.5*(z4_1_5 - 1.0)*(z4_1_5 - 1.0) + \
    0.5*(x5_1_5 - 1.0)*(x5_1_5 - 1.0) + 0.5*(y5_1_5 - 1.0)*(y5_1_5 - 1.0) + \
    0.5*(z5_1_5 - 1.0)*(z5_1_5 - 1.0) + 0.5*(x6_1_5 - 1.0)*(x6_1_5 - 1.0) + \
    0.5*(y6_1_5 - 1.0)*(y6_1_5 - 1.0) + 0.5*(z6_1_5 - 1.0)*(z6_1_5 - 1.0) + \
    0.5*(x7_1_5 - 1.0)*(x7_1_5 - 1.0) + 0.5*(y7_1_5 - 1.0)*(y7_1_5 - 1.0) + \
    0.5*(z7_1_5 - 1.0)*(z7_1_5 - 1.0) + 0.5*(x8_1_5 - 1.0)*(x8_1_5 - 1.0) + \
    0.5*(y8_1_5 - 1.0)*(y8_1_5 - 1.0) + 0.5*(z8_1_5 - 1.0)*(z8_1_5 - 1.0) + \
    0.5*(x9_1_5 - 1.0)*(x9_1_5 - 1.0) + 0.5*(y9_1_5 - 1.0)*(y9_1_5 - 1.0) + \
    0.5*(z9_1_5 - 1.0)*(z9_1_5 - 1.0) + 0.5*(x1_2_5 - 1.0)*(x1_2_5 - 1.0) + \
    0.5*(y1_2_5 - 1.0)*(y1_2_5 - 1.0) + 0.5*(z1_2_5 - 1.0)*(z1_2_5 - 1.0) + \
    0.5*(x2_2_5 - 1.0)*(x2_2_5 - 1.0) + 0.5*(y2_2_5 - 1.0)*(y2_2_5 - 1.0) + \
    0.5*(z2_2_5 - 1.0)*(z2_2_5 - 1.0) + 0.5*(x3_2_5 - 1.0)*(x3_2_5 - 1.0) + \
    0.5*(y3_2_5 - 1.0)*(y3_2_5 - 1.0) + 0.5*(z3_2_5 - 1.0)*(z3_2_5 - 1.0) + \
    0.5*(x4_2_5 - 1.0)*(x4_2_5 - 1.0) + 0.5*(y4_2_5 - 1.0)*(y4_2_5 - 1.0) + \
    0.5*(z4_2_5 - 1.0)*(z4_2_5 - 1.0) + 0.5*(x5_2_5 - 1.0)*(x5_2_5 - 1.0) + \
    0.5*(y5_2_5 - 1.0)*(y5_2_5 - 1.0) + 0.5*(z5_2_5 - 1.0)*(z5_2_5 - 1.0) + \
    0.5*(x6_2_5 - 1.0)*(x6_2_5 - 1.0) + 0.5*(y6_2_5 - 1.0)*(y6_2_5 - 1.0) + \
    0.5*(z6_2_5 - 1.0)*(z6_2_5 - 1.0) + 0.5*(x7_2_5 - 1.0)*(x7_2_5 - 1.0) + \
    0.5*(y7_2_5 - 1.0)*(y7_2_5 - 1.0) + 0.5*(z7_2_5 - 1.0)*(z7_2_5 - 1.0) + \
    0.5*(x8_2_5 - 1.0)*(x8_2_5 - 1.0) + 0.5*(y8_2_5 - 1.0)*(y8_2_5 - 1.0) + \
    0.5*(z8_2_5 - 1.0)*(z8_2_5 - 1.0) + 0.5*(x9_2_5 - 1.0)*(x9_2_5 - 1.0) + \
    0.5*(y9_2_5 - 1.0)*(y9_2_5 - 1.0) + 0.5*(z9_2_5 - 1.0)*(z9_2_5 - 1.0) + \
    0.5*(x1_3_5 - 1.0)*(x1_3_5 - 1.0) + 0.5*(y1_3_5 - 1.0)*(y1_3_5 - 1.0) + \
    0.5*(z1_3_5 - 1.0)*(z1_3_5 - 1.0) + 0.5*(x2_3_5 - 1.0)*(x2_3_5 - 1.0) + \
    0.5*(y2_3_5 - 1.0)*(y2_3_5 - 1.0) + 0.5*(z2_3_5 - 1.0)*(z2_3_5 - 1.0) + \
    0.5*(x3_3_5 - 1.0)*(x3_3_5 - 1.0) + 0.5*(y3_3_5 - 1.0)*(y3_3_5 - 1.0) + \
    0.5*(z3_3_5 - 1.0)*(z3_3_5 - 1.0) + 0.5*(x4_3_5 - 1.0)*(x4_3_5 - 1.0) + \
    0.5*(y4_3_5 - 1.0)*(y4_3_5 - 1.0) + 0.5*(z4_3_5 - 1.0)*(z4_3_5 - 1.0) + \
    0.5*(x5_3_5 - 1.0)*(x5_3_5 - 1.0) + 0.5*(y5_3_5 - 1.0)*(y5_3_5 - 1.0) + \
    0.5*(z5_3_5 - 1.0)*(z5_3_5 - 1.0) + 0.5*(x6_3_5 - 1.0)*(x6_3_5 - 1.0) + \
    0.5*(y6_3_5 - 1.0)*(y6_3_5 - 1.0) + 0.5*(z6_3_5 - 1.0)*(z6_3_5 - 1.0) + \
    0.5*(x7_3_5 - 1.0)*(x7_3_5 - 1.0) + 0.5*(y7_3_5 - 1.0)*(y7_3_5 - 1.0) + \
    0.5*(z7_3_5 - 1.0)*(z7_3_5 - 1.0) + 0.5*(x8_3_5 - 1.0)*(x8_3_5 - 1.0) + \
    0.5*(y8_3_5 - 1.0)*(y8_3_5 - 1.0) + 0.5*(z8_3_5 - 1.0)*(z8_3_5 - 1.0) + \
    0.5*(x9_3_5 - 1.0)*(x9_3_5 - 1.0) + 0.5*(y9_3_5 - 1.0)*(y9_3_5 - 1.0) + \
    0.5*(z9_3_5 - 1.0)*(z9_3_5 - 1.0) + 0.5*(x1_4_5 - 1.0)*(x1_4_5 - 1.0) + \
    0.5*(y1_4_5 - 1.0)*(y1_4_5 - 1.0) + 0.5*(z1_4_5 - 1.0)*(z1_4_5 - 1.0) + \
    0.5*(x2_4_5 - 1.0)*(x2_4_5 - 1.0) + 0.5*(y2_4_5 - 1.0)*(y2_4_5 - 1.0) + \
    0.5*(z2_4_5 - 1.0)*(z2_4_5 - 1.0) + 0.5*(x3_4_5 - 1.0)*(x3_4_5 - 1.0) + \
    0.5*(y3_4_5 - 1.0)*(y3_4_5 - 1.0) + 0.5*(z3_4_5 - 1.0)*(z3_4_5 - 1.0) + \
    0.5*(x4_4_5 - 1.0)*(x4_4_5 - 1.0) + 0.5*(y4_4_5 - 1.0)*(y4_4_5 - 1.0) + \
    0.5*(z4_4_5 - 1.0)*(z4_4_5 - 1.0) + 0.5*(x5_4_5 - 1.0)*(x5_4_5 - 1.0) + \
    0.5*(y5_4_5 - 1.0)*(y5_4_5 - 1.0) + 0.5*(z5_4_5 - 1.0)*(z5_4_5 - 1.0) + \
    0.5*(x6_4_5 - 1.0)*(x6_4_5 - 1.0) + 0.5*(y6_4_5 - 1.0)*(y6_4_5 - 1.0) + \
    0.5*(z6_4_5 - 1.0)*(z6_4_5 - 1.0) + 0.5*(x7_4_5 - 1.0)*(x7_4_5 - 1.0) + \
    0.5*(y7_4_5 - 1.0)*(y7_4_5 - 1.0) + 0.5*(z7_4_5 - 1.0)*(z7_4_5 - 1.0) + \
    0.5*(x8_4_5 - 1.0)*(x8_4_5 - 1.0) + 0.5*(y8_4_5 - 1.0)*(y8_4_5 - 1.0) + \
    0.5*(z8_4_5 - 1.0)*(z8_4_5 - 1.0) + 0.5*(x9_4_5 - 1.0)*(x9_4_5 - 1.0) + \
    0.5*(y9_4_5 - 1.0)*(y9_4_5 - 1.0) + 0.5*(z9_4_5 - 1.0)*(z9_4_5 - 1.0) + \
    0.5*(x1_5_5 - 1.0)*(x1_5_5 - 1.0) + 0.5*(y1_5_5 - 1.0)*(y1_5_5 - 1.0) + \
    0.5*(z1_5_5 - 1.0)*(z1_5_5 - 1.0) + 0.5*(x2_5_5 - 1.0)*(x2_5_5 - 1.0) + \
    0.5*(y2_5_5 - 1.0)*(y2_5_5 - 1.0) + 0.5*(z2_5_5 - 1.0)*(z2_5_5 - 1.0) + \
    0.5*(x3_5_5 - 1.0)*(x3_5_5 - 1.0) + 0.5*(y3_5_5 - 1.0)*(y3_5_5 - 1.0) + \
    0.5*(z3_5_5 - 1.0)*(z3_5_5 - 1.0) + 0.5*(x4_5_5 - 1.0)*(x4_5_5 - 1.0) + \
    0.5*(y4_5_5 - 1.0)*(y4_5_5 - 1.0) + 0.5*(z4_5_5 - 1.0)*(z4_5_5 - 1.0) + \
    0.5*(x5_5_5 - 1.0)*(x5_5_5 - 1.0) + 0.5*(y5_5_5 - 1.0)*(y5_5_5 - 1.0) + \
    0.5*(z5_5_5 - 1.0)*(z5_5_5 - 1.0) + 0.5*(x6_5_5 - 1.0)*(x6_5_5 - 1.0) + \
    0.5*(y6_5_5 - 1.0)*(y6_5_5 - 1.0) + 0.5*(z6_5_5 - 1.0)*(z6_5_5 - 1.0) + \
    0.5*(x7_5_5 - 1.0)*(x7_5_5 - 1.0) + 0.5*(y7_5_5 - 1.0)*(y7_5_5 - 1.0) + \
    0.5*(z7_5_5 - 1.0)*(z7_5_5 - 1.0) + 0.5*(x8_5_5 - 1.0)*(x8_5_5 - 1.0) + \
    0.5*(y8_5_5 - 1.0)*(y8_5_5 - 1.0) + 0.5*(z8_5_5 - 1.0)*(z8_5_5 - 1.0) + \
    0.5*(x9_5_5 - 1.0)*(x9_5_5 - 1.0) + 0.5*(y9_5_5 - 1.0)*(y9_5_5 - 1.0) + \
    0.5*(z9_5_5 - 1.0)*(z9_5_5 - 1.0) + 0.5*(x1_6_5 - 1.0)*(x1_6_5 - 1.0) + \
    0.5*(y1_6_5 - 1.0)*(y1_6_5 - 1.0) + 0.5*(z1_6_5 - 1.0)*(z1_6_5 - 1.0) + \
    0.5*(x2_6_5 - 1.0)*(x2_6_5 - 1.0) + 0.5*(y2_6_5 - 1.0)*(y2_6_5 - 1.0) + \
    0.5*(z2_6_5 - 1.0)*(z2_6_5 - 1.0) + 0.5*(x3_6_5 - 1.0)*(x3_6_5 - 1.0) + \
    0.5*(y3_6_5 - 1.0)*(y3_6_5 - 1.0) + 0.5*(z3_6_5 - 1.0)*(z3_6_5 - 1.0) + \
    0.5*(x4_6_5 - 1.0)*(x4_6_5 - 1.0) + 0.5*(y4_6_5 - 1.0)*(y4_6_5 - 1.0) + \
    0.5*(z4_6_5 - 1.0)*(z4_6_5 - 1.0) + 0.5*(x5_6_5 - 1.0)*(x5_6_5 - 1.0) + \
    0.5*(y5_6_5 - 1.0)*(y5_6_5 - 1.0) + 0.5*(z5_6_5 - 1.0)*(z5_6_5 - 1.0) + \
    0.5*(x6_6_5 - 1.0)*(x6_6_5 - 1.0) + 0.5*(y6_6_5 - 1.0)*(y6_6_5 - 1.0) + \
    0.5*(z6_6_5 - 1.0)*(z6_6_5 - 1.0) + 0.5*(x7_6_5 - 1.0)*(x7_6_5 - 1.0) + \
    0.5*(y7_6_5 - 1.0)*(y7_6_5 - 1.0) + 0.5*(z7_6_5 - 1.0)*(z7_6_5 - 1.0) + \
    0.5*(x8_6_5 - 1.0)*(x8_6_5 - 1.0) + 0.5*(y8_6_5 - 1.0)*(y8_6_5 - 1.0) + \
    0.5*(z8_6_5 - 1.0)*(z8_6_5 - 1.0) + 0.5*(x9_6_5 - 1.0)*(x9_6_5 - 1.0) + \
    0.5*(y9_6_5 - 1.0)*(y9_6_5 - 1.0) + 0.5*(z9_6_5 - 1.0)*(z9_6_5 - 1.0) + \
    0.5*(x1_7_5 - 1.0)*(x1_7_5 - 1.0) + 0.5*(y1_7_5 - 1.0)*(y1_7_5 - 1.0) + \
    0.5*(z1_7_5 - 1.0)*(z1_7_5 - 1.0) + 0.5*(x2_7_5 - 1.0)*(x2_7_5 - 1.0) + \
    0.5*(y2_7_5 - 1.0)*(y2_7_5 - 1.0) + 0.5*(z2_7_5 - 1.0)*(z2_7_5 - 1.0) + \
    0.5*(x3_7_5 - 1.0)*(x3_7_5 - 1.0) + 0.5*(y3_7_5 - 1.0)*(y3_7_5 - 1.0) + \
    0.5*(z3_7_5 - 1.0)*(z3_7_5 - 1.0) + 0.5*(x4_7_5 - 1.0)*(x4_7_5 - 1.0) + \
    0.5*(y4_7_5 - 1.0)*(y4_7_5 - 1.0) + 0.5*(z4_7_5 - 1.0)*(z4_7_5 - 1.0) + \
    0.5*(x5_7_5 - 1.0)*(x5_7_5 - 1.0) + 0.5*(y5_7_5 - 1.0)*(y5_7_5 - 1.0) + \
    0.5*(z5_7_5 - 1.0)*(z5_7_5 - 1.0) + 0.5*(x6_7_5 - 1.0)*(x6_7_5 - 1.0) + \
    0.5*(y6_7_5 - 1.0)*(y6_7_5 - 1.0) + 0.5*(z6_7_5 - 1.0)*(z6_7_5 - 1.0) + \
    0.5*(x7_7_5 - 1.0)*(x7_7_5 - 1.0) + 0.5*(y7_7_5 - 1.0)*(y7_7_5 - 1.0) + \
    0.5*(z7_7_5 - 1.0)*(z7_7_5 - 1.0) + 0.5*(x8_7_5 - 1.0)*(x8_7_5 - 1.0) + \
    0.5*(y8_7_5 - 1.0)*(y8_7_5 - 1.0) + 0.5*(z8_7_5 - 1.0)*(z8_7_5 - 1.0) + \
    0.5*(x9_7_5 - 1.0)*(x9_7_5 - 1.0) + 0.5*(y9_7_5 - 1.0)*(y9_7_5 - 1.0) + \
    0.5*(z9_7_5 - 1.0)*(z9_7_5 - 1.0) + 0.5*(x1_8_5 - 1.0)*(x1_8_5 - 1.0) + \
    0.5*(y1_8_5 - 1.0)*(y1_8_5 - 1.0) + 0.5*(z1_8_5 - 1.0)*(z1_8_5 - 1.0) + \
    0.5*(x2_8_5 - 1.0)*(x2_8_5 - 1.0) + 0.5*(y2_8_5 - 1.0)*(y2_8_5 - 1.0) + \
    0.5*(z2_8_5 - 1.0)*(z2_8_5 - 1.0) + 0.5*(x3_8_5 - 1.0)*(x3_8_5 - 1.0) + \
    0.5*(y3_8_5 - 1.0)*(y3_8_5 - 1.0) + 0.5*(z3_8_5 - 1.0)*(z3_8_5 - 1.0) + \
    0.5*(x4_8_5 - 1.0)*(x4_8_5 - 1.0) + 0.5*(y4_8_5 - 1.0)*(y4_8_5 - 1.0) + \
    0.5*(z4_8_5 - 1.0)*(z4_8_5 - 1.0) + 0.5*(x5_8_5 - 1.0)*(x5_8_5 - 1.0) + \
    0.5*(y5_8_5 - 1.0)*(y5_8_5 - 1.0) + 0.5*(z5_8_5 - 1.0)*(z5_8_5 - 1.0) + \
    0.5*(x6_8_5 - 1.0)*(x6_8_5 - 1.0) + 0.5*(y6_8_5 - 1.0)*(y6_8_5 - 1.0) + \
    0.5*(z6_8_5 - 1.0)*(z6_8_5 - 1.0) + 0.5*(x7_8_5 - 1.0)*(x7_8_5 - 1.0) + \
    0.5*(y7_8_5 - 1.0)*(y7_8_5 - 1.0) + 0.5*(z7_8_5 - 1.0)*(z7_8_5 - 1.0) + \
    0.5*(x8_8_5 - 1.0)*(x8_8_5 - 1.0) + 0.5*(y8_8_5 - 1.0)*(y8_8_5 - 1.0) + \
    0.5*(z8_8_5 - 1.0)*(z8_8_5 - 1.0) + 0.5*(x9_8_5 - 1.0)*(x9_8_5 - 1.0) + \
    0.5*(y9_8_5 - 1.0)*(y9_8_5 - 1.0) + 0.5*(z9_8_5 - 1.0)*(z9_8_5 - 1.0) + \
    0.5*(x1_9_5 - 1.0)*(x1_9_5 - 1.0) + 0.5*(y1_9_5 - 1.0)*(y1_9_5 - 1.0) + \
    0.5*(z1_9_5 - 1.0)*(z1_9_5 - 1.0) + 0.5*(x2_9_5 - 1.0)*(x2_9_5 - 1.0) + \
    0.5*(y2_9_5 - 1.0)*(y2_9_5 - 1.0) + 0.5*(z2_9_5 - 1.0)*(z2_9_5 - 1.0) + \
    0.5*(x3_9_5 - 1.0)*(x3_9_5 - 1.0) + 0.5*(y3_9_5 - 1.0)*(y3_9_5 - 1.0) + \
    0.5*(z3_9_5 - 1.0)*(z3_9_5 - 1.0) + 0.5*(x4_9_5 - 1.0)*(x4_9_5 - 1.0) + \
    0.5*(y4_9_5 - 1.0)*(y4_9_5 - 1.0) + 0.5*(z4_9_5 - 1.0)*(z4_9_5 - 1.0) + \
    0.5*(x5_9_5 - 1.0)*(x5_9_5 - 1.0) + 0.5*(y5_9_5 - 1.0)*(y5_9_5 - 1.0) + \
    0.5*(z5_9_5 - 1.0)*(z5_9_5 - 1.0) + 0.5*(x6_9_5 - 1.0)*(x6_9_5 - 1.0) + \
    0.5*(y6_9_5 - 1.0)*(y6_9_5 - 1.0) + 0.5*(z6_9_5 - 1.0)*(z6_9_5 - 1.0) + \
    0.5*(x7_9_5 - 1.0)*(x7_9_5 - 1.0) + 0.5*(y7_9_5 - 1.0)*(y7_9_5 - 1.0) + \
    0.5*(z7_9_5 - 1.0)*(z7_9_5 - 1.0) + 0.5*(x8_9_5 - 1.0)*(x8_9_5 - 1.0) + \
    0.5*(y8_9_5 - 1.0)*(y8_9_5 - 1.0) + 0.5*(z8_9_5 - 1.0)*(z8_9_5 - 1.0) + \
    0.5*(x9_9_5 - 1.0)*(x9_9_5 - 1.0) + 0.5*(y9_9_5 - 1.0)*(y9_9_5 - 1.0) + \
    0.5*(z9_9_5 - 1.0)*(z9_9_5 - 1.0) + 0.5*(x1_1_6 - 1.0)*(x1_1_6 - 1.0) + \
    0.5*(y1_1_6 - 1.0)*(y1_1_6 - 1.0) + 0.5*(z1_1_6 - 1.0)*(z1_1_6 - 1.0) + \
    0.5*(x2_1_6 - 1.0)*(x2_1_6 - 1.0) + 0.5*(y2_1_6 - 1.0)*(y2_1_6 - 1.0) + \
    0.5*(z2_1_6 - 1.0)*(z2_1_6 - 1.0) + 0.5*(x3_1_6 - 1.0)*(x3_1_6 - 1.0) + \
    0.5*(y3_1_6 - 1.0)*(y3_1_6 - 1.0) + 0.5*(z3_1_6 - 1.0)*(z3_1_6 - 1.0) + \
    0.5*(x4_1_6 - 1.0)*(x4_1_6 - 1.0) + 0.5*(y4_1_6 - 1.0)*(y4_1_6 - 1.0) + \
    0.5*(z4_1_6 - 1.0)*(z4_1_6 - 1.0) + 0.5*(x5_1_6 - 1.0)*(x5_1_6 - 1.0) + \
    0.5*(y5_1_6 - 1.0)*(y5_1_6 - 1.0) + 0.5*(z5_1_6 - 1.0)*(z5_1_6 - 1.0) + \
    0.5*(x6_1_6 - 1.0)*(x6_1_6 - 1.0) + 0.5*(y6_1_6 - 1.0)*(y6_1_6 - 1.0) + \
    0.5*(z6_1_6 - 1.0)*(z6_1_6 - 1.0) + 0.5*(x7_1_6 - 1.0)*(x7_1_6 - 1.0) + \
    0.5*(y7_1_6 - 1.0)*(y7_1_6 - 1.0) + 0.5*(z7_1_6 - 1.0)*(z7_1_6 - 1.0) + \
    0.5*(x8_1_6 - 1.0)*(x8_1_6 - 1.0) + 0.5*(y8_1_6 - 1.0)*(y8_1_6 - 1.0) + \
    0.5*(z8_1_6 - 1.0)*(z8_1_6 - 1.0) + 0.5*(x9_1_6 - 1.0)*(x9_1_6 - 1.0) + \
    0.5*(y9_1_6 - 1.0)*(y9_1_6 - 1.0) + 0.5*(z9_1_6 - 1.0)*(z9_1_6 - 1.0) + \
    0.5*(x1_2_6 - 1.0)*(x1_2_6 - 1.0) + 0.5*(y1_2_6 - 1.0)*(y1_2_6 - 1.0) + \
    0.5*(z1_2_6 - 1.0)*(z1_2_6 - 1.0) + 0.5*(x2_2_6 - 1.0)*(x2_2_6 - 1.0) + \
    0.5*(y2_2_6 - 1.0)*(y2_2_6 - 1.0) + 0.5*(z2_2_6 - 1.0)*(z2_2_6 - 1.0) + \
    0.5*(x3_2_6 - 1.0)*(x3_2_6 - 1.0) + 0.5*(y3_2_6 - 1.0)*(y3_2_6 - 1.0) + \
    0.5*(z3_2_6 - 1.0)*(z3_2_6 - 1.0) + 0.5*(x4_2_6 - 1.0)*(x4_2_6 - 1.0) + \
    0.5*(y4_2_6 - 1.0)*(y4_2_6 - 1.0) + 0.5*(z4_2_6 - 1.0)*(z4_2_6 - 1.0) + \
    0.5*(x5_2_6 - 1.0)*(x5_2_6 - 1.0) + 0.5*(y5_2_6 - 1.0)*(y5_2_6 - 1.0) + \
    0.5*(z5_2_6 - 1.0)*(z5_2_6 - 1.0) + 0.5*(x6_2_6 - 1.0)*(x6_2_6 - 1.0) + \
    0.5*(y6_2_6 - 1.0)*(y6_2_6 - 1.0) + 0.5*(z6_2_6 - 1.0)*(z6_2_6 - 1.0) + \
    0.5*(x7_2_6 - 1.0)*(x7_2_6 - 1.0) + 0.5*(y7_2_6 - 1.0)*(y7_2_6 - 1.0) + \
    0.5*(z7_2_6 - 1.0)*(z7_2_6 - 1.0) + 0.5*(x8_2_6 - 1.0)*(x8_2_6 - 1.0) + \
    0.5*(y8_2_6 - 1.0)*(y8_2_6 - 1.0) + 0.5*(z8_2_6 - 1.0)*(z8_2_6 - 1.0) + \
    0.5*(x9_2_6 - 1.0)*(x9_2_6 - 1.0) + 0.5*(y9_2_6 - 1.0)*(y9_2_6 - 1.0) + \
    0.5*(z9_2_6 - 1.0)*(z9_2_6 - 1.0) + 0.5*(x1_3_6 - 1.0)*(x1_3_6 - 1.0) + \
    0.5*(y1_3_6 - 1.0)*(y1_3_6 - 1.0) + 0.5*(z1_3_6 - 1.0)*(z1_3_6 - 1.0) + \
    0.5*(x2_3_6 - 1.0)*(x2_3_6 - 1.0) + 0.5*(y2_3_6 - 1.0)*(y2_3_6 - 1.0) + \
    0.5*(z2_3_6 - 1.0)*(z2_3_6 - 1.0) + 0.5*(x3_3_6 - 1.0)*(x3_3_6 - 1.0) + \
    0.5*(y3_3_6 - 1.0)*(y3_3_6 - 1.0) + 0.5*(z3_3_6 - 1.0)*(z3_3_6 - 1.0) + \
    0.5*(x4_3_6 - 1.0)*(x4_3_6 - 1.0) + 0.5*(y4_3_6 - 1.0)*(y4_3_6 - 1.0) + \
    0.5*(z4_3_6 - 1.0)*(z4_3_6 - 1.0) + 0.5*(x5_3_6 - 1.0)*(x5_3_6 - 1.0) + \
    0.5*(y5_3_6 - 1.0)*(y5_3_6 - 1.0) + 0.5*(z5_3_6 - 1.0)*(z5_3_6 - 1.0) + \
    0.5*(x6_3_6 - 1.0)*(x6_3_6 - 1.0) + 0.5*(y6_3_6 - 1.0)*(y6_3_6 - 1.0) + \
    0.5*(z6_3_6 - 1.0)*(z6_3_6 - 1.0) + 0.5*(x7_3_6 - 1.0)*(x7_3_6 - 1.0) + \
    0.5*(y7_3_6 - 1.0)*(y7_3_6 - 1.0) + 0.5*(z7_3_6 - 1.0)*(z7_3_6 - 1.0) + \
    0.5*(x8_3_6 - 1.0)*(x8_3_6 - 1.0) + 0.5*(y8_3_6 - 1.0)*(y8_3_6 - 1.0) + \
    0.5*(z8_3_6 - 1.0)*(z8_3_6 - 1.0) + 0.5*(x9_3_6 - 1.0)*(x9_3_6 - 1.0) + \
    0.5*(y9_3_6 - 1.0)*(y9_3_6 - 1.0) + 0.5*(z9_3_6 - 1.0)*(z9_3_6 - 1.0) + \
    0.5*(x1_4_6 - 1.0)*(x1_4_6 - 1.0) + 0.5*(y1_4_6 - 1.0)*(y1_4_6 - 1.0) + \
    0.5*(z1_4_6 - 1.0)*(z1_4_6 - 1.0) + 0.5*(x2_4_6 - 1.0)*(x2_4_6 - 1.0) + \
    0.5*(y2_4_6 - 1.0)*(y2_4_6 - 1.0) + 0.5*(z2_4_6 - 1.0)*(z2_4_6 - 1.0) + \
    0.5*(x3_4_6 - 1.0)*(x3_4_6 - 1.0) + 0.5*(y3_4_6 - 1.0)*(y3_4_6 - 1.0) + \
    0.5*(z3_4_6 - 1.0)*(z3_4_6 - 1.0) + 0.5*(x4_4_6 - 1.0)*(x4_4_6 - 1.0) + \
    0.5*(y4_4_6 - 1.0)*(y4_4_6 - 1.0) + 0.5*(z4_4_6 - 1.0)*(z4_4_6 - 1.0) + \
    0.5*(x5_4_6 - 1.0)*(x5_4_6 - 1.0) + 0.5*(y5_4_6 - 1.0)*(y5_4_6 - 1.0) + \
    0.5*(z5_4_6 - 1.0)*(z5_4_6 - 1.0) + 0.5*(x6_4_6 - 1.0)*(x6_4_6 - 1.0) + \
    0.5*(y6_4_6 - 1.0)*(y6_4_6 - 1.0) + 0.5*(z6_4_6 - 1.0)*(z6_4_6 - 1.0) + \
    0.5*(x7_4_6 - 1.0)*(x7_4_6 - 1.0) + 0.5*(y7_4_6 - 1.0)*(y7_4_6 - 1.0) + \
    0.5*(z7_4_6 - 1.0)*(z7_4_6 - 1.0) + 0.5*(x8_4_6 - 1.0)*(x8_4_6 - 1.0) + \
    0.5*(y8_4_6 - 1.0)*(y8_4_6 - 1.0) + 0.5*(z8_4_6 - 1.0)*(z8_4_6 - 1.0) + \
    0.5*(x9_4_6 - 1.0)*(x9_4_6 - 1.0) + 0.5*(y9_4_6 - 1.0)*(y9_4_6 - 1.0) + \
    0.5*(z9_4_6 - 1.0)*(z9_4_6 - 1.0) + 0.5*(x1_5_6 - 1.0)*(x1_5_6 - 1.0) + \
    0.5*(y1_5_6 - 1.0)*(y1_5_6 - 1.0) + 0.5*(z1_5_6 - 1.0)*(z1_5_6 - 1.0) + \
    0.5*(x2_5_6 - 1.0)*(x2_5_6 - 1.0) + 0.5*(y2_5_6 - 1.0)*(y2_5_6 - 1.0) + \
    0.5*(z2_5_6 - 1.0)*(z2_5_6 - 1.0) + 0.5*(x3_5_6 - 1.0)*(x3_5_6 - 1.0) + \
    0.5*(y3_5_6 - 1.0)*(y3_5_6 - 1.0) + 0.5*(z3_5_6 - 1.0)*(z3_5_6 - 1.0) + \
    0.5*(x4_5_6 - 1.0)*(x4_5_6 - 1.0) + 0.5*(y4_5_6 - 1.0)*(y4_5_6 - 1.0) + \
    0.5*(z4_5_6 - 1.0)*(z4_5_6 - 1.0) + 0.5*(x5_5_6 - 1.0)*(x5_5_6 - 1.0) + \
    0.5*(y5_5_6 - 1.0)*(y5_5_6 - 1.0) + 0.5*(z5_5_6 - 1.0)*(z5_5_6 - 1.0) + \
    0.5*(x6_5_6 - 1.0)*(x6_5_6 - 1.0) + 0.5*(y6_5_6 - 1.0)*(y6_5_6 - 1.0) + \
    0.5*(z6_5_6 - 1.0)*(z6_5_6 - 1.0) + 0.5*(x7_5_6 - 1.0)*(x7_5_6 - 1.0) + \
    0.5*(y7_5_6 - 1.0)*(y7_5_6 - 1.0) + 0.5*(z7_5_6 - 1.0)*(z7_5_6 - 1.0) + \
    0.5*(x8_5_6 - 1.0)*(x8_5_6 - 1.0) + 0.5*(y8_5_6 - 1.0)*(y8_5_6 - 1.0) + \
    0.5*(z8_5_6 - 1.0)*(z8_5_6 - 1.0) + 0.5*(x9_5_6 - 1.0)*(x9_5_6 - 1.0) + \
    0.5*(y9_5_6 - 1.0)*(y9_5_6 - 1.0) + 0.5*(z9_5_6 - 1.0)*(z9_5_6 - 1.0) + \
    0.5*(x1_6_6 - 1.0)*(x1_6_6 - 1.0) + 0.5*(y1_6_6 - 1.0)*(y1_6_6 - 1.0) + \
    0.5*(z1_6_6 - 1.0)*(z1_6_6 - 1.0) + 0.5*(x2_6_6 - 1.0)*(x2_6_6 - 1.0) + \
    0.5*(y2_6_6 - 1.0)*(y2_6_6 - 1.0) + 0.5*(z2_6_6 - 1.0)*(z2_6_6 - 1.0) + \
    0.5*(x3_6_6 - 1.0)*(x3_6_6 - 1.0) + 0.5*(y3_6_6 - 1.0)*(y3_6_6 - 1.0) + \
    0.5*(z3_6_6 - 1.0)*(z3_6_6 - 1.0) + 0.5*(x4_6_6 - 1.0)*(x4_6_6 - 1.0) + \
    0.5*(y4_6_6 - 1.0)*(y4_6_6 - 1.0) + 0.5*(z4_6_6 - 1.0)*(z4_6_6 - 1.0) + \
    0.5*(x5_6_6 - 1.0)*(x5_6_6 - 1.0) + 0.5*(y5_6_6 - 1.0)*(y5_6_6 - 1.0) + \
    0.5*(z5_6_6 - 1.0)*(z5_6_6 - 1.0) + 0.5*(x6_6_6 - 1.0)*(x6_6_6 - 1.0) + \
    0.5*(y6_6_6 - 1.0)*(y6_6_6 - 1.0) + 0.5*(z6_6_6 - 1.0)*(z6_6_6 - 1.0) + \
    0.5*(x7_6_6 - 1.0)*(x7_6_6 - 1.0) + 0.5*(y7_6_6 - 1.0)*(y7_6_6 - 1.0) + \
    0.5*(z7_6_6 - 1.0)*(z7_6_6 - 1.0) + 0.5*(x8_6_6 - 1.0)*(x8_6_6 - 1.0) + \
    0.5*(y8_6_6 - 1.0)*(y8_6_6 - 1.0) + 0.5*(z8_6_6 - 1.0)*(z8_6_6 - 1.0) + \
    0.5*(x9_6_6 - 1.0)*(x9_6_6 - 1.0) + 0.5*(y9_6_6 - 1.0)*(y9_6_6 - 1.0) + \
    0.5*(z9_6_6 - 1.0)*(z9_6_6 - 1.0) + 0.5*(x1_7_6 - 1.0)*(x1_7_6 - 1.0) + \
    0.5*(y1_7_6 - 1.0)*(y1_7_6 - 1.0) + 0.5*(z1_7_6 - 1.0)*(z1_7_6 - 1.0) + \
    0.5*(x2_7_6 - 1.0)*(x2_7_6 - 1.0) + 0.5*(y2_7_6 - 1.0)*(y2_7_6 - 1.0) + \
    0.5*(z2_7_6 - 1.0)*(z2_7_6 - 1.0) + 0.5*(x3_7_6 - 1.0)*(x3_7_6 - 1.0) + \
    0.5*(y3_7_6 - 1.0)*(y3_7_6 - 1.0) + 0.5*(z3_7_6 - 1.0)*(z3_7_6 - 1.0) + \
    0.5*(x4_7_6 - 1.0)*(x4_7_6 - 1.0) + 0.5*(y4_7_6 - 1.0)*(y4_7_6 - 1.0) + \
    0.5*(z4_7_6 - 1.0)*(z4_7_6 - 1.0) + 0.5*(x5_7_6 - 1.0)*(x5_7_6 - 1.0) + \
    0.5*(y5_7_6 - 1.0)*(y5_7_6 - 1.0) + 0.5*(z5_7_6 - 1.0)*(z5_7_6 - 1.0) + \
    0.5*(x6_7_6 - 1.0)*(x6_7_6 - 1.0) + 0.5*(y6_7_6 - 1.0)*(y6_7_6 - 1.0) + \
    0.5*(z6_7_6 - 1.0)*(z6_7_6 - 1.0) + 0.5*(x7_7_6 - 1.0)*(x7_7_6 - 1.0) + \
    0.5*(y7_7_6 - 1.0)*(y7_7_6 - 1.0) + 0.5*(z7_7_6 - 1.0)*(z7_7_6 - 1.0) + \
    0.5*(x8_7_6 - 1.0)*(x8_7_6 - 1.0) + 0.5*(y8_7_6 - 1.0)*(y8_7_6 - 1.0) + \
    0.5*(z8_7_6 - 1.0)*(z8_7_6 - 1.0) + 0.5*(x9_7_6 - 1.0)*(x9_7_6 - 1.0) + \
    0.5*(y9_7_6 - 1.0)*(y9_7_6 - 1.0) + 0.5*(z9_7_6 - 1.0)*(z9_7_6 - 1.0) + \
    0.5*(x1_8_6 - 1.0)*(x1_8_6 - 1.0) + 0.5*(y1_8_6 - 1.0)*(y1_8_6 - 1.0) + \
    0.5*(z1_8_6 - 1.0)*(z1_8_6 - 1.0) + 0.5*(x2_8_6 - 1.0)*(x2_8_6 - 1.0) + \
    0.5*(y2_8_6 - 1.0)*(y2_8_6 - 1.0) + 0.5*(z2_8_6 - 1.0)*(z2_8_6 - 1.0) + \
    0.5*(x3_8_6 - 1.0)*(x3_8_6 - 1.0) + 0.5*(y3_8_6 - 1.0)*(y3_8_6 - 1.0) + \
    0.5*(z3_8_6 - 1.0)*(z3_8_6 - 1.0) + 0.5*(x4_8_6 - 1.0)*(x4_8_6 - 1.0) + \
    0.5*(y4_8_6 - 1.0)*(y4_8_6 - 1.0) + 0.5*(z4_8_6 - 1.0)*(z4_8_6 - 1.0) + \
    0.5*(x5_8_6 - 1.0)*(x5_8_6 - 1.0) + 0.5*(y5_8_6 - 1.0)*(y5_8_6 - 1.0) + \
    0.5*(z5_8_6 - 1.0)*(z5_8_6 - 1.0) + 0.5*(x6_8_6 - 1.0)*(x6_8_6 - 1.0) + \
    0.5*(y6_8_6 - 1.0)*(y6_8_6 - 1.0) + 0.5*(z6_8_6 - 1.0)*(z6_8_6 - 1.0) + \
    0.5*(x7_8_6 - 1.0)*(x7_8_6 - 1.0) + 0.5*(y7_8_6 - 1.0)*(y7_8_6 - 1.0) + \
    0.5*(z7_8_6 - 1.0)*(z7_8_6 - 1.0) + 0.5*(x8_8_6 - 1.0)*(x8_8_6 - 1.0) + \
    0.5*(y8_8_6 - 1.0)*(y8_8_6 - 1.0) + 0.5*(z8_8_6 - 1.0)*(z8_8_6 - 1.0) + \
    0.5*(x9_8_6 - 1.0)*(x9_8_6 - 1.0) + 0.5*(y9_8_6 - 1.0)*(y9_8_6 - 1.0) + \
    0.5*(z9_8_6 - 1.0)*(z9_8_6 - 1.0) + 0.5*(x1_9_6 - 1.0)*(x1_9_6 - 1.0) + \
    0.5*(y1_9_6 - 1.0)*(y1_9_6 - 1.0) + 0.5*(z1_9_6 - 1.0)*(z1_9_6 - 1.0) + \
    0.5*(x2_9_6 - 1.0)*(x2_9_6 - 1.0) + 0.5*(y2_9_6 - 1.0)*(y2_9_6 - 1.0) + \
    0.5*(z2_9_6 - 1.0)*(z2_9_6 - 1.0) + 0.5*(x3_9_6 - 1.0)*(x3_9_6 - 1.0) + \
    0.5*(y3_9_6 - 1.0)*(y3_9_6 - 1.0) + 0.5*(z3_9_6 - 1.0)*(z3_9_6 - 1.0) + \
    0.5*(x4_9_6 - 1.0)*(x4_9_6 - 1.0) + 0.5*(y4_9_6 - 1.0)*(y4_9_6 - 1.0) + \
    0.5*(z4_9_6 - 1.0)*(z4_9_6 - 1.0) + 0.5*(x5_9_6 - 1.0)*(x5_9_6 - 1.0) + \
    0.5*(y5_9_6 - 1.0)*(y5_9_6 - 1.0) + 0.5*(z5_9_6 - 1.0)*(z5_9_6 - 1.0) + \
    0.5*(x6_9_6 - 1.0)*(x6_9_6 - 1.0) + 0.5*(y6_9_6 - 1.0)*(y6_9_6 - 1.0) + \
    0.5*(z6_9_6 - 1.0)*(z6_9_6 - 1.0) + 0.5*(x7_9_6 - 1.0)*(x7_9_6 - 1.0) + \
    0.5*(y7_9_6 - 1.0)*(y7_9_6 - 1.0) + 0.5*(z7_9_6 - 1.0)*(z7_9_6 - 1.0) + \
    0.5*(x8_9_6 - 1.0)*(x8_9_6 - 1.0) + 0.5*(y8_9_6 - 1.0)*(y8_9_6 - 1.0) + \
    0.5*(z8_9_6 - 1.0)*(z8_9_6 - 1.0) + 0.5*(x9_9_6 - 1.0)*(x9_9_6 - 1.0) + \
    0.5*(y9_9_6 - 1.0)*(y9_9_6 - 1.0) + 0.5*(z9_9_6 - 1.0)*(z9_9_6 - 1.0) + \
    0.5*(x1_1_7 - 1.0)*(x1_1_7 - 1.0) + 0.5*(y1_1_7 - 1.0)*(y1_1_7 - 1.0) + \
    0.5*(z1_1_7 - 1.0)*(z1_1_7 - 1.0) + 0.5*(x2_1_7 - 1.0)*(x2_1_7 - 1.0) + \
    0.5*(y2_1_7 - 1.0)*(y2_1_7 - 1.0) + 0.5*(z2_1_7 - 1.0)*(z2_1_7 - 1.0) + \
    0.5*(x3_1_7 - 1.0)*(x3_1_7 - 1.0) + 0.5*(y3_1_7 - 1.0)*(y3_1_7 - 1.0) + \
    0.5*(z3_1_7 - 1.0)*(z3_1_7 - 1.0) + 0.5*(x4_1_7 - 1.0)*(x4_1_7 - 1.0) + \
    0.5*(y4_1_7 - 1.0)*(y4_1_7 - 1.0) + 0.5*(z4_1_7 - 1.0)*(z4_1_7 - 1.0) + \
    0.5*(x5_1_7 - 1.0)*(x5_1_7 - 1.0) + 0.5*(y5_1_7 - 1.0)*(y5_1_7 - 1.0) + \
    0.5*(z5_1_7 - 1.0)*(z5_1_7 - 1.0) + 0.5*(x6_1_7 - 1.0)*(x6_1_7 - 1.0) + \
    0.5*(y6_1_7 - 1.0)*(y6_1_7 - 1.0) + 0.5*(z6_1_7 - 1.0)*(z6_1_7 - 1.0) + \
    0.5*(x7_1_7 - 1.0)*(x7_1_7 - 1.0) + 0.5*(y7_1_7 - 1.0)*(y7_1_7 - 1.0) + \
    0.5*(z7_1_7 - 1.0)*(z7_1_7 - 1.0) + 0.5*(x8_1_7 - 1.0)*(x8_1_7 - 1.0) + \
    0.5*(y8_1_7 - 1.0)*(y8_1_7 - 1.0) + 0.5*(z8_1_7 - 1.0)*(z8_1_7 - 1.0) + \
    0.5*(x9_1_7 - 1.0)*(x9_1_7 - 1.0) + 0.5*(y9_1_7 - 1.0)*(y9_1_7 - 1.0) + \
    0.5*(z9_1_7 - 1.0)*(z9_1_7 - 1.0) + 0.5*(x1_2_7 - 1.0)*(x1_2_7 - 1.0) + \
    0.5*(y1_2_7 - 1.0)*(y1_2_7 - 1.0) + 0.5*(z1_2_7 - 1.0)*(z1_2_7 - 1.0) + \
    0.5*(x2_2_7 - 1.0)*(x2_2_7 - 1.0) + 0.5*(y2_2_7 - 1.0)*(y2_2_7 - 1.0) + \
    0.5*(z2_2_7 - 1.0)*(z2_2_7 - 1.0) + 0.5*(x3_2_7 - 1.0)*(x3_2_7 - 1.0) + \
    0.5*(y3_2_7 - 1.0)*(y3_2_7 - 1.0) + 0.5*(z3_2_7 - 1.0)*(z3_2_7 - 1.0) + \
    0.5*(x4_2_7 - 1.0)*(x4_2_7 - 1.0) + 0.5*(y4_2_7 - 1.0)*(y4_2_7 - 1.0) + \
    0.5*(z4_2_7 - 1.0)*(z4_2_7 - 1.0) + 0.5*(x5_2_7 - 1.0)*(x5_2_7 - 1.0) + \
    0.5*(y5_2_7 - 1.0)*(y5_2_7 - 1.0) + 0.5*(z5_2_7 - 1.0)*(z5_2_7 - 1.0) + \
    0.5*(x6_2_7 - 1.0)*(x6_2_7 - 1.0) + 0.5*(y6_2_7 - 1.0)*(y6_2_7 - 1.0) + \
    0.5*(z6_2_7 - 1.0)*(z6_2_7 - 1.0) + 0.5*(x7_2_7 - 1.0)*(x7_2_7 - 1.0) + \
    0.5*(y7_2_7 - 1.0)*(y7_2_7 - 1.0) + 0.5*(z7_2_7 - 1.0)*(z7_2_7 - 1.0) + \
    0.5*(x8_2_7 - 1.0)*(x8_2_7 - 1.0) + 0.5*(y8_2_7 - 1.0)*(y8_2_7 - 1.0) + \
    0.5*(z8_2_7 - 1.0)*(z8_2_7 - 1.0) + 0.5*(x9_2_7 - 1.0)*(x9_2_7 - 1.0) + \
    0.5*(y9_2_7 - 1.0)*(y9_2_7 - 1.0) + 0.5*(z9_2_7 - 1.0)*(z9_2_7 - 1.0) + \
    0.5*(x1_3_7 - 1.0)*(x1_3_7 - 1.0) + 0.5*(y1_3_7 - 1.0)*(y1_3_7 - 1.0) + \
    0.5*(z1_3_7 - 1.0)*(z1_3_7 - 1.0) + 0.5*(x2_3_7 - 1.0)*(x2_3_7 - 1.0) + \
    0.5*(y2_3_7 - 1.0)*(y2_3_7 - 1.0) + 0.5*(z2_3_7 - 1.0)*(z2_3_7 - 1.0) + \
    0.5*(x3_3_7 - 1.0)*(x3_3_7 - 1.0) + 0.5*(y3_3_7 - 1.0)*(y3_3_7 - 1.0) + \
    0.5*(z3_3_7 - 1.0)*(z3_3_7 - 1.0) + 0.5*(x4_3_7 - 1.0)*(x4_3_7 - 1.0) + \
    0.5*(y4_3_7 - 1.0)*(y4_3_7 - 1.0) + 0.5*(z4_3_7 - 1.0)*(z4_3_7 - 1.0) + \
    0.5*(x5_3_7 - 1.0)*(x5_3_7 - 1.0) + 0.5*(y5_3_7 - 1.0)*(y5_3_7 - 1.0) + \
    0.5*(z5_3_7 - 1.0)*(z5_3_7 - 1.0) + 0.5*(x6_3_7 - 1.0)*(x6_3_7 - 1.0) + \
    0.5*(y6_3_7 - 1.0)*(y6_3_7 - 1.0) + 0.5*(z6_3_7 - 1.0)*(z6_3_7 - 1.0) + \
    0.5*(x7_3_7 - 1.0)*(x7_3_7 - 1.0) + 0.5*(y7_3_7 - 1.0)*(y7_3_7 - 1.0) + \
    0.5*(z7_3_7 - 1.0)*(z7_3_7 - 1.0) + 0.5*(x8_3_7 - 1.0)*(x8_3_7 - 1.0) + \
    0.5*(y8_3_7 - 1.0)*(y8_3_7 - 1.0) + 0.5*(z8_3_7 - 1.0)*(z8_3_7 - 1.0) + \
    0.5*(x9_3_7 - 1.0)*(x9_3_7 - 1.0) + 0.5*(y9_3_7 - 1.0)*(y9_3_7 - 1.0) + \
    0.5*(z9_3_7 - 1.0)*(z9_3_7 - 1.0) + 0.5*(x1_4_7 - 1.0)*(x1_4_7 - 1.0) + \
    0.5*(y1_4_7 - 1.0)*(y1_4_7 - 1.0) + 0.5*(z1_4_7 - 1.0)*(z1_4_7 - 1.0) + \
    0.5*(x2_4_7 - 1.0)*(x2_4_7 - 1.0) + 0.5*(y2_4_7 - 1.0)*(y2_4_7 - 1.0) + \
    0.5*(z2_4_7 - 1.0)*(z2_4_7 - 1.0) + 0.5*(x3_4_7 - 1.0)*(x3_4_7 - 1.0) + \
    0.5*(y3_4_7 - 1.0)*(y3_4_7 - 1.0) + 0.5*(z3_4_7 - 1.0)*(z3_4_7 - 1.0) + \
    0.5*(x4_4_7 - 1.0)*(x4_4_7 - 1.0) + 0.5*(y4_4_7 - 1.0)*(y4_4_7 - 1.0) + \
    0.5*(z4_4_7 - 1.0)*(z4_4_7 - 1.0) + 0.5*(x5_4_7 - 1.0)*(x5_4_7 - 1.0) + \
    0.5*(y5_4_7 - 1.0)*(y5_4_7 - 1.0) + 0.5*(z5_4_7 - 1.0)*(z5_4_7 - 1.0) + \
    0.5*(x6_4_7 - 1.0)*(x6_4_7 - 1.0) + 0.5*(y6_4_7 - 1.0)*(y6_4_7 - 1.0) + \
    0.5*(z6_4_7 - 1.0)*(z6_4_7 - 1.0) + 0.5*(x7_4_7 - 1.0)*(x7_4_7 - 1.0) + \
    0.5*(y7_4_7 - 1.0)*(y7_4_7 - 1.0) + 0.5*(z7_4_7 - 1.0)*(z7_4_7 - 1.0) + \
    0.5*(x8_4_7 - 1.0)*(x8_4_7 - 1.0) + 0.5*(y8_4_7 - 1.0)*(y8_4_7 - 1.0) + \
    0.5*(z8_4_7 - 1.0)*(z8_4_7 - 1.0) + 0.5*(x9_4_7 - 1.0)*(x9_4_7 - 1.0) + \
    0.5*(y9_4_7 - 1.0)*(y9_4_7 - 1.0) + 0.5*(z9_4_7 - 1.0)*(z9_4_7 - 1.0) + \
    0.5*(x1_5_7 - 1.0)*(x1_5_7 - 1.0) + 0.5*(y1_5_7 - 1.0)*(y1_5_7 - 1.0) + \
    0.5*(z1_5_7 - 1.0)*(z1_5_7 - 1.0) + 0.5*(x2_5_7 - 1.0)*(x2_5_7 - 1.0) + \
    0.5*(y2_5_7 - 1.0)*(y2_5_7 - 1.0) + 0.5*(z2_5_7 - 1.0)*(z2_5_7 - 1.0) + \
    0.5*(x3_5_7 - 1.0)*(x3_5_7 - 1.0) + 0.5*(y3_5_7 - 1.0)*(y3_5_7 - 1.0) + \
    0.5*(z3_5_7 - 1.0)*(z3_5_7 - 1.0) + 0.5*(x4_5_7 - 1.0)*(x4_5_7 - 1.0) + \
    0.5*(y4_5_7 - 1.0)*(y4_5_7 - 1.0) + 0.5*(z4_5_7 - 1.0)*(z4_5_7 - 1.0) + \
    0.5*(x5_5_7 - 1.0)*(x5_5_7 - 1.0) + 0.5*(y5_5_7 - 1.0)*(y5_5_7 - 1.0) + \
    0.5*(z5_5_7 - 1.0)*(z5_5_7 - 1.0) + 0.5*(x6_5_7 - 1.0)*(x6_5_7 - 1.0) + \
    0.5*(y6_5_7 - 1.0)*(y6_5_7 - 1.0) + 0.5*(z6_5_7 - 1.0)*(z6_5_7 - 1.0) + \
    0.5*(x7_5_7 - 1.0)*(x7_5_7 - 1.0) + 0.5*(y7_5_7 - 1.0)*(y7_5_7 - 1.0) + \
    0.5*(z7_5_7 - 1.0)*(z7_5_7 - 1.0) + 0.5*(x8_5_7 - 1.0)*(x8_5_7 - 1.0) + \
    0.5*(y8_5_7 - 1.0)*(y8_5_7 - 1.0) + 0.5*(z8_5_7 - 1.0)*(z8_5_7 - 1.0) + \
    0.5*(x9_5_7 - 1.0)*(x9_5_7 - 1.0) + 0.5*(y9_5_7 - 1.0)*(y9_5_7 - 1.0) + \
    0.5*(z9_5_7 - 1.0)*(z9_5_7 - 1.0) + 0.5*(x1_6_7 - 1.0)*(x1_6_7 - 1.0) + \
    0.5*(y1_6_7 - 1.0)*(y1_6_7 - 1.0) + 0.5*(z1_6_7 - 1.0)*(z1_6_7 - 1.0) + \
    0.5*(x2_6_7 - 1.0)*(x2_6_7 - 1.0) + 0.5*(y2_6_7 - 1.0)*(y2_6_7 - 1.0) + \
    0.5*(z2_6_7 - 1.0)*(z2_6_7 - 1.0) + 0.5*(x3_6_7 - 1.0)*(x3_6_7 - 1.0) + \
    0.5*(y3_6_7 - 1.0)*(y3_6_7 - 1.0) + 0.5*(z3_6_7 - 1.0)*(z3_6_7 - 1.0) + \
    0.5*(x4_6_7 - 1.0)*(x4_6_7 - 1.0) + 0.5*(y4_6_7 - 1.0)*(y4_6_7 - 1.0) + \
    0.5*(z4_6_7 - 1.0)*(z4_6_7 - 1.0) + 0.5*(x5_6_7 - 1.0)*(x5_6_7 - 1.0) + \
    0.5*(y5_6_7 - 1.0)*(y5_6_7 - 1.0) + 0.5*(z5_6_7 - 1.0)*(z5_6_7 - 1.0) + \
    0.5*(x6_6_7 - 1.0)*(x6_6_7 - 1.0) + 0.5*(y6_6_7 - 1.0)*(y6_6_7 - 1.0) + \
    0.5*(z6_6_7 - 1.0)*(z6_6_7 - 1.0) + 0.5*(x7_6_7 - 1.0)*(x7_6_7 - 1.0) + \
    0.5*(y7_6_7 - 1.0)*(y7_6_7 - 1.0) + 0.5*(z7_6_7 - 1.0)*(z7_6_7 - 1.0) + \
    0.5*(x8_6_7 - 1.0)*(x8_6_7 - 1.0) + 0.5*(y8_6_7 - 1.0)*(y8_6_7 - 1.0) + \
    0.5*(z8_6_7 - 1.0)*(z8_6_7 - 1.0) + 0.5*(x9_6_7 - 1.0)*(x9_6_7 - 1.0) + \
    0.5*(y9_6_7 - 1.0)*(y9_6_7 - 1.0) + 0.5*(z9_6_7 - 1.0)*(z9_6_7 - 1.0) + \
    0.5*(x1_7_7 - 1.0)*(x1_7_7 - 1.0) + 0.5*(y1_7_7 - 1.0)*(y1_7_7 - 1.0) + \
    0.5*(z1_7_7 - 1.0)*(z1_7_7 - 1.0) + 0.5*(x2_7_7 - 1.0)*(x2_7_7 - 1.0) + \
    0.5*(y2_7_7 - 1.0)*(y2_7_7 - 1.0) + 0.5*(z2_7_7 - 1.0)*(z2_7_7 - 1.0) + \
    0.5*(x3_7_7 - 1.0)*(x3_7_7 - 1.0) + 0.5*(y3_7_7 - 1.0)*(y3_7_7 - 1.0) + \
    0.5*(z3_7_7 - 1.0)*(z3_7_7 - 1.0) + 0.5*(x4_7_7 - 1.0)*(x4_7_7 - 1.0) + \
    0.5*(y4_7_7 - 1.0)*(y4_7_7 - 1.0) + 0.5*(z4_7_7 - 1.0)*(z4_7_7 - 1.0) + \
    0.5*(x5_7_7 - 1.0)*(x5_7_7 - 1.0) + 0.5*(y5_7_7 - 1.0)*(y5_7_7 - 1.0) + \
    0.5*(z5_7_7 - 1.0)*(z5_7_7 - 1.0) + 0.5*(x6_7_7 - 1.0)*(x6_7_7 - 1.0) + \
    0.5*(y6_7_7 - 1.0)*(y6_7_7 - 1.0) + 0.5*(z6_7_7 - 1.0)*(z6_7_7 - 1.0) + \
    0.5*(x7_7_7 - 1.0)*(x7_7_7 - 1.0) + 0.5*(y7_7_7 - 1.0)*(y7_7_7 - 1.0) + \
    0.5*(z7_7_7 - 1.0)*(z7_7_7 - 1.0) + 0.5*(x8_7_7 - 1.0)*(x8_7_7 - 1.0) + \
    0.5*(y8_7_7 - 1.0)*(y8_7_7 - 1.0) + 0.5*(z8_7_7 - 1.0)*(z8_7_7 - 1.0) + \
    0.5*(x9_7_7 - 1.0)*(x9_7_7 - 1.0) + 0.5*(y9_7_7 - 1.0)*(y9_7_7 - 1.0) + \
    0.5*(z9_7_7 - 1.0)*(z9_7_7 - 1.0) + 0.5*(x1_8_7 - 1.0)*(x1_8_7 - 1.0) + \
    0.5*(y1_8_7 - 1.0)*(y1_8_7 - 1.0) + 0.5*(z1_8_7 - 1.0)*(z1_8_7 - 1.0) + \
    0.5*(x2_8_7 - 1.0)*(x2_8_7 - 1.0) + 0.5*(y2_8_7 - 1.0)*(y2_8_7 - 1.0) + \
    0.5*(z2_8_7 - 1.0)*(z2_8_7 - 1.0) + 0.5*(x3_8_7 - 1.0)*(x3_8_7 - 1.0) + \
    0.5*(y3_8_7 - 1.0)*(y3_8_7 - 1.0) + 0.5*(z3_8_7 - 1.0)*(z3_8_7 - 1.0) + \
    0.5*(x4_8_7 - 1.0)*(x4_8_7 - 1.0) + 0.5*(y4_8_7 - 1.0)*(y4_8_7 - 1.0) + \
    0.5*(z4_8_7 - 1.0)*(z4_8_7 - 1.0) + 0.5*(x5_8_7 - 1.0)*(x5_8_7 - 1.0) + \
    0.5*(y5_8_7 - 1.0)*(y5_8_7 - 1.0) + 0.5*(z5_8_7 - 1.0)*(z5_8_7 - 1.0) + \
    0.5*(x6_8_7 - 1.0)*(x6_8_7 - 1.0) + 0.5*(y6_8_7 - 1.0)*(y6_8_7 - 1.0) + \
    0.5*(z6_8_7 - 1.0)*(z6_8_7 - 1.0) + 0.5*(x7_8_7 - 1.0)*(x7_8_7 - 1.0) + \
    0.5*(y7_8_7 - 1.0)*(y7_8_7 - 1.0) + 0.5*(z7_8_7 - 1.0)*(z7_8_7 - 1.0) + \
    0.5*(x8_8_7 - 1.0)*(x8_8_7 - 1.0) + 0.5*(y8_8_7 - 1.0)*(y8_8_7 - 1.0) + \
    0.5*(z8_8_7 - 1.0)*(z8_8_7 - 1.0) + 0.5*(x9_8_7 - 1.0)*(x9_8_7 - 1.0) + \
    0.5*(y9_8_7 - 1.0)*(y9_8_7 - 1.0) + 0.5*(z9_8_7 - 1.0)*(z9_8_7 - 1.0) + \
    0.5*(x1_9_7 - 1.0)*(x1_9_7 - 1.0) + 0.5*(y1_9_7 - 1.0)*(y1_9_7 - 1.0) + \
    0.5*(z1_9_7 - 1.0)*(z1_9_7 - 1.0) + 0.5*(x2_9_7 - 1.0)*(x2_9_7 - 1.0) + \
    0.5*(y2_9_7 - 1.0)*(y2_9_7 - 1.0) + 0.5*(z2_9_7 - 1.0)*(z2_9_7 - 1.0) + \
    0.5*(x3_9_7 - 1.0)*(x3_9_7 - 1.0) + 0.5*(y3_9_7 - 1.0)*(y3_9_7 - 1.0) + \
    0.5*(z3_9_7 - 1.0)*(z3_9_7 - 1.0) + 0.5*(x4_9_7 - 1.0)*(x4_9_7 - 1.0) + \
    0.5*(y4_9_7 - 1.0)*(y4_9_7 - 1.0) + 0.5*(z4_9_7 - 1.0)*(z4_9_7 - 1.0) + \
    0.5*(x5_9_7 - 1.0)*(x5_9_7 - 1.0) + 0.5*(y5_9_7 - 1.0)*(y5_9_7 - 1.0) + \
    0.5*(z5_9_7 - 1.0)*(z5_9_7 - 1.0) + 0.5*(x6_9_7 - 1.0)*(x6_9_7 - 1.0) + \
    0.5*(y6_9_7 - 1.0)*(y6_9_7 - 1.0) + 0.5*(z6_9_7 - 1.0)*(z6_9_7 - 1.0) + \
    0.5*(x7_9_7 - 1.0)*(x7_9_7 - 1.0) + 0.5*(y7_9_7 - 1.0)*(y7_9_7 - 1.0) + \
    0.5*(z7_9_7 - 1.0)*(z7_9_7 - 1.0) + 0.5*(x8_9_7 - 1.0)*(x8_9_7 - 1.0) + \
    0.5*(y8_9_7 - 1.0)*(y8_9_7 - 1.0) + 0.5*(z8_9_7 - 1.0)*(z8_9_7 - 1.0) + \
    0.5*(x9_9_7 - 1.0)*(x9_9_7 - 1.0) + 0.5*(y9_9_7 - 1.0)*(y9_9_7 - 1.0) + \
    0.5*(z9_9_7 - 1.0)*(z9_9_7 - 1.0) + 0.5*(x1_1_8 - 1.0)*(x1_1_8 - 1.0) + \
    0.5*(y1_1_8 - 1.0)*(y1_1_8 - 1.0) + 0.5*(z1_1_8 - 1.0)*(z1_1_8 - 1.0) + \
    0.5*(x2_1_8 - 1.0)*(x2_1_8 - 1.0) + 0.5*(y2_1_8 - 1.0)*(y2_1_8 - 1.0) + \
    0.5*(z2_1_8 - 1.0)*(z2_1_8 - 1.0) + 0.5*(x3_1_8 - 1.0)*(x3_1_8 - 1.0) + \
    0.5*(y3_1_8 - 1.0)*(y3_1_8 - 1.0) + 0.5*(z3_1_8 - 1.0)*(z3_1_8 - 1.0) + \
    0.5*(x4_1_8 - 1.0)*(x4_1_8 - 1.0) + 0.5*(y4_1_8 - 1.0)*(y4_1_8 - 1.0) + \
    0.5*(z4_1_8 - 1.0)*(z4_1_8 - 1.0) + 0.5*(x5_1_8 - 1.0)*(x5_1_8 - 1.0) + \
    0.5*(y5_1_8 - 1.0)*(y5_1_8 - 1.0) + 0.5*(z5_1_8 - 1.0)*(z5_1_8 - 1.0) + \
    0.5*(x6_1_8 - 1.0)*(x6_1_8 - 1.0) + 0.5*(y6_1_8 - 1.0)*(y6_1_8 - 1.0) + \
    0.5*(z6_1_8 - 1.0)*(z6_1_8 - 1.0) + 0.5*(x7_1_8 - 1.0)*(x7_1_8 - 1.0) + \
    0.5*(y7_1_8 - 1.0)*(y7_1_8 - 1.0) + 0.5*(z7_1_8 - 1.0)*(z7_1_8 - 1.0) + \
    0.5*(x8_1_8 - 1.0)*(x8_1_8 - 1.0) + 0.5*(y8_1_8 - 1.0)*(y8_1_8 - 1.0) + \
    0.5*(z8_1_8 - 1.0)*(z8_1_8 - 1.0) + 0.5*(x9_1_8 - 1.0)*(x9_1_8 - 1.0) + \
    0.5*(y9_1_8 - 1.0)*(y9_1_8 - 1.0) + 0.5*(z9_1_8 - 1.0)*(z9_1_8 - 1.0) + \
    0.5*(x1_2_8 - 1.0)*(x1_2_8 - 1.0) + 0.5*(y1_2_8 - 1.0)*(y1_2_8 - 1.0) + \
    0.5*(z1_2_8 - 1.0)*(z1_2_8 - 1.0) + 0.5*(x2_2_8 - 1.0)*(x2_2_8 - 1.0) + \
    0.5*(y2_2_8 - 1.0)*(y2_2_8 - 1.0) + 0.5*(z2_2_8 - 1.0)*(z2_2_8 - 1.0) + \
    0.5*(x3_2_8 - 1.0)*(x3_2_8 - 1.0) + 0.5*(y3_2_8 - 1.0)*(y3_2_8 - 1.0) + \
    0.5*(z3_2_8 - 1.0)*(z3_2_8 - 1.0) + 0.5*(x4_2_8 - 1.0)*(x4_2_8 - 1.0) + \
    0.5*(y4_2_8 - 1.0)*(y4_2_8 - 1.0) + 0.5*(z4_2_8 - 1.0)*(z4_2_8 - 1.0) + \
    0.5*(x5_2_8 - 1.0)*(x5_2_8 - 1.0) + 0.5*(y5_2_8 - 1.0)*(y5_2_8 - 1.0) + \
    0.5*(z5_2_8 - 1.0)*(z5_2_8 - 1.0) + 0.5*(x6_2_8 - 1.0)*(x6_2_8 - 1.0) + \
    0.5*(y6_2_8 - 1.0)*(y6_2_8 - 1.0) + 0.5*(z6_2_8 - 1.0)*(z6_2_8 - 1.0) + \
    0.5*(x7_2_8 - 1.0)*(x7_2_8 - 1.0) + 0.5*(y7_2_8 - 1.0)*(y7_2_8 - 1.0) + \
    0.5*(z7_2_8 - 1.0)*(z7_2_8 - 1.0) + 0.5*(x8_2_8 - 1.0)*(x8_2_8 - 1.0) + \
    0.5*(y8_2_8 - 1.0)*(y8_2_8 - 1.0) + 0.5*(z8_2_8 - 1.0)*(z8_2_8 - 1.0) + \
    0.5*(x9_2_8 - 1.0)*(x9_2_8 - 1.0) + 0.5*(y9_2_8 - 1.0)*(y9_2_8 - 1.0) + \
    0.5*(z9_2_8 - 1.0)*(z9_2_8 - 1.0) + 0.5*(x1_3_8 - 1.0)*(x1_3_8 - 1.0) + \
    0.5*(y1_3_8 - 1.0)*(y1_3_8 - 1.0) + 0.5*(z1_3_8 - 1.0)*(z1_3_8 - 1.0) + \
    0.5*(x2_3_8 - 1.0)*(x2_3_8 - 1.0) + 0.5*(y2_3_8 - 1.0)*(y2_3_8 - 1.0) + \
    0.5*(z2_3_8 - 1.0)*(z2_3_8 - 1.0) + 0.5*(x3_3_8 - 1.0)*(x3_3_8 - 1.0) + \
    0.5*(y3_3_8 - 1.0)*(y3_3_8 - 1.0) + 0.5*(z3_3_8 - 1.0)*(z3_3_8 - 1.0) + \
    0.5*(x4_3_8 - 1.0)*(x4_3_8 - 1.0) + 0.5*(y4_3_8 - 1.0)*(y4_3_8 - 1.0) + \
    0.5*(z4_3_8 - 1.0)*(z4_3_8 - 1.0) + 0.5*(x5_3_8 - 1.0)*(x5_3_8 - 1.0) + \
    0.5*(y5_3_8 - 1.0)*(y5_3_8 - 1.0) + 0.5*(z5_3_8 - 1.0)*(z5_3_8 - 1.0) + \
    0.5*(x6_3_8 - 1.0)*(x6_3_8 - 1.0) + 0.5*(y6_3_8 - 1.0)*(y6_3_8 - 1.0) + \
    0.5*(z6_3_8 - 1.0)*(z6_3_8 - 1.0) + 0.5*(x7_3_8 - 1.0)*(x7_3_8 - 1.0) + \
    0.5*(y7_3_8 - 1.0)*(y7_3_8 - 1.0) + 0.5*(z7_3_8 - 1.0)*(z7_3_8 - 1.0) + \
    0.5*(x8_3_8 - 1.0)*(x8_3_8 - 1.0) + 0.5*(y8_3_8 - 1.0)*(y8_3_8 - 1.0) + \
    0.5*(z8_3_8 - 1.0)*(z8_3_8 - 1.0) + 0.5*(x9_3_8 - 1.0)*(x9_3_8 - 1.0) + \
    0.5*(y9_3_8 - 1.0)*(y9_3_8 - 1.0) + 0.5*(z9_3_8 - 1.0)*(z9_3_8 - 1.0) + \
    0.5*(x1_4_8 - 1.0)*(x1_4_8 - 1.0) + 0.5*(y1_4_8 - 1.0)*(y1_4_8 - 1.0) + \
    0.5*(z1_4_8 - 1.0)*(z1_4_8 - 1.0) + 0.5*(x2_4_8 - 1.0)*(x2_4_8 - 1.0) + \
    0.5*(y2_4_8 - 1.0)*(y2_4_8 - 1.0) + 0.5*(z2_4_8 - 1.0)*(z2_4_8 - 1.0) + \
    0.5*(x3_4_8 - 1.0)*(x3_4_8 - 1.0) + 0.5*(y3_4_8 - 1.0)*(y3_4_8 - 1.0) + \
    0.5*(z3_4_8 - 1.0)*(z3_4_8 - 1.0) + 0.5*(x4_4_8 - 1.0)*(x4_4_8 - 1.0) + \
    0.5*(y4_4_8 - 1.0)*(y4_4_8 - 1.0) + 0.5*(z4_4_8 - 1.0)*(z4_4_8 - 1.0) + \
    0.5*(x5_4_8 - 1.0)*(x5_4_8 - 1.0) + 0.5*(y5_4_8 - 1.0)*(y5_4_8 - 1.0) + \
    0.5*(z5_4_8 - 1.0)*(z5_4_8 - 1.0) + 0.5*(x6_4_8 - 1.0)*(x6_4_8 - 1.0) + \
    0.5*(y6_4_8 - 1.0)*(y6_4_8 - 1.0) + 0.5*(z6_4_8 - 1.0)*(z6_4_8 - 1.0) + \
    0.5*(x7_4_8 - 1.0)*(x7_4_8 - 1.0) + 0.5*(y7_4_8 - 1.0)*(y7_4_8 - 1.0) + \
    0.5*(z7_4_8 - 1.0)*(z7_4_8 - 1.0) + 0.5*(x8_4_8 - 1.0)*(x8_4_8 - 1.0) + \
    0.5*(y8_4_8 - 1.0)*(y8_4_8 - 1.0) + 0.5*(z8_4_8 - 1.0)*(z8_4_8 - 1.0) + \
    0.5*(x9_4_8 - 1.0)*(x9_4_8 - 1.0) + 0.5*(y9_4_8 - 1.0)*(y9_4_8 - 1.0) + \
    0.5*(z9_4_8 - 1.0)*(z9_4_8 - 1.0) + 0.5*(x1_5_8 - 1.0)*(x1_5_8 - 1.0) + \
    0.5*(y1_5_8 - 1.0)*(y1_5_8 - 1.0) + 0.5*(z1_5_8 - 1.0)*(z1_5_8 - 1.0) + \
    0.5*(x2_5_8 - 1.0)*(x2_5_8 - 1.0) + 0.5*(y2_5_8 - 1.0)*(y2_5_8 - 1.0) + \
    0.5*(z2_5_8 - 1.0)*(z2_5_8 - 1.0) + 0.5*(x3_5_8 - 1.0)*(x3_5_8 - 1.0) + \
    0.5*(y3_5_8 - 1.0)*(y3_5_8 - 1.0) + 0.5*(z3_5_8 - 1.0)*(z3_5_8 - 1.0) + \
    0.5*(x4_5_8 - 1.0)*(x4_5_8 - 1.0) + 0.5*(y4_5_8 - 1.0)*(y4_5_8 - 1.0) + \
    0.5*(z4_5_8 - 1.0)*(z4_5_8 - 1.0) + 0.5*(x5_5_8 - 1.0)*(x5_5_8 - 1.0) + \
    0.5*(y5_5_8 - 1.0)*(y5_5_8 - 1.0) + 0.5*(z5_5_8 - 1.0)*(z5_5_8 - 1.0) + \
    0.5*(x6_5_8 - 1.0)*(x6_5_8 - 1.0) + 0.5*(y6_5_8 - 1.0)*(y6_5_8 - 1.0) + \
    0.5*(z6_5_8 - 1.0)*(z6_5_8 - 1.0) + 0.5*(x7_5_8 - 1.0)*(x7_5_8 - 1.0) + \
    0.5*(y7_5_8 - 1.0)*(y7_5_8 - 1.0) + 0.5*(z7_5_8 - 1.0)*(z7_5_8 - 1.0) + \
    0.5*(x8_5_8 - 1.0)*(x8_5_8 - 1.0) + 0.5*(y8_5_8 - 1.0)*(y8_5_8 - 1.0) + \
    0.5*(z8_5_8 - 1.0)*(z8_5_8 - 1.0) + 0.5*(x9_5_8 - 1.0)*(x9_5_8 - 1.0) + \
    0.5*(y9_5_8 - 1.0)*(y9_5_8 - 1.0) + 0.5*(z9_5_8 - 1.0)*(z9_5_8 - 1.0) + \
    0.5*(x1_6_8 - 1.0)*(x1_6_8 - 1.0) + 0.5*(y1_6_8 - 1.0)*(y1_6_8 - 1.0) + \
    0.5*(z1_6_8 - 1.0)*(z1_6_8 - 1.0) + 0.5*(x2_6_8 - 1.0)*(x2_6_8 - 1.0) + \
    0.5*(y2_6_8 - 1.0)*(y2_6_8 - 1.0) + 0.5*(z2_6_8 - 1.0)*(z2_6_8 - 1.0) + \
    0.5*(x3_6_8 - 1.0)*(x3_6_8 - 1.0) + 0.5*(y3_6_8 - 1.0)*(y3_6_8 - 1.0) + \
    0.5*(z3_6_8 - 1.0)*(z3_6_8 - 1.0) + 0.5*(x4_6_8 - 1.0)*(x4_6_8 - 1.0) + \
    0.5*(y4_6_8 - 1.0)*(y4_6_8 - 1.0) + 0.5*(z4_6_8 - 1.0)*(z4_6_8 - 1.0) + \
    0.5*(x5_6_8 - 1.0)*(x5_6_8 - 1.0) + 0.5*(y5_6_8 - 1.0)*(y5_6_8 - 1.0) + \
    0.5*(z5_6_8 - 1.0)*(z5_6_8 - 1.0) + 0.5*(x6_6_8 - 1.0)*(x6_6_8 - 1.0) + \
    0.5*(y6_6_8 - 1.0)*(y6_6_8 - 1.0) + 0.5*(z6_6_8 - 1.0)*(z6_6_8 - 1.0) + \
    0.5*(x7_6_8 - 1.0)*(x7_6_8 - 1.0) + 0.5*(y7_6_8 - 1.0)*(y7_6_8 - 1.0) + \
    0.5*(z7_6_8 - 1.0)*(z7_6_8 - 1.0) + 0.5*(x8_6_8 - 1.0)*(x8_6_8 - 1.0) + \
    0.5*(y8_6_8 - 1.0)*(y8_6_8 - 1.0) + 0.5*(z8_6_8 - 1.0)*(z8_6_8 - 1.0) + \
    0.5*(x9_6_8 - 1.0)*(x9_6_8 - 1.0) + 0.5*(y9_6_8 - 1.0)*(y9_6_8 - 1.0) + \
    0.5*(z9_6_8 - 1.0)*(z9_6_8 - 1.0) + 0.5*(x1_7_8 - 1.0)*(x1_7_8 - 1.0) + \
    0.5*(y1_7_8 - 1.0)*(y1_7_8 - 1.0) + 0.5*(z1_7_8 - 1.0)*(z1_7_8 - 1.0) + \
    0.5*(x2_7_8 - 1.0)*(x2_7_8 - 1.0) + 0.5*(y2_7_8 - 1.0)*(y2_7_8 - 1.0) + \
    0.5*(z2_7_8 - 1.0)*(z2_7_8 - 1.0) + 0.5*(x3_7_8 - 1.0)*(x3_7_8 - 1.0) + \
    0.5*(y3_7_8 - 1.0)*(y3_7_8 - 1.0) + 0.5*(z3_7_8 - 1.0)*(z3_7_8 - 1.0) + \
    0.5*(x4_7_8 - 1.0)*(x4_7_8 - 1.0) + 0.5*(y4_7_8 - 1.0)*(y4_7_8 - 1.0) + \
    0.5*(z4_7_8 - 1.0)*(z4_7_8 - 1.0) + 0.5*(x5_7_8 - 1.0)*(x5_7_8 - 1.0) + \
    0.5*(y5_7_8 - 1.0)*(y5_7_8 - 1.0) + 0.5*(z5_7_8 - 1.0)*(z5_7_8 - 1.0) + \
    0.5*(x6_7_8 - 1.0)*(x6_7_8 - 1.0) + 0.5*(y6_7_8 - 1.0)*(y6_7_8 - 1.0) + \
    0.5*(z6_7_8 - 1.0)*(z6_7_8 - 1.0) + 0.5*(x7_7_8 - 1.0)*(x7_7_8 - 1.0) + \
    0.5*(y7_7_8 - 1.0)*(y7_7_8 - 1.0) + 0.5*(z7_7_8 - 1.0)*(z7_7_8 - 1.0) + \
    0.5*(x8_7_8 - 1.0)*(x8_7_8 - 1.0) + 0.5*(y8_7_8 - 1.0)*(y8_7_8 - 1.0) + \
    0.5*(z8_7_8 - 1.0)*(z8_7_8 - 1.0) + 0.5*(x9_7_8 - 1.0)*(x9_7_8 - 1.0) + \
    0.5*(y9_7_8 - 1.0)*(y9_7_8 - 1.0) + 0.5*(z9_7_8 - 1.0)*(z9_7_8 - 1.0) + \
    0.5*(x1_8_8 - 1.0)*(x1_8_8 - 1.0) + 0.5*(y1_8_8 - 1.0)*(y1_8_8 - 1.0) + \
    0.5*(z1_8_8 - 1.0)*(z1_8_8 - 1.0) + 0.5*(x2_8_8 - 1.0)*(x2_8_8 - 1.0) + \
    0.5*(y2_8_8 - 1.0)*(y2_8_8 - 1.0) + 0.5*(z2_8_8 - 1.0)*(z2_8_8 - 1.0) + \
    0.5*(x3_8_8 - 1.0)*(x3_8_8 - 1.0) + 0.5*(y3_8_8 - 1.0)*(y3_8_8 - 1.0) + \
    0.5*(z3_8_8 - 1.0)*(z3_8_8 - 1.0) + 0.5*(x4_8_8 - 1.0)*(x4_8_8 - 1.0) + \
    0.5*(y4_8_8 - 1.0)*(y4_8_8 - 1.0) + 0.5*(z4_8_8 - 1.0)*(z4_8_8 - 1.0) + \
    0.5*(x5_8_8 - 1.0)*(x5_8_8 - 1.0) + 0.5*(y5_8_8 - 1.0)*(y5_8_8 - 1.0) + \
    0.5*(z5_8_8 - 1.0)*(z5_8_8 - 1.0) + 0.5*(x6_8_8 - 1.0)*(x6_8_8 - 1.0) + \
    0.5*(y6_8_8 - 1.0)*(y6_8_8 - 1.0) + 0.5*(z6_8_8 - 1.0)*(z6_8_8 - 1.0) + \
    0.5*(x7_8_8 - 1.0)*(x7_8_8 - 1.0) + 0.5*(y7_8_8 - 1.0)*(y7_8_8 - 1.0) + \
    0.5*(z7_8_8 - 1.0)*(z7_8_8 - 1.0) + 0.5*(x8_8_8 - 1.0)*(x8_8_8 - 1.0) + \
    0.5*(y8_8_8 - 1.0)*(y8_8_8 - 1.0) + 0.5*(z8_8_8 - 1.0)*(z8_8_8 - 1.0) + \
    0.5*(x9_8_8 - 1.0)*(x9_8_8 - 1.0) + 0.5*(y9_8_8 - 1.0)*(y9_8_8 - 1.0) + \
    0.5*(z9_8_8 - 1.0)*(z9_8_8 - 1.0) + 0.5*(x1_9_8 - 1.0)*(x1_9_8 - 1.0) + \
    0.5*(y1_9_8 - 1.0)*(y1_9_8 - 1.0) + 0.5*(z1_9_8 - 1.0)*(z1_9_8 - 1.0) + \
    0.5*(x2_9_8 - 1.0)*(x2_9_8 - 1.0) + 0.5*(y2_9_8 - 1.0)*(y2_9_8 - 1.0) + \
    0.5*(z2_9_8 - 1.0)*(z2_9_8 - 1.0) + 0.5*(x3_9_8 - 1.0)*(x3_9_8 - 1.0) + \
    0.5*(y3_9_8 - 1.0)*(y3_9_8 - 1.0) + 0.5*(z3_9_8 - 1.0)*(z3_9_8 - 1.0) + \
    0.5*(x4_9_8 - 1.0)*(x4_9_8 - 1.0) + 0.5*(y4_9_8 - 1.0)*(y4_9_8 - 1.0) + \
    0.5*(z4_9_8 - 1.0)*(z4_9_8 - 1.0) + 0.5*(x5_9_8 - 1.0)*(x5_9_8 - 1.0) + \
    0.5*(y5_9_8 - 1.0)*(y5_9_8 - 1.0) + 0.5*(z5_9_8 - 1.0)*(z5_9_8 - 1.0) + \
    0.5*(x6_9_8 - 1.0)*(x6_9_8 - 1.0) + 0.5*(y6_9_8 - 1.0)*(y6_9_8 - 1.0) + \
    0.5*(z6_9_8 - 1.0)*(z6_9_8 - 1.0) + 0.5*(x7_9_8 - 1.0)*(x7_9_8 - 1.0) + \
    0.5*(y7_9_8 - 1.0)*(y7_9_8 - 1.0) + 0.5*(z7_9_8 - 1.0)*(z7_9_8 - 1.0) + \
    0.5*(x8_9_8 - 1.0)*(x8_9_8 - 1.0) + 0.5*(y8_9_8 - 1.0)*(y8_9_8 - 1.0) + \
    0.5*(z8_9_8 - 1.0)*(z8_9_8 - 1.0) + 0.5*(x9_9_8 - 1.0)*(x9_9_8 - 1.0) + \
    0.5*(y9_9_8 - 1.0)*(y9_9_8 - 1.0) + 0.5*(z9_9_8 - 1.0)*(z9_9_8 - 1.0) + \
    0.5*(x1_1_9 - 1.0)*(x1_1_9 - 1.0) + 0.5*(y1_1_9 - 1.0)*(y1_1_9 - 1.0) + \
    0.5*(z1_1_9 - 1.0)*(z1_1_9 - 1.0) + 0.5*(x2_1_9 - 1.0)*(x2_1_9 - 1.0) + \
    0.5*(y2_1_9 - 1.0)*(y2_1_9 - 1.0) + 0.5*(z2_1_9 - 1.0)*(z2_1_9 - 1.0) + \
    0.5*(x3_1_9 - 1.0)*(x3_1_9 - 1.0) + 0.5*(y3_1_9 - 1.0)*(y3_1_9 - 1.0) + \
    0.5*(z3_1_9 - 1.0)*(z3_1_9 - 1.0) + 0.5*(x4_1_9 - 1.0)*(x4_1_9 - 1.0) + \
    0.5*(y4_1_9 - 1.0)*(y4_1_9 - 1.0) + 0.5*(z4_1_9 - 1.0)*(z4_1_9 - 1.0) + \
    0.5*(x5_1_9 - 1.0)*(x5_1_9 - 1.0) + 0.5*(y5_1_9 - 1.0)*(y5_1_9 - 1.0) + \
    0.5*(z5_1_9 - 1.0)*(z5_1_9 - 1.0) + 0.5*(x6_1_9 - 1.0)*(x6_1_9 - 1.0) + \
    0.5*(y6_1_9 - 1.0)*(y6_1_9 - 1.0) + 0.5*(z6_1_9 - 1.0)*(z6_1_9 - 1.0) + \
    0.5*(x7_1_9 - 1.0)*(x7_1_9 - 1.0) + 0.5*(y7_1_9 - 1.0)*(y7_1_9 - 1.0) + \
    0.5*(z7_1_9 - 1.0)*(z7_1_9 - 1.0) + 0.5*(x8_1_9 - 1.0)*(x8_1_9 - 1.0) + \
    0.5*(y8_1_9 - 1.0)*(y8_1_9 - 1.0) + 0.5*(z8_1_9 - 1.0)*(z8_1_9 - 1.0) + \
    0.5*(x9_1_9 - 1.0)*(x9_1_9 - 1.0) + 0.5*(y9_1_9 - 1.0)*(y9_1_9 - 1.0) + \
    0.5*(z9_1_9 - 1.0)*(z9_1_9 - 1.0) + 0.5*(x1_2_9 - 1.0)*(x1_2_9 - 1.0) + \
    0.5*(y1_2_9 - 1.0)*(y1_2_9 - 1.0) + 0.5*(z1_2_9 - 1.0)*(z1_2_9 - 1.0) + \
    0.5*(x2_2_9 - 1.0)*(x2_2_9 - 1.0) + 0.5*(y2_2_9 - 1.0)*(y2_2_9 - 1.0) + \
    0.5*(z2_2_9 - 1.0)*(z2_2_9 - 1.0) + 0.5*(x3_2_9 - 1.0)*(x3_2_9 - 1.0) + \
    0.5*(y3_2_9 - 1.0)*(y3_2_9 - 1.0) + 0.5*(z3_2_9 - 1.0)*(z3_2_9 - 1.0) + \
    0.5*(x4_2_9 - 1.0)*(x4_2_9 - 1.0) + 0.5*(y4_2_9 - 1.0)*(y4_2_9 - 1.0) + \
    0.5*(z4_2_9 - 1.0)*(z4_2_9 - 1.0) + 0.5*(x5_2_9 - 1.0)*(x5_2_9 - 1.0) + \
    0.5*(y5_2_9 - 1.0)*(y5_2_9 - 1.0) + 0.5*(z5_2_9 - 1.0)*(z5_2_9 - 1.0) + \
    0.5*(x6_2_9 - 1.0)*(x6_2_9 - 1.0) + 0.5*(y6_2_9 - 1.0)*(y6_2_9 - 1.0) + \
    0.5*(z6_2_9 - 1.0)*(z6_2_9 - 1.0) + 0.5*(x7_2_9 - 1.0)*(x7_2_9 - 1.0) + \
    0.5*(y7_2_9 - 1.0)*(y7_2_9 - 1.0) + 0.5*(z7_2_9 - 1.0)*(z7_2_9 - 1.0) + \
    0.5*(x8_2_9 - 1.0)*(x8_2_9 - 1.0) + 0.5*(y8_2_9 - 1.0)*(y8_2_9 - 1.0) + \
    0.5*(z8_2_9 - 1.0)*(z8_2_9 - 1.0) + 0.5*(x9_2_9 - 1.0)*(x9_2_9 - 1.0) + \
    0.5*(y9_2_9 - 1.0)*(y9_2_9 - 1.0) + 0.5*(z9_2_9 - 1.0)*(z9_2_9 - 1.0) + \
    0.5*(x1_3_9 - 1.0)*(x1_3_9 - 1.0) + 0.5*(y1_3_9 - 1.0)*(y1_3_9 - 1.0) + \
    0.5*(z1_3_9 - 1.0)*(z1_3_9 - 1.0) + 0.5*(x2_3_9 - 1.0)*(x2_3_9 - 1.0) + \
    0.5*(y2_3_9 - 1.0)*(y2_3_9 - 1.0) + 0.5*(z2_3_9 - 1.0)*(z2_3_9 - 1.0) + \
    0.5*(x3_3_9 - 1.0)*(x3_3_9 - 1.0) + 0.5*(y3_3_9 - 1.0)*(y3_3_9 - 1.0) + \
    0.5*(z3_3_9 - 1.0)*(z3_3_9 - 1.0) + 0.5*(x4_3_9 - 1.0)*(x4_3_9 - 1.0) + \
    0.5*(y4_3_9 - 1.0)*(y4_3_9 - 1.0) + 0.5*(z4_3_9 - 1.0)*(z4_3_9 - 1.0) + \
    0.5*(x5_3_9 - 1.0)*(x5_3_9 - 1.0) + 0.5*(y5_3_9 - 1.0)*(y5_3_9 - 1.0) + \
    0.5*(z5_3_9 - 1.0)*(z5_3_9 - 1.0) + 0.5*(x6_3_9 - 1.0)*(x6_3_9 - 1.0) + \
    0.5*(y6_3_9 - 1.0)*(y6_3_9 - 1.0) + 0.5*(z6_3_9 - 1.0)*(z6_3_9 - 1.0) + \
    0.5*(x7_3_9 - 1.0)*(x7_3_9 - 1.0) + 0.5*(y7_3_9 - 1.0)*(y7_3_9 - 1.0) + \
    0.5*(z7_3_9 - 1.0)*(z7_3_9 - 1.0) + 0.5*(x8_3_9 - 1.0)*(x8_3_9 - 1.0) + \
    0.5*(y8_3_9 - 1.0)*(y8_3_9 - 1.0) + 0.5*(z8_3_9 - 1.0)*(z8_3_9 - 1.0) + \
    0.5*(x9_3_9 - 1.0)*(x9_3_9 - 1.0) + 0.5*(y9_3_9 - 1.0)*(y9_3_9 - 1.0) + \
    0.5*(z9_3_9 - 1.0)*(z9_3_9 - 1.0) + 0.5*(x1_4_9 - 1.0)*(x1_4_9 - 1.0) + \
    0.5*(y1_4_9 - 1.0)*(y1_4_9 - 1.0) + 0.5*(z1_4_9 - 1.0)*(z1_4_9 - 1.0) + \
    0.5*(x2_4_9 - 1.0)*(x2_4_9 - 1.0) + 0.5*(y2_4_9 - 1.0)*(y2_4_9 - 1.0) + \
    0.5*(z2_4_9 - 1.0)*(z2_4_9 - 1.0) + 0.5*(x3_4_9 - 1.0)*(x3_4_9 - 1.0) + \
    0.5*(y3_4_9 - 1.0)*(y3_4_9 - 1.0) + 0.5*(z3_4_9 - 1.0)*(z3_4_9 - 1.0) + \
    0.5*(x4_4_9 - 1.0)*(x4_4_9 - 1.0) + 0.5*(y4_4_9 - 1.0)*(y4_4_9 - 1.0) + \
    0.5*(z4_4_9 - 1.0)*(z4_4_9 - 1.0) + 0.5*(x5_4_9 - 1.0)*(x5_4_9 - 1.0) + \
    0.5*(y5_4_9 - 1.0)*(y5_4_9 - 1.0) + 0.5*(z5_4_9 - 1.0)*(z5_4_9 - 1.0) + \
    0.5*(x6_4_9 - 1.0)*(x6_4_9 - 1.0) + 0.5*(y6_4_9 - 1.0)*(y6_4_9 - 1.0) + \
    0.5*(z6_4_9 - 1.0)*(z6_4_9 - 1.0) + 0.5*(x7_4_9 - 1.0)*(x7_4_9 - 1.0) + \
    0.5*(y7_4_9 - 1.0)*(y7_4_9 - 1.0) + 0.5*(z7_4_9 - 1.0)*(z7_4_9 - 1.0) + \
    0.5*(x8_4_9 - 1.0)*(x8_4_9 - 1.0) + 0.5*(y8_4_9 - 1.0)*(y8_4_9 - 1.0) + \
    0.5*(z8_4_9 - 1.0)*(z8_4_9 - 1.0) + 0.5*(x9_4_9 - 1.0)*(x9_4_9 - 1.0) + \
    0.5*(y9_4_9 - 1.0)*(y9_4_9 - 1.0) + 0.5*(z9_4_9 - 1.0)*(z9_4_9 - 1.0) + \
    0.5*(x1_5_9 - 1.0)*(x1_5_9 - 1.0) + 0.5*(y1_5_9 - 1.0)*(y1_5_9 - 1.0) + \
    0.5*(z1_5_9 - 1.0)*(z1_5_9 - 1.0) + 0.5*(x2_5_9 - 1.0)*(x2_5_9 - 1.0) + \
    0.5*(y2_5_9 - 1.0)*(y2_5_9 - 1.0) + 0.5*(z2_5_9 - 1.0)*(z2_5_9 - 1.0) + \
    0.5*(x3_5_9 - 1.0)*(x3_5_9 - 1.0) + 0.5*(y3_5_9 - 1.0)*(y3_5_9 - 1.0) + \
    0.5*(z3_5_9 - 1.0)*(z3_5_9 - 1.0) + 0.5*(x4_5_9 - 1.0)*(x4_5_9 - 1.0) + \
    0.5*(y4_5_9 - 1.0)*(y4_5_9 - 1.0) + 0.5*(z4_5_9 - 1.0)*(z4_5_9 - 1.0) + \
    0.5*(x5_5_9 - 1.0)*(x5_5_9 - 1.0) + 0.5*(y5_5_9 - 1.0)*(y5_5_9 - 1.0) + \
    0.5*(z5_5_9 - 1.0)*(z5_5_9 - 1.0) + 0.5*(x6_5_9 - 1.0)*(x6_5_9 - 1.0) + \
    0.5*(y6_5_9 - 1.0)*(y6_5_9 - 1.0) + 0.5*(z6_5_9 - 1.0)*(z6_5_9 - 1.0) + \
    0.5*(x7_5_9 - 1.0)*(x7_5_9 - 1.0) + 0.5*(y7_5_9 - 1.0)*(y7_5_9 - 1.0) + \
    0.5*(z7_5_9 - 1.0)*(z7_5_9 - 1.0) + 0.5*(x8_5_9 - 1.0)*(x8_5_9 - 1.0) + \
    0.5*(y8_5_9 - 1.0)*(y8_5_9 - 1.0) + 0.5*(z8_5_9 - 1.0)*(z8_5_9 - 1.0) + \
    0.5*(x9_5_9 - 1.0)*(x9_5_9 - 1.0) + 0.5*(y9_5_9 - 1.0)*(y9_5_9 - 1.0) + \
    0.5*(z9_5_9 - 1.0)*(z9_5_9 - 1.0) + 0.5*(x1_6_9 - 1.0)*(x1_6_9 - 1.0) + \
    0.5*(y1_6_9 - 1.0)*(y1_6_9 - 1.0) + 0.5*(z1_6_9 - 1.0)*(z1_6_9 - 1.0) + \
    0.5*(x2_6_9 - 1.0)*(x2_6_9 - 1.0) + 0.5*(y2_6_9 - 1.0)*(y2_6_9 - 1.0) + \
    0.5*(z2_6_9 - 1.0)*(z2_6_9 - 1.0) + 0.5*(x3_6_9 - 1.0)*(x3_6_9 - 1.0) + \
    0.5*(y3_6_9 - 1.0)*(y3_6_9 - 1.0) + 0.5*(z3_6_9 - 1.0)*(z3_6_9 - 1.0) + \
    0.5*(x4_6_9 - 1.0)*(x4_6_9 - 1.0) + 0.5*(y4_6_9 - 1.0)*(y4_6_9 - 1.0) + \
    0.5*(z4_6_9 - 1.0)*(z4_6_9 - 1.0) + 0.5*(x5_6_9 - 1.0)*(x5_6_9 - 1.0) + \
    0.5*(y5_6_9 - 1.0)*(y5_6_9 - 1.0) + 0.5*(z5_6_9 - 1.0)*(z5_6_9 - 1.0) + \
    0.5*(x6_6_9 - 1.0)*(x6_6_9 - 1.0) + 0.5*(y6_6_9 - 1.0)*(y6_6_9 - 1.0) + \
    0.5*(z6_6_9 - 1.0)*(z6_6_9 - 1.0) + 0.5*(x7_6_9 - 1.0)*(x7_6_9 - 1.0) + \
    0.5*(y7_6_9 - 1.0)*(y7_6_9 - 1.0) + 0.5*(z7_6_9 - 1.0)*(z7_6_9 - 1.0) + \
    0.5*(x8_6_9 - 1.0)*(x8_6_9 - 1.0) + 0.5*(y8_6_9 - 1.0)*(y8_6_9 - 1.0) + \
    0.5*(z8_6_9 - 1.0)*(z8_6_9 - 1.0) + 0.5*(x9_6_9 - 1.0)*(x9_6_9 - 1.0) + \
    0.5*(y9_6_9 - 1.0)*(y9_6_9 - 1.0) + 0.5*(z9_6_9 - 1.0)*(z9_6_9 - 1.0) + \
    0.5*(x1_7_9 - 1.0)*(x1_7_9 - 1.0) + 0.5*(y1_7_9 - 1.0)*(y1_7_9 - 1.0) + \
    0.5*(z1_7_9 - 1.0)*(z1_7_9 - 1.0) + 0.5*(x2_7_9 - 1.0)*(x2_7_9 - 1.0) + \
    0.5*(y2_7_9 - 1.0)*(y2_7_9 - 1.0) + 0.5*(z2_7_9 - 1.0)*(z2_7_9 - 1.0) + \
    0.5*(x3_7_9 - 1.0)*(x3_7_9 - 1.0) + 0.5*(y3_7_9 - 1.0)*(y3_7_9 - 1.0) + \
    0.5*(z3_7_9 - 1.0)*(z3_7_9 - 1.0) + 0.5*(x4_7_9 - 1.0)*(x4_7_9 - 1.0) + \
    0.5*(y4_7_9 - 1.0)*(y4_7_9 - 1.0) + 0.5*(z4_7_9 - 1.0)*(z4_7_9 - 1.0) + \
    0.5*(x5_7_9 - 1.0)*(x5_7_9 - 1.0) + 0.5*(y5_7_9 - 1.0)*(y5_7_9 - 1.0) + \
    0.5*(z5_7_9 - 1.0)*(z5_7_9 - 1.0) + 0.5*(x6_7_9 - 1.0)*(x6_7_9 - 1.0) + \
    0.5*(y6_7_9 - 1.0)*(y6_7_9 - 1.0) + 0.5*(z6_7_9 - 1.0)*(z6_7_9 - 1.0) + \
    0.5*(x7_7_9 - 1.0)*(x7_7_9 - 1.0) + 0.5*(y7_7_9 - 1.0)*(y7_7_9 - 1.0) + \
    0.5*(z7_7_9 - 1.0)*(z7_7_9 - 1.0) + 0.5*(x8_7_9 - 1.0)*(x8_7_9 - 1.0) + \
    0.5*(y8_7_9 - 1.0)*(y8_7_9 - 1.0) + 0.5*(z8_7_9 - 1.0)*(z8_7_9 - 1.0) + \
    0.5*(x9_7_9 - 1.0)*(x9_7_9 - 1.0) + 0.5*(y9_7_9 - 1.0)*(y9_7_9 - 1.0) + \
    0.5*(z9_7_9 - 1.0)*(z9_7_9 - 1.0) + 0.5*(x1_8_9 - 1.0)*(x1_8_9 - 1.0) + \
    0.5*(y1_8_9 - 1.0)*(y1_8_9 - 1.0) + 0.5*(z1_8_9 - 1.0)*(z1_8_9 - 1.0) + \
    0.5*(x2_8_9 - 1.0)*(x2_8_9 - 1.0) + 0.5*(y2_8_9 - 1.0)*(y2_8_9 - 1.0) + \
    0.5*(z2_8_9 - 1.0)*(z2_8_9 - 1.0) + 0.5*(x3_8_9 - 1.0)*(x3_8_9 - 1.0) + \
    0.5*(y3_8_9 - 1.0)*(y3_8_9 - 1.0) + 0.5*(z3_8_9 - 1.0)*(z3_8_9 - 1.0) + \
    0.5*(x4_8_9 - 1.0)*(x4_8_9 - 1.0) + 0.5*(y4_8_9 - 1.0)*(y4_8_9 - 1.0) + \
    0.5*(z4_8_9 - 1.0)*(z4_8_9 - 1.0) + 0.5*(x5_8_9 - 1.0)*(x5_8_9 - 1.0) + \
    0.5*(y5_8_9 - 1.0)*(y5_8_9 - 1.0) + 0.5*(z5_8_9 - 1.0)*(z5_8_9 - 1.0) + \
    0.5*(x6_8_9 - 1.0)*(x6_8_9 - 1.0) + 0.5*(y6_8_9 - 1.0)*(y6_8_9 - 1.0) + \
    0.5*(z6_8_9 - 1.0)*(z6_8_9 - 1.0) + 0.5*(x7_8_9 - 1.0)*(x7_8_9 - 1.0) + \
    0.5*(y7_8_9 - 1.0)*(y7_8_9 - 1.0) + 0.5*(z7_8_9 - 1.0)*(z7_8_9 - 1.0) + \
    0.5*(x8_8_9 - 1.0)*(x8_8_9 - 1.0) + 0.5*(y8_8_9 - 1.0)*(y8_8_9 - 1.0) + \
    0.5*(z8_8_9 - 1.0)*(z8_8_9 - 1.0) + 0.5*(x9_8_9 - 1.0)*(x9_8_9 - 1.0) + \
    0.5*(y9_8_9 - 1.0)*(y9_8_9 - 1.0) + 0.5*(z9_8_9 - 1.0)*(z9_8_9 - 1.0) + \
    0.5*(x1_9_9 - 1.0)*(x1_9_9 - 1.0) + 0.5*(y1_9_9 - 1.0)*(y1_9_9 - 1.0) + \
    0.5*(z1_9_9 - 1.0)*(z1_9_9 - 1.0) + 0.5*(x2_9_9 - 1.0)*(x2_9_9 - 1.0) + \
    0.5*(y2_9_9 - 1.0)*(y2_9_9 - 1.0) + 0.5*(z2_9_9 - 1.0)*(z2_9_9 - 1.0) + \
    0.5*(x3_9_9 - 1.0)*(x3_9_9 - 1.0) + 0.5*(y3_9_9 - 1.0)*(y3_9_9 - 1.0) + \
    0.5*(z3_9_9 - 1.0)*(z3_9_9 - 1.0) + 0.5*(x4_9_9 - 1.0)*(x4_9_9 - 1.0) + \
    0.5*(y4_9_9 - 1.0)*(y4_9_9 - 1.0) + 0.5*(z4_9_9 - 1.0)*(z4_9_9 - 1.0) + \
    0.5*(x5_9_9 - 1.0)*(x5_9_9 - 1.0) + 0.5*(y5_9_9 - 1.0)*(y5_9_9 - 1.0) + \
    0.5*(z5_9_9 - 1.0)*(z5_9_9 - 1.0) + 0.5*(x6_9_9 - 1.0)*(x6_9_9 - 1.0) + \
    0.5*(y6_9_9 - 1.0)*(y6_9_9 - 1.0) + 0.5*(z6_9_9 - 1.0)*(z6_9_9 - 1.0) + \
    0.5*(x7_9_9 - 1.0)*(x7_9_9 - 1.0) + 0.5*(y7_9_9 - 1.0)*(y7_9_9 - 1.0) + \
    0.5*(z7_9_9 - 1.0)*(z7_9_9 - 1.0) + 0.5*(x8_9_9 - 1.0)*(x8_9_9 - 1.0) + \
    0.5*(y8_9_9 - 1.0)*(y8_9_9 - 1.0) + 0.5*(z8_9_9 - 1.0)*(z8_9_9 - 1.0) + \
    0.5*(x9_9_9 - 1.0)*(x9_9_9 - 1.0) + 0.5*(y9_9_9 - 1.0)*(y9_9_9 - 1.0) + \
    0.5*(z9_9_9 - 1.0)*(z9_9_9 - 1.0) + 0.5*(y10_1_1 - 1.0)*(y10_1_1 - 1.0) + \
    0.5*(z10_1_1 - 1.0)*(z10_1_1 - 1.0) + 0.5*(y10_2_1 - 1.0)*(y10_2_1 - 1.0) \
    + 0.5*(z10_2_1 - 1.0)*(z10_2_1 - 1.0) + 0.5*(y10_3_1 - 1.0)*(y10_3_1 - \
    1.0) + 0.5*(z10_3_1 - 1.0)*(z10_3_1 - 1.0) + 0.5*(y10_4_1 - 1.0)*(y10_4_1 \
    - 1.0) + 0.5*(z10_4_1 - 1.0)*(z10_4_1 - 1.0) + 0.5*(y10_5_1 - \
    1.0)*(y10_5_1 - 1.0) + 0.5*(z10_5_1 - 1.0)*(z10_5_1 - 1.0) + 0.5*(y10_6_1 \
    - 1.0)*(y10_6_1 - 1.0) + 0.5*(z10_6_1 - 1.0)*(z10_6_1 - 1.0) + \
    0.5*(y10_7_1 - 1.0)*(y10_7_1 - 1.0) + 0.5*(z10_7_1 - 1.0)*(z10_7_1 - 1.0) \
    + 0.5*(y10_8_1 - 1.0)*(y10_8_1 - 1.0) + 0.5*(z10_8_1 - 1.0)*(z10_8_1 - \
    1.0) + 0.5*(y10_9_1 - 1.0)*(y10_9_1 - 1.0) + 0.5*(z10_9_1 - 1.0)*(z10_9_1 \
    - 1.0) + 0.5*(y10_1_2 - 1.0)*(y10_1_2 - 1.0) + 0.5*(z10_1_2 - \
    1.0)*(z10_1_2 - 1.0) + 0.5*(y10_2_2 - 1.0)*(y10_2_2 - 1.0) + 0.5*(z10_2_2 \
    - 1.0)*(z10_2_2 - 1.0) + 0.5*(y10_3_2 - 1.0)*(y10_3_2 - 1.0) + \
    0.5*(z10_3_2 - 1.0)*(z10_3_2 - 1.0) + 0.5*(y10_4_2 - 1.0)*(y10_4_2 - 1.0) \
    + 0.5*(z10_4_2 - 1.0)*(z10_4_2 - 1.0) + 0.5*(y10_5_2 - 1.0)*(y10_5_2 - \
    1.0) + 0.5*(z10_5_2 - 1.0)*(z10_5_2 - 1.0) + 0.5*(y10_6_2 - 1.0)*(y10_6_2 \
    - 1.0) + 0.5*(z10_6_2 - 1.0)*(z10_6_2 - 1.0) + 0.5*(y10_7_2 - \
    1.0)*(y10_7_2 - 1.0) + 0.5*(z10_7_2 - 1.0)*(z10_7_2 - 1.0) + 0.5*(y10_8_2 \
    - 1.0)*(y10_8_2 - 1.0) + 0.5*(z10_8_2 - 1.0)*(z10_8_2 - 1.0) + \
    0.5*(y10_9_2 - 1.0)*(y10_9_2 - 1.0) + 0.5*(z10_9_2 - 1.0)*(z10_9_2 - 1.0) \
    + 0.5*(y10_1_3 - 1.0)*(y10_1_3 - 1.0) + 0.5*(z10_1_3 - 1.0)*(z10_1_3 - \
    1.0) + 0.5*(y10_2_3 - 1.0)*(y10_2_3 - 1.0) + 0.5*(z10_2_3 - 1.0)*(z10_2_3 \
    - 1.0) + 0.5*(y10_3_3 - 1.0)*(y10_3_3 - 1.0) + 0.5*(z10_3_3 - \
    1.0)*(z10_3_3 - 1.0) + 0.5*(y10_4_3 - 1.0)*(y10_4_3 - 1.0) + 0.5*(z10_4_3 \
    - 1.0)*(z10_4_3 - 1.0) + 0.5*(y10_5_3 - 1.0)*(y10_5_3 - 1.0) + \
    0.5*(z10_5_3 - 1.0)*(z10_5_3 - 1.0) + 0.5*(y10_6_3 - 1.0)*(y10_6_3 - 1.0) \
    + 0.5*(z10_6_3 - 1.0)*(z10_6_3 - 1.0) + 0.5*(y10_7_3 - 1.0)*(y10_7_3 - \
    1.0) + 0.5*(z10_7_3 - 1.0)*(z10_7_3 - 1.0) + 0.5*(y10_8_3 - 1.0)*(y10_8_3 \
    - 1.0) + 0.5*(z10_8_3 - 1.0)*(z10_8_3 - 1.0) + 0.5*(y10_9_3 - \
    1.0)*(y10_9_3 - 1.0) + 0.5*(z10_9_3 - 1.0)*(z10_9_3 - 1.0) + 0.5*(y10_1_4 \
    - 1.0)*(y10_1_4 - 1.0) + 0.5*(z10_1_4 - 1.0)*(z10_1_4 - 1.0) + \
    0.5*(y10_2_4 - 1.0)*(y10_2_4 - 1.0) + 0.5*(z10_2_4 - 1.0)*(z10_2_4 - 1.0) \
    + 0.5*(y10_3_4 - 1.0)*(y10_3_4 - 1.0) + 0.5*(z10_3_4 - 1.0)*(z10_3_4 - \
    1.0) + 0.5*(y10_4_4 - 1.0)*(y10_4_4 - 1.0) + 0.5*(z10_4_4 - 1.0)*(z10_4_4 \
    - 1.0) + 0.5*(y10_5_4 - 1.0)*(y10_5_4 - 1.0) + 0.5*(z10_5_4 - \
    1.0)*(z10_5_4 - 1.0) + 0.5*(y10_6_4 - 1.0)*(y10_6_4 - 1.0) + 0.5*(z10_6_4 \
    - 1.0)*(z10_6_4 - 1.0) + 0.5*(y10_7_4 - 1.0)*(y10_7_4 - 1.0) + \
    0.5*(z10_7_4 - 1.0)*(z10_7_4 - 1.0) + 0.5*(y10_8_4 - 1.0)*(y10_8_4 - 1.0) \
    + 0.5*(z10_8_4 - 1.0)*(z10_8_4 - 1.0) + 0.5*(y10_9_4 - 1.0)*(y10_9_4 - \
    1.0) + 0.5*(z10_9_4 - 1.0)*(z10_9_4 - 1.0) + 0.5*(y10_1_5 - 1.0)*(y10_1_5 \
    - 1.0) + 0.5*(z10_1_5 - 1.0)*(z10_1_5 - 1.0) + 0.5*(y10_2_5 - \
    1.0)*(y10_2_5 - 1.0) + 0.5*(z10_2_5 - 1.0)*(z10_2_5 - 1.0) + 0.5*(y10_3_5 \
    - 1.0)*(y10_3_5 - 1.0) + 0.5*(z10_3_5 - 1.0)*(z10_3_5 - 1.0) + \
    0.5*(y10_4_5 - 1.0)*(y10_4_5 - 1.0) + 0.5*(z10_4_5 - 1.0)*(z10_4_5 - 1.0) \
    + 0.5*(y10_5_5 - 1.0)*(y10_5_5 - 1.0) + 0.5*(z10_5_5 - 1.0)*(z10_5_5 - \
    1.0) + 0.5*(y10_6_5 - 1.0)*(y10_6_5 - 1.0) + 0.5*(z10_6_5 - 1.0)*(z10_6_5 \
    - 1.0) + 0.5*(y10_7_5 - 1.0)*(y10_7_5 - 1.0) + 0.5*(z10_7_5 - \
    1.0)*(z10_7_5 - 1.0) + 0.5*(y10_8_5 - 1.0)*(y10_8_5 - 1.0) + 0.5*(z10_8_5 \
    - 1.0)*(z10_8_5 - 1.0) + 0.5*(y10_9_5 - 1.0)*(y10_9_5 - 1.0) + \
    0.5*(z10_9_5 - 1.0)*(z10_9_5 - 1.0) + 0.5*(y10_1_6 - 1.0)*(y10_1_6 - 1.0) \
    + 0.5*(z10_1_6 - 1.0)*(z10_1_6 - 1.0) + 0.5*(y10_2_6 - 1.0)*(y10_2_6 - \
    1.0) + 0.5*(z10_2_6 - 1.0)*(z10_2_6 - 1.0) + 0.5*(y10_3_6 - 1.0)*(y10_3_6 \
    - 1.0) + 0.5*(z10_3_6 - 1.0)*(z10_3_6 - 1.0) + 0.5*(y10_4_6 - \
    1.0)*(y10_4_6 - 1.0) + 0.5*(z10_4_6 - 1.0)*(z10_4_6 - 1.0) + 0.5*(y10_5_6 \
    - 1.0)*(y10_5_6 - 1.0) + 0.5*(z10_5_6 - 1.0)*(z10_5_6 - 1.0) + \
    0.5*(y10_6_6 - 1.0)*(y10_6_6 - 1.0) + 0.5*(z10_6_6 - 1.0)*(z10_6_6 - 1.0) \
    + 0.5*(y10_7_6 - 1.0)*(y10_7_6 - 1.0) + 0.5*(z10_7_6 - 1.0)*(z10_7_6 - \
    1.0) + 0.5*(y10_8_6 - 1.0)*(y10_8_6 - 1.0) + 0.5*(z10_8_6 - 1.0)*(z10_8_6 \
    - 1.0) + 0.5*(y10_9_6 - 1.0)*(y10_9_6 - 1.0) + 0.5*(z10_9_6 - \
    1.0)*(z10_9_6 - 1.0) + 0.5*(y10_1_7 - 1.0)*(y10_1_7 - 1.0) + 0.5*(z10_1_7 \
    - 1.0)*(z10_1_7 - 1.0) + 0.5*(y10_2_7 - 1.0)*(y10_2_7 - 1.0) + \
    0.5*(z10_2_7 - 1.0)*(z10_2_7 - 1.0) + 0.5*(y10_3_7 - 1.0)*(y10_3_7 - 1.0) \
    + 0.5*(z10_3_7 - 1.0)*(z10_3_7 - 1.0) + 0.5*(y10_4_7 - 1.0)*(y10_4_7 - \
    1.0) + 0.5*(z10_4_7 - 1.0)*(z10_4_7 - 1.0) + 0.5*(y10_5_7 - 1.0)*(y10_5_7 \
    - 1.0) + 0.5*(z10_5_7 - 1.0)*(z10_5_7 - 1.0) + 0.5*(y10_6_7 - \
    1.0)*(y10_6_7 - 1.0) + 0.5*(z10_6_7 - 1.0)*(z10_6_7 - 1.0) + 0.5*(y10_7_7 \
    - 1.0)*(y10_7_7 - 1.0) + 0.5*(z10_7_7 - 1.0)*(z10_7_7 - 1.0) + \
    0.5*(y10_8_7 - 1.0)*(y10_8_7 - 1.0) + 0.5*(z10_8_7 - 1.0)*(z10_8_7 - 1.0) \
    + 0.5*(y10_9_7 - 1.0)*(y10_9_7 - 1.0) + 0.5*(z10_9_7 - 1.0)*(z10_9_7 - \
    1.0) + 0.5*(y10_1_8 - 1.0)*(y10_1_8 - 1.0) + 0.5*(z10_1_8 - 1.0)*(z10_1_8 \
    - 1.0) + 0.5*(y10_2_8 - 1.0)*(y10_2_8 - 1.0) + 0.5*(z10_2_8 - \
    1.0)*(z10_2_8 - 1.0) + 0.5*(y10_3_8 - 1.0)*(y10_3_8 - 1.0) + 0.5*(z10_3_8 \
    - 1.0)*(z10_3_8 - 1.0) + 0.5*(y10_4_8 - 1.0)*(y10_4_8 - 1.0) + \
    0.5*(z10_4_8 - 1.0)*(z10_4_8 - 1.0) + 0.5*(y10_5_8 - 1.0)*(y10_5_8 - 1.0) \
    + 0.5*(z10_5_8 - 1.0)*(z10_5_8 - 1.0) + 0.5*(y10_6_8 - 1.0)*(y10_6_8 - \
    1.0) + 0.5*(z10_6_8 - 1.0)*(z10_6_8 - 1.0) + 0.5*(y10_7_8 - 1.0)*(y10_7_8 \
    - 1.0) + 0.5*(z10_7_8 - 1.0)*(z10_7_8 - 1.0) + 0.5*(y10_8_8 - \
    1.0)*(y10_8_8 - 1.0) + 0.5*(z10_8_8 - 1.0)*(z10_8_8 - 1.0) + 0.5*(y10_9_8 \
    - 1.0)*(y10_9_8 - 1.0) + 0.5*(z10_9_8 - 1.0)*(z10_9_8 - 1.0) + \
    0.5*(y10_1_9 - 1.0)*(y10_1_9 - 1.0) + 0.5*(z10_1_9 - 1.0)*(z10_1_9 - 1.0) \
    + 0.5*(y10_2_9 - 1.0)*(y10_2_9 - 1.0) + 0.5*(z10_2_9 - 1.0)*(z10_2_9 - \
    1.0) + 0.5*(y10_3_9 - 1.0)*(y10_3_9 - 1.0) + 0.5*(z10_3_9 - 1.0)*(z10_3_9 \
    - 1.0) + 0.5*(y10_4_9 - 1.0)*(y10_4_9 - 1.0) + 0.5*(z10_4_9 - \
    1.0)*(z10_4_9 - 1.0) + 0.5*(y10_5_9 - 1.0)*(y10_5_9 - 1.0) + 0.5*(z10_5_9 \
    - 1.0)*(z10_5_9 - 1.0) + 0.5*(y10_6_9 - 1.0)*(y10_6_9 - 1.0) + \
    0.5*(z10_6_9 - 1.0)*(z10_6_9 - 1.0) + 0.5*(y10_7_9 - 1.0)*(y10_7_9 - 1.0) \
    + 0.5*(z10_7_9 - 1.0)*(z10_7_9 - 1.0) + 0.5*(y10_8_9 - 1.0)*(y10_8_9 - \
    1.0) + 0.5*(z10_8_9 - 1.0)*(z10_8_9 - 1.0) + 0.5*(y10_9_9 - 1.0)*(y10_9_9 \
    - 1.0) + 0.5*(z10_9_9 - 1.0)*(z10_9_9 - 1.0) + 0.5*(x1_10_1 - \
    1.0)*(x1_10_1 - 1.0) + 0.5*(z1_10_1 - 1.0)*(z1_10_1 - 1.0) + 0.5*(x2_10_1 \
    - 1.0)*(x2_10_1 - 1.0) + 0.5*(z2_10_1 - 1.0)*(z2_10_1 - 1.0) + \
    0.5*(x3_10_1 - 1.0)*(x3_10_1 - 1.0) + 0.5*(z3_10_1 - 1.0)*(z3_10_1 - 1.0) \
    + 0.5*(x4_10_1 - 1.0)*(x4_10_1 - 1.0) + 0.5*(z4_10_1 - 1.0)*(z4_10_1 - \
    1.0) + 0.5*(x5_10_1 - 1.0)*(x5_10_1 - 1.0) + 0.5*(z5_10_1 - 1.0)*(z5_10_1 \
    - 1.0) + 0.5*(x6_10_1 - 1.0)*(x6_10_1 - 1.0) + 0.5*(z6_10_1 - \
    1.0)*(z6_10_1 - 1.0) + 0.5*(x7_10_1 - 1.0)*(x7_10_1 - 1.0) + 0.5*(z7_10_1 \
    - 1.0)*(z7_10_1 - 1.0) + 0.5*(x8_10_1 - 1.0)*(x8_10_1 - 1.0) + \
    0.5*(z8_10_1 - 1.0)*(z8_10_1 - 1.0) + 0.5*(x9_10_1 - 1.0)*(x9_10_1 - 1.0) \
    + 0.5*(z9_10_1 - 1.0)*(z9_10_1 - 1.0) + 0.5*(x1_10_2 - 1.0)*(x1_10_2 - \
    1.0) + 0.5*(z1_10_2 - 1.0)*(z1_10_2 - 1.0) + 0.5*(x2_10_2 - 1.0)*(x2_10_2 \
    - 1.0) + 0.5*(z2_10_2 - 1.0)*(z2_10_2 - 1.0) + 0.5*(x3_10_2 - \
    1.0)*(x3_10_2 - 1.0) + 0.5*(z3_10_2 - 1.0)*(z3_10_2 - 1.0) + 0.5*(x4_10_2 \
    - 1.0)*(x4_10_2 - 1.0) + 0.5*(z4_10_2 - 1.0)*(z4_10_2 - 1.0) + \
    0.5*(x5_10_2 - 1.0)*(x5_10_2 - 1.0) + 0.5*(z5_10_2 - 1.0)*(z5_10_2 - 1.0) \
    + 0.5*(x6_10_2 - 1.0)*(x6_10_2 - 1.0) + 0.5*(z6_10_2 - 1.0)*(z6_10_2 - \
    1.0) + 0.5*(x7_10_2 - 1.0)*(x7_10_2 - 1.0) + 0.5*(z7_10_2 - 1.0)*(z7_10_2 \
    - 1.0) + 0.5*(x8_10_2 - 1.0)*(x8_10_2 - 1.0) + 0.5*(z8_10_2 - \
    1.0)*(z8_10_2 - 1.0) + 0.5*(x9_10_2 - 1.0)*(x9_10_2 - 1.0) + 0.5*(z9_10_2 \
    - 1.0)*(z9_10_2 - 1.0) + 0.5*(x1_10_3 - 1.0)*(x1_10_3 - 1.0) + \
    0.5*(z1_10_3 - 1.0)*(z1_10_3 - 1.0) + 0.5*(x2_10_3 - 1.0)*(x2_10_3 - 1.0) \
    + 0.5*(z2_10_3 - 1.0)*(z2_10_3 - 1.0) + 0.5*(x3_10_3 - 1.0)*(x3_10_3 - \
    1.0) + 0.5*(z3_10_3 - 1.0)*(z3_10_3 - 1.0) + 0.5*(x4_10_3 - 1.0)*(x4_10_3 \
    - 1.0) + 0.5*(z4_10_3 - 1.0)*(z4_10_3 - 1.0) + 0.5*(x5_10_3 - \
    1.0)*(x5_10_3 - 1.0) + 0.5*(z5_10_3 - 1.0)*(z5_10_3 - 1.0) + 0.5*(x6_10_3 \
    - 1.0)*(x6_10_3 - 1.0) + 0.5*(z6_10_3 - 1.0)*(z6_10_3 - 1.0) + \
    0.5*(x7_10_3 - 1.0)*(x7_10_3 - 1.0) + 0.5*(z7_10_3 - 1.0)*(z7_10_3 - 1.0) \
    + 0.5*(x8_10_3 - 1.0)*(x8_10_3 - 1.0) + 0.5*(z8_10_3 - 1.0)*(z8_10_3 - \
    1.0) + 0.5*(x9_10_3 - 1.0)*(x9_10_3 - 1.0) + 0.5*(z9_10_3 - 1.0)*(z9_10_3 \
    - 1.0) + 0.5*(x1_10_4 - 1.0)*(x1_10_4 - 1.0) + 0.5*(z1_10_4 - \
    1.0)*(z1_10_4 - 1.0) + 0.5*(x2_10_4 - 1.0)*(x2_10_4 - 1.0) + 0.5*(z2_10_4 \
    - 1.0)*(z2_10_4 - 1.0) + 0.5*(x3_10_4 - 1.0)*(x3_10_4 - 1.0) + \
    0.5*(z3_10_4 - 1.0)*(z3_10_4 - 1.0) + 0.5*(x4_10_4 - 1.0)*(x4_10_4 - 1.0) \
    + 0.5*(z4_10_4 - 1.0)*(z4_10_4 - 1.0) + 0.5*(x5_10_4 - 1.0)*(x5_10_4 - \
    1.0) + 0.5*(z5_10_4 - 1.0)*(z5_10_4 - 1.0) + 0.5*(x6_10_4 - 1.0)*(x6_10_4 \
    - 1.0) + 0.5*(z6_10_4 - 1.0)*(z6_10_4 - 1.0) + 0.5*(x7_10_4 - \
    1.0)*(x7_10_4 - 1.0) + 0.5*(z7_10_4 - 1.0)*(z7_10_4 - 1.0) + 0.5*(x8_10_4 \
    - 1.0)*(x8_10_4 - 1.0) + 0.5*(z8_10_4 - 1.0)*(z8_10_4 - 1.0) + \
    0.5*(x9_10_4 - 1.0)*(x9_10_4 - 1.0) + 0.5*(z9_10_4 - 1.0)*(z9_10_4 - 1.0) \
    + 0.5*(x1_10_5 - 1.0)*(x1_10_5 - 1.0) + 0.5*(z1_10_5 - 1.0)*(z1_10_5 - \
    1.0) + 0.5*(x2_10_5 - 1.0)*(x2_10_5 - 1.0) + 0.5*(z2_10_5 - 1.0)*(z2_10_5 \
    - 1.0) + 0.5*(x3_10_5 - 1.0)*(x3_10_5 - 1.0) + 0.5*(z3_10_5 - \
    1.0)*(z3_10_5 - 1.0) + 0.5*(x4_10_5 - 1.0)*(x4_10_5 - 1.0) + 0.5*(z4_10_5 \
    - 1.0)*(z4_10_5 - 1.0) + 0.5*(x5_10_5 - 1.0)*(x5_10_5 - 1.0) + \
    0.5*(z5_10_5 - 1.0)*(z5_10_5 - 1.0) + 0.5*(x6_10_5 - 1.0)*(x6_10_5 - 1.0) \
    + 0.5*(z6_10_5 - 1.0)*(z6_10_5 - 1.0) + 0.5*(x7_10_5 - 1.0)*(x7_10_5 - \
    1.0) + 0.5*(z7_10_5 - 1.0)*(z7_10_5 - 1.0) + 0.5*(x8_10_5 - 1.0)*(x8_10_5 \
    - 1.0) + 0.5*(z8_10_5 - 1.0)*(z8_10_5 - 1.0) + 0.5*(x9_10_5 - \
    1.0)*(x9_10_5 - 1.0) + 0.5*(z9_10_5 - 1.0)*(z9_10_5 - 1.0) + 0.5*(x1_10_6 \
    - 1.0)*(x1_10_6 - 1.0) + 0.5*(z1_10_6 - 1.0)*(z1_10_6 - 1.0) + \
    0.5*(x2_10_6 - 1.0)*(x2_10_6 - 1.0) + 0.5*(z2_10_6 - 1.0)*(z2_10_6 - 1.0) \
    + 0.5*(x3_10_6 - 1.0)*(x3_10_6 - 1.0) + 0.5*(z3_10_6 - 1.0)*(z3_10_6 - \
    1.0) + 0.5*(x4_10_6 - 1.0)*(x4_10_6 - 1.0) + 0.5*(z4_10_6 - 1.0)*(z4_10_6 \
    - 1.0) + 0.5*(x5_10_6 - 1.0)*(x5_10_6 - 1.0) + 0.5*(z5_10_6 - \
    1.0)*(z5_10_6 - 1.0) + 0.5*(x6_10_6 - 1.0)*(x6_10_6 - 1.0) + 0.5*(z6_10_6 \
    - 1.0)*(z6_10_6 - 1.0) + 0.5*(x7_10_6 - 1.0)*(x7_10_6 - 1.0) + \
    0.5*(z7_10_6 - 1.0)*(z7_10_6 - 1.0) + 0.5*(x8_10_6 - 1.0)*(x8_10_6 - 1.0) \
    + 0.5*(z8_10_6 - 1.0)*(z8_10_6 - 1.0) + 0.5*(x9_10_6 - 1.0)*(x9_10_6 - \
    1.0) + 0.5*(z9_10_6 - 1.0)*(z9_10_6 - 1.0) + 0.5*(x1_10_7 - 1.0)*(x1_10_7 \
    - 1.0) + 0.5*(z1_10_7 - 1.0)*(z1_10_7 - 1.0) + 0.5*(x2_10_7 - \
    1.0)*(x2_10_7 - 1.0) + 0.5*(z2_10_7 - 1.0)*(z2_10_7 - 1.0) + 0.5*(x3_10_7 \
    - 1.0)*(x3_10_7 - 1.0) + 0.5*(z3_10_7 - 1.0)*(z3_10_7 - 1.0) + \
    0.5*(x4_10_7 - 1.0)*(x4_10_7 - 1.0) + 0.5*(z4_10_7 - 1.0)*(z4_10_7 - 1.0) \
    + 0.5*(x5_10_7 - 1.0)*(x5_10_7 - 1.0) + 0.5*(z5_10_7 - 1.0)*(z5_10_7 - \
    1.0) + 0.5*(x6_10_7 - 1.0)*(x6_10_7 - 1.0) + 0.5*(z6_10_7 - 1.0)*(z6_10_7 \
    - 1.0) + 0.5*(x7_10_7 - 1.0)*(x7_10_7 - 1.0) + 0.5*(z7_10_7 - \
    1.0)*(z7_10_7 - 1.0) + 0.5*(x8_10_7 - 1.0)*(x8_10_7 - 1.0) + 0.5*(z8_10_7 \
    - 1.0)*(z8_10_7 - 1.0) + 0.5*(x9_10_7 - 1.0)*(x9_10_7 - 1.0) + \
    0.5*(z9_10_7 - 1.0)*(z9_10_7 - 1.0) + 0.5*(x1_10_8 - 1.0)*(x1_10_8 - 1.0) \
    + 0.5*(z1_10_8 - 1.0)*(z1_10_8 - 1.0) + 0.5*(x2_10_8 - 1.0)*(x2_10_8 - \
    1.0) + 0.5*(z2_10_8 - 1.0)*(z2_10_8 - 1.0) + 0.5*(x3_10_8 - 1.0)*(x3_10_8 \
    - 1.0) + 0.5*(z3_10_8 - 1.0)*(z3_10_8 - 1.0) + 0.5*(x4_10_8 - \
    1.0)*(x4_10_8 - 1.0) + 0.5*(z4_10_8 - 1.0)*(z4_10_8 - 1.0) + 0.5*(x5_10_8 \
    - 1.0)*(x5_10_8 - 1.0) + 0.5*(z5_10_8 - 1.0)*(z5_10_8 - 1.0) + \
    0.5*(x6_10_8 - 1.0)*(x6_10_8 - 1.0) + 0.5*(z6_10_8 - 1.0)*(z6_10_8 - 1.0) \
    + 0.5*(x7_10_8 - 1.0)*(x7_10_8 - 1.0) + 0.5*(z7_10_8 - 1.0)*(z7_10_8 - \
    1.0) + 0.5*(x8_10_8 - 1.0)*(x8_10_8 - 1.0) + 0.5*(z8_10_8 - 1.0)*(z8_10_8 \
    - 1.0) + 0.5*(x9_10_8 - 1.0)*(x9_10_8 - 1.0) + 0.5*(z9_10_8 - \
    1.0)*(z9_10_8 - 1.0) + 0.5*(x1_10_9 - 1.0)*(x1_10_9 - 1.0) + 0.5*(z1_10_9 \
    - 1.0)*(z1_10_9 - 1.0) + 0.5*(x2_10_9 - 1.0)*(x2_10_9 - 1.0) + \
    0.5*(z2_10_9 - 1.0)*(z2_10_9 - 1.0) + 0.5*(x3_10_9 - 1.0)*(x3_10_9 - 1.0) \
    + 0.5*(z3_10_9 - 1.0)*(z3_10_9 - 1.0) + 0.5*(x4_10_9 - 1.0)*(x4_10_9 - \
    1.0) + 0.5*(z4_10_9 - 1.0)*(z4_10_9 - 1.0) + 0.5*(x5_10_9 - 1.0)*(x5_10_9 \
    - 1.0) + 0.5*(z5_10_9 - 1.0)*(z5_10_9 - 1.0) + 0.5*(x6_10_9 - \
    1.0)*(x6_10_9 - 1.0) + 0.5*(z6_10_9 - 1.0)*(z6_10_9 - 1.0) + 0.5*(x7_10_9 \
    - 1.0)*(x7_10_9 - 1.0) + 0.5*(z7_10_9 - 1.0)*(z7_10_9 - 1.0) + \
    0.5*(x8_10_9 - 1.0)*(x8_10_9 - 1.0) + 0.5*(z8_10_9 - 1.0)*(z8_10_9 - 1.0) \
    + 0.5*(x9_10_9 - 1.0)*(x9_10_9 - 1.0) + 0.5*(z9_10_9 - 1.0)*(z9_10_9 - \
    1.0) + 0.5*(x1_1_10 - 1.0)*(x1_1_10 - 1.0) + 0.5*(y1_1_10 - 1.0)*(y1_1_10 \
    - 1.0) + 0.5*(x2_1_10 - 1.0)*(x2_1_10 - 1.0) + 0.5*(y2_1_10 - \
    1.0)*(y2_1_10 - 1.0) + 0.5*(x3_1_10 - 1.0)*(x3_1_10 - 1.0) + 0.5*(y3_1_10 \
    - 1.0)*(y3_1_10 - 1.0) + 0.5*(x4_1_10 - 1.0)*(x4_1_10 - 1.0) + \
    0.5*(y4_1_10 - 1.0)*(y4_1_10 - 1.0) + 0.5*(x5_1_10 - 1.0)*(x5_1_10 - 1.0) \
    + 0.5*(y5_1_10 - 1.0)*(y5_1_10 - 1.0) + 0.5*(x6_1_10 - 1.0)*(x6_1_10 - \
    1.0) + 0.5*(y6_1_10 - 1.0)*(y6_1_10 - 1.0) + 0.5*(x7_1_10 - 1.0)*(x7_1_10 \
    - 1.0) + 0.5*(y7_1_10 - 1.0)*(y7_1_10 - 1.0) + 0.5*(x8_1_10 - \
    1.0)*(x8_1_10 - 1.0) + 0.5*(y8_1_10 - 1.0)*(y8_1_10 - 1.0) + 0.5*(x9_1_10 \
    - 1.0)*(x9_1_10 - 1.0) + 0.5*(y9_1_10 - 1.0)*(y9_1_10 - 1.0) + \
    0.5*(x1_2_10 - 1.0)*(x1_2_10 - 1.0) + 0.5*(y1_2_10 - 1.0)*(y1_2_10 - 1.0) \
    + 0.5*(x2_2_10 - 1.0)*(x2_2_10 - 1.0) + 0.5*(y2_2_10 - 1.0)*(y2_2_10 - \
    1.0) + 0.5*(x3_2_10 - 1.0)*(x3_2_10 - 1.0) + 0.5*(y3_2_10 - 1.0)*(y3_2_10 \
    - 1.0) + 0.5*(x4_2_10 - 1.0)*(x4_2_10 - 1.0) + 0.5*(y4_2_10 - \
    1.0)*(y4_2_10 - 1.0) + 0.5*(x5_2_10 - 1.0)*(x5_2_10 - 1.0) + 0.5*(y5_2_10 \
    - 1.0)*(y5_2_10 - 1.0) + 0.5*(x6_2_10 - 1.0)*(x6_2_10 - 1.0) + \
    0.5*(y6_2_10 - 1.0)*(y6_2_10 - 1.0) + 0.5*(x7_2_10 - 1.0)*(x7_2_10 - 1.0) \
    + 0.5*(y7_2_10 - 1.0)*(y7_2_10 - 1.0) + 0.5*(x8_2_10 - 1.0)*(x8_2_10 - \
    1.0) + 0.5*(y8_2_10 - 1.0)*(y8_2_10 - 1.0) + 0.5*(x9_2_10 - 1.0)*(x9_2_10 \
    - 1.0) + 0.5*(y9_2_10 - 1.0)*(y9_2_10 - 1.0) + 0.5*(x1_3_10 - \
    1.0)*(x1_3_10 - 1.0) + 0.5*(y1_3_10 - 1.0)*(y1_3_10 - 1.0) + 0.5*(x2_3_10 \
    - 1.0)*(x2_3_10 - 1.0) + 0.5*(y2_3_10 - 1.0)*(y2_3_10 - 1.0) + \
    0.5*(x3_3_10 - 1.0)*(x3_3_10 - 1.0) + 0.5*(y3_3_10 - 1.0)*(y3_3_10 - 1.0) \
    + 0.5*(x4_3_10 - 1.0)*(x4_3_10 - 1.0) + 0.5*(y4_3_10 - 1.0)*(y4_3_10 - \
    1.0) + 0.5*(x5_3_10 - 1.0)*(x5_3_10 - 1.0) + 0.5*(y5_3_10 - 1.0)*(y5_3_10 \
    - 1.0) + 0.5*(x6_3_10 - 1.0)*(x6_3_10 - 1.0) + 0.5*(y6_3_10 - \
    1.0)*(y6_3_10 - 1.0) + 0.5*(x7_3_10 - 1.0)*(x7_3_10 - 1.0) + 0.5*(y7_3_10 \
    - 1.0)*(y7_3_10 - 1.0) + 0.5*(x8_3_10 - 1.0)*(x8_3_10 - 1.0) + \
    0.5*(y8_3_10 - 1.0)*(y8_3_10 - 1.0) + 0.5*(x9_3_10 - 1.0)*(x9_3_10 - 1.0) \
    + 0.5*(y9_3_10 - 1.0)*(y9_3_10 - 1.0) + 0.5*(x1_4_10 - 1.0)*(x1_4_10 - \
    1.0) + 0.5*(y1_4_10 - 1.0)*(y1_4_10 - 1.0) + 0.5*(x2_4_10 - 1.0)*(x2_4_10 \
    - 1.0) + 0.5*(y2_4_10 - 1.0)*(y2_4_10 - 1.0) + 0.5*(x3_4_10 - \
    1.0)*(x3_4_10 - 1.0) + 0.5*(y3_4_10 - 1.0)*(y3_4_10 - 1.0) + 0.5*(x4_4_10 \
    - 1.0)*(x4_4_10 - 1.0) + 0.5*(y4_4_10 - 1.0)*(y4_4_10 - 1.0) + \
    0.5*(x5_4_10 - 1.0)*(x5_4_10 - 1.0) + 0.5*(y5_4_10 - 1.0)*(y5_4_10 - 1.0) \
    + 0.5*(x6_4_10 - 1.0)*(x6_4_10 - 1.0) + 0.5*(y6_4_10 - 1.0)*(y6_4_10 - \
    1.0) + 0.5*(x7_4_10 - 1.0)*(x7_4_10 - 1.0) + 0.5*(y7_4_10 - 1.0)*(y7_4_10 \
    - 1.0) + 0.5*(x8_4_10 - 1.0)*(x8_4_10 - 1.0) + 0.5*(y8_4_10 - \
    1.0)*(y8_4_10 - 1.0) + 0.5*(x9_4_10 - 1.0)*(x9_4_10 - 1.0) + 0.5*(y9_4_10 \
    - 1.0)*(y9_4_10 - 1.0) + 0.5*(x1_5_10 - 1.0)*(x1_5_10 - 1.0) + \
    0.5*(y1_5_10 - 1.0)*(y1_5_10 - 1.0) + 0.5*(x2_5_10 - 1.0)*(x2_5_10 - 1.0) \
    + 0.5*(y2_5_10 - 1.0)*(y2_5_10 - 1.0) + 0.5*(x3_5_10 - 1.0)*(x3_5_10 - \
    1.0) + 0.5*(y3_5_10 - 1.0)*(y3_5_10 - 1.0) + 0.5*(x4_5_10 - 1.0)*(x4_5_10 \
    - 1.0) + 0.5*(y4_5_10 - 1.0)*(y4_5_10 - 1.0) + 0.5*(x5_5_10 - \
    1.0)*(x5_5_10 - 1.0) + 0.5*(y5_5_10 - 1.0)*(y5_5_10 - 1.0) + 0.5*(x6_5_10 \
    - 1.0)*(x6_5_10 - 1.0) + 0.5*(y6_5_10 - 1.0)*(y6_5_10 - 1.0) + \
    0.5*(x7_5_10 - 1.0)*(x7_5_10 - 1.0) + 0.5*(y7_5_10 - 1.0)*(y7_5_10 - 1.0) \
    + 0.5*(x8_5_10 - 1.0)*(x8_5_10 - 1.0) + 0.5*(y8_5_10 - 1.0)*(y8_5_10 - \
    1.0) + 0.5*(x9_5_10 - 1.0)*(x9_5_10 - 1.0) + 0.5*(y9_5_10 - 1.0)*(y9_5_10 \
    - 1.0) + 0.5*(x1_6_10 - 1.0)*(x1_6_10 - 1.0) + 0.5*(y1_6_10 - \
    1.0)*(y1_6_10 - 1.0) + 0.5*(x2_6_10 - 1.0)*(x2_6_10 - 1.0) + 0.5*(y2_6_10 \
    - 1.0)*(y2_6_10 - 1.0) + 0.5*(x3_6_10 - 1.0)*(x3_6_10 - 1.0) + \
    0.5*(y3_6_10 - 1.0)*(y3_6_10 - 1.0) + 0.5*(x4_6_10 - 1.0)*(x4_6_10 - 1.0) \
    + 0.5*(y4_6_10 - 1.0)*(y4_6_10 - 1.0) + 0.5*(x5_6_10 - 1.0)*(x5_6_10 - \
    1.0) + 0.5*(y5_6_10 - 1.0)*(y5_6_10 - 1.0) + 0.5*(x6_6_10 - 1.0)*(x6_6_10 \
    - 1.0) + 0.5*(y6_6_10 - 1.0)*(y6_6_10 - 1.0) + 0.5*(x7_6_10 - \
    1.0)*(x7_6_10 - 1.0) + 0.5*(y7_6_10 - 1.0)*(y7_6_10 - 1.0) + 0.5*(x8_6_10 \
    - 1.0)*(x8_6_10 - 1.0) + 0.5*(y8_6_10 - 1.0)*(y8_6_10 - 1.0) + \
    0.5*(x9_6_10 - 1.0)*(x9_6_10 - 1.0) + 0.5*(y9_6_10 - 1.0)*(y9_6_10 - 1.0) \
    + 0.5*(x1_7_10 - 1.0)*(x1_7_10 - 1.0) + 0.5*(y1_7_10 - 1.0)*(y1_7_10 - \
    1.0) + 0.5*(x2_7_10 - 1.0)*(x2_7_10 - 1.0) + 0.5*(y2_7_10 - 1.0)*(y2_7_10 \
    - 1.0) + 0.5*(x3_7_10 - 1.0)*(x3_7_10 - 1.0) + 0.5*(y3_7_10 - \
    1.0)*(y3_7_10 - 1.0) + 0.5*(x4_7_10 - 1.0)*(x4_7_10 - 1.0) + 0.5*(y4_7_10 \
    - 1.0)*(y4_7_10 - 1.0) + 0.5*(x5_7_10 - 1.0)*(x5_7_10 - 1.0) + \
    0.5*(y5_7_10 - 1.0)*(y5_7_10 - 1.0) + 0.5*(x6_7_10 - 1.0)*(x6_7_10 - 1.0) \
    + 0.5*(y6_7_10 - 1.0)*(y6_7_10 - 1.0) + 0.5*(x7_7_10 - 1.0)*(x7_7_10 - \
    1.0) + 0.5*(y7_7_10 - 1.0)*(y7_7_10 - 1.0) + 0.5*(x8_7_10 - 1.0)*(x8_7_10 \
    - 1.0) + 0.5*(y8_7_10 - 1.0)*(y8_7_10 - 1.0) + 0.5*(x9_7_10 - \
    1.0)*(x9_7_10 - 1.0) + 0.5*(y9_7_10 - 1.0)*(y9_7_10 - 1.0) + 0.5*(x1_8_10 \
    - 1.0)*(x1_8_10 - 1.0) + 0.5*(y1_8_10 - 1.0)*(y1_8_10 - 1.0) + \
    0.5*(x2_8_10 - 1.0)*(x2_8_10 - 1.0) + 0.5*(y2_8_10 - 1.0)*(y2_8_10 - 1.0) \
    + 0.5*(x3_8_10 - 1.0)*(x3_8_10 - 1.0) + 0.5*(y3_8_10 - 1.0)*(y3_8_10 - \
    1.0) + 0.5*(x4_8_10 - 1.0)*(x4_8_10 - 1.0) + 0.5*(y4_8_10 - 1.0)*(y4_8_10 \
    - 1.0) + 0.5*(x5_8_10 - 1.0)*(x5_8_10 - 1.0) + 0.5*(y5_8_10 - \
    1.0)*(y5_8_10 - 1.0) + 0.5*(x6_8_10 - 1.0)*(x6_8_10 - 1.0) + 0.5*(y6_8_10 \
    - 1.0)*(y6_8_10 - 1.0) + 0.5*(x7_8_10 - 1.0)*(x7_8_10 - 1.0) + \
    0.5*(y7_8_10 - 1.0)*(y7_8_10 - 1.0) + 0.5*(x8_8_10 - 1.0)*(x8_8_10 - 1.0) \
    + 0.5*(y8_8_10 - 1.0)*(y8_8_10 - 1.0) + 0.5*(x9_8_10 - 1.0)*(x9_8_10 - \
    1.0) + 0.5*(y9_8_10 - 1.0)*(y9_8_10 - 1.0) + 0.5*(x1_9_10 - 1.0)*(x1_9_10 \
    - 1.0) + 0.5*(y1_9_10 - 1.0)*(y1_9_10 - 1.0) + 0.5*(x2_9_10 - \
    1.0)*(x2_9_10 - 1.0) + 0.5*(y2_9_10 - 1.0)*(y2_9_10 - 1.0) + 0.5*(x3_9_10 \
    - 1.0)*(x3_9_10 - 1.0) + 0.5*(y3_9_10 - 1.0)*(y3_9_10 - 1.0) + \
    0.5*(x4_9_10 - 1.0)*(x4_9_10 - 1.0) + 0.5*(y4_9_10 - 1.0)*(y4_9_10 - 1.0) \
    + 0.5*(x5_9_10 - 1.0)*(x5_9_10 - 1.0) + 0.5*(y5_9_10 - 1.0)*(y5_9_10 - \
    1.0) + 0.5*(x6_9_10 - 1.0)*(x6_9_10 - 1.0) + 0.5*(y6_9_10 - 1.0)*(y6_9_10 \
    - 1.0) + 0.5*(x7_9_10 - 1.0)*(x7_9_10 - 1.0) + 0.5*(y7_9_10 - \
    1.0)*(y7_9_10 - 1.0) + 0.5*(x8_9_10 - 1.0)*(x8_9_10 - 1.0) + 0.5*(y8_9_10 \
    - 1.0)*(y8_9_10 - 1.0) + 0.5*(x9_9_10 - 1.0)*(x9_9_10 - 1.0) + \
    0.5*(y9_9_10 - 1.0)*(y9_9_10 - 1.0))

model.add(x1_1_1 + y1_1_1 + z1_1_1 + y0_1_1 + z0_1_1 + x1_0_1 + z1_0_1 + x1_1_0 + y1_1_0 - 1.0 == 0)
model.add(-x1_1_1 + x2_1_1 + y2_1_1 + z2_1_1 + x2_0_1 + z2_0_1 + x2_1_0 + y2_1_0 - 1.0 == 0)
model.add(-x2_1_1 + x3_1_1 + y3_1_1 + z3_1_1 + x3_0_1 + z3_0_1 + x3_1_0 + y3_1_0 - 1.0 == 0)
model.add(-x3_1_1 + x4_1_1 + y4_1_1 + z4_1_1 + x4_0_1 + z4_0_1 + x4_1_0 + y4_1_0 - 1.0 == 0)
model.add(-x4_1_1 + x5_1_1 + y5_1_1 + z5_1_1 + x5_0_1 + z5_0_1 + x5_1_0 + y5_1_0 - 1.0 == 0)
model.add(-x5_1_1 + x6_1_1 + y6_1_1 + z6_1_1 + x6_0_1 + z6_0_1 + x6_1_0 + y6_1_0 - 1.0 == 0)
model.add(-x6_1_1 + x7_1_1 + y7_1_1 + z7_1_1 + x7_0_1 + z7_0_1 + x7_1_0 + y7_1_0 - 1.0 == 0)
model.add(-x7_1_1 + x8_1_1 + y8_1_1 + z8_1_1 + x8_0_1 + z8_0_1 + x8_1_0 + y8_1_0 - 1.0 == 0)
model.add(-x8_1_1 + x9_1_1 + y9_1_1 + z9_1_1 + x9_0_1 + z9_0_1 + x9_1_0 + y9_1_0 - 1.0 == 0)
model.add(-x9_1_1 + y10_1_1 + z10_1_1 + y11_1_1 + z11_1_1 + x10_0_1 + z10_0_1 + x10_1_0 + y10_1_0 - 1.0 == 0)
model.add(-y1_1_1 + x1_2_1 + y1_2_1 + z1_2_1 + y0_2_1 + z0_2_1 + x1_2_0 + y1_2_0 - 1.0 == 0)
model.add(-y2_1_1 - x1_2_1 + x2_2_1 + y2_2_1 + z2_2_1 + x2_2_0 + y2_2_0 - 1.0 == 0)
model.add(-y3_1_1 - x2_2_1 + x3_2_1 + y3_2_1 + z3_2_1 + x3_2_0 + y3_2_0 - 1.0 == 0)
model.add(-y4_1_1 - x3_2_1 + x4_2_1 + y4_2_1 + z4_2_1 + x4_2_0 + y4_2_0 - 1.0 == 0)
model.add(-y5_1_1 - x4_2_1 + x5_2_1 + y5_2_1 + z5_2_1 + x5_2_0 + y5_2_0 - 1.0 == 0)
model.add(-y6_1_1 - x5_2_1 + x6_2_1 + y6_2_1 + z6_2_1 + x6_2_0 + y6_2_0 - 1.0 == 0)
model.add(-y7_1_1 - x6_2_1 + x7_2_1 + y7_2_1 + z7_2_1 + x7_2_0 + y7_2_0 - 1.0 == 0)
model.add(-y8_1_1 - x7_2_1 + x8_2_1 + y8_2_1 + z8_2_1 + x8_2_0 + y8_2_0 - 1.0 == 0)
model.add(-y9_1_1 - x8_2_1 + x9_2_1 + y9_2_1 + z9_2_1 + x9_2_0 + y9_2_0 - 1.0 == 0)
model.add(-x9_2_1 - y10_1_1 + y10_2_1 + z10_2_1 + y11_2_1 + z11_2_1 + x10_2_0 + y10_2_0 - 1.0 == 0)
model.add(-y1_2_1 + x1_3_1 + y1_3_1 + z1_3_1 + y0_3_1 + z0_3_1 + x1_3_0 + y1_3_0 - 1.0 == 0)
model.add(-y2_2_1 - x1_3_1 + x2_3_1 + y2_3_1 + z2_3_1 + x2_3_0 + y2_3_0 - 1.0 == 0)
model.add(-y3_2_1 - x2_3_1 + x3_3_1 + y3_3_1 + z3_3_1 + x3_3_0 + y3_3_0 - 1.0 == 0)
model.add(-y4_2_1 - x3_3_1 + x4_3_1 + y4_3_1 + z4_3_1 + x4_3_0 + y4_3_0 - 1.0 == 0)
model.add(-y5_2_1 - x4_3_1 + x5_3_1 + y5_3_1 + z5_3_1 + x5_3_0 + y5_3_0 - 1.0 == 0)
model.add(-y6_2_1 - x5_3_1 + x6_3_1 + y6_3_1 + z6_3_1 + x6_3_0 + y6_3_0 - 1.0 == 0)
model.add(-y7_2_1 - x6_3_1 + x7_3_1 + y7_3_1 + z7_3_1 + x7_3_0 + y7_3_0 - 1.0 == 0)
model.add(-y8_2_1 - x7_3_1 + x8_3_1 + y8_3_1 + z8_3_1 + x8_3_0 + y8_3_0 - 1.0 == 0)
model.add(-y9_2_1 - x8_3_1 + x9_3_1 + y9_3_1 + z9_3_1 + x9_3_0 + y9_3_0 - 1.0 == 0)
model.add(-x9_3_1 - y10_2_1 + y10_3_1 + z10_3_1 + y11_3_1 + z11_3_1 + x10_3_0 + y10_3_0 - 1.0 == 0)
model.add(-y1_3_1 + x1_4_1 + y1_4_1 + z1_4_1 + y0_4_1 + z0_4_1 + x1_4_0 + y1_4_0 - 1.0 == 0)
model.add(-y2_3_1 - x1_4_1 + x2_4_1 + y2_4_1 + z2_4_1 + x2_4_0 + y2_4_0 - 1.0 == 0)
model.add(-y3_3_1 - x2_4_1 + x3_4_1 + y3_4_1 + z3_4_1 + x3_4_0 + y3_4_0 - 1.0 == 0)
model.add(-y4_3_1 - x3_4_1 + x4_4_1 + y4_4_1 + z4_4_1 + x4_4_0 + y4_4_0 - 1.0 == 0)
model.add(-y5_3_1 - x4_4_1 + x5_4_1 + y5_4_1 + z5_4_1 + x5_4_0 + y5_4_0 - 1.0 == 0)
model.add(-y6_3_1 - x5_4_1 + x6_4_1 + y6_4_1 + z6_4_1 + x6_4_0 + y6_4_0 - 1.0 == 0)
model.add(-y7_3_1 - x6_4_1 + x7_4_1 + y7_4_1 + z7_4_1 + x7_4_0 + y7_4_0 - 1.0 == 0)
model.add(-y8_3_1 - x7_4_1 + x8_4_1 + y8_4_1 + z8_4_1 + x8_4_0 + y8_4_0 - 1.0 == 0)
model.add(-y9_3_1 - x8_4_1 + x9_4_1 + y9_4_1 + z9_4_1 + x9_4_0 + y9_4_0 - 1.0 == 0)
model.add(-x9_4_1 - y10_3_1 + y10_4_1 + z10_4_1 + y11_4_1 + z11_4_1 + x10_4_0 + y10_4_0 - 1.0 == 0)
model.add(-y1_4_1 + x1_5_1 + y1_5_1 + z1_5_1 + y0_5_1 + z0_5_1 + x1_5_0 + y1_5_0 - 1.0 == 0)
model.add(-y2_4_1 - x1_5_1 + x2_5_1 + y2_5_1 + z2_5_1 + x2_5_0 + y2_5_0 - 1.0 == 0)
model.add(-y3_4_1 - x2_5_1 + x3_5_1 + y3_5_1 + z3_5_1 + x3_5_0 + y3_5_0 - 1.0 == 0)
model.add(-y4_4_1 - x3_5_1 + x4_5_1 + y4_5_1 + z4_5_1 + x4_5_0 + y4_5_0 - 1.0 == 0)
model.add(-y5_4_1 - x4_5_1 + x5_5_1 + y5_5_1 + z5_5_1 + x5_5_0 + y5_5_0 - 1.0 == 0)
model.add(-y6_4_1 - x5_5_1 + x6_5_1 + y6_5_1 + z6_5_1 + x6_5_0 + y6_5_0 - 1.0 == 0)
model.add(-y7_4_1 - x6_5_1 + x7_5_1 + y7_5_1 + z7_5_1 + x7_5_0 + y7_5_0 - 1.0 == 0)
model.add(-y8_4_1 - x7_5_1 + x8_5_1 + y8_5_1 + z8_5_1 + x8_5_0 + y8_5_0 - 1.0 == 0)
model.add(-y9_4_1 - x8_5_1 + x9_5_1 + y9_5_1 + z9_5_1 + x9_5_0 + y9_5_0 - 1.0 == 0)
model.add(-x9_5_1 - y10_4_1 + y10_5_1 + z10_5_1 + y11_5_1 + z11_5_1 + x10_5_0 + y10_5_0 - 1.0 == 0)
model.add(-y1_5_1 + x1_6_1 + y1_6_1 + z1_6_1 + y0_6_1 + z0_6_1 + x1_6_0 + y1_6_0 - 1.0 == 0)
model.add(-y2_5_1 - x1_6_1 + x2_6_1 + y2_6_1 + z2_6_1 + x2_6_0 + y2_6_0 - 1.0 == 0)
model.add(-y3_5_1 - x2_6_1 + x3_6_1 + y3_6_1 + z3_6_1 + x3_6_0 + y3_6_0 - 1.0 == 0)
model.add(-y4_5_1 - x3_6_1 + x4_6_1 + y4_6_1 + z4_6_1 + x4_6_0 + y4_6_0 - 1.0 == 0)
model.add(-y5_5_1 - x4_6_1 + x5_6_1 + y5_6_1 + z5_6_1 + x5_6_0 + y5_6_0 - 1.0 == 0)
model.add(-y6_5_1 - x5_6_1 + x6_6_1 + y6_6_1 + z6_6_1 + x6_6_0 + y6_6_0 - 1.0 == 0)
model.add(-y7_5_1 - x6_6_1 + x7_6_1 + y7_6_1 + z7_6_1 + x7_6_0 + y7_6_0 - 1.0 == 0)
model.add(-y8_5_1 - x7_6_1 + x8_6_1 + y8_6_1 + z8_6_1 + x8_6_0 + y8_6_0 - 1.0 == 0)
model.add(-y9_5_1 - x8_6_1 + x9_6_1 + y9_6_1 + z9_6_1 + x9_6_0 + y9_6_0 - 1.0 == 0)
model.add(-x9_6_1 - y10_5_1 + y10_6_1 + z10_6_1 + y11_6_1 + z11_6_1 + x10_6_0 + y10_6_0 - 1.0 == 0)
model.add(-y1_6_1 + x1_7_1 + y1_7_1 + z1_7_1 + y0_7_1 + z0_7_1 + x1_7_0 + y1_7_0 - 1.0 == 0)
model.add(-y2_6_1 - x1_7_1 + x2_7_1 + y2_7_1 + z2_7_1 + x2_7_0 + y2_7_0 - 1.0 == 0)
model.add(-y3_6_1 - x2_7_1 + x3_7_1 + y3_7_1 + z3_7_1 + x3_7_0 + y3_7_0 - 1.0 == 0)
model.add(-y4_6_1 - x3_7_1 + x4_7_1 + y4_7_1 + z4_7_1 + x4_7_0 + y4_7_0 - 1.0 == 0)
model.add(-y5_6_1 - x4_7_1 + x5_7_1 + y5_7_1 + z5_7_1 + x5_7_0 + y5_7_0 - 1.0 == 0)
model.add(-y6_6_1 - x5_7_1 + x6_7_1 + y6_7_1 + z6_7_1 + x6_7_0 + y6_7_0 - 1.0 == 0)
model.add(-y7_6_1 - x6_7_1 + x7_7_1 + y7_7_1 + z7_7_1 + x7_7_0 + y7_7_0 - 1.0 == 0)
model.add(-y8_6_1 - x7_7_1 + x8_7_1 + y8_7_1 + z8_7_1 + x8_7_0 + y8_7_0 - 1.0 == 0)
model.add(-y9_6_1 - x8_7_1 + x9_7_1 + y9_7_1 + z9_7_1 + x9_7_0 + y9_7_0 - 1.0 == 0)
model.add(-x9_7_1 - y10_6_1 + y10_7_1 + z10_7_1 + y11_7_1 + z11_7_1 + x10_7_0 + y10_7_0 - 1.0 == 0)
model.add(-y1_7_1 + x1_8_1 + y1_8_1 + z1_8_1 + y0_8_1 + z0_8_1 + x1_8_0 + y1_8_0 - 1.0 == 0)
model.add(-y2_7_1 - x1_8_1 + x2_8_1 + y2_8_1 + z2_8_1 + x2_8_0 + y2_8_0 - 1.0 == 0)
model.add(-y3_7_1 - x2_8_1 + x3_8_1 + y3_8_1 + z3_8_1 + x3_8_0 + y3_8_0 - 1.0 == 0)
model.add(-y4_7_1 - x3_8_1 + x4_8_1 + y4_8_1 + z4_8_1 + x4_8_0 + y4_8_0 - 1.0 == 0)
model.add(-y5_7_1 - x4_8_1 + x5_8_1 + y5_8_1 + z5_8_1 + x5_8_0 + y5_8_0 - 1.0 == 0)
model.add(-y6_7_1 - x5_8_1 + x6_8_1 + y6_8_1 + z6_8_1 + x6_8_0 + y6_8_0 - 1.0 == 0)
model.add(-y7_7_1 - x6_8_1 + x7_8_1 + y7_8_1 + z7_8_1 + x7_8_0 + y7_8_0 - 1.0 == 0)
model.add(-y8_7_1 - x7_8_1 + x8_8_1 + y8_8_1 + z8_8_1 + x8_8_0 + y8_8_0 - 1.0 == 0)
model.add(-y9_7_1 - x8_8_1 + x9_8_1 + y9_8_1 + z9_8_1 + x9_8_0 + y9_8_0 - 1.0 == 0)
model.add(-x9_8_1 - y10_7_1 + y10_8_1 + z10_8_1 + y11_8_1 + z11_8_1 + x10_8_0 + y10_8_0 - 1.0 == 0)
model.add(-y1_8_1 + x1_9_1 + y1_9_1 + z1_9_1 + y0_9_1 + z0_9_1 + x1_9_0 + y1_9_0 - 1.0 == 0)
model.add(-y2_8_1 - x1_9_1 + x2_9_1 + y2_9_1 + z2_9_1 + x2_9_0 + y2_9_0 - 1.0 == 0)
model.add(-y3_8_1 - x2_9_1 + x3_9_1 + y3_9_1 + z3_9_1 + x3_9_0 + y3_9_0 - 1.0 == 0)
model.add(-y4_8_1 - x3_9_1 + x4_9_1 + y4_9_1 + z4_9_1 + x4_9_0 + y4_9_0 - 1.0 == 0)
model.add(-y5_8_1 - x4_9_1 + x5_9_1 + y5_9_1 + z5_9_1 + x5_9_0 + y5_9_0 - 1.0 == 0)
model.add(-y6_8_1 - x5_9_1 + x6_9_1 + y6_9_1 + z6_9_1 + x6_9_0 + y6_9_0 - 1.0 == 0)
model.add(-y7_8_1 - x6_9_1 + x7_9_1 + y7_9_1 + z7_9_1 + x7_9_0 + y7_9_0 - 1.0 == 0)
model.add(-y8_8_1 - x7_9_1 + x8_9_1 + y8_9_1 + z8_9_1 + x8_9_0 + y8_9_0 - 1.0 == 0)
model.add(-y9_8_1 - x8_9_1 + x9_9_1 + y9_9_1 + z9_9_1 + x9_9_0 + y9_9_0 - 1.0 == 0)
model.add(-x9_9_1 - y10_8_1 + y10_9_1 + z10_9_1 + y11_9_1 + z11_9_1 + x10_9_0 + y10_9_0 - 1.0 == 0)
model.add(-y1_9_1 + x1_10_1 + z1_10_1 + y0_10_1 + z0_10_1 + x1_11_1 + z1_11_1 + x1_10_0 + y1_10_0 - 1.0 == 0)
model.add(-y2_9_1 - x1_10_1 + x2_10_1 + z2_10_1 + x2_11_1 + z2_11_1 + x2_10_0 + y2_10_0 - 1.0 == 0)
model.add(-y3_9_1 - x2_10_1 + x3_10_1 + z3_10_1 + x3_11_1 + z3_11_1 + x3_10_0 + y3_10_0 - 1.0 == 0)
model.add(-y4_9_1 - x3_10_1 + x4_10_1 + z4_10_1 + x4_11_1 + z4_11_1 + x4_10_0 + y4_10_0 - 1.0 == 0)
model.add(-y5_9_1 - x4_10_1 + x5_10_1 + z5_10_1 + x5_11_1 + z5_11_1 + x5_10_0 + y5_10_0 - 1.0 == 0)
model.add(-y6_9_1 - x5_10_1 + x6_10_1 + z6_10_1 + x6_11_1 + z6_11_1 + x6_10_0 + y6_10_0 - 1.0 == 0)
model.add(-y7_9_1 - x6_10_1 + x7_10_1 + z7_10_1 + x7_11_1 + z7_11_1 + x7_10_0 + y7_10_0 - 1.0 == 0)
model.add(-y8_9_1 - x7_10_1 + x8_10_1 + z8_10_1 + x8_11_1 + z8_11_1 + x8_10_0 + y8_10_0 - 1.0 == 0)
model.add(-y9_9_1 - x8_10_1 + x9_10_1 + z9_10_1 + x9_11_1 + z9_11_1 + x9_10_0 + y9_10_0 - 1.0 == 0)
model.add(-y10_9_1 - x9_10_1 + y11_10_1 + z11_10_1 + x10_11_1 + z10_11_1 + x10_10_0 + y10_10_0 - 1.0 == 0)
model.add(-z1_1_1 + x1_1_2 + y1_1_2 + z1_1_2 + y0_1_2 + z0_1_2 + x1_0_2 + z1_0_2 - 1.0 == 0)
model.add(-z2_1_1 - x1_1_2 + x2_1_2 + y2_1_2 + z2_1_2 + x2_0_2 + z2_0_2 - 1.0 == 0)
model.add(-z3_1_1 - x2_1_2 + x3_1_2 + y3_1_2 + z3_1_2 + x3_0_2 + z3_0_2 - 1.0 == 0)
model.add(-z4_1_1 - x3_1_2 + x4_1_2 + y4_1_2 + z4_1_2 + x4_0_2 + z4_0_2 - 1.0 == 0)
model.add(-z5_1_1 - x4_1_2 + x5_1_2 + y5_1_2 + z5_1_2 + x5_0_2 + z5_0_2 - 1.0 == 0)
model.add(-z6_1_1 - x5_1_2 + x6_1_2 + y6_1_2 + z6_1_2 + x6_0_2 + z6_0_2 - 1.0 == 0)
model.add(-z7_1_1 - x6_1_2 + x7_1_2 + y7_1_2 + z7_1_2 + x7_0_2 + z7_0_2 - 1.0 == 0)
model.add(-z8_1_1 - x7_1_2 + x8_1_2 + y8_1_2 + z8_1_2 + x8_0_2 + z8_0_2 - 1.0 == 0)
model.add(-z9_1_1 - x8_1_2 + x9_1_2 + y9_1_2 + z9_1_2 + x9_0_2 + z9_0_2 - 1.0 == 0)
model.add(-x9_1_2 - z10_1_1 + y10_1_2 + z10_1_2 + y11_1_2 + z11_1_2 + x10_0_2 + z10_0_2 - 1.0 == 0)
model.add(-z1_2_1 - y1_1_2 + x1_2_2 + y1_2_2 + z1_2_2 + y0_2_2 + z0_2_2 - 1.0 == 0)
model.add(-z2_2_1 - y2_1_2 - x1_2_2 + x2_2_2 + y2_2_2 + z2_2_2 - 1.0 == 0)
model.add(-z3_2_1 - y3_1_2 - x2_2_2 + x3_2_2 + y3_2_2 + z3_2_2 - 1.0 == 0)
model.add(-z4_2_1 - y4_1_2 - x3_2_2 + x4_2_2 + y4_2_2 + z4_2_2 - 1.0 == 0)
model.add(-z5_2_1 - y5_1_2 - x4_2_2 + x5_2_2 + y5_2_2 + z5_2_2 - 1.0 == 0)
model.add(-z6_2_1 - y6_1_2 - x5_2_2 + x6_2_2 + y6_2_2 + z6_2_2 - 1.0 == 0)
model.add(-z7_2_1 - y7_1_2 - x6_2_2 + x7_2_2 + y7_2_2 + z7_2_2 - 1.0 == 0)
model.add(-z8_2_1 - y8_1_2 - x7_2_2 + x8_2_2 + y8_2_2 + z8_2_2 - 1.0 == 0)
model.add(-z9_2_1 - y9_1_2 - x8_2_2 + x9_2_2 + y9_2_2 + z9_2_2 - 1.0 == 0)
model.add(-x9_2_2 - z10_2_1 - y10_1_2 + y10_2_2 + z10_2_2 + y11_2_2 + z11_2_2 - 1.0 == 0)
model.add(-z1_3_1 - y1_2_2 + x1_3_2 + y1_3_2 + z1_3_2 + y0_3_2 + z0_3_2 - 1.0 == 0)
model.add(-z2_3_1 - y2_2_2 - x1_3_2 + x2_3_2 + y2_3_2 + z2_3_2 - 1.0 == 0)
model.add(-z3_3_1 - y3_2_2 - x2_3_2 + x3_3_2 + y3_3_2 + z3_3_2 - 1.0 == 0)
model.add(-z4_3_1 - y4_2_2 - x3_3_2 + x4_3_2 + y4_3_2 + z4_3_2 - 1.0 == 0)
model.add(-z5_3_1 - y5_2_2 - x4_3_2 + x5_3_2 + y5_3_2 + z5_3_2 - 1.0 == 0)
model.add(-z6_3_1 - y6_2_2 - x5_3_2 + x6_3_2 + y6_3_2 + z6_3_2 - 1.0 == 0)
model.add(-z7_3_1 - y7_2_2 - x6_3_2 + x7_3_2 + y7_3_2 + z7_3_2 - 1.0 == 0)
model.add(-z8_3_1 - y8_2_2 - x7_3_2 + x8_3_2 + y8_3_2 + z8_3_2 - 1.0 == 0)
model.add(-z9_3_1 - y9_2_2 - x8_3_2 + x9_3_2 + y9_3_2 + z9_3_2 - 1.0 == 0)
model.add(-x9_3_2 - z10_3_1 - y10_2_2 + y10_3_2 + z10_3_2 + y11_3_2 + z11_3_2 - 1.0 == 0)
model.add(-z1_4_1 - y1_3_2 + x1_4_2 + y1_4_2 + z1_4_2 + y0_4_2 + z0_4_2 - 1.0 == 0)
model.add(-z2_4_1 - y2_3_2 - x1_4_2 + x2_4_2 + y2_4_2 + z2_4_2 - 1.0 == 0)
model.add(-z3_4_1 - y3_3_2 - x2_4_2 + x3_4_2 + y3_4_2 + z3_4_2 - 1.0 == 0)
model.add(-z4_4_1 - y4_3_2 - x3_4_2 + x4_4_2 + y4_4_2 + z4_4_2 - 1.0 == 0)
model.add(-z5_4_1 - y5_3_2 - x4_4_2 + x5_4_2 + y5_4_2 + z5_4_2 - 1.0 == 0)
model.add(-z6_4_1 - y6_3_2 - x5_4_2 + x6_4_2 + y6_4_2 + z6_4_2 - 1.0 == 0)
model.add(-z7_4_1 - y7_3_2 - x6_4_2 + x7_4_2 + y7_4_2 + z7_4_2 - 1.0 == 0)
model.add(-z8_4_1 - y8_3_2 - x7_4_2 + x8_4_2 + y8_4_2 + z8_4_2 - 1.0 == 0)
model.add(-z9_4_1 - y9_3_2 - x8_4_2 + x9_4_2 + y9_4_2 + z9_4_2 - 1.0 == 0)
model.add(-x9_4_2 - z10_4_1 - y10_3_2 + y10_4_2 + z10_4_2 + y11_4_2 + z11_4_2 - 1.0 == 0)
model.add(-z1_5_1 - y1_4_2 + x1_5_2 + y1_5_2 + z1_5_2 + y0_5_2 + z0_5_2 - 1.0 == 0)
model.add(-z2_5_1 - y2_4_2 - x1_5_2 + x2_5_2 + y2_5_2 + z2_5_2 - 1.0 == 0)
model.add(-z3_5_1 - y3_4_2 - x2_5_2 + x3_5_2 + y3_5_2 + z3_5_2 - 1.0 == 0)
model.add(-z4_5_1 - y4_4_2 - x3_5_2 + x4_5_2 + y4_5_2 + z4_5_2 - 1.0 == 0)
model.add(-z5_5_1 - y5_4_2 - x4_5_2 + x5_5_2 + y5_5_2 + z5_5_2 - 1.0 == 0)
model.add(-z6_5_1 - y6_4_2 - x5_5_2 + x6_5_2 + y6_5_2 + z6_5_2 - 1.0 == 0)
model.add(-z7_5_1 - y7_4_2 - x6_5_2 + x7_5_2 + y7_5_2 + z7_5_2 - 1.0 == 0)
model.add(-z8_5_1 - y8_4_2 - x7_5_2 + x8_5_2 + y8_5_2 + z8_5_2 - 1.0 == 0)
model.add(-z9_5_1 - y9_4_2 - x8_5_2 + x9_5_2 + y9_5_2 + z9_5_2 - 1.0 == 0)
model.add(-x9_5_2 - z10_5_1 - y10_4_2 + y10_5_2 + z10_5_2 + y11_5_2 + z11_5_2 - 1.0 == 0)
model.add(-z1_6_1 - y1_5_2 + x1_6_2 + y1_6_2 + z1_6_2 + y0_6_2 + z0_6_2 - 1.0 == 0)
model.add(-z2_6_1 - y2_5_2 - x1_6_2 + x2_6_2 + y2_6_2 + z2_6_2 - 1.0 == 0)
model.add(-z3_6_1 - y3_5_2 - x2_6_2 + x3_6_2 + y3_6_2 + z3_6_2 - 1.0 == 0)
model.add(-z4_6_1 - y4_5_2 - x3_6_2 + x4_6_2 + y4_6_2 + z4_6_2 - 1.0 == 0)
model.add(-z5_6_1 - y5_5_2 - x4_6_2 + x5_6_2 + y5_6_2 + z5_6_2 - 1.0 == 0)
model.add(-z6_6_1 - y6_5_2 - x5_6_2 + x6_6_2 + y6_6_2 + z6_6_2 - 1.0 == 0)
model.add(-z7_6_1 - y7_5_2 - x6_6_2 + x7_6_2 + y7_6_2 + z7_6_2 - 1.0 == 0)
model.add(-z8_6_1 - y8_5_2 - x7_6_2 + x8_6_2 + y8_6_2 + z8_6_2 - 1.0 == 0)
model.add(-z9_6_1 - y9_5_2 - x8_6_2 + x9_6_2 + y9_6_2 + z9_6_2 - 1.0 == 0)
model.add(-x9_6_2 - z10_6_1 - y10_5_2 + y10_6_2 + z10_6_2 + y11_6_2 + z11_6_2 - 1.0 == 0)
model.add(-z1_7_1 - y1_6_2 + x1_7_2 + y1_7_2 + z1_7_2 + y0_7_2 + z0_7_2 - 1.0 == 0)
model.add(-z2_7_1 - y2_6_2 - x1_7_2 + x2_7_2 + y2_7_2 + z2_7_2 - 1.0 == 0)
model.add(-z3_7_1 - y3_6_2 - x2_7_2 + x3_7_2 + y3_7_2 + z3_7_2 - 1.0 == 0)
model.add(-z4_7_1 - y4_6_2 - x3_7_2 + x4_7_2 + y4_7_2 + z4_7_2 - 1.0 == 0)
model.add(-z5_7_1 - y5_6_2 - x4_7_2 + x5_7_2 + y5_7_2 + z5_7_2 - 1.0 == 0)
model.add(-z6_7_1 - y6_6_2 - x5_7_2 + x6_7_2 + y6_7_2 + z6_7_2 - 1.0 == 0)
model.add(-z7_7_1 - y7_6_2 - x6_7_2 + x7_7_2 + y7_7_2 + z7_7_2 - 1.0 == 0)
model.add(-z8_7_1 - y8_6_2 - x7_7_2 + x8_7_2 + y8_7_2 + z8_7_2 - 1.0 == 0)
model.add(-z9_7_1 - y9_6_2 - x8_7_2 + x9_7_2 + y9_7_2 + z9_7_2 - 1.0 == 0)
model.add(-x9_7_2 - z10_7_1 - y10_6_2 + y10_7_2 + z10_7_2 + y11_7_2 + z11_7_2 - 1.0 == 0)
model.add(-z1_8_1 - y1_7_2 + x1_8_2 + y1_8_2 + z1_8_2 + y0_8_2 + z0_8_2 - 1.0 == 0)
model.add(-z2_8_1 - y2_7_2 - x1_8_2 + x2_8_2 + y2_8_2 + z2_8_2 - 1.0 == 0)
model.add(-z3_8_1 - y3_7_2 - x2_8_2 + x3_8_2 + y3_8_2 + z3_8_2 - 1.0 == 0)
model.add(-z4_8_1 - y4_7_2 - x3_8_2 + x4_8_2 + y4_8_2 + z4_8_2 - 1.0 == 0)
model.add(-z5_8_1 - y5_7_2 - x4_8_2 + x5_8_2 + y5_8_2 + z5_8_2 - 1.0 == 0)
model.add(-z6_8_1 - y6_7_2 - x5_8_2 + x6_8_2 + y6_8_2 + z6_8_2 - 1.0 == 0)
model.add(-z7_8_1 - y7_7_2 - x6_8_2 + x7_8_2 + y7_8_2 + z7_8_2 - 1.0 == 0)
model.add(-z8_8_1 - y8_7_2 - x7_8_2 + x8_8_2 + y8_8_2 + z8_8_2 - 1.0 == 0)
model.add(-z9_8_1 - y9_7_2 - x8_8_2 + x9_8_2 + y9_8_2 + z9_8_2 - 1.0 == 0)
model.add(-x9_8_2 - z10_8_1 - y10_7_2 + y10_8_2 + z10_8_2 + y11_8_2 + z11_8_2 - 1.0 == 0)
model.add(-z1_9_1 - y1_8_2 + x1_9_2 + y1_9_2 + z1_9_2 + y0_9_2 + z0_9_2 - 1.0 == 0)
model.add(-z2_9_1 - y2_8_2 - x1_9_2 + x2_9_2 + y2_9_2 + z2_9_2 - 1.0 == 0)
model.add(-z3_9_1 - y3_8_2 - x2_9_2 + x3_9_2 + y3_9_2 + z3_9_2 - 1.0 == 0)
model.add(-z4_9_1 - y4_8_2 - x3_9_2 + x4_9_2 + y4_9_2 + z4_9_2 - 1.0 == 0)
model.add(-z5_9_1 - y5_8_2 - x4_9_2 + x5_9_2 + y5_9_2 + z5_9_2 - 1.0 == 0)
model.add(-z6_9_1 - y6_8_2 - x5_9_2 + x6_9_2 + y6_9_2 + z6_9_2 - 1.0 == 0)
model.add(-z7_9_1 - y7_8_2 - x6_9_2 + x7_9_2 + y7_9_2 + z7_9_2 - 1.0 == 0)
model.add(-z8_9_1 - y8_8_2 - x7_9_2 + x8_9_2 + y8_9_2 + z8_9_2 - 1.0 == 0)
model.add(-z9_9_1 - y9_8_2 - x8_9_2 + x9_9_2 + y9_9_2 + z9_9_2 - 1.0 == 0)
model.add(-x9_9_2 - z10_9_1 - y10_8_2 + y10_9_2 + z10_9_2 + y11_9_2 + z11_9_2 - 1.0 == 0)
model.add(-y1_9_2 - z1_10_1 + x1_10_2 + z1_10_2 + y0_10_2 + z0_10_2 + x1_11_2 + z1_11_2 - 1.0 == 0)
model.add(-y2_9_2 - z2_10_1 - x1_10_2 + x2_10_2 + z2_10_2 + x2_11_2 + z2_11_2 - 1.0 == 0)
model.add(-y3_9_2 - z3_10_1 - x2_10_2 + x3_10_2 + z3_10_2 + x3_11_2 + z3_11_2 - 1.0 == 0)
model.add(-y4_9_2 - z4_10_1 - x3_10_2 + x4_10_2 + z4_10_2 + x4_11_2 + z4_11_2 - 1.0 == 0)
model.add(-y5_9_2 - z5_10_1 - x4_10_2 + x5_10_2 + z5_10_2 + x5_11_2 + z5_11_2 - 1.0 == 0)
model.add(-y6_9_2 - z6_10_1 - x5_10_2 + x6_10_2 + z6_10_2 + x6_11_2 + z6_11_2 - 1.0 == 0)
model.add(-y7_9_2 - z7_10_1 - x6_10_2 + x7_10_2 + z7_10_2 + x7_11_2 + z7_11_2 - 1.0 == 0)
model.add(-y8_9_2 - z8_10_1 - x7_10_2 + x8_10_2 + z8_10_2 + x8_11_2 + z8_11_2 - 1.0 == 0)
model.add(-y9_9_2 - z9_10_1 - x8_10_2 + x9_10_2 + z9_10_2 + x9_11_2 + z9_11_2 - 1.0 == 0)
model.add(-y10_9_2 - x9_10_2 + y11_10_2 + z11_10_2 + x10_11_2 + z10_11_2 - 1.0 == 0)
model.add(-z1_1_2 + x1_1_3 + y1_1_3 + z1_1_3 + y0_1_3 + z0_1_3 + x1_0_3 + z1_0_3 - 1.0 == 0)
model.add(-z2_1_2 - x1_1_3 + x2_1_3 + y2_1_3 + z2_1_3 + x2_0_3 + z2_0_3 - 1.0 == 0)
model.add(-z3_1_2 - x2_1_3 + x3_1_3 + y3_1_3 + z3_1_3 + x3_0_3 + z3_0_3 - 1.0 == 0)
model.add(-z4_1_2 - x3_1_3 + x4_1_3 + y4_1_3 + z4_1_3 + x4_0_3 + z4_0_3 - 1.0 == 0)
model.add(-z5_1_2 - x4_1_3 + x5_1_3 + y5_1_3 + z5_1_3 + x5_0_3 + z5_0_3 - 1.0 == 0)
model.add(-z6_1_2 - x5_1_3 + x6_1_3 + y6_1_3 + z6_1_3 + x6_0_3 + z6_0_3 - 1.0 == 0)
model.add(-z7_1_2 - x6_1_3 + x7_1_3 + y7_1_3 + z7_1_3 + x7_0_3 + z7_0_3 - 1.0 == 0)
model.add(-z8_1_2 - x7_1_3 + x8_1_3 + y8_1_3 + z8_1_3 + x8_0_3 + z8_0_3 - 1.0 == 0)
model.add(-z9_1_2 - x8_1_3 + x9_1_3 + y9_1_3 + z9_1_3 + x9_0_3 + z9_0_3 - 1.0 == 0)
model.add(-x9_1_3 - z10_1_2 + y10_1_3 + z10_1_3 + y11_1_3 + z11_1_3 + x10_0_3 + z10_0_3 - 1.0 == 0)
model.add(-z1_2_2 - y1_1_3 + x1_2_3 + y1_2_3 + z1_2_3 + y0_2_3 + z0_2_3 - 1.0 == 0)
model.add(-z2_2_2 - y2_1_3 - x1_2_3 + x2_2_3 + y2_2_3 + z2_2_3 - 1.0 == 0)
model.add(-z3_2_2 - y3_1_3 - x2_2_3 + x3_2_3 + y3_2_3 + z3_2_3 - 1.0 == 0)
model.add(-z4_2_2 - y4_1_3 - x3_2_3 + x4_2_3 + y4_2_3 + z4_2_3 - 1.0 == 0)
model.add(-z5_2_2 - y5_1_3 - x4_2_3 + x5_2_3 + y5_2_3 + z5_2_3 - 1.0 == 0)
model.add(-z6_2_2 - y6_1_3 - x5_2_3 + x6_2_3 + y6_2_3 + z6_2_3 - 1.0 == 0)
model.add(-z7_2_2 - y7_1_3 - x6_2_3 + x7_2_3 + y7_2_3 + z7_2_3 - 1.0 == 0)
model.add(-z8_2_2 - y8_1_3 - x7_2_3 + x8_2_3 + y8_2_3 + z8_2_3 - 1.0 == 0)
model.add(-z9_2_2 - y9_1_3 - x8_2_3 + x9_2_3 + y9_2_3 + z9_2_3 - 1.0 == 0)
model.add(-x9_2_3 - z10_2_2 - y10_1_3 + y10_2_3 + z10_2_3 + y11_2_3 + z11_2_3 - 1.0 == 0)
model.add(-z1_3_2 - y1_2_3 + x1_3_3 + y1_3_3 + z1_3_3 + y0_3_3 + z0_3_3 - 1.0 == 0)
model.add(-z2_3_2 - y2_2_3 - x1_3_3 + x2_3_3 + y2_3_3 + z2_3_3 - 1.0 == 0)
model.add(-z3_3_2 - y3_2_3 - x2_3_3 + x3_3_3 + y3_3_3 + z3_3_3 - 1.0 == 0)
model.add(-z4_3_2 - y4_2_3 - x3_3_3 + x4_3_3 + y4_3_3 + z4_3_3 - 1.0 == 0)
model.add(-z5_3_2 - y5_2_3 - x4_3_3 + x5_3_3 + y5_3_3 + z5_3_3 - 1.0 == 0)
model.add(-z6_3_2 - y6_2_3 - x5_3_3 + x6_3_3 + y6_3_3 + z6_3_3 - 1.0 == 0)
model.add(-z7_3_2 - y7_2_3 - x6_3_3 + x7_3_3 + y7_3_3 + z7_3_3 - 1.0 == 0)
model.add(-z8_3_2 - y8_2_3 - x7_3_3 + x8_3_3 + y8_3_3 + z8_3_3 - 1.0 == 0)
model.add(-z9_3_2 - y9_2_3 - x8_3_3 + x9_3_3 + y9_3_3 + z9_3_3 - 1.0 == 0)
model.add(-x9_3_3 - z10_3_2 - y10_2_3 + y10_3_3 + z10_3_3 + y11_3_3 + z11_3_3 - 1.0 == 0)
model.add(-z1_4_2 - y1_3_3 + x1_4_3 + y1_4_3 + z1_4_3 + y0_4_3 + z0_4_3 - 1.0 == 0)
model.add(-z2_4_2 - y2_3_3 - x1_4_3 + x2_4_3 + y2_4_3 + z2_4_3 - 1.0 == 0)
model.add(-z3_4_2 - y3_3_3 - x2_4_3 + x3_4_3 + y3_4_3 + z3_4_3 - 1.0 == 0)
model.add(-z4_4_2 - y4_3_3 - x3_4_3 + x4_4_3 + y4_4_3 + z4_4_3 - 1.0 == 0)
model.add(-z5_4_2 - y5_3_3 - x4_4_3 + x5_4_3 + y5_4_3 + z5_4_3 - 1.0 == 0)
model.add(-z6_4_2 - y6_3_3 - x5_4_3 + x6_4_3 + y6_4_3 + z6_4_3 - 1.0 == 0)
model.add(-z7_4_2 - y7_3_3 - x6_4_3 + x7_4_3 + y7_4_3 + z7_4_3 - 1.0 == 0)
model.add(-z8_4_2 - y8_3_3 - x7_4_3 + x8_4_3 + y8_4_3 + z8_4_3 - 1.0 == 0)
model.add(-z9_4_2 - y9_3_3 - x8_4_3 + x9_4_3 + y9_4_3 + z9_4_3 - 1.0 == 0)
model.add(-x9_4_3 - z10_4_2 - y10_3_3 + y10_4_3 + z10_4_3 + y11_4_3 + z11_4_3 - 1.0 == 0)
model.add(-z1_5_2 - y1_4_3 + x1_5_3 + y1_5_3 + z1_5_3 + y0_5_3 + z0_5_3 - 1.0 == 0)
model.add(-z2_5_2 - y2_4_3 - x1_5_3 + x2_5_3 + y2_5_3 + z2_5_3 - 1.0 == 0)
model.add(-z3_5_2 - y3_4_3 - x2_5_3 + x3_5_3 + y3_5_3 + z3_5_3 - 1.0 == 0)
model.add(-z4_5_2 - y4_4_3 - x3_5_3 + x4_5_3 + y4_5_3 + z4_5_3 - 1.0 == 0)
model.add(-z5_5_2 - y5_4_3 - x4_5_3 + x5_5_3 + y5_5_3 + z5_5_3 - 1.0 == 0)
model.add(-z6_5_2 - y6_4_3 - x5_5_3 + x6_5_3 + y6_5_3 + z6_5_3 - 1.0 == 0)
model.add(-z7_5_2 - y7_4_3 - x6_5_3 + x7_5_3 + y7_5_3 + z7_5_3 - 1.0 == 0)
model.add(-z8_5_2 - y8_4_3 - x7_5_3 + x8_5_3 + y8_5_3 + z8_5_3 - 1.0 == 0)
model.add(-z9_5_2 - y9_4_3 - x8_5_3 + x9_5_3 + y9_5_3 + z9_5_3 - 1.0 == 0)
model.add(-x9_5_3 - z10_5_2 - y10_4_3 + y10_5_3 + z10_5_3 + y11_5_3 + z11_5_3 - 1.0 == 0)
model.add(-z1_6_2 - y1_5_3 + x1_6_3 + y1_6_3 + z1_6_3 + y0_6_3 + z0_6_3 - 1.0 == 0)
model.add(-z2_6_2 - y2_5_3 - x1_6_3 + x2_6_3 + y2_6_3 + z2_6_3 - 1.0 == 0)
model.add(-z3_6_2 - y3_5_3 - x2_6_3 + x3_6_3 + y3_6_3 + z3_6_3 - 1.0 == 0)
model.add(-z4_6_2 - y4_5_3 - x3_6_3 + x4_6_3 + y4_6_3 + z4_6_3 - 1.0 == 0)
model.add(-z5_6_2 - y5_5_3 - x4_6_3 + x5_6_3 + y5_6_3 + z5_6_3 - 1.0 == 0)
model.add(-z6_6_2 - y6_5_3 - x5_6_3 + x6_6_3 + y6_6_3 + z6_6_3 - 1.0 == 0)
model.add(-z7_6_2 - y7_5_3 - x6_6_3 + x7_6_3 + y7_6_3 + z7_6_3 - 1.0 == 0)
model.add(-z8_6_2 - y8_5_3 - x7_6_3 + x8_6_3 + y8_6_3 + z8_6_3 - 1.0 == 0)
model.add(-z9_6_2 - y9_5_3 - x8_6_3 + x9_6_3 + y9_6_3 + z9_6_3 - 1.0 == 0)
model.add(-x9_6_3 - z10_6_2 - y10_5_3 + y10_6_3 + z10_6_3 + y11_6_3 + z11_6_3 - 1.0 == 0)
model.add(-z1_7_2 - y1_6_3 + x1_7_3 + y1_7_3 + z1_7_3 + y0_7_3 + z0_7_3 - 1.0 == 0)
model.add(-z2_7_2 - y2_6_3 - x1_7_3 + x2_7_3 + y2_7_3 + z2_7_3 - 1.0 == 0)
model.add(-z3_7_2 - y3_6_3 - x2_7_3 + x3_7_3 + y3_7_3 + z3_7_3 - 1.0 == 0)
model.add(-z4_7_2 - y4_6_3 - x3_7_3 + x4_7_3 + y4_7_3 + z4_7_3 - 1.0 == 0)
model.add(-z5_7_2 - y5_6_3 - x4_7_3 + x5_7_3 + y5_7_3 + z5_7_3 - 1.0 == 0)
model.add(-z6_7_2 - y6_6_3 - x5_7_3 + x6_7_3 + y6_7_3 + z6_7_3 - 1.0 == 0)
model.add(-z7_7_2 - y7_6_3 - x6_7_3 + x7_7_3 + y7_7_3 + z7_7_3 - 1.0 == 0)
model.add(-z8_7_2 - y8_6_3 - x7_7_3 + x8_7_3 + y8_7_3 + z8_7_3 - 1.0 == 0)
model.add(-z9_7_2 - y9_6_3 - x8_7_3 + x9_7_3 + y9_7_3 + z9_7_3 - 1.0 == 0)
model.add(-x9_7_3 - z10_7_2 - y10_6_3 + y10_7_3 + z10_7_3 + y11_7_3 + z11_7_3 - 1.0 == 0)
model.add(-z1_8_2 - y1_7_3 + x1_8_3 + y1_8_3 + z1_8_3 + y0_8_3 + z0_8_3 - 1.0 == 0)
model.add(-z2_8_2 - y2_7_3 - x1_8_3 + x2_8_3 + y2_8_3 + z2_8_3 - 1.0 == 0)
model.add(-z3_8_2 - y3_7_3 - x2_8_3 + x3_8_3 + y3_8_3 + z3_8_3 - 1.0 == 0)
model.add(-z4_8_2 - y4_7_3 - x3_8_3 + x4_8_3 + y4_8_3 + z4_8_3 - 1.0 == 0)
model.add(-z5_8_2 - y5_7_3 - x4_8_3 + x5_8_3 + y5_8_3 + z5_8_3 - 1.0 == 0)
model.add(-z6_8_2 - y6_7_3 - x5_8_3 + x6_8_3 + y6_8_3 + z6_8_3 - 1.0 == 0)
model.add(-z7_8_2 - y7_7_3 - x6_8_3 + x7_8_3 + y7_8_3 + z7_8_3 - 1.0 == 0)
model.add(-z8_8_2 - y8_7_3 - x7_8_3 + x8_8_3 + y8_8_3 + z8_8_3 - 1.0 == 0)
model.add(-z9_8_2 - y9_7_3 - x8_8_3 + x9_8_3 + y9_8_3 + z9_8_3 - 1.0 == 0)
model.add(-x9_8_3 - z10_8_2 - y10_7_3 + y10_8_3 + z10_8_3 + y11_8_3 + z11_8_3 - 1.0 == 0)
model.add(-z1_9_2 - y1_8_3 + x1_9_3 + y1_9_3 + z1_9_3 + y0_9_3 + z0_9_3 - 1.0 == 0)
model.add(-z2_9_2 - y2_8_3 - x1_9_3 + x2_9_3 + y2_9_3 + z2_9_3 - 1.0 == 0)
model.add(-z3_9_2 - y3_8_3 - x2_9_3 + x3_9_3 + y3_9_3 + z3_9_3 - 1.0 == 0)
model.add(-z4_9_2 - y4_8_3 - x3_9_3 + x4_9_3 + y4_9_3 + z4_9_3 - 1.0 == 0)
model.add(-z5_9_2 - y5_8_3 - x4_9_3 + x5_9_3 + y5_9_3 + z5_9_3 - 1.0 == 0)
model.add(-z6_9_2 - y6_8_3 - x5_9_3 + x6_9_3 + y6_9_3 + z6_9_3 - 1.0 == 0)
model.add(-z7_9_2 - y7_8_3 - x6_9_3 + x7_9_3 + y7_9_3 + z7_9_3 - 1.0 == 0)
model.add(-z8_9_2 - y8_8_3 - x7_9_3 + x8_9_3 + y8_9_3 + z8_9_3 - 1.0 == 0)
model.add(-z9_9_2 - y9_8_3 - x8_9_3 + x9_9_3 + y9_9_3 + z9_9_3 - 1.0 == 0)
model.add(-x9_9_3 - z10_9_2 - y10_8_3 + y10_9_3 + z10_9_3 + y11_9_3 + z11_9_3 - 1.0 == 0)
model.add(-y1_9_3 - z1_10_2 + x1_10_3 + z1_10_3 + y0_10_3 + z0_10_3 + x1_11_3 + z1_11_3 - 1.0 == 0)
model.add(-y2_9_3 - z2_10_2 - x1_10_3 + x2_10_3 + z2_10_3 + x2_11_3 + z2_11_3 - 1.0 == 0)
model.add(-y3_9_3 - z3_10_2 - x2_10_3 + x3_10_3 + z3_10_3 + x3_11_3 + z3_11_3 - 1.0 == 0)
model.add(-y4_9_3 - z4_10_2 - x3_10_3 + x4_10_3 + z4_10_3 + x4_11_3 + z4_11_3 - 1.0 == 0)
model.add(-y5_9_3 - z5_10_2 - x4_10_3 + x5_10_3 + z5_10_3 + x5_11_3 + z5_11_3 - 1.0 == 0)
model.add(-y6_9_3 - z6_10_2 - x5_10_3 + x6_10_3 + z6_10_3 + x6_11_3 + z6_11_3 - 1.0 == 0)
model.add(-y7_9_3 - z7_10_2 - x6_10_3 + x7_10_3 + z7_10_3 + x7_11_3 + z7_11_3 - 1.0 == 0)
model.add(-y8_9_3 - z8_10_2 - x7_10_3 + x8_10_3 + z8_10_3 + x8_11_3 + z8_11_3 - 1.0 == 0)
model.add(-y9_9_3 - z9_10_2 - x8_10_3 + x9_10_3 + z9_10_3 + x9_11_3 + z9_11_3 - 1.0 == 0)
model.add(-y10_9_3 - x9_10_3 + y11_10_3 + z11_10_3 + x10_11_3 + z10_11_3 - 1.0 == 0)
model.add(-z1_1_3 + x1_1_4 + y1_1_4 + z1_1_4 + y0_1_4 + z0_1_4 + x1_0_4 + z1_0_4 - 1.0 == 0)
model.add(-z2_1_3 - x1_1_4 + x2_1_4 + y2_1_4 + z2_1_4 + x2_0_4 + z2_0_4 - 1.0 == 0)
model.add(-z3_1_3 - x2_1_4 + x3_1_4 + y3_1_4 + z3_1_4 + x3_0_4 + z3_0_4 - 1.0 == 0)
model.add(-z4_1_3 - x3_1_4 + x4_1_4 + y4_1_4 + z4_1_4 + x4_0_4 + z4_0_4 - 1.0 == 0)
model.add(-z5_1_3 - x4_1_4 + x5_1_4 + y5_1_4 + z5_1_4 + x5_0_4 + z5_0_4 - 1.0 == 0)
model.add(-z6_1_3 - x5_1_4 + x6_1_4 + y6_1_4 + z6_1_4 + x6_0_4 + z6_0_4 - 1.0 == 0)
model.add(-z7_1_3 - x6_1_4 + x7_1_4 + y7_1_4 + z7_1_4 + x7_0_4 + z7_0_4 - 1.0 == 0)
model.add(-z8_1_3 - x7_1_4 + x8_1_4 + y8_1_4 + z8_1_4 + x8_0_4 + z8_0_4 - 1.0 == 0)
model.add(-z9_1_3 - x8_1_4 + x9_1_4 + y9_1_4 + z9_1_4 + x9_0_4 + z9_0_4 - 1.0 == 0)
model.add(-x9_1_4 - z10_1_3 + y10_1_4 + z10_1_4 + y11_1_4 + z11_1_4 + x10_0_4 + z10_0_4 - 1.0 == 0)
model.add(-z1_2_3 - y1_1_4 + x1_2_4 + y1_2_4 + z1_2_4 + y0_2_4 + z0_2_4 - 1.0 == 0)
model.add(-z2_2_3 - y2_1_4 - x1_2_4 + x2_2_4 + y2_2_4 + z2_2_4 - 1.0 == 0)
model.add(-z3_2_3 - y3_1_4 - x2_2_4 + x3_2_4 + y3_2_4 + z3_2_4 - 1.0 == 0)
model.add(-z4_2_3 - y4_1_4 - x3_2_4 + x4_2_4 + y4_2_4 + z4_2_4 - 1.0 == 0)
model.add(-z5_2_3 - y5_1_4 - x4_2_4 + x5_2_4 + y5_2_4 + z5_2_4 - 1.0 == 0)
model.add(-z6_2_3 - y6_1_4 - x5_2_4 + x6_2_4 + y6_2_4 + z6_2_4 - 1.0 == 0)
model.add(-z7_2_3 - y7_1_4 - x6_2_4 + x7_2_4 + y7_2_4 + z7_2_4 - 1.0 == 0)
model.add(-z8_2_3 - y8_1_4 - x7_2_4 + x8_2_4 + y8_2_4 + z8_2_4 - 1.0 == 0)
model.add(-z9_2_3 - y9_1_4 - x8_2_4 + x9_2_4 + y9_2_4 + z9_2_4 - 1.0 == 0)
model.add(-x9_2_4 - z10_2_3 - y10_1_4 + y10_2_4 + z10_2_4 + y11_2_4 + z11_2_4 - 1.0 == 0)
model.add(-z1_3_3 - y1_2_4 + x1_3_4 + y1_3_4 + z1_3_4 + y0_3_4 + z0_3_4 - 1.0 == 0)
model.add(-z2_3_3 - y2_2_4 - x1_3_4 + x2_3_4 + y2_3_4 + z2_3_4 - 1.0 == 0)
model.add(-z3_3_3 - y3_2_4 - x2_3_4 + x3_3_4 + y3_3_4 + z3_3_4 - 1.0 == 0)
model.add(-z4_3_3 - y4_2_4 - x3_3_4 + x4_3_4 + y4_3_4 + z4_3_4 - 1.0 == 0)
model.add(-z5_3_3 - y5_2_4 - x4_3_4 + x5_3_4 + y5_3_4 + z5_3_4 - 1.0 == 0)
model.add(-z6_3_3 - y6_2_4 - x5_3_4 + x6_3_4 + y6_3_4 + z6_3_4 - 1.0 == 0)
model.add(-z7_3_3 - y7_2_4 - x6_3_4 + x7_3_4 + y7_3_4 + z7_3_4 - 1.0 == 0)
model.add(-z8_3_3 - y8_2_4 - x7_3_4 + x8_3_4 + y8_3_4 + z8_3_4 - 1.0 == 0)
model.add(-z9_3_3 - y9_2_4 - x8_3_4 + x9_3_4 + y9_3_4 + z9_3_4 - 1.0 == 0)
model.add(-x9_3_4 - z10_3_3 - y10_2_4 + y10_3_4 + z10_3_4 + y11_3_4 + z11_3_4 - 1.0 == 0)
model.add(-z1_4_3 - y1_3_4 + x1_4_4 + y1_4_4 + z1_4_4 + y0_4_4 + z0_4_4 - 1.0 == 0)
model.add(-z2_4_3 - y2_3_4 - x1_4_4 + x2_4_4 + y2_4_4 + z2_4_4 - 1.0 == 0)
model.add(-z3_4_3 - y3_3_4 - x2_4_4 + x3_4_4 + y3_4_4 + z3_4_4 - 1.0 == 0)
model.add(-z4_4_3 - y4_3_4 - x3_4_4 + x4_4_4 + y4_4_4 + z4_4_4 - 1.0 == 0)
model.add(-z5_4_3 - y5_3_4 - x4_4_4 + x5_4_4 + y5_4_4 + z5_4_4 - 1.0 == 0)
model.add(-z6_4_3 - y6_3_4 - x5_4_4 + x6_4_4 + y6_4_4 + z6_4_4 - 1.0 == 0)
model.add(-z7_4_3 - y7_3_4 - x6_4_4 + x7_4_4 + y7_4_4 + z7_4_4 - 1.0 == 0)
model.add(-z8_4_3 - y8_3_4 - x7_4_4 + x8_4_4 + y8_4_4 + z8_4_4 - 1.0 == 0)
model.add(-z9_4_3 - y9_3_4 - x8_4_4 + x9_4_4 + y9_4_4 + z9_4_4 - 1.0 == 0)
model.add(-x9_4_4 - z10_4_3 - y10_3_4 + y10_4_4 + z10_4_4 + y11_4_4 + z11_4_4 - 1.0 == 0)
model.add(-z1_5_3 - y1_4_4 + x1_5_4 + y1_5_4 + z1_5_4 + y0_5_4 + z0_5_4 - 1.0 == 0)
model.add(-z2_5_3 - y2_4_4 - x1_5_4 + x2_5_4 + y2_5_4 + z2_5_4 - 1.0 == 0)
model.add(-z3_5_3 - y3_4_4 - x2_5_4 + x3_5_4 + y3_5_4 + z3_5_4 - 1.0 == 0)
model.add(-z4_5_3 - y4_4_4 - x3_5_4 + x4_5_4 + y4_5_4 + z4_5_4 - 1.0 == 0)
model.add(-z5_5_3 - y5_4_4 - x4_5_4 + x5_5_4 + y5_5_4 + z5_5_4 - 1.0 == 0)
model.add(-z6_5_3 - y6_4_4 - x5_5_4 + x6_5_4 + y6_5_4 + z6_5_4 - 1.0 == 0)
model.add(-z7_5_3 - y7_4_4 - x6_5_4 + x7_5_4 + y7_5_4 + z7_5_4 - 1.0 == 0)
model.add(-z8_5_3 - y8_4_4 - x7_5_4 + x8_5_4 + y8_5_4 + z8_5_4 - 1.0 == 0)
model.add(-z9_5_3 - y9_4_4 - x8_5_4 + x9_5_4 + y9_5_4 + z9_5_4 - 1.0 == 0)
model.add(-x9_5_4 - z10_5_3 - y10_4_4 + y10_5_4 + z10_5_4 + y11_5_4 + z11_5_4 - 1.0 == 0)
model.add(-z1_6_3 - y1_5_4 + x1_6_4 + y1_6_4 + z1_6_4 + y0_6_4 + z0_6_4 - 1.0 == 0)
model.add(-z2_6_3 - y2_5_4 - x1_6_4 + x2_6_4 + y2_6_4 + z2_6_4 - 1.0 == 0)
model.add(-z3_6_3 - y3_5_4 - x2_6_4 + x3_6_4 + y3_6_4 + z3_6_4 - 1.0 == 0)
model.add(-z4_6_3 - y4_5_4 - x3_6_4 + x4_6_4 + y4_6_4 + z4_6_4 - 1.0 == 0)
model.add(-z5_6_3 - y5_5_4 - x4_6_4 + x5_6_4 + y5_6_4 + z5_6_4 - 1.0 == 0)
model.add(-z6_6_3 - y6_5_4 - x5_6_4 + x6_6_4 + y6_6_4 + z6_6_4 - 1.0 == 0)
model.add(-z7_6_3 - y7_5_4 - x6_6_4 + x7_6_4 + y7_6_4 + z7_6_4 - 1.0 == 0)
model.add(-z8_6_3 - y8_5_4 - x7_6_4 + x8_6_4 + y8_6_4 + z8_6_4 - 1.0 == 0)
model.add(-z9_6_3 - y9_5_4 - x8_6_4 + x9_6_4 + y9_6_4 + z9_6_4 - 1.0 == 0)
model.add(-x9_6_4 - z10_6_3 - y10_5_4 + y10_6_4 + z10_6_4 + y11_6_4 + z11_6_4 - 1.0 == 0)
model.add(-z1_7_3 - y1_6_4 + x1_7_4 + y1_7_4 + z1_7_4 + y0_7_4 + z0_7_4 - 1.0 == 0)
model.add(-z2_7_3 - y2_6_4 - x1_7_4 + x2_7_4 + y2_7_4 + z2_7_4 - 1.0 == 0)
model.add(-z3_7_3 - y3_6_4 - x2_7_4 + x3_7_4 + y3_7_4 + z3_7_4 - 1.0 == 0)
model.add(-z4_7_3 - y4_6_4 - x3_7_4 + x4_7_4 + y4_7_4 + z4_7_4 - 1.0 == 0)
model.add(-z5_7_3 - y5_6_4 - x4_7_4 + x5_7_4 + y5_7_4 + z5_7_4 - 1.0 == 0)
model.add(-z6_7_3 - y6_6_4 - x5_7_4 + x6_7_4 + y6_7_4 + z6_7_4 - 1.0 == 0)
model.add(-z7_7_3 - y7_6_4 - x6_7_4 + x7_7_4 + y7_7_4 + z7_7_4 - 1.0 == 0)
model.add(-z8_7_3 - y8_6_4 - x7_7_4 + x8_7_4 + y8_7_4 + z8_7_4 - 1.0 == 0)
model.add(-z9_7_3 - y9_6_4 - x8_7_4 + x9_7_4 + y9_7_4 + z9_7_4 - 1.0 == 0)
model.add(-x9_7_4 - z10_7_3 - y10_6_4 + y10_7_4 + z10_7_4 + y11_7_4 + z11_7_4 - 1.0 == 0)
model.add(-z1_8_3 - y1_7_4 + x1_8_4 + y1_8_4 + z1_8_4 + y0_8_4 + z0_8_4 - 1.0 == 0)
model.add(-z2_8_3 - y2_7_4 - x1_8_4 + x2_8_4 + y2_8_4 + z2_8_4 - 1.0 == 0)
model.add(-z3_8_3 - y3_7_4 - x2_8_4 + x3_8_4 + y3_8_4 + z3_8_4 - 1.0 == 0)
model.add(-z4_8_3 - y4_7_4 - x3_8_4 + x4_8_4 + y4_8_4 + z4_8_4 - 1.0 == 0)
model.add(-z5_8_3 - y5_7_4 - x4_8_4 + x5_8_4 + y5_8_4 + z5_8_4 - 1.0 == 0)
model.add(-z6_8_3 - y6_7_4 - x5_8_4 + x6_8_4 + y6_8_4 + z6_8_4 - 1.0 == 0)
model.add(-z7_8_3 - y7_7_4 - x6_8_4 + x7_8_4 + y7_8_4 + z7_8_4 - 1.0 == 0)
model.add(-z8_8_3 - y8_7_4 - x7_8_4 + x8_8_4 + y8_8_4 + z8_8_4 - 1.0 == 0)
model.add(-z9_8_3 - y9_7_4 - x8_8_4 + x9_8_4 + y9_8_4 + z9_8_4 - 1.0 == 0)
model.add(-x9_8_4 - z10_8_3 - y10_7_4 + y10_8_4 + z10_8_4 + y11_8_4 + z11_8_4 - 1.0 == 0)
model.add(-z1_9_3 - y1_8_4 + x1_9_4 + y1_9_4 + z1_9_4 + y0_9_4 + z0_9_4 - 1.0 == 0)
model.add(-z2_9_3 - y2_8_4 - x1_9_4 + x2_9_4 + y2_9_4 + z2_9_4 - 1.0 == 0)
model.add(-z3_9_3 - y3_8_4 - x2_9_4 + x3_9_4 + y3_9_4 + z3_9_4 - 1.0 == 0)
model.add(-z4_9_3 - y4_8_4 - x3_9_4 + x4_9_4 + y4_9_4 + z4_9_4 - 1.0 == 0)
model.add(-z5_9_3 - y5_8_4 - x4_9_4 + x5_9_4 + y5_9_4 + z5_9_4 - 1.0 == 0)
model.add(-z6_9_3 - y6_8_4 - x5_9_4 + x6_9_4 + y6_9_4 + z6_9_4 - 1.0 == 0)
model.add(-z7_9_3 - y7_8_4 - x6_9_4 + x7_9_4 + y7_9_4 + z7_9_4 - 1.0 == 0)
model.add(-z8_9_3 - y8_8_4 - x7_9_4 + x8_9_4 + y8_9_4 + z8_9_4 - 1.0 == 0)
model.add(-z9_9_3 - y9_8_4 - x8_9_4 + x9_9_4 + y9_9_4 + z9_9_4 - 1.0 == 0)
model.add(-x9_9_4 - z10_9_3 - y10_8_4 + y10_9_4 + z10_9_4 + y11_9_4 + z11_9_4 - 1.0 == 0)
model.add(-y1_9_4 - z1_10_3 + x1_10_4 + z1_10_4 + y0_10_4 + z0_10_4 + x1_11_4 + z1_11_4 - 1.0 == 0)
model.add(-y2_9_4 - z2_10_3 - x1_10_4 + x2_10_4 + z2_10_4 + x2_11_4 + z2_11_4 - 1.0 == 0)
model.add(-y3_9_4 - z3_10_3 - x2_10_4 + x3_10_4 + z3_10_4 + x3_11_4 + z3_11_4 - 1.0 == 0)
model.add(-y4_9_4 - z4_10_3 - x3_10_4 + x4_10_4 + z4_10_4 + x4_11_4 + z4_11_4 - 1.0 == 0)
model.add(-y5_9_4 - z5_10_3 - x4_10_4 + x5_10_4 + z5_10_4 + x5_11_4 + z5_11_4 - 1.0 == 0)
model.add(-y6_9_4 - z6_10_3 - x5_10_4 + x6_10_4 + z6_10_4 + x6_11_4 + z6_11_4 - 1.0 == 0)
model.add(-y7_9_4 - z7_10_3 - x6_10_4 + x7_10_4 + z7_10_4 + x7_11_4 + z7_11_4 - 1.0 == 0)
model.add(-y8_9_4 - z8_10_3 - x7_10_4 + x8_10_4 + z8_10_4 + x8_11_4 + z8_11_4 - 1.0 == 0)
model.add(-y9_9_4 - z9_10_3 - x8_10_4 + x9_10_4 + z9_10_4 + x9_11_4 + z9_11_4 - 1.0 == 0)
model.add(-y10_9_4 - x9_10_4 + y11_10_4 + z11_10_4 + x10_11_4 + z10_11_4 - 1.0 == 0)
model.add(-z1_1_4 + x1_1_5 + y1_1_5 + z1_1_5 + y0_1_5 + z0_1_5 + x1_0_5 + z1_0_5 - 1.0 == 0)
model.add(-z2_1_4 - x1_1_5 + x2_1_5 + y2_1_5 + z2_1_5 + x2_0_5 + z2_0_5 - 1.0 == 0)
model.add(-z3_1_4 - x2_1_5 + x3_1_5 + y3_1_5 + z3_1_5 + x3_0_5 + z3_0_5 - 1.0 == 0)
model.add(-z4_1_4 - x3_1_5 + x4_1_5 + y4_1_5 + z4_1_5 + x4_0_5 + z4_0_5 - 1.0 == 0)
model.add(-z5_1_4 - x4_1_5 + x5_1_5 + y5_1_5 + z5_1_5 + x5_0_5 + z5_0_5 - 1.0 == 0)
model.add(-z6_1_4 - x5_1_5 + x6_1_5 + y6_1_5 + z6_1_5 + x6_0_5 + z6_0_5 - 1.0 == 0)
model.add(-z7_1_4 - x6_1_5 + x7_1_5 + y7_1_5 + z7_1_5 + x7_0_5 + z7_0_5 - 1.0 == 0)
model.add(-z8_1_4 - x7_1_5 + x8_1_5 + y8_1_5 + z8_1_5 + x8_0_5 + z8_0_5 - 1.0 == 0)
model.add(-z9_1_4 - x8_1_5 + x9_1_5 + y9_1_5 + z9_1_5 + x9_0_5 + z9_0_5 - 1.0 == 0)
model.add(-x9_1_5 - z10_1_4 + y10_1_5 + z10_1_5 + y11_1_5 + z11_1_5 + x10_0_5 + z10_0_5 - 1.0 == 0)
model.add(-z1_2_4 - y1_1_5 + x1_2_5 + y1_2_5 + z1_2_5 + y0_2_5 + z0_2_5 - 1.0 == 0)
model.add(-z2_2_4 - y2_1_5 - x1_2_5 + x2_2_5 + y2_2_5 + z2_2_5 - 1.0 == 0)
model.add(-z3_2_4 - y3_1_5 - x2_2_5 + x3_2_5 + y3_2_5 + z3_2_5 - 1.0 == 0)
model.add(-z4_2_4 - y4_1_5 - x3_2_5 + x4_2_5 + y4_2_5 + z4_2_5 - 1.0 == 0)
model.add(-z5_2_4 - y5_1_5 - x4_2_5 + x5_2_5 + y5_2_5 + z5_2_5 - 1.0 == 0)
model.add(-z6_2_4 - y6_1_5 - x5_2_5 + x6_2_5 + y6_2_5 + z6_2_5 - 1.0 == 0)
model.add(-z7_2_4 - y7_1_5 - x6_2_5 + x7_2_5 + y7_2_5 + z7_2_5 - 1.0 == 0)
model.add(-z8_2_4 - y8_1_5 - x7_2_5 + x8_2_5 + y8_2_5 + z8_2_5 - 1.0 == 0)
model.add(-z9_2_4 - y9_1_5 - x8_2_5 + x9_2_5 + y9_2_5 + z9_2_5 - 1.0 == 0)
model.add(-x9_2_5 - z10_2_4 - y10_1_5 + y10_2_5 + z10_2_5 + y11_2_5 + z11_2_5 - 1.0 == 0)
model.add(-z1_3_4 - y1_2_5 + x1_3_5 + y1_3_5 + z1_3_5 + y0_3_5 + z0_3_5 - 1.0 == 0)
model.add(-z2_3_4 - y2_2_5 - x1_3_5 + x2_3_5 + y2_3_5 + z2_3_5 - 1.0 == 0)
model.add(-z3_3_4 - y3_2_5 - x2_3_5 + x3_3_5 + y3_3_5 + z3_3_5 - 1.0 == 0)
model.add(-z4_3_4 - y4_2_5 - x3_3_5 + x4_3_5 + y4_3_5 + z4_3_5 - 1.0 == 0)
model.add(-z5_3_4 - y5_2_5 - x4_3_5 + x5_3_5 + y5_3_5 + z5_3_5 - 1.0 == 0)
model.add(-z6_3_4 - y6_2_5 - x5_3_5 + x6_3_5 + y6_3_5 + z6_3_5 - 1.0 == 0)
model.add(-z7_3_4 - y7_2_5 - x6_3_5 + x7_3_5 + y7_3_5 + z7_3_5 - 1.0 == 0)
model.add(-z8_3_4 - y8_2_5 - x7_3_5 + x8_3_5 + y8_3_5 + z8_3_5 - 1.0 == 0)
model.add(-z9_3_4 - y9_2_5 - x8_3_5 + x9_3_5 + y9_3_5 + z9_3_5 - 1.0 == 0)
model.add(-x9_3_5 - z10_3_4 - y10_2_5 + y10_3_5 + z10_3_5 + y11_3_5 + z11_3_5 - 1.0 == 0)
model.add(-z1_4_4 - y1_3_5 + x1_4_5 + y1_4_5 + z1_4_5 + y0_4_5 + z0_4_5 - 1.0 == 0)
model.add(-z2_4_4 - y2_3_5 - x1_4_5 + x2_4_5 + y2_4_5 + z2_4_5 - 1.0 == 0)
model.add(-z3_4_4 - y3_3_5 - x2_4_5 + x3_4_5 + y3_4_5 + z3_4_5 - 1.0 == 0)
model.add(-z4_4_4 - y4_3_5 - x3_4_5 + x4_4_5 + y4_4_5 + z4_4_5 - 1.0 == 0)
model.add(-z5_4_4 - y5_3_5 - x4_4_5 + x5_4_5 + y5_4_5 + z5_4_5 - 1.0 == 0)
model.add(-z6_4_4 - y6_3_5 - x5_4_5 + x6_4_5 + y6_4_5 + z6_4_5 - 1.0 == 0)
model.add(-z7_4_4 - y7_3_5 - x6_4_5 + x7_4_5 + y7_4_5 + z7_4_5 - 1.0 == 0)
model.add(-z8_4_4 - y8_3_5 - x7_4_5 + x8_4_5 + y8_4_5 + z8_4_5 - 1.0 == 0)
model.add(-z9_4_4 - y9_3_5 - x8_4_5 + x9_4_5 + y9_4_5 + z9_4_5 - 1.0 == 0)
model.add(-x9_4_5 - z10_4_4 - y10_3_5 + y10_4_5 + z10_4_5 + y11_4_5 + z11_4_5 - 1.0 == 0)
model.add(-z1_5_4 - y1_4_5 + x1_5_5 + y1_5_5 + z1_5_5 + y0_5_5 + z0_5_5 - 1.0 == 0)
model.add(-z2_5_4 - y2_4_5 - x1_5_5 + x2_5_5 + y2_5_5 + z2_5_5 - 1.0 == 0)
model.add(-z3_5_4 - y3_4_5 - x2_5_5 + x3_5_5 + y3_5_5 + z3_5_5 - 1.0 == 0)
model.add(-z4_5_4 - y4_4_5 - x3_5_5 + x4_5_5 + y4_5_5 + z4_5_5 - 1.0 == 0)
model.add(-z5_5_4 - y5_4_5 - x4_5_5 + x5_5_5 + y5_5_5 + z5_5_5 - 1.0 == 0)
model.add(-z6_5_4 - y6_4_5 - x5_5_5 + x6_5_5 + y6_5_5 + z6_5_5 - 1.0 == 0)
model.add(-z7_5_4 - y7_4_5 - x6_5_5 + x7_5_5 + y7_5_5 + z7_5_5 - 1.0 == 0)
model.add(-z8_5_4 - y8_4_5 - x7_5_5 + x8_5_5 + y8_5_5 + z8_5_5 - 1.0 == 0)
model.add(-z9_5_4 - y9_4_5 - x8_5_5 + x9_5_5 + y9_5_5 + z9_5_5 - 1.0 == 0)
model.add(-x9_5_5 - z10_5_4 - y10_4_5 + y10_5_5 + z10_5_5 + y11_5_5 + z11_5_5 - 1.0 == 0)
model.add(-z1_6_4 - y1_5_5 + x1_6_5 + y1_6_5 + z1_6_5 + y0_6_5 + z0_6_5 - 1.0 == 0)
model.add(-z2_6_4 - y2_5_5 - x1_6_5 + x2_6_5 + y2_6_5 + z2_6_5 - 1.0 == 0)
model.add(-z3_6_4 - y3_5_5 - x2_6_5 + x3_6_5 + y3_6_5 + z3_6_5 - 1.0 == 0)
model.add(-z4_6_4 - y4_5_5 - x3_6_5 + x4_6_5 + y4_6_5 + z4_6_5 - 1.0 == 0)
model.add(-z5_6_4 - y5_5_5 - x4_6_5 + x5_6_5 + y5_6_5 + z5_6_5 - 1.0 == 0)
model.add(-z6_6_4 - y6_5_5 - x5_6_5 + x6_6_5 + y6_6_5 + z6_6_5 - 1.0 == 0)
model.add(-z7_6_4 - y7_5_5 - x6_6_5 + x7_6_5 + y7_6_5 + z7_6_5 - 1.0 == 0)
model.add(-z8_6_4 - y8_5_5 - x7_6_5 + x8_6_5 + y8_6_5 + z8_6_5 - 1.0 == 0)
model.add(-z9_6_4 - y9_5_5 - x8_6_5 + x9_6_5 + y9_6_5 + z9_6_5 - 1.0 == 0)
model.add(-x9_6_5 - z10_6_4 - y10_5_5 + y10_6_5 + z10_6_5 + y11_6_5 + z11_6_5 - 1.0 == 0)
model.add(-z1_7_4 - y1_6_5 + x1_7_5 + y1_7_5 + z1_7_5 + y0_7_5 + z0_7_5 - 1.0 == 0)
model.add(-z2_7_4 - y2_6_5 - x1_7_5 + x2_7_5 + y2_7_5 + z2_7_5 - 1.0 == 0)
model.add(-z3_7_4 - y3_6_5 - x2_7_5 + x3_7_5 + y3_7_5 + z3_7_5 - 1.0 == 0)
model.add(-z4_7_4 - y4_6_5 - x3_7_5 + x4_7_5 + y4_7_5 + z4_7_5 - 1.0 == 0)
model.add(-z5_7_4 - y5_6_5 - x4_7_5 + x5_7_5 + y5_7_5 + z5_7_5 - 1.0 == 0)
model.add(-z6_7_4 - y6_6_5 - x5_7_5 + x6_7_5 + y6_7_5 + z6_7_5 - 1.0 == 0)
model.add(-z7_7_4 - y7_6_5 - x6_7_5 + x7_7_5 + y7_7_5 + z7_7_5 - 1.0 == 0)
model.add(-z8_7_4 - y8_6_5 - x7_7_5 + x8_7_5 + y8_7_5 + z8_7_5 - 1.0 == 0)
model.add(-z9_7_4 - y9_6_5 - x8_7_5 + x9_7_5 + y9_7_5 + z9_7_5 - 1.0 == 0)
model.add(-x9_7_5 - z10_7_4 - y10_6_5 + y10_7_5 + z10_7_5 + y11_7_5 + z11_7_5 - 1.0 == 0)
model.add(-z1_8_4 - y1_7_5 + x1_8_5 + y1_8_5 + z1_8_5 + y0_8_5 + z0_8_5 - 1.0 == 0)
model.add(-z2_8_4 - y2_7_5 - x1_8_5 + x2_8_5 + y2_8_5 + z2_8_5 - 1.0 == 0)
model.add(-z3_8_4 - y3_7_5 - x2_8_5 + x3_8_5 + y3_8_5 + z3_8_5 - 1.0 == 0)
model.add(-z4_8_4 - y4_7_5 - x3_8_5 + x4_8_5 + y4_8_5 + z4_8_5 - 1.0 == 0)
model.add(-z5_8_4 - y5_7_5 - x4_8_5 + x5_8_5 + y5_8_5 + z5_8_5 - 1.0 == 0)
model.add(-z6_8_4 - y6_7_5 - x5_8_5 + x6_8_5 + y6_8_5 + z6_8_5 - 1.0 == 0)
model.add(-z7_8_4 - y7_7_5 - x6_8_5 + x7_8_5 + y7_8_5 + z7_8_5 - 1.0 == 0)
model.add(-z8_8_4 - y8_7_5 - x7_8_5 + x8_8_5 + y8_8_5 + z8_8_5 - 1.0 == 0)
model.add(-z9_8_4 - y9_7_5 - x8_8_5 + x9_8_5 + y9_8_5 + z9_8_5 - 1.0 == 0)
model.add(-x9_8_5 - z10_8_4 - y10_7_5 + y10_8_5 + z10_8_5 + y11_8_5 + z11_8_5 - 1.0 == 0)
model.add(-z1_9_4 - y1_8_5 + x1_9_5 + y1_9_5 + z1_9_5 + y0_9_5 + z0_9_5 - 1.0 == 0)
model.add(-z2_9_4 - y2_8_5 - x1_9_5 + x2_9_5 + y2_9_5 + z2_9_5 - 1.0 == 0)
model.add(-z3_9_4 - y3_8_5 - x2_9_5 + x3_9_5 + y3_9_5 + z3_9_5 - 1.0 == 0)
model.add(-z4_9_4 - y4_8_5 - x3_9_5 + x4_9_5 + y4_9_5 + z4_9_5 - 1.0 == 0)
model.add(-z5_9_4 - y5_8_5 - x4_9_5 + x5_9_5 + y5_9_5 + z5_9_5 - 1.0 == 0)
model.add(-z6_9_4 - y6_8_5 - x5_9_5 + x6_9_5 + y6_9_5 + z6_9_5 - 1.0 == 0)
model.add(-z7_9_4 - y7_8_5 - x6_9_5 + x7_9_5 + y7_9_5 + z7_9_5 - 1.0 == 0)
model.add(-z8_9_4 - y8_8_5 - x7_9_5 + x8_9_5 + y8_9_5 + z8_9_5 - 1.0 == 0)
model.add(-z9_9_4 - y9_8_5 - x8_9_5 + x9_9_5 + y9_9_5 + z9_9_5 - 1.0 == 0)
model.add(-x9_9_5 - z10_9_4 - y10_8_5 + y10_9_5 + z10_9_5 + y11_9_5 + z11_9_5 - 1.0 == 0)
model.add(-y1_9_5 - z1_10_4 + x1_10_5 + z1_10_5 + y0_10_5 + z0_10_5 + x1_11_5 + z1_11_5 - 1.0 == 0)
model.add(-y2_9_5 - z2_10_4 - x1_10_5 + x2_10_5 + z2_10_5 + x2_11_5 + z2_11_5 - 1.0 == 0)
model.add(-y3_9_5 - z3_10_4 - x2_10_5 + x3_10_5 + z3_10_5 + x3_11_5 + z3_11_5 - 1.0 == 0)
model.add(-y4_9_5 - z4_10_4 - x3_10_5 + x4_10_5 + z4_10_5 + x4_11_5 + z4_11_5 - 1.0 == 0)
model.add(-y5_9_5 - z5_10_4 - x4_10_5 + x5_10_5 + z5_10_5 + x5_11_5 + z5_11_5 - 1.0 == 0)
model.add(-y6_9_5 - z6_10_4 - x5_10_5 + x6_10_5 + z6_10_5 + x6_11_5 + z6_11_5 - 1.0 == 0)
model.add(-y7_9_5 - z7_10_4 - x6_10_5 + x7_10_5 + z7_10_5 + x7_11_5 + z7_11_5 - 1.0 == 0)
model.add(-y8_9_5 - z8_10_4 - x7_10_5 + x8_10_5 + z8_10_5 + x8_11_5 + z8_11_5 - 1.0 == 0)
model.add(-y9_9_5 - z9_10_4 - x8_10_5 + x9_10_5 + z9_10_5 + x9_11_5 + z9_11_5 - 1.0 == 0)
model.add(-y10_9_5 - x9_10_5 + y11_10_5 + z11_10_5 + x10_11_5 + z10_11_5 - 1.0 == 0)
model.add(-z1_1_5 + x1_1_6 + y1_1_6 + z1_1_6 + y0_1_6 + z0_1_6 + x1_0_6 + z1_0_6 - 1.0 == 0)
model.add(-z2_1_5 - x1_1_6 + x2_1_6 + y2_1_6 + z2_1_6 + x2_0_6 + z2_0_6 - 1.0 == 0)
model.add(-z3_1_5 - x2_1_6 + x3_1_6 + y3_1_6 + z3_1_6 + x3_0_6 + z3_0_6 - 1.0 == 0)
model.add(-z4_1_5 - x3_1_6 + x4_1_6 + y4_1_6 + z4_1_6 + x4_0_6 + z4_0_6 - 1.0 == 0)
model.add(-z5_1_5 - x4_1_6 + x5_1_6 + y5_1_6 + z5_1_6 + x5_0_6 + z5_0_6 - 1.0 == 0)
model.add(-z6_1_5 - x5_1_6 + x6_1_6 + y6_1_6 + z6_1_6 + x6_0_6 + z6_0_6 - 1.0 == 0)
model.add(-z7_1_5 - x6_1_6 + x7_1_6 + y7_1_6 + z7_1_6 + x7_0_6 + z7_0_6 - 1.0 == 0)
model.add(-z8_1_5 - x7_1_6 + x8_1_6 + y8_1_6 + z8_1_6 + x8_0_6 + z8_0_6 - 1.0 == 0)
model.add(-z9_1_5 - x8_1_6 + x9_1_6 + y9_1_6 + z9_1_6 + x9_0_6 + z9_0_6 - 1.0 == 0)
model.add(-x9_1_6 - z10_1_5 + y10_1_6 + z10_1_6 + y11_1_6 + z11_1_6 + x10_0_6 + z10_0_6 - 1.0 == 0)
model.add(-z1_2_5 - y1_1_6 + x1_2_6 + y1_2_6 + z1_2_6 + y0_2_6 + z0_2_6 - 1.0 == 0)
model.add(-z2_2_5 - y2_1_6 - x1_2_6 + x2_2_6 + y2_2_6 + z2_2_6 - 1.0 == 0)
model.add(-z3_2_5 - y3_1_6 - x2_2_6 + x3_2_6 + y3_2_6 + z3_2_6 - 1.0 == 0)
model.add(-z4_2_5 - y4_1_6 - x3_2_6 + x4_2_6 + y4_2_6 + z4_2_6 - 1.0 == 0)
model.add(-z5_2_5 - y5_1_6 - x4_2_6 + x5_2_6 + y5_2_6 + z5_2_6 - 1.0 == 0)
model.add(-z6_2_5 - y6_1_6 - x5_2_6 + x6_2_6 + y6_2_6 + z6_2_6 - 1.0 == 0)
model.add(-z7_2_5 - y7_1_6 - x6_2_6 + x7_2_6 + y7_2_6 + z7_2_6 - 1.0 == 0)
model.add(-z8_2_5 - y8_1_6 - x7_2_6 + x8_2_6 + y8_2_6 + z8_2_6 - 1.0 == 0)
model.add(-z9_2_5 - y9_1_6 - x8_2_6 + x9_2_6 + y9_2_6 + z9_2_6 - 1.0 == 0)
model.add(-x9_2_6 - z10_2_5 - y10_1_6 + y10_2_6 + z10_2_6 + y11_2_6 + z11_2_6 - 1.0 == 0)
model.add(-z1_3_5 - y1_2_6 + x1_3_6 + y1_3_6 + z1_3_6 + y0_3_6 + z0_3_6 - 1.0 == 0)
model.add(-z2_3_5 - y2_2_6 - x1_3_6 + x2_3_6 + y2_3_6 + z2_3_6 - 1.0 == 0)
model.add(-z3_3_5 - y3_2_6 - x2_3_6 + x3_3_6 + y3_3_6 + z3_3_6 - 1.0 == 0)
model.add(-z4_3_5 - y4_2_6 - x3_3_6 + x4_3_6 + y4_3_6 + z4_3_6 - 1.0 == 0)
model.add(-z5_3_5 - y5_2_6 - x4_3_6 + x5_3_6 + y5_3_6 + z5_3_6 - 1.0 == 0)
model.add(-z6_3_5 - y6_2_6 - x5_3_6 + x6_3_6 + y6_3_6 + z6_3_6 - 1.0 == 0)
model.add(-z7_3_5 - y7_2_6 - x6_3_6 + x7_3_6 + y7_3_6 + z7_3_6 - 1.0 == 0)
model.add(-z8_3_5 - y8_2_6 - x7_3_6 + x8_3_6 + y8_3_6 + z8_3_6 - 1.0 == 0)
model.add(-z9_3_5 - y9_2_6 - x8_3_6 + x9_3_6 + y9_3_6 + z9_3_6 - 1.0 == 0)
model.add(-x9_3_6 - z10_3_5 - y10_2_6 + y10_3_6 + z10_3_6 + y11_3_6 + z11_3_6 - 1.0 == 0)
model.add(-z1_4_5 - y1_3_6 + x1_4_6 + y1_4_6 + z1_4_6 + y0_4_6 + z0_4_6 - 1.0 == 0)
model.add(-z2_4_5 - y2_3_6 - x1_4_6 + x2_4_6 + y2_4_6 + z2_4_6 - 1.0 == 0)
model.add(-z3_4_5 - y3_3_6 - x2_4_6 + x3_4_6 + y3_4_6 + z3_4_6 - 1.0 == 0)
model.add(-z4_4_5 - y4_3_6 - x3_4_6 + x4_4_6 + y4_4_6 + z4_4_6 - 1.0 == 0)
model.add(-z5_4_5 - y5_3_6 - x4_4_6 + x5_4_6 + y5_4_6 + z5_4_6 - 1.0 == 0)
model.add(-z6_4_5 - y6_3_6 - x5_4_6 + x6_4_6 + y6_4_6 + z6_4_6 - 1.0 == 0)
model.add(-z7_4_5 - y7_3_6 - x6_4_6 + x7_4_6 + y7_4_6 + z7_4_6 - 1.0 == 0)
model.add(-z8_4_5 - y8_3_6 - x7_4_6 + x8_4_6 + y8_4_6 + z8_4_6 - 1.0 == 0)
model.add(-z9_4_5 - y9_3_6 - x8_4_6 + x9_4_6 + y9_4_6 + z9_4_6 - 1.0 == 0)
model.add(-x9_4_6 - z10_4_5 - y10_3_6 + y10_4_6 + z10_4_6 + y11_4_6 + z11_4_6 - 1.0 == 0)
model.add(-z1_5_5 - y1_4_6 + x1_5_6 + y1_5_6 + z1_5_6 + y0_5_6 + z0_5_6 - 1.0 == 0)
model.add(-z2_5_5 - y2_4_6 - x1_5_6 + x2_5_6 + y2_5_6 + z2_5_6 - 1.0 == 0)
model.add(-z3_5_5 - y3_4_6 - x2_5_6 + x3_5_6 + y3_5_6 + z3_5_6 - 1.0 == 0)
model.add(-z4_5_5 - y4_4_6 - x3_5_6 + x4_5_6 + y4_5_6 + z4_5_6 - 1.0 == 0)
model.add(-z5_5_5 - y5_4_6 - x4_5_6 + x5_5_6 + y5_5_6 + z5_5_6 - 1.0 == 0)
model.add(-z6_5_5 - y6_4_6 - x5_5_6 + x6_5_6 + y6_5_6 + z6_5_6 - 1.0 == 0)
model.add(-z7_5_5 - y7_4_6 - x6_5_6 + x7_5_6 + y7_5_6 + z7_5_6 - 1.0 == 0)
model.add(-z8_5_5 - y8_4_6 - x7_5_6 + x8_5_6 + y8_5_6 + z8_5_6 - 1.0 == 0)
model.add(-z9_5_5 - y9_4_6 - x8_5_6 + x9_5_6 + y9_5_6 + z9_5_6 - 1.0 == 0)
model.add(-x9_5_6 - z10_5_5 - y10_4_6 + y10_5_6 + z10_5_6 + y11_5_6 + z11_5_6 - 1.0 == 0)
model.add(-z1_6_5 - y1_5_6 + x1_6_6 + y1_6_6 + z1_6_6 + y0_6_6 + z0_6_6 - 1.0 == 0)
model.add(-z2_6_5 - y2_5_6 - x1_6_6 + x2_6_6 + y2_6_6 + z2_6_6 - 1.0 == 0)
model.add(-z3_6_5 - y3_5_6 - x2_6_6 + x3_6_6 + y3_6_6 + z3_6_6 - 1.0 == 0)
model.add(-z4_6_5 - y4_5_6 - x3_6_6 + x4_6_6 + y4_6_6 + z4_6_6 - 1.0 == 0)
model.add(-z5_6_5 - y5_5_6 - x4_6_6 + x5_6_6 + y5_6_6 + z5_6_6 - 1.0 == 0)
model.add(-z6_6_5 - y6_5_6 - x5_6_6 + x6_6_6 + y6_6_6 + z6_6_6 - 1.0 == 0)
model.add(-z7_6_5 - y7_5_6 - x6_6_6 + x7_6_6 + y7_6_6 + z7_6_6 - 1.0 == 0)
model.add(-z8_6_5 - y8_5_6 - x7_6_6 + x8_6_6 + y8_6_6 + z8_6_6 - 1.0 == 0)
model.add(-z9_6_5 - y9_5_6 - x8_6_6 + x9_6_6 + y9_6_6 + z9_6_6 - 1.0 == 0)
model.add(-x9_6_6 - z10_6_5 - y10_5_6 + y10_6_6 + z10_6_6 + y11_6_6 + z11_6_6 - 1.0 == 0)
model.add(-z1_7_5 - y1_6_6 + x1_7_6 + y1_7_6 + z1_7_6 + y0_7_6 + z0_7_6 - 1.0 == 0)
model.add(-z2_7_5 - y2_6_6 - x1_7_6 + x2_7_6 + y2_7_6 + z2_7_6 - 1.0 == 0)
model.add(-z3_7_5 - y3_6_6 - x2_7_6 + x3_7_6 + y3_7_6 + z3_7_6 - 1.0 == 0)
model.add(-z4_7_5 - y4_6_6 - x3_7_6 + x4_7_6 + y4_7_6 + z4_7_6 - 1.0 == 0)
model.add(-z5_7_5 - y5_6_6 - x4_7_6 + x5_7_6 + y5_7_6 + z5_7_6 - 1.0 == 0)
model.add(-z6_7_5 - y6_6_6 - x5_7_6 + x6_7_6 + y6_7_6 + z6_7_6 - 1.0 == 0)
model.add(-z7_7_5 - y7_6_6 - x6_7_6 + x7_7_6 + y7_7_6 + z7_7_6 - 1.0 == 0)
model.add(-z8_7_5 - y8_6_6 - x7_7_6 + x8_7_6 + y8_7_6 + z8_7_6 - 1.0 == 0)
model.add(-z9_7_5 - y9_6_6 - x8_7_6 + x9_7_6 + y9_7_6 + z9_7_6 - 1.0 == 0)
model.add(-x9_7_6 - z10_7_5 - y10_6_6 + y10_7_6 + z10_7_6 + y11_7_6 + z11_7_6 - 1.0 == 0)
model.add(-z1_8_5 - y1_7_6 + x1_8_6 + y1_8_6 + z1_8_6 + y0_8_6 + z0_8_6 - 1.0 == 0)
model.add(-z2_8_5 - y2_7_6 - x1_8_6 + x2_8_6 + y2_8_6 + z2_8_6 - 1.0 == 0)
model.add(-z3_8_5 - y3_7_6 - x2_8_6 + x3_8_6 + y3_8_6 + z3_8_6 - 1.0 == 0)
model.add(-z4_8_5 - y4_7_6 - x3_8_6 + x4_8_6 + y4_8_6 + z4_8_6 - 1.0 == 0)
model.add(-z5_8_5 - y5_7_6 - x4_8_6 + x5_8_6 + y5_8_6 + z5_8_6 - 1.0 == 0)
model.add(-z6_8_5 - y6_7_6 - x5_8_6 + x6_8_6 + y6_8_6 + z6_8_6 - 1.0 == 0)
model.add(-z7_8_5 - y7_7_6 - x6_8_6 + x7_8_6 + y7_8_6 + z7_8_6 - 1.0 == 0)
model.add(-z8_8_5 - y8_7_6 - x7_8_6 + x8_8_6 + y8_8_6 + z8_8_6 - 1.0 == 0)
model.add(-z9_8_5 - y9_7_6 - x8_8_6 + x9_8_6 + y9_8_6 + z9_8_6 - 1.0 == 0)
model.add(-x9_8_6 - z10_8_5 - y10_7_6 + y10_8_6 + z10_8_6 + y11_8_6 + z11_8_6 - 1.0 == 0)
model.add(-z1_9_5 - y1_8_6 + x1_9_6 + y1_9_6 + z1_9_6 + y0_9_6 + z0_9_6 - 1.0 == 0)
model.add(-z2_9_5 - y2_8_6 - x1_9_6 + x2_9_6 + y2_9_6 + z2_9_6 - 1.0 == 0)
model.add(-z3_9_5 - y3_8_6 - x2_9_6 + x3_9_6 + y3_9_6 + z3_9_6 - 1.0 == 0)
model.add(-z4_9_5 - y4_8_6 - x3_9_6 + x4_9_6 + y4_9_6 + z4_9_6 - 1.0 == 0)
model.add(-z5_9_5 - y5_8_6 - x4_9_6 + x5_9_6 + y5_9_6 + z5_9_6 - 1.0 == 0)
model.add(-z6_9_5 - y6_8_6 - x5_9_6 + x6_9_6 + y6_9_6 + z6_9_6 - 1.0 == 0)
model.add(-z7_9_5 - y7_8_6 - x6_9_6 + x7_9_6 + y7_9_6 + z7_9_6 - 1.0 == 0)
model.add(-z8_9_5 - y8_8_6 - x7_9_6 + x8_9_6 + y8_9_6 + z8_9_6 - 1.0 == 0)
model.add(-z9_9_5 - y9_8_6 - x8_9_6 + x9_9_6 + y9_9_6 + z9_9_6 - 1.0 == 0)
model.add(-x9_9_6 - z10_9_5 - y10_8_6 + y10_9_6 + z10_9_6 + y11_9_6 + z11_9_6 - 1.0 == 0)
model.add(-y1_9_6 - z1_10_5 + x1_10_6 + z1_10_6 + y0_10_6 + z0_10_6 + x1_11_6 + z1_11_6 - 1.0 == 0)
model.add(-y2_9_6 - z2_10_5 - x1_10_6 + x2_10_6 + z2_10_6 + x2_11_6 + z2_11_6 - 1.0 == 0)
model.add(-y3_9_6 - z3_10_5 - x2_10_6 + x3_10_6 + z3_10_6 + x3_11_6 + z3_11_6 - 1.0 == 0)
model.add(-y4_9_6 - z4_10_5 - x3_10_6 + x4_10_6 + z4_10_6 + x4_11_6 + z4_11_6 - 1.0 == 0)
model.add(-y5_9_6 - z5_10_5 - x4_10_6 + x5_10_6 + z5_10_6 + x5_11_6 + z5_11_6 - 1.0 == 0)
model.add(-y6_9_6 - z6_10_5 - x5_10_6 + x6_10_6 + z6_10_6 + x6_11_6 + z6_11_6 - 1.0 == 0)
model.add(-y7_9_6 - z7_10_5 - x6_10_6 + x7_10_6 + z7_10_6 + x7_11_6 + z7_11_6 - 1.0 == 0)
model.add(-y8_9_6 - z8_10_5 - x7_10_6 + x8_10_6 + z8_10_6 + x8_11_6 + z8_11_6 - 1.0 == 0)
model.add(-y9_9_6 - z9_10_5 - x8_10_6 + x9_10_6 + z9_10_6 + x9_11_6 + z9_11_6 - 1.0 == 0)
model.add(-y10_9_6 - x9_10_6 + y11_10_6 + z11_10_6 + x10_11_6 + z10_11_6 - 1.0 == 0)
model.add(-z1_1_6 + x1_1_7 + y1_1_7 + z1_1_7 + y0_1_7 + z0_1_7 + x1_0_7 + z1_0_7 - 1.0 == 0)
model.add(-z2_1_6 - x1_1_7 + x2_1_7 + y2_1_7 + z2_1_7 + x2_0_7 + z2_0_7 - 1.0 == 0)
model.add(-z3_1_6 - x2_1_7 + x3_1_7 + y3_1_7 + z3_1_7 + x3_0_7 + z3_0_7 - 1.0 == 0)
model.add(-z4_1_6 - x3_1_7 + x4_1_7 + y4_1_7 + z4_1_7 + x4_0_7 + z4_0_7 - 1.0 == 0)
model.add(-z5_1_6 - x4_1_7 + x5_1_7 + y5_1_7 + z5_1_7 + x5_0_7 + z5_0_7 - 1.0 == 0)
model.add(-z6_1_6 - x5_1_7 + x6_1_7 + y6_1_7 + z6_1_7 + x6_0_7 + z6_0_7 - 1.0 == 0)
model.add(-z7_1_6 - x6_1_7 + x7_1_7 + y7_1_7 + z7_1_7 + x7_0_7 + z7_0_7 - 1.0 == 0)
model.add(-z8_1_6 - x7_1_7 + x8_1_7 + y8_1_7 + z8_1_7 + x8_0_7 + z8_0_7 - 1.0 == 0)
model.add(-z9_1_6 - x8_1_7 + x9_1_7 + y9_1_7 + z9_1_7 + x9_0_7 + z9_0_7 - 1.0 == 0)
model.add(-x9_1_7 - z10_1_6 + y10_1_7 + z10_1_7 + y11_1_7 + z11_1_7 + x10_0_7 + z10_0_7 - 1.0 == 0)
model.add(-z1_2_6 - y1_1_7 + x1_2_7 + y1_2_7 + z1_2_7 + y0_2_7 + z0_2_7 - 1.0 == 0)
model.add(-z2_2_6 - y2_1_7 - x1_2_7 + x2_2_7 + y2_2_7 + z2_2_7 - 1.0 == 0)
model.add(-z3_2_6 - y3_1_7 - x2_2_7 + x3_2_7 + y3_2_7 + z3_2_7 - 1.0 == 0)
model.add(-z4_2_6 - y4_1_7 - x3_2_7 + x4_2_7 + y4_2_7 + z4_2_7 - 1.0 == 0)
model.add(-z5_2_6 - y5_1_7 - x4_2_7 + x5_2_7 + y5_2_7 + z5_2_7 - 1.0 == 0)
model.add(-z6_2_6 - y6_1_7 - x5_2_7 + x6_2_7 + y6_2_7 + z6_2_7 - 1.0 == 0)
model.add(-z7_2_6 - y7_1_7 - x6_2_7 + x7_2_7 + y7_2_7 + z7_2_7 - 1.0 == 0)
model.add(-z8_2_6 - y8_1_7 - x7_2_7 + x8_2_7 + y8_2_7 + z8_2_7 - 1.0 == 0)
model.add(-z9_2_6 - y9_1_7 - x8_2_7 + x9_2_7 + y9_2_7 + z9_2_7 - 1.0 == 0)
model.add(-x9_2_7 - z10_2_6 - y10_1_7 + y10_2_7 + z10_2_7 + y11_2_7 + z11_2_7 - 1.0 == 0)
model.add(-z1_3_6 - y1_2_7 + x1_3_7 + y1_3_7 + z1_3_7 + y0_3_7 + z0_3_7 - 1.0 == 0)
model.add(-z2_3_6 - y2_2_7 - x1_3_7 + x2_3_7 + y2_3_7 + z2_3_7 - 1.0 == 0)
model.add(-z3_3_6 - y3_2_7 - x2_3_7 + x3_3_7 + y3_3_7 + z3_3_7 - 1.0 == 0)
model.add(-z4_3_6 - y4_2_7 - x3_3_7 + x4_3_7 + y4_3_7 + z4_3_7 - 1.0 == 0)
model.add(-z5_3_6 - y5_2_7 - x4_3_7 + x5_3_7 + y5_3_7 + z5_3_7 - 1.0 == 0)
model.add(-z6_3_6 - y6_2_7 - x5_3_7 + x6_3_7 + y6_3_7 + z6_3_7 - 1.0 == 0)
model.add(-z7_3_6 - y7_2_7 - x6_3_7 + x7_3_7 + y7_3_7 + z7_3_7 - 1.0 == 0)
model.add(-z8_3_6 - y8_2_7 - x7_3_7 + x8_3_7 + y8_3_7 + z8_3_7 - 1.0 == 0)
model.add(-z9_3_6 - y9_2_7 - x8_3_7 + x9_3_7 + y9_3_7 + z9_3_7 - 1.0 == 0)
model.add(-x9_3_7 - z10_3_6 - y10_2_7 + y10_3_7 + z10_3_7 + y11_3_7 + z11_3_7 - 1.0 == 0)
model.add(-z1_4_6 - y1_3_7 + x1_4_7 + y1_4_7 + z1_4_7 + y0_4_7 + z0_4_7 - 1.0 == 0)
model.add(-z2_4_6 - y2_3_7 - x1_4_7 + x2_4_7 + y2_4_7 + z2_4_7 - 1.0 == 0)
model.add(-z3_4_6 - y3_3_7 - x2_4_7 + x3_4_7 + y3_4_7 + z3_4_7 - 1.0 == 0)
model.add(-z4_4_6 - y4_3_7 - x3_4_7 + x4_4_7 + y4_4_7 + z4_4_7 - 1.0 == 0)
model.add(-z5_4_6 - y5_3_7 - x4_4_7 + x5_4_7 + y5_4_7 + z5_4_7 - 1.0 == 0)
model.add(-z6_4_6 - y6_3_7 - x5_4_7 + x6_4_7 + y6_4_7 + z6_4_7 - 1.0 == 0)
model.add(-z7_4_6 - y7_3_7 - x6_4_7 + x7_4_7 + y7_4_7 + z7_4_7 - 1.0 == 0)
model.add(-z8_4_6 - y8_3_7 - x7_4_7 + x8_4_7 + y8_4_7 + z8_4_7 - 1.0 == 0)
model.add(-z9_4_6 - y9_3_7 - x8_4_7 + x9_4_7 + y9_4_7 + z9_4_7 - 1.0 == 0)
model.add(-x9_4_7 - z10_4_6 - y10_3_7 + y10_4_7 + z10_4_7 + y11_4_7 + z11_4_7 - 1.0 == 0)
model.add(-z1_5_6 - y1_4_7 + x1_5_7 + y1_5_7 + z1_5_7 + y0_5_7 + z0_5_7 - 1.0 == 0)
model.add(-z2_5_6 - y2_4_7 - x1_5_7 + x2_5_7 + y2_5_7 + z2_5_7 - 1.0 == 0)
model.add(-z3_5_6 - y3_4_7 - x2_5_7 + x3_5_7 + y3_5_7 + z3_5_7 - 1.0 == 0)
model.add(-z4_5_6 - y4_4_7 - x3_5_7 + x4_5_7 + y4_5_7 + z4_5_7 - 1.0 == 0)
model.add(-z5_5_6 - y5_4_7 - x4_5_7 + x5_5_7 + y5_5_7 + z5_5_7 - 1.0 == 0)
model.add(-z6_5_6 - y6_4_7 - x5_5_7 + x6_5_7 + y6_5_7 + z6_5_7 - 1.0 == 0)
model.add(-z7_5_6 - y7_4_7 - x6_5_7 + x7_5_7 + y7_5_7 + z7_5_7 - 1.0 == 0)
model.add(-z8_5_6 - y8_4_7 - x7_5_7 + x8_5_7 + y8_5_7 + z8_5_7 - 1.0 == 0)
model.add(-z9_5_6 - y9_4_7 - x8_5_7 + x9_5_7 + y9_5_7 + z9_5_7 - 1.0 == 0)
model.add(-x9_5_7 - z10_5_6 - y10_4_7 + y10_5_7 + z10_5_7 + y11_5_7 + z11_5_7 - 1.0 == 0)
model.add(-z1_6_6 - y1_5_7 + x1_6_7 + y1_6_7 + z1_6_7 + y0_6_7 + z0_6_7 - 1.0 == 0)
model.add(-z2_6_6 - y2_5_7 - x1_6_7 + x2_6_7 + y2_6_7 + z2_6_7 - 1.0 == 0)
model.add(-z3_6_6 - y3_5_7 - x2_6_7 + x3_6_7 + y3_6_7 + z3_6_7 - 1.0 == 0)
model.add(-z4_6_6 - y4_5_7 - x3_6_7 + x4_6_7 + y4_6_7 + z4_6_7 - 1.0 == 0)
model.add(-z5_6_6 - y5_5_7 - x4_6_7 + x5_6_7 + y5_6_7 + z5_6_7 - 1.0 == 0)
model.add(-z6_6_6 - y6_5_7 - x5_6_7 + x6_6_7 + y6_6_7 + z6_6_7 - 1.0 == 0)
model.add(-z7_6_6 - y7_5_7 - x6_6_7 + x7_6_7 + y7_6_7 + z7_6_7 - 1.0 == 0)
model.add(-z8_6_6 - y8_5_7 - x7_6_7 + x8_6_7 + y8_6_7 + z8_6_7 - 1.0 == 0)
model.add(-z9_6_6 - y9_5_7 - x8_6_7 + x9_6_7 + y9_6_7 + z9_6_7 - 1.0 == 0)
model.add(-x9_6_7 - z10_6_6 - y10_5_7 + y10_6_7 + z10_6_7 + y11_6_7 + z11_6_7 - 1.0 == 0)
model.add(-z1_7_6 - y1_6_7 + x1_7_7 + y1_7_7 + z1_7_7 + y0_7_7 + z0_7_7 - 1.0 == 0)
model.add(-z2_7_6 - y2_6_7 - x1_7_7 + x2_7_7 + y2_7_7 + z2_7_7 - 1.0 == 0)
model.add(-z3_7_6 - y3_6_7 - x2_7_7 + x3_7_7 + y3_7_7 + z3_7_7 - 1.0 == 0)
model.add(-z4_7_6 - y4_6_7 - x3_7_7 + x4_7_7 + y4_7_7 + z4_7_7 - 1.0 == 0)
model.add(-z5_7_6 - y5_6_7 - x4_7_7 + x5_7_7 + y5_7_7 + z5_7_7 - 1.0 == 0)
model.add(-z6_7_6 - y6_6_7 - x5_7_7 + x6_7_7 + y6_7_7 + z6_7_7 - 1.0 == 0)
model.add(-z7_7_6 - y7_6_7 - x6_7_7 + x7_7_7 + y7_7_7 + z7_7_7 - 1.0 == 0)
model.add(-z8_7_6 - y8_6_7 - x7_7_7 + x8_7_7 + y8_7_7 + z8_7_7 - 1.0 == 0)
model.add(-z9_7_6 - y9_6_7 - x8_7_7 + x9_7_7 + y9_7_7 + z9_7_7 - 1.0 == 0)
model.add(-x9_7_7 - z10_7_6 - y10_6_7 + y10_7_7 + z10_7_7 + y11_7_7 + z11_7_7 - 1.0 == 0)
model.add(-z1_8_6 - y1_7_7 + x1_8_7 + y1_8_7 + z1_8_7 + y0_8_7 + z0_8_7 - 1.0 == 0)
model.add(-z2_8_6 - y2_7_7 - x1_8_7 + x2_8_7 + y2_8_7 + z2_8_7 - 1.0 == 0)
model.add(-z3_8_6 - y3_7_7 - x2_8_7 + x3_8_7 + y3_8_7 + z3_8_7 - 1.0 == 0)
model.add(-z4_8_6 - y4_7_7 - x3_8_7 + x4_8_7 + y4_8_7 + z4_8_7 - 1.0 == 0)
model.add(-z5_8_6 - y5_7_7 - x4_8_7 + x5_8_7 + y5_8_7 + z5_8_7 - 1.0 == 0)
model.add(-z6_8_6 - y6_7_7 - x5_8_7 + x6_8_7 + y6_8_7 + z6_8_7 - 1.0 == 0)
model.add(-z7_8_6 - y7_7_7 - x6_8_7 + x7_8_7 + y7_8_7 + z7_8_7 - 1.0 == 0)
model.add(-z8_8_6 - y8_7_7 - x7_8_7 + x8_8_7 + y8_8_7 + z8_8_7 - 1.0 == 0)
model.add(-z9_8_6 - y9_7_7 - x8_8_7 + x9_8_7 + y9_8_7 + z9_8_7 - 1.0 == 0)
model.add(-x9_8_7 - z10_8_6 - y10_7_7 + y10_8_7 + z10_8_7 + y11_8_7 + z11_8_7 - 1.0 == 0)
model.add(-z1_9_6 - y1_8_7 + x1_9_7 + y1_9_7 + z1_9_7 + y0_9_7 + z0_9_7 - 1.0 == 0)
model.add(-z2_9_6 - y2_8_7 - x1_9_7 + x2_9_7 + y2_9_7 + z2_9_7 - 1.0 == 0)
model.add(-z3_9_6 - y3_8_7 - x2_9_7 + x3_9_7 + y3_9_7 + z3_9_7 - 1.0 == 0)
model.add(-z4_9_6 - y4_8_7 - x3_9_7 + x4_9_7 + y4_9_7 + z4_9_7 - 1.0 == 0)
model.add(-z5_9_6 - y5_8_7 - x4_9_7 + x5_9_7 + y5_9_7 + z5_9_7 - 1.0 == 0)
model.add(-z6_9_6 - y6_8_7 - x5_9_7 + x6_9_7 + y6_9_7 + z6_9_7 - 1.0 == 0)
model.add(-z7_9_6 - y7_8_7 - x6_9_7 + x7_9_7 + y7_9_7 + z7_9_7 - 1.0 == 0)
model.add(-z8_9_6 - y8_8_7 - x7_9_7 + x8_9_7 + y8_9_7 + z8_9_7 - 1.0 == 0)
model.add(-z9_9_6 - y9_8_7 - x8_9_7 + x9_9_7 + y9_9_7 + z9_9_7 - 1.0 == 0)
model.add(-x9_9_7 - z10_9_6 - y10_8_7 + y10_9_7 + z10_9_7 + y11_9_7 + z11_9_7 - 1.0 == 0)
model.add(-y1_9_7 - z1_10_6 + x1_10_7 + z1_10_7 + y0_10_7 + z0_10_7 + x1_11_7 + z1_11_7 - 1.0 == 0)
model.add(-y2_9_7 - z2_10_6 - x1_10_7 + x2_10_7 + z2_10_7 + x2_11_7 + z2_11_7 - 1.0 == 0)
model.add(-y3_9_7 - z3_10_6 - x2_10_7 + x3_10_7 + z3_10_7 + x3_11_7 + z3_11_7 - 1.0 == 0)
model.add(-y4_9_7 - z4_10_6 - x3_10_7 + x4_10_7 + z4_10_7 + x4_11_7 + z4_11_7 - 1.0 == 0)
model.add(-y5_9_7 - z5_10_6 - x4_10_7 + x5_10_7 + z5_10_7 + x5_11_7 + z5_11_7 - 1.0 == 0)
model.add(-y6_9_7 - z6_10_6 - x5_10_7 + x6_10_7 + z6_10_7 + x6_11_7 + z6_11_7 - 1.0 == 0)
model.add(-y7_9_7 - z7_10_6 - x6_10_7 + x7_10_7 + z7_10_7 + x7_11_7 + z7_11_7 - 1.0 == 0)
model.add(-y8_9_7 - z8_10_6 - x7_10_7 + x8_10_7 + z8_10_7 + x8_11_7 + z8_11_7 - 1.0 == 0)
model.add(-y9_9_7 - z9_10_6 - x8_10_7 + x9_10_7 + z9_10_7 + x9_11_7 + z9_11_7 - 1.0 == 0)
model.add(-y10_9_7 - x9_10_7 + y11_10_7 + z11_10_7 + x10_11_7 + z10_11_7 - 1.0 == 0)
model.add(-z1_1_7 + x1_1_8 + y1_1_8 + z1_1_8 + y0_1_8 + z0_1_8 + x1_0_8 + z1_0_8 - 1.0 == 0)
model.add(-z2_1_7 - x1_1_8 + x2_1_8 + y2_1_8 + z2_1_8 + x2_0_8 + z2_0_8 - 1.0 == 0)
model.add(-z3_1_7 - x2_1_8 + x3_1_8 + y3_1_8 + z3_1_8 + x3_0_8 + z3_0_8 - 1.0 == 0)
model.add(-z4_1_7 - x3_1_8 + x4_1_8 + y4_1_8 + z4_1_8 + x4_0_8 + z4_0_8 - 1.0 == 0)
model.add(-z5_1_7 - x4_1_8 + x5_1_8 + y5_1_8 + z5_1_8 + x5_0_8 + z5_0_8 - 1.0 == 0)
model.add(-z6_1_7 - x5_1_8 + x6_1_8 + y6_1_8 + z6_1_8 + x6_0_8 + z6_0_8 - 1.0 == 0)
model.add(-z7_1_7 - x6_1_8 + x7_1_8 + y7_1_8 + z7_1_8 + x7_0_8 + z7_0_8 - 1.0 == 0)
model.add(-z8_1_7 - x7_1_8 + x8_1_8 + y8_1_8 + z8_1_8 + x8_0_8 + z8_0_8 - 1.0 == 0)
model.add(-z9_1_7 - x8_1_8 + x9_1_8 + y9_1_8 + z9_1_8 + x9_0_8 + z9_0_8 - 1.0 == 0)
model.add(-x9_1_8 - z10_1_7 + y10_1_8 + z10_1_8 + y11_1_8 + z11_1_8 + x10_0_8 + z10_0_8 - 1.0 == 0)
model.add(-z1_2_7 - y1_1_8 + x1_2_8 + y1_2_8 + z1_2_8 + y0_2_8 + z0_2_8 - 1.0 == 0)
model.add(-z2_2_7 - y2_1_8 - x1_2_8 + x2_2_8 + y2_2_8 + z2_2_8 - 1.0 == 0)
model.add(-z3_2_7 - y3_1_8 - x2_2_8 + x3_2_8 + y3_2_8 + z3_2_8 - 1.0 == 0)
model.add(-z4_2_7 - y4_1_8 - x3_2_8 + x4_2_8 + y4_2_8 + z4_2_8 - 1.0 == 0)
model.add(-z5_2_7 - y5_1_8 - x4_2_8 + x5_2_8 + y5_2_8 + z5_2_8 - 1.0 == 0)
model.add(-z6_2_7 - y6_1_8 - x5_2_8 + x6_2_8 + y6_2_8 + z6_2_8 - 1.0 == 0)
model.add(-z7_2_7 - y7_1_8 - x6_2_8 + x7_2_8 + y7_2_8 + z7_2_8 - 1.0 == 0)
model.add(-z8_2_7 - y8_1_8 - x7_2_8 + x8_2_8 + y8_2_8 + z8_2_8 - 1.0 == 0)
model.add(-z9_2_7 - y9_1_8 - x8_2_8 + x9_2_8 + y9_2_8 + z9_2_8 - 1.0 == 0)
model.add(-x9_2_8 - z10_2_7 - y10_1_8 + y10_2_8 + z10_2_8 + y11_2_8 + z11_2_8 - 1.0 == 0)
model.add(-z1_3_7 - y1_2_8 + x1_3_8 + y1_3_8 + z1_3_8 + y0_3_8 + z0_3_8 - 1.0 == 0)
model.add(-z2_3_7 - y2_2_8 - x1_3_8 + x2_3_8 + y2_3_8 + z2_3_8 - 1.0 == 0)
model.add(-z3_3_7 - y3_2_8 - x2_3_8 + x3_3_8 + y3_3_8 + z3_3_8 - 1.0 == 0)
model.add(-z4_3_7 - y4_2_8 - x3_3_8 + x4_3_8 + y4_3_8 + z4_3_8 - 1.0 == 0)
model.add(-z5_3_7 - y5_2_8 - x4_3_8 + x5_3_8 + y5_3_8 + z5_3_8 - 1.0 == 0)
model.add(-z6_3_7 - y6_2_8 - x5_3_8 + x6_3_8 + y6_3_8 + z6_3_8 - 1.0 == 0)
model.add(-z7_3_7 - y7_2_8 - x6_3_8 + x7_3_8 + y7_3_8 + z7_3_8 - 1.0 == 0)
model.add(-z8_3_7 - y8_2_8 - x7_3_8 + x8_3_8 + y8_3_8 + z8_3_8 - 1.0 == 0)
model.add(-z9_3_7 - y9_2_8 - x8_3_8 + x9_3_8 + y9_3_8 + z9_3_8 - 1.0 == 0)
model.add(-x9_3_8 - z10_3_7 - y10_2_8 + y10_3_8 + z10_3_8 + y11_3_8 + z11_3_8 - 1.0 == 0)
model.add(-z1_4_7 - y1_3_8 + x1_4_8 + y1_4_8 + z1_4_8 + y0_4_8 + z0_4_8 - 1.0 == 0)
model.add(-z2_4_7 - y2_3_8 - x1_4_8 + x2_4_8 + y2_4_8 + z2_4_8 - 1.0 == 0)
model.add(-z3_4_7 - y3_3_8 - x2_4_8 + x3_4_8 + y3_4_8 + z3_4_8 - 1.0 == 0)
model.add(-z4_4_7 - y4_3_8 - x3_4_8 + x4_4_8 + y4_4_8 + z4_4_8 - 1.0 == 0)
model.add(-z5_4_7 - y5_3_8 - x4_4_8 + x5_4_8 + y5_4_8 + z5_4_8 - 1.0 == 0)
model.add(-z6_4_7 - y6_3_8 - x5_4_8 + x6_4_8 + y6_4_8 + z6_4_8 - 1.0 == 0)
model.add(-z7_4_7 - y7_3_8 - x6_4_8 + x7_4_8 + y7_4_8 + z7_4_8 - 1.0 == 0)
model.add(-z8_4_7 - y8_3_8 - x7_4_8 + x8_4_8 + y8_4_8 + z8_4_8 - 1.0 == 0)
model.add(-z9_4_7 - y9_3_8 - x8_4_8 + x9_4_8 + y9_4_8 + z9_4_8 - 1.0 == 0)
model.add(-x9_4_8 - z10_4_7 - y10_3_8 + y10_4_8 + z10_4_8 + y11_4_8 + z11_4_8 - 1.0 == 0)
model.add(-z1_5_7 - y1_4_8 + x1_5_8 + y1_5_8 + z1_5_8 + y0_5_8 + z0_5_8 - 1.0 == 0)
model.add(-z2_5_7 - y2_4_8 - x1_5_8 + x2_5_8 + y2_5_8 + z2_5_8 - 1.0 == 0)
model.add(-z3_5_7 - y3_4_8 - x2_5_8 + x3_5_8 + y3_5_8 + z3_5_8 - 1.0 == 0)
model.add(-z4_5_7 - y4_4_8 - x3_5_8 + x4_5_8 + y4_5_8 + z4_5_8 - 1.0 == 0)
model.add(-z5_5_7 - y5_4_8 - x4_5_8 + x5_5_8 + y5_5_8 + z5_5_8 - 1.0 == 0)
model.add(-z6_5_7 - y6_4_8 - x5_5_8 + x6_5_8 + y6_5_8 + z6_5_8 - 1.0 == 0)
model.add(-z7_5_7 - y7_4_8 - x6_5_8 + x7_5_8 + y7_5_8 + z7_5_8 - 1.0 == 0)
model.add(-z8_5_7 - y8_4_8 - x7_5_8 + x8_5_8 + y8_5_8 + z8_5_8 - 1.0 == 0)
model.add(-z9_5_7 - y9_4_8 - x8_5_8 + x9_5_8 + y9_5_8 + z9_5_8 - 1.0 == 0)
model.add(-x9_5_8 - z10_5_7 - y10_4_8 + y10_5_8 + z10_5_8 + y11_5_8 + z11_5_8 - 1.0 == 0)
model.add(-z1_6_7 - y1_5_8 + x1_6_8 + y1_6_8 + z1_6_8 + y0_6_8 + z0_6_8 - 1.0 == 0)
model.add(-z2_6_7 - y2_5_8 - x1_6_8 + x2_6_8 + y2_6_8 + z2_6_8 - 1.0 == 0)
model.add(-z3_6_7 - y3_5_8 - x2_6_8 + x3_6_8 + y3_6_8 + z3_6_8 - 1.0 == 0)
model.add(-z4_6_7 - y4_5_8 - x3_6_8 + x4_6_8 + y4_6_8 + z4_6_8 - 1.0 == 0)
model.add(-z5_6_7 - y5_5_8 - x4_6_8 + x5_6_8 + y5_6_8 + z5_6_8 - 1.0 == 0)
model.add(-z6_6_7 - y6_5_8 - x5_6_8 + x6_6_8 + y6_6_8 + z6_6_8 - 1.0 == 0)
model.add(-z7_6_7 - y7_5_8 - x6_6_8 + x7_6_8 + y7_6_8 + z7_6_8 - 1.0 == 0)
model.add(-z8_6_7 - y8_5_8 - x7_6_8 + x8_6_8 + y8_6_8 + z8_6_8 - 1.0 == 0)
model.add(-z9_6_7 - y9_5_8 - x8_6_8 + x9_6_8 + y9_6_8 + z9_6_8 - 1.0 == 0)
model.add(-x9_6_8 - z10_6_7 - y10_5_8 + y10_6_8 + z10_6_8 + y11_6_8 + z11_6_8 - 1.0 == 0)
model.add(-z1_7_7 - y1_6_8 + x1_7_8 + y1_7_8 + z1_7_8 + y0_7_8 + z0_7_8 - 1.0 == 0)
model.add(-z2_7_7 - y2_6_8 - x1_7_8 + x2_7_8 + y2_7_8 + z2_7_8 - 1.0 == 0)
model.add(-z3_7_7 - y3_6_8 - x2_7_8 + x3_7_8 + y3_7_8 + z3_7_8 - 1.0 == 0)
model.add(-z4_7_7 - y4_6_8 - x3_7_8 + x4_7_8 + y4_7_8 + z4_7_8 - 1.0 == 0)
model.add(-z5_7_7 - y5_6_8 - x4_7_8 + x5_7_8 + y5_7_8 + z5_7_8 - 1.0 == 0)
model.add(-z6_7_7 - y6_6_8 - x5_7_8 + x6_7_8 + y6_7_8 + z6_7_8 - 1.0 == 0)
model.add(-z7_7_7 - y7_6_8 - x6_7_8 + x7_7_8 + y7_7_8 + z7_7_8 - 1.0 == 0)
model.add(-z8_7_7 - y8_6_8 - x7_7_8 + x8_7_8 + y8_7_8 + z8_7_8 - 1.0 == 0)
model.add(-z9_7_7 - y9_6_8 - x8_7_8 + x9_7_8 + y9_7_8 + z9_7_8 - 1.0 == 0)
model.add(-x9_7_8 - z10_7_7 - y10_6_8 + y10_7_8 + z10_7_8 + y11_7_8 + z11_7_8 - 1.0 == 0)
model.add(-z1_8_7 - y1_7_8 + x1_8_8 + y1_8_8 + z1_8_8 + y0_8_8 + z0_8_8 - 1.0 == 0)
model.add(-z2_8_7 - y2_7_8 - x1_8_8 + x2_8_8 + y2_8_8 + z2_8_8 - 1.0 == 0)
model.add(-z3_8_7 - y3_7_8 - x2_8_8 + x3_8_8 + y3_8_8 + z3_8_8 - 1.0 == 0)
model.add(-z4_8_7 - y4_7_8 - x3_8_8 + x4_8_8 + y4_8_8 + z4_8_8 - 1.0 == 0)
model.add(-z5_8_7 - y5_7_8 - x4_8_8 + x5_8_8 + y5_8_8 + z5_8_8 - 1.0 == 0)
model.add(-z6_8_7 - y6_7_8 - x5_8_8 + x6_8_8 + y6_8_8 + z6_8_8 - 1.0 == 0)
model.add(-z7_8_7 - y7_7_8 - x6_8_8 + x7_8_8 + y7_8_8 + z7_8_8 - 1.0 == 0)
model.add(-z8_8_7 - y8_7_8 - x7_8_8 + x8_8_8 + y8_8_8 + z8_8_8 - 1.0 == 0)
model.add(-z9_8_7 - y9_7_8 - x8_8_8 + x9_8_8 + y9_8_8 + z9_8_8 - 1.0 == 0)
model.add(-x9_8_8 - z10_8_7 - y10_7_8 + y10_8_8 + z10_8_8 + y11_8_8 + z11_8_8 - 1.0 == 0)
model.add(-z1_9_7 - y1_8_8 + x1_9_8 + y1_9_8 + z1_9_8 + y0_9_8 + z0_9_8 - 1.0 == 0)
model.add(-z2_9_7 - y2_8_8 - x1_9_8 + x2_9_8 + y2_9_8 + z2_9_8 - 1.0 == 0)
model.add(-z3_9_7 - y3_8_8 - x2_9_8 + x3_9_8 + y3_9_8 + z3_9_8 - 1.0 == 0)
model.add(-z4_9_7 - y4_8_8 - x3_9_8 + x4_9_8 + y4_9_8 + z4_9_8 - 1.0 == 0)
model.add(-z5_9_7 - y5_8_8 - x4_9_8 + x5_9_8 + y5_9_8 + z5_9_8 - 1.0 == 0)
model.add(-z6_9_7 - y6_8_8 - x5_9_8 + x6_9_8 + y6_9_8 + z6_9_8 - 1.0 == 0)
model.add(-z7_9_7 - y7_8_8 - x6_9_8 + x7_9_8 + y7_9_8 + z7_9_8 - 1.0 == 0)
model.add(-z8_9_7 - y8_8_8 - x7_9_8 + x8_9_8 + y8_9_8 + z8_9_8 - 1.0 == 0)
model.add(-z9_9_7 - y9_8_8 - x8_9_8 + x9_9_8 + y9_9_8 + z9_9_8 - 1.0 == 0)
model.add(-x9_9_8 - z10_9_7 - y10_8_8 + y10_9_8 + z10_9_8 + y11_9_8 + z11_9_8 - 1.0 == 0)
model.add(-y1_9_8 - z1_10_7 + x1_10_8 + z1_10_8 + y0_10_8 + z0_10_8 + x1_11_8 + z1_11_8 - 1.0 == 0)
model.add(-y2_9_8 - z2_10_7 - x1_10_8 + x2_10_8 + z2_10_8 + x2_11_8 + z2_11_8 - 1.0 == 0)
model.add(-y3_9_8 - z3_10_7 - x2_10_8 + x3_10_8 + z3_10_8 + x3_11_8 + z3_11_8 - 1.0 == 0)
model.add(-y4_9_8 - z4_10_7 - x3_10_8 + x4_10_8 + z4_10_8 + x4_11_8 + z4_11_8 - 1.0 == 0)
model.add(-y5_9_8 - z5_10_7 - x4_10_8 + x5_10_8 + z5_10_8 + x5_11_8 + z5_11_8 - 1.0 == 0)
model.add(-y6_9_8 - z6_10_7 - x5_10_8 + x6_10_8 + z6_10_8 + x6_11_8 + z6_11_8 - 1.0 == 0)
model.add(-y7_9_8 - z7_10_7 - x6_10_8 + x7_10_8 + z7_10_8 + x7_11_8 + z7_11_8 - 1.0 == 0)
model.add(-y8_9_8 - z8_10_7 - x7_10_8 + x8_10_8 + z8_10_8 + x8_11_8 + z8_11_8 - 1.0 == 0)
model.add(-y9_9_8 - z9_10_7 - x8_10_8 + x9_10_8 + z9_10_8 + x9_11_8 + z9_11_8 - 1.0 == 0)
model.add(-y10_9_8 - x9_10_8 + y11_10_8 + z11_10_8 + x10_11_8 + z10_11_8 - 1.0 == 0)
model.add(-z1_1_8 + x1_1_9 + y1_1_9 + z1_1_9 + y0_1_9 + z0_1_9 + x1_0_9 + z1_0_9 - 1.0 == 0)
model.add(-z2_1_8 - x1_1_9 + x2_1_9 + y2_1_9 + z2_1_9 + x2_0_9 + z2_0_9 - 1.0 == 0)
model.add(-z3_1_8 - x2_1_9 + x3_1_9 + y3_1_9 + z3_1_9 + x3_0_9 + z3_0_9 - 1.0 == 0)
model.add(-z4_1_8 - x3_1_9 + x4_1_9 + y4_1_9 + z4_1_9 + x4_0_9 + z4_0_9 - 1.0 == 0)
model.add(-z5_1_8 - x4_1_9 + x5_1_9 + y5_1_9 + z5_1_9 + x5_0_9 + z5_0_9 - 1.0 == 0)
model.add(-z6_1_8 - x5_1_9 + x6_1_9 + y6_1_9 + z6_1_9 + x6_0_9 + z6_0_9 - 1.0 == 0)
model.add(-z7_1_8 - x6_1_9 + x7_1_9 + y7_1_9 + z7_1_9 + x7_0_9 + z7_0_9 - 1.0 == 0)
model.add(-z8_1_8 - x7_1_9 + x8_1_9 + y8_1_9 + z8_1_9 + x8_0_9 + z8_0_9 - 1.0 == 0)
model.add(-z9_1_8 - x8_1_9 + x9_1_9 + y9_1_9 + z9_1_9 + x9_0_9 + z9_0_9 - 1.0 == 0)
model.add(-x9_1_9 - z10_1_8 + y10_1_9 + z10_1_9 + y11_1_9 + z11_1_9 + x10_0_9 + z10_0_9 - 1.0 == 0)
model.add(-z1_2_8 - y1_1_9 + x1_2_9 + y1_2_9 + z1_2_9 + y0_2_9 + z0_2_9 - 1.0 == 0)
model.add(-z2_2_8 - y2_1_9 - x1_2_9 + x2_2_9 + y2_2_9 + z2_2_9 - 1.0 == 0)
model.add(-z3_2_8 - y3_1_9 - x2_2_9 + x3_2_9 + y3_2_9 + z3_2_9 - 1.0 == 0)
model.add(-z4_2_8 - y4_1_9 - x3_2_9 + x4_2_9 + y4_2_9 + z4_2_9 - 1.0 == 0)
model.add(-z5_2_8 - y5_1_9 - x4_2_9 + x5_2_9 + y5_2_9 + z5_2_9 - 1.0 == 0)
model.add(-z6_2_8 - y6_1_9 - x5_2_9 + x6_2_9 + y6_2_9 + z6_2_9 - 1.0 == 0)
model.add(-z7_2_8 - y7_1_9 - x6_2_9 + x7_2_9 + y7_2_9 + z7_2_9 - 1.0 == 0)
model.add(-z8_2_8 - y8_1_9 - x7_2_9 + x8_2_9 + y8_2_9 + z8_2_9 - 1.0 == 0)
model.add(-z9_2_8 - y9_1_9 - x8_2_9 + x9_2_9 + y9_2_9 + z9_2_9 - 1.0 == 0)
model.add(-x9_2_9 - z10_2_8 - y10_1_9 + y10_2_9 + z10_2_9 + y11_2_9 + z11_2_9 - 1.0 == 0)
model.add(-z1_3_8 - y1_2_9 + x1_3_9 + y1_3_9 + z1_3_9 + y0_3_9 + z0_3_9 - 1.0 == 0)
model.add(-z2_3_8 - y2_2_9 - x1_3_9 + x2_3_9 + y2_3_9 + z2_3_9 - 1.0 == 0)
model.add(-z3_3_8 - y3_2_9 - x2_3_9 + x3_3_9 + y3_3_9 + z3_3_9 - 1.0 == 0)
model.add(-z4_3_8 - y4_2_9 - x3_3_9 + x4_3_9 + y4_3_9 + z4_3_9 - 1.0 == 0)
model.add(-z5_3_8 - y5_2_9 - x4_3_9 + x5_3_9 + y5_3_9 + z5_3_9 - 1.0 == 0)
model.add(-z6_3_8 - y6_2_9 - x5_3_9 + x6_3_9 + y6_3_9 + z6_3_9 - 1.0 == 0)
model.add(-z7_3_8 - y7_2_9 - x6_3_9 + x7_3_9 + y7_3_9 + z7_3_9 - 1.0 == 0)
model.add(-z8_3_8 - y8_2_9 - x7_3_9 + x8_3_9 + y8_3_9 + z8_3_9 - 1.0 == 0)
model.add(-z9_3_8 - y9_2_9 - x8_3_9 + x9_3_9 + y9_3_9 + z9_3_9 - 1.0 == 0)
model.add(-x9_3_9 - z10_3_8 - y10_2_9 + y10_3_9 + z10_3_9 + y11_3_9 + z11_3_9 - 1.0 == 0)
model.add(-z1_4_8 - y1_3_9 + x1_4_9 + y1_4_9 + z1_4_9 + y0_4_9 + z0_4_9 - 1.0 == 0)
model.add(-z2_4_8 - y2_3_9 - x1_4_9 + x2_4_9 + y2_4_9 + z2_4_9 - 1.0 == 0)
model.add(-z3_4_8 - y3_3_9 - x2_4_9 + x3_4_9 + y3_4_9 + z3_4_9 - 1.0 == 0)
model.add(-z4_4_8 - y4_3_9 - x3_4_9 + x4_4_9 + y4_4_9 + z4_4_9 - 1.0 == 0)
model.add(-z5_4_8 - y5_3_9 - x4_4_9 + x5_4_9 + y5_4_9 + z5_4_9 - 1.0 == 0)
model.add(-z6_4_8 - y6_3_9 - x5_4_9 + x6_4_9 + y6_4_9 + z6_4_9 - 1.0 == 0)
model.add(-z7_4_8 - y7_3_9 - x6_4_9 + x7_4_9 + y7_4_9 + z7_4_9 - 1.0 == 0)
model.add(-z8_4_8 - y8_3_9 - x7_4_9 + x8_4_9 + y8_4_9 + z8_4_9 - 1.0 == 0)
model.add(-z9_4_8 - y9_3_9 - x8_4_9 + x9_4_9 + y9_4_9 + z9_4_9 - 1.0 == 0)
model.add(-x9_4_9 - z10_4_8 - y10_3_9 + y10_4_9 + z10_4_9 + y11_4_9 + z11_4_9 - 1.0 == 0)
model.add(-z1_5_8 - y1_4_9 + x1_5_9 + y1_5_9 + z1_5_9 + y0_5_9 + z0_5_9 - 1.0 == 0)
model.add(-z2_5_8 - y2_4_9 - x1_5_9 + x2_5_9 + y2_5_9 + z2_5_9 - 1.0 == 0)
model.add(-z3_5_8 - y3_4_9 - x2_5_9 + x3_5_9 + y3_5_9 + z3_5_9 - 1.0 == 0)
model.add(-z4_5_8 - y4_4_9 - x3_5_9 + x4_5_9 + y4_5_9 + z4_5_9 - 1.0 == 0)
model.add(-z5_5_8 - y5_4_9 - x4_5_9 + x5_5_9 + y5_5_9 + z5_5_9 - 1.0 == 0)
model.add(-z6_5_8 - y6_4_9 - x5_5_9 + x6_5_9 + y6_5_9 + z6_5_9 - 1.0 == 0)
model.add(-z7_5_8 - y7_4_9 - x6_5_9 + x7_5_9 + y7_5_9 + z7_5_9 - 1.0 == 0)
model.add(-z8_5_8 - y8_4_9 - x7_5_9 + x8_5_9 + y8_5_9 + z8_5_9 - 1.0 == 0)
model.add(-z9_5_8 - y9_4_9 - x8_5_9 + x9_5_9 + y9_5_9 + z9_5_9 - 1.0 == 0)
model.add(-x9_5_9 - z10_5_8 - y10_4_9 + y10_5_9 + z10_5_9 + y11_5_9 + z11_5_9 - 1.0 == 0)
model.add(-z1_6_8 - y1_5_9 + x1_6_9 + y1_6_9 + z1_6_9 + y0_6_9 + z0_6_9 - 1.0 == 0)
model.add(-z2_6_8 - y2_5_9 - x1_6_9 + x2_6_9 + y2_6_9 + z2_6_9 - 1.0 == 0)
model.add(-z3_6_8 - y3_5_9 - x2_6_9 + x3_6_9 + y3_6_9 + z3_6_9 - 1.0 == 0)
model.add(-z4_6_8 - y4_5_9 - x3_6_9 + x4_6_9 + y4_6_9 + z4_6_9 - 1.0 == 0)
model.add(-z5_6_8 - y5_5_9 - x4_6_9 + x5_6_9 + y5_6_9 + z5_6_9 - 1.0 == 0)
model.add(-z6_6_8 - y6_5_9 - x5_6_9 + x6_6_9 + y6_6_9 + z6_6_9 - 1.0 == 0)
model.add(-z7_6_8 - y7_5_9 - x6_6_9 + x7_6_9 + y7_6_9 + z7_6_9 - 1.0 == 0)
model.add(-z8_6_8 - y8_5_9 - x7_6_9 + x8_6_9 + y8_6_9 + z8_6_9 - 1.0 == 0)
model.add(-z9_6_8 - y9_5_9 - x8_6_9 + x9_6_9 + y9_6_9 + z9_6_9 - 1.0 == 0)
model.add(-x9_6_9 - z10_6_8 - y10_5_9 + y10_6_9 + z10_6_9 + y11_6_9 + z11_6_9 - 1.0 == 0)
model.add(-z1_7_8 - y1_6_9 + x1_7_9 + y1_7_9 + z1_7_9 + y0_7_9 + z0_7_9 - 1.0 == 0)
model.add(-z2_7_8 - y2_6_9 - x1_7_9 + x2_7_9 + y2_7_9 + z2_7_9 - 1.0 == 0)
model.add(-z3_7_8 - y3_6_9 - x2_7_9 + x3_7_9 + y3_7_9 + z3_7_9 - 1.0 == 0)
model.add(-z4_7_8 - y4_6_9 - x3_7_9 + x4_7_9 + y4_7_9 + z4_7_9 - 1.0 == 0)
model.add(-z5_7_8 - y5_6_9 - x4_7_9 + x5_7_9 + y5_7_9 + z5_7_9 - 1.0 == 0)
model.add(-z6_7_8 - y6_6_9 - x5_7_9 + x6_7_9 + y6_7_9 + z6_7_9 - 1.0 == 0)
model.add(-z7_7_8 - y7_6_9 - x6_7_9 + x7_7_9 + y7_7_9 + z7_7_9 - 1.0 == 0)
model.add(-z8_7_8 - y8_6_9 - x7_7_9 + x8_7_9 + y8_7_9 + z8_7_9 - 1.0 == 0)
model.add(-z9_7_8 - y9_6_9 - x8_7_9 + x9_7_9 + y9_7_9 + z9_7_9 - 1.0 == 0)
model.add(-x9_7_9 - z10_7_8 - y10_6_9 + y10_7_9 + z10_7_9 + y11_7_9 + z11_7_9 - 1.0 == 0)
model.add(-z1_8_8 - y1_7_9 + x1_8_9 + y1_8_9 + z1_8_9 + y0_8_9 + z0_8_9 - 1.0 == 0)
model.add(-z2_8_8 - y2_7_9 - x1_8_9 + x2_8_9 + y2_8_9 + z2_8_9 - 1.0 == 0)
model.add(-z3_8_8 - y3_7_9 - x2_8_9 + x3_8_9 + y3_8_9 + z3_8_9 - 1.0 == 0)
model.add(-z4_8_8 - y4_7_9 - x3_8_9 + x4_8_9 + y4_8_9 + z4_8_9 - 1.0 == 0)
model.add(-z5_8_8 - y5_7_9 - x4_8_9 + x5_8_9 + y5_8_9 + z5_8_9 - 1.0 == 0)
model.add(-z6_8_8 - y6_7_9 - x5_8_9 + x6_8_9 + y6_8_9 + z6_8_9 - 1.0 == 0)
model.add(-z7_8_8 - y7_7_9 - x6_8_9 + x7_8_9 + y7_8_9 + z7_8_9 - 1.0 == 0)
model.add(-z8_8_8 - y8_7_9 - x7_8_9 + x8_8_9 + y8_8_9 + z8_8_9 - 1.0 == 0)
model.add(-z9_8_8 - y9_7_9 - x8_8_9 + x9_8_9 + y9_8_9 + z9_8_9 - 1.0 == 0)
model.add(-x9_8_9 - z10_8_8 - y10_7_9 + y10_8_9 + z10_8_9 + y11_8_9 + z11_8_9 - 1.0 == 0)
model.add(-z1_9_8 - y1_8_9 + x1_9_9 + y1_9_9 + z1_9_9 + y0_9_9 + z0_9_9 - 1.0 == 0)
model.add(-z2_9_8 - y2_8_9 - x1_9_9 + x2_9_9 + y2_9_9 + z2_9_9 - 1.0 == 0)
model.add(-z3_9_8 - y3_8_9 - x2_9_9 + x3_9_9 + y3_9_9 + z3_9_9 - 1.0 == 0)
model.add(-z4_9_8 - y4_8_9 - x3_9_9 + x4_9_9 + y4_9_9 + z4_9_9 - 1.0 == 0)
model.add(-z5_9_8 - y5_8_9 - x4_9_9 + x5_9_9 + y5_9_9 + z5_9_9 - 1.0 == 0)
model.add(-z6_9_8 - y6_8_9 - x5_9_9 + x6_9_9 + y6_9_9 + z6_9_9 - 1.0 == 0)
model.add(-z7_9_8 - y7_8_9 - x6_9_9 + x7_9_9 + y7_9_9 + z7_9_9 - 1.0 == 0)
model.add(-z8_9_8 - y8_8_9 - x7_9_9 + x8_9_9 + y8_9_9 + z8_9_9 - 1.0 == 0)
model.add(-z9_9_8 - y9_8_9 - x8_9_9 + x9_9_9 + y9_9_9 + z9_9_9 - 1.0 == 0)
model.add(-x9_9_9 - z10_9_8 - y10_8_9 + y10_9_9 + z10_9_9 + y11_9_9 + z11_9_9 - 1.0 == 0)
model.add(-y1_9_9 - z1_10_8 + x1_10_9 + z1_10_9 + y0_10_9 + z0_10_9 + x1_11_9 + z1_11_9 - 1.0 == 0)
model.add(-y2_9_9 - z2_10_8 - x1_10_9 + x2_10_9 + z2_10_9 + x2_11_9 + z2_11_9 - 1.0 == 0)
model.add(-y3_9_9 - z3_10_8 - x2_10_9 + x3_10_9 + z3_10_9 + x3_11_9 + z3_11_9 - 1.0 == 0)
model.add(-y4_9_9 - z4_10_8 - x3_10_9 + x4_10_9 + z4_10_9 + x4_11_9 + z4_11_9 - 1.0 == 0)
model.add(-y5_9_9 - z5_10_8 - x4_10_9 + x5_10_9 + z5_10_9 + x5_11_9 + z5_11_9 - 1.0 == 0)
model.add(-y6_9_9 - z6_10_8 - x5_10_9 + x6_10_9 + z6_10_9 + x6_11_9 + z6_11_9 - 1.0 == 0)
model.add(-y7_9_9 - z7_10_8 - x6_10_9 + x7_10_9 + z7_10_9 + x7_11_9 + z7_11_9 - 1.0 == 0)
model.add(-y8_9_9 - z8_10_8 - x7_10_9 + x8_10_9 + z8_10_9 + x8_11_9 + z8_11_9 - 1.0 == 0)
model.add(-y9_9_9 - z9_10_8 - x8_10_9 + x9_10_9 + z9_10_9 + x9_11_9 + z9_11_9 - 1.0 == 0)
model.add(-y10_9_9 - x9_10_9 + y11_10_9 + z11_10_9 + x10_11_9 + z10_11_9 - 1.0 == 0)
model.add(-z1_1_9 + x1_1_10 + y1_1_10 + y0_1_10 + z0_1_10 + x1_0_10 + z1_0_10 + x1_1_11 + y1_1_11 - 1.0 == 0)
model.add(-z2_1_9 - x1_1_10 + x2_1_10 + y2_1_10 + x2_0_10 + z2_0_10 + x2_1_11 + y2_1_11 - 1.0 == 0)
model.add(-z3_1_9 - x2_1_10 + x3_1_10 + y3_1_10 + x3_0_10 + z3_0_10 + x3_1_11 + y3_1_11 - 1.0 == 0)
model.add(-z4_1_9 - x3_1_10 + x4_1_10 + y4_1_10 + x4_0_10 + z4_0_10 + x4_1_11 + y4_1_11 - 1.0 == 0)
model.add(-z5_1_9 - x4_1_10 + x5_1_10 + y5_1_10 + x5_0_10 + z5_0_10 + x5_1_11 + y5_1_11 - 1.0 == 0)
model.add(-z6_1_9 - x5_1_10 + x6_1_10 + y6_1_10 + x6_0_10 + z6_0_10 + x6_1_11 + y6_1_11 - 1.0 == 0)
model.add(-z7_1_9 - x6_1_10 + x7_1_10 + y7_1_10 + x7_0_10 + z7_0_10 + x7_1_11 + y7_1_11 - 1.0 == 0)
model.add(-z8_1_9 - x7_1_10 + x8_1_10 + y8_1_10 + x8_0_10 + z8_0_10 + x8_1_11 + y8_1_11 - 1.0 == 0)
model.add(-z9_1_9 - x8_1_10 + x9_1_10 + y9_1_10 + x9_0_10 + z9_0_10 + x9_1_11 + y9_1_11 - 1.0 == 0)
model.add(-z10_1_9 - x9_1_10 + y11_1_10 + z11_1_10 + x10_0_10 + z10_0_10 + x10_1_11 + y10_1_11 - 1.0 == 0)
model.add(-z1_2_9 - y1_1_10 + x1_2_10 + y1_2_10 + y0_2_10 + z0_2_10 + x1_2_11 + y1_2_11 - 1.0 == 0)
model.add(-z2_2_9 - y2_1_10 - x1_2_10 + x2_2_10 + y2_2_10 + x2_2_11 + y2_2_11 - 1.0 == 0)
model.add(-z3_2_9 - y3_1_10 - x2_2_10 + x3_2_10 + y3_2_10 + x3_2_11 + y3_2_11 - 1.0 == 0)
model.add(-z4_2_9 - y4_1_10 - x3_2_10 + x4_2_10 + y4_2_10 + x4_2_11 + y4_2_11 - 1.0 == 0)
model.add(-z5_2_9 - y5_1_10 - x4_2_10 + x5_2_10 + y5_2_10 + x5_2_11 + y5_2_11 - 1.0 == 0)
model.add(-z6_2_9 - y6_1_10 - x5_2_10 + x6_2_10 + y6_2_10 + x6_2_11 + y6_2_11 - 1.0 == 0)
model.add(-z7_2_9 - y7_1_10 - x6_2_10 + x7_2_10 + y7_2_10 + x7_2_11 + y7_2_11 - 1.0 == 0)
model.add(-z8_2_9 - y8_1_10 - x7_2_10 + x8_2_10 + y8_2_10 + x8_2_11 + y8_2_11 - 1.0 == 0)
model.add(-z9_2_9 - y9_1_10 - x8_2_10 + x9_2_10 + y9_2_10 + x9_2_11 + y9_2_11 - 1.0 == 0)
model.add(-z10_2_9 - x9_2_10 + y11_2_10 + z11_2_10 + x10_2_11 + y10_2_11 - 1.0 == 0)
model.add(-z1_3_9 - y1_2_10 + x1_3_10 + y1_3_10 + y0_3_10 + z0_3_10 + x1_3_11 + y1_3_11 - 1.0 == 0)
model.add(-z2_3_9 - y2_2_10 - x1_3_10 + x2_3_10 + y2_3_10 + x2_3_11 + y2_3_11 - 1.0 == 0)
model.add(-z3_3_9 - y3_2_10 - x2_3_10 + x3_3_10 + y3_3_10 + x3_3_11 + y3_3_11 - 1.0 == 0)
model.add(-z4_3_9 - y4_2_10 - x3_3_10 + x4_3_10 + y4_3_10 + x4_3_11 + y4_3_11 - 1.0 == 0)
model.add(-z5_3_9 - y5_2_10 - x4_3_10 + x5_3_10 + y5_3_10 + x5_3_11 + y5_3_11 - 1.0 == 0)
model.add(-z6_3_9 - y6_2_10 - x5_3_10 + x6_3_10 + y6_3_10 + x6_3_11 + y6_3_11 - 1.0 == 0)
model.add(-z7_3_9 - y7_2_10 - x6_3_10 + x7_3_10 + y7_3_10 + x7_3_11 + y7_3_11 - 1.0 == 0)
model.add(-z8_3_9 - y8_2_10 - x7_3_10 + x8_3_10 + y8_3_10 + x8_3_11 + y8_3_11 - 1.0 == 0)
model.add(-z9_3_9 - y9_2_10 - x8_3_10 + x9_3_10 + y9_3_10 + x9_3_11 + y9_3_11 - 1.0 == 0)
model.add(-z10_3_9 - x9_3_10 + y11_3_10 + z11_3_10 + x10_3_11 + y10_3_11 - 1.0 == 0)
model.add(-z1_4_9 - y1_3_10 + x1_4_10 + y1_4_10 + y0_4_10 + z0_4_10 + x1_4_11 + y1_4_11 - 1.0 == 0)
model.add(-z2_4_9 - y2_3_10 - x1_4_10 + x2_4_10 + y2_4_10 + x2_4_11 + y2_4_11 - 1.0 == 0)
model.add(-z3_4_9 - y3_3_10 - x2_4_10 + x3_4_10 + y3_4_10 + x3_4_11 + y3_4_11 - 1.0 == 0)
model.add(-z4_4_9 - y4_3_10 - x3_4_10 + x4_4_10 + y4_4_10 + x4_4_11 + y4_4_11 - 1.0 == 0)
model.add(-z5_4_9 - y5_3_10 - x4_4_10 + x5_4_10 + y5_4_10 + x5_4_11 + y5_4_11 - 1.0 == 0)
model.add(-z6_4_9 - y6_3_10 - x5_4_10 + x6_4_10 + y6_4_10 + x6_4_11 + y6_4_11 - 1.0 == 0)
model.add(-z7_4_9 - y7_3_10 - x6_4_10 + x7_4_10 + y7_4_10 + x7_4_11 + y7_4_11 - 1.0 == 0)
model.add(-z8_4_9 - y8_3_10 - x7_4_10 + x8_4_10 + y8_4_10 + x8_4_11 + y8_4_11 - 1.0 == 0)
model.add(-z9_4_9 - y9_3_10 - x8_4_10 + x9_4_10 + y9_4_10 + x9_4_11 + y9_4_11 - 1.0 == 0)
model.add(-z10_4_9 - x9_4_10 + y11_4_10 + z11_4_10 + x10_4_11 + y10_4_11 - 1.0 == 0)
model.add(-z1_5_9 - y1_4_10 + x1_5_10 + y1_5_10 + y0_5_10 + z0_5_10 + x1_5_11 + y1_5_11 - 1.0 == 0)
model.add(-z2_5_9 - y2_4_10 - x1_5_10 + x2_5_10 + y2_5_10 + x2_5_11 + y2_5_11 - 1.0 == 0)
model.add(-z3_5_9 - y3_4_10 - x2_5_10 + x3_5_10 + y3_5_10 + x3_5_11 + y3_5_11 - 1.0 == 0)
model.add(-z4_5_9 - y4_4_10 - x3_5_10 + x4_5_10 + y4_5_10 + x4_5_11 + y4_5_11 - 1.0 == 0)
model.add(-z5_5_9 - y5_4_10 - x4_5_10 + x5_5_10 + y5_5_10 + x5_5_11 + y5_5_11 - 1.0 == 0)
model.add(-z6_5_9 - y6_4_10 - x5_5_10 + x6_5_10 + y6_5_10 + x6_5_11 + y6_5_11 - 1.0 == 0)
model.add(-z7_5_9 - y7_4_10 - x6_5_10 + x7_5_10 + y7_5_10 + x7_5_11 + y7_5_11 - 1.0 == 0)
model.add(-z8_5_9 - y8_4_10 - x7_5_10 + x8_5_10 + y8_5_10 + x8_5_11 + y8_5_11 - 1.0 == 0)
model.add(-z9_5_9 - y9_4_10 - x8_5_10 + x9_5_10 + y9_5_10 + x9_5_11 + y9_5_11 - 1.0 == 0)
model.add(-z10_5_9 - x9_5_10 + y11_5_10 + z11_5_10 + x10_5_11 + y10_5_11 - 1.0 == 0)
model.add(-z1_6_9 - y1_5_10 + x1_6_10 + y1_6_10 + y0_6_10 + z0_6_10 + x1_6_11 + y1_6_11 - 1.0 == 0)
model.add(-z2_6_9 - y2_5_10 - x1_6_10 + x2_6_10 + y2_6_10 + x2_6_11 + y2_6_11 - 1.0 == 0)
model.add(-z3_6_9 - y3_5_10 - x2_6_10 + x3_6_10 + y3_6_10 + x3_6_11 + y3_6_11 - 1.0 == 0)
model.add(-z4_6_9 - y4_5_10 - x3_6_10 + x4_6_10 + y4_6_10 + x4_6_11 + y4_6_11 - 1.0 == 0)
model.add(-z5_6_9 - y5_5_10 - x4_6_10 + x5_6_10 + y5_6_10 + x5_6_11 + y5_6_11 - 1.0 == 0)
model.add(-z6_6_9 - y6_5_10 - x5_6_10 + x6_6_10 + y6_6_10 + x6_6_11 + y6_6_11 - 1.0 == 0)
model.add(-z7_6_9 - y7_5_10 - x6_6_10 + x7_6_10 + y7_6_10 + x7_6_11 + y7_6_11 - 1.0 == 0)
model.add(-z8_6_9 - y8_5_10 - x7_6_10 + x8_6_10 + y8_6_10 + x8_6_11 + y8_6_11 - 1.0 == 0)
model.add(-z9_6_9 - y9_5_10 - x8_6_10 + x9_6_10 + y9_6_10 + x9_6_11 + y9_6_11 - 1.0 == 0)
model.add(-z10_6_9 - x9_6_10 + y11_6_10 + z11_6_10 + x10_6_11 + y10_6_11 - 1.0 == 0)
model.add(-z1_7_9 - y1_6_10 + x1_7_10 + y1_7_10 + y0_7_10 + z0_7_10 + x1_7_11 + y1_7_11 - 1.0 == 0)
model.add(-z2_7_9 - y2_6_10 - x1_7_10 + x2_7_10 + y2_7_10 + x2_7_11 + y2_7_11 - 1.0 == 0)
model.add(-z3_7_9 - y3_6_10 - x2_7_10 + x3_7_10 + y3_7_10 + x3_7_11 + y3_7_11 - 1.0 == 0)
model.add(-z4_7_9 - y4_6_10 - x3_7_10 + x4_7_10 + y4_7_10 + x4_7_11 + y4_7_11 - 1.0 == 0)
model.add(-z5_7_9 - y5_6_10 - x4_7_10 + x5_7_10 + y5_7_10 + x5_7_11 + y5_7_11 - 1.0 == 0)
model.add(-z6_7_9 - y6_6_10 - x5_7_10 + x6_7_10 + y6_7_10 + x6_7_11 + y6_7_11 - 1.0 == 0)
model.add(-z7_7_9 - y7_6_10 - x6_7_10 + x7_7_10 + y7_7_10 + x7_7_11 + y7_7_11 - 1.0 == 0)
model.add(-z8_7_9 - y8_6_10 - x7_7_10 + x8_7_10 + y8_7_10 + x8_7_11 + y8_7_11 - 1.0 == 0)
model.add(-z9_7_9 - y9_6_10 - x8_7_10 + x9_7_10 + y9_7_10 + x9_7_11 + y9_7_11 - 1.0 == 0)
model.add(-z10_7_9 - x9_7_10 + y11_7_10 + z11_7_10 + x10_7_11 + y10_7_11 - 1.0 == 0)
model.add(-z1_8_9 - y1_7_10 + x1_8_10 + y1_8_10 + y0_8_10 + z0_8_10 + x1_8_11 + y1_8_11 - 1.0 == 0)
model.add(-z2_8_9 - y2_7_10 - x1_8_10 + x2_8_10 + y2_8_10 + x2_8_11 + y2_8_11 - 1.0 == 0)
model.add(-z3_8_9 - y3_7_10 - x2_8_10 + x3_8_10 + y3_8_10 + x3_8_11 + y3_8_11 - 1.0 == 0)
model.add(-z4_8_9 - y4_7_10 - x3_8_10 + x4_8_10 + y4_8_10 + x4_8_11 + y4_8_11 - 1.0 == 0)
model.add(-z5_8_9 - y5_7_10 - x4_8_10 + x5_8_10 + y5_8_10 + x5_8_11 + y5_8_11 - 1.0 == 0)
model.add(-z6_8_9 - y6_7_10 - x5_8_10 + x6_8_10 + y6_8_10 + x6_8_11 + y6_8_11 - 1.0 == 0)
model.add(-z7_8_9 - y7_7_10 - x6_8_10 + x7_8_10 + y7_8_10 + x7_8_11 + y7_8_11 - 1.0 == 0)
model.add(-z8_8_9 - y8_7_10 - x7_8_10 + x8_8_10 + y8_8_10 + x8_8_11 + y8_8_11 - 1.0 == 0)
model.add(-z9_8_9 - y9_7_10 - x8_8_10 + x9_8_10 + y9_8_10 + x9_8_11 + y9_8_11 - 1.0 == 0)
model.add(-z10_8_9 - x9_8_10 + y11_8_10 + z11_8_10 + x10_8_11 + y10_8_11 - 1.0 == 0)
model.add(-z1_9_9 - y1_8_10 + x1_9_10 + y1_9_10 + y0_9_10 + z0_9_10 + x1_9_11 + y1_9_11 - 1.0 == 0)
model.add(-z2_9_9 - y2_8_10 - x1_9_10 + x2_9_10 + y2_9_10 + x2_9_11 + y2_9_11 - 1.0 == 0)
model.add(-z3_9_9 - y3_8_10 - x2_9_10 + x3_9_10 + y3_9_10 + x3_9_11 + y3_9_11 - 1.0 == 0)
model.add(-z4_9_9 - y4_8_10 - x3_9_10 + x4_9_10 + y4_9_10 + x4_9_11 + y4_9_11 - 1.0 == 0)
model.add(-z5_9_9 - y5_8_10 - x4_9_10 + x5_9_10 + y5_9_10 + x5_9_11 + y5_9_11 - 1.0 == 0)
model.add(-z6_9_9 - y6_8_10 - x5_9_10 + x6_9_10 + y6_9_10 + x6_9_11 + y6_9_11 - 1.0 == 0)
model.add(-z7_9_9 - y7_8_10 - x6_9_10 + x7_9_10 + y7_9_10 + x7_9_11 + y7_9_11 - 1.0 == 0)
model.add(-z8_9_9 - y8_8_10 - x7_9_10 + x8_9_10 + y8_9_10 + x8_9_11 + y8_9_11 - 1.0 == 0)
model.add(-z9_9_9 - y9_8_10 - x8_9_10 + x9_9_10 + y9_9_10 + x9_9_11 + y9_9_11 - 1.0 == 0)
model.add(-z10_9_9 - x9_9_10 + y11_9_10 + z11_9_10 + x10_9_11 + y10_9_11 - 1.0 == 0)
model.add(-z1_10_9 - y1_9_10 + y0_10_10 + z0_10_10 + x1_11_10 + z1_11_10 + x1_10_11 + y1_10_11 - 1.0 == 0)
model.add(-z2_10_9 - y2_9_10 + x2_11_10 + z2_11_10 + x2_10_11 + y2_10_11 - 1.0 == 0)
model.add(-z3_10_9 - y3_9_10 + x3_11_10 + z3_11_10 + x3_10_11 + y3_10_11 - 1.0 == 0)
model.add(-z4_10_9 - y4_9_10 + x4_11_10 + z4_11_10 + x4_10_11 + y4_10_11 - 1.0 == 0)
model.add(-z5_10_9 - y5_9_10 + x5_11_10 + z5_11_10 + x5_10_11 + y5_10_11 - 1.0 == 0)
model.add(-z6_10_9 - y6_9_10 + x6_11_10 + z6_11_10 + x6_10_11 + y6_10_11 - 1.0 == 0)
model.add(-z7_10_9 - y7_9_10 + x7_11_10 + z7_11_10 + x7_10_11 + y7_10_11 - 1.0 == 0)
model.add(-z8_10_9 - y8_9_10 + x8_11_10 + z8_11_10 + x8_10_11 + y8_10_11 - 1.0 == 0)
model.add(-z9_10_9 - y9_9_10 + x9_11_10 + z9_11_10 + x9_10_11 + y9_10_11 - 1.0 == 0)
model.add(y11_10_10 + z11_10_10 + x10_11_10 + z10_11_10 + x10_10_11 + y10_10_11 - 1.0 == 0)
