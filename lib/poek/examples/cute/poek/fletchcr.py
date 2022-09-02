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

#   Source:  The second problem given by
#   R. Fletcher,
#   "An optimal positive definite update for sparse Hessian matrices"
#   Numerical Analysis report NA/145, University of Dundee, 1992.

#   SIF input: Nick Gould, Oct 1992.

#   classification OUR2-AN-V-0

import poek as pk


model = pk.model()

N = 100

x = model.add_variable(index=range(1,N+1), value=0.0)

model.add_objective( sum(100*(x[i+1]-x[i]+1-x[i]**2)**2 for i in range(1,N)) )
