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

#   Source: 
#   M.M. Makela,
#   "Nonsmooth optimization",
#   Ph.D. thesis, Jyvaskyla University, 1990

#   SIF input: Ph. Toint, Nov 1993.

#   classification  LLR2-AN-51-50

import poek as pk


model = pk.model()

ri = 50
t = -25.5 + 50

x = model.add_variable(index=range(1,ri+1))
for j in range(1,ri+1):
    x[j].value = -25.5+j

u = model.add_variable() 

model.add_objective(u)

for i in range(1,ri+1):
    model.add_constraint( u >= 50*x[i] - sum(x[i] for i in x) )
