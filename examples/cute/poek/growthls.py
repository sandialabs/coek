# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
# Taken from:

#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   GROWTH problem in 3 variables
#   Fit the observed growth g(n) from Gaussian Elimination
#   with complete pivoting to a function of the form
#
#        u1 * n ** ( u2 + LOG(n) * u3 )
#   SIF input: Nick Gould, Nov, 1991, modified by Ph. Toint, March 1994.
#   classification SUR2-AN-3-0
#   Solution

import poek as pk
from math import log

model = pk.model()

n = 3

u1 = model.variable(value=100.0)
u2 = model.variable()
u3 = model.variable()

model.add( (u1 * (8.0**(u2+(log(8.0))*u3)) - 8.0)*(u1 * (8.0**(u2+(log(8.0))*u3)) - 8.0) + \
    (u1 * (9.0**(u2+(log(9.0))*u3)) - 8.4305)*(u1 * (9.0**(u2+(log(9.0))*u3)) - \
    8.4305) + (u1 * (10.0**(u2+(log(10.0))*u3)) - 9.5294)*(u1 * \
    (10.0**(u2+(log(10.0))*u3)) - 9.5294) + (u1 * (11.0**(u2+(log(11.0))*u3)) -\
    10.4627)*(u1 * (11.0**(u2+(log(11.0))*u3)) - 10.4627) + (u1 * \
    (12.0**(u2+(log(12.0))*u3)) - 12.0)*(u1 * (12.0**(u2+(log(12.0))*u3)) - 12.0) +\
    (u1 * (13.0**(u2+(log(13.0))*u3)) - 13.0205)*(u1 * (13.0**(u2+(log(13.0))*u3)) -\
    13.0205) + (u1 * (14.0**(u2+(log(14.0))*u3)) - 14.5949)*(u1 * \
    (14.0**(u2+(log(14.0))*u3)) - 14.5949) + (u1 * (15.0**(u2+(log(15.0))*u3)) -\
    16.1078)*(u1 * (15.0**(u2+(log(15.0))*u3)) - 16.1078) + (u1 * \
    (16.0**(u2+(log(16.0))*u3)) - 18.0596)*(u1 * (16.0**(u2+(log(16.0))*u3)) -\
    18.0596) + (u1 * (18.0**(u2+(log(18.0))*u3)) - 20.4569)*(u1 * \
    (18.0**(u2+(log(18.0))*u3)) - 20.4569) + (u1 * (20.0**(u2+(log(20.0))*u3)) -\
    24.25)*(u1 * (20.0**(u2+(log(20.0))*u3)) - 24.25) + (u1 * \
    (25.0**(u2+(log(25.0))*u3)) - 32.9863)*(u1 * (25.0**(u2+(log(25.0))*u3)) -\
    32.9863) )
