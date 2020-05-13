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

#   Source: problem MAXTPH in
#   Vasko and Stott
#   "Optimizing continuous caster product dimensions:
#    an example of a nonlinear design problem in the steel industry"
#   SIAM Review, Vol 37 No, 1 pp.82-84, 1995

#   SIF input: A.R. Conn April 1995

#   classification LOR2-RN-5-4

import poek as pk


model = pk.model()

density = 0.284
lenmax = 60.0
maxaspr = 2.0
minthick = 7.0
minarea = 200.0
maxarea = 250.0
k = 1.0

thick = model.variable(lb=minthick, value=0.5)
wid = model.variable(lb=0.0, value=0.5)
len = model.variable(lb=0.0, ub=lenmax, value=0.5)
tph = model.variable(lb=0.0, value=0.5)
ipm = model.variable(lb=0.0, value=0.5)

model.add( -tph )

model.add( 117.370892*tph/(wid*thick)-ipm == 0.0 )
model.add( thick**2*ipm/48.0-len == 0.0 )
model.add( wid/thick <= maxaspr )
model.add( pk.inequality(0.0, thick*wid - minarea, maxarea-minarea) )
    
