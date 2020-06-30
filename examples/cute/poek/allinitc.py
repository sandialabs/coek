# TODO
# Formulated in Pyomo by Gabe Hackebeil
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
#   N. Gould: private communication.

#   SIF input: Nick Gould, June 1990.

#   classification OOR2-AY-4-1

import poek as pk
sin = pk.sin


model = pk.model()

x = model.add_variable(index=[1,2,3,4])

model.add_objective(x[3]-1 + x[1]**2+\
    x[2]**2 + (x[3]+x[4])**2 +\
    sin(x[3])**2 + x[1]**2*x[2]**2 + x[4]-3 +\
    sin(x[3])**2 +\
    (x[4]-1)**2 +\
    (x[2]**2)**2+\
    (x[3]**2 + (x[4]+x[1])**2)**2 +\
    (x[1]-4 + sin(x[4])**2 + x[2]**2*x[3]**2)**2 +\
    sin(x[4])**4)

model.add_constraint(x[2]>=1)
model.add_constraint(pk.inequality(-1e10,x[3],1))
model.add_constraint(x[4]==2)
model.add_constraint(x[1]**2+x[2]**2-1<=0)
