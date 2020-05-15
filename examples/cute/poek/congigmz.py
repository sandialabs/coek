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
#   M.C. Pinar and S.A. Zenios,
#   "Approximately Exact Smoothing Functions for Exact Penalty Methods",
#   Institute for Numerical Analysis, TUD, Lyngby, Denmark.

#   SIF input: Ph. Toint, August 1993.

#   classification LQR2-AN-3-5

import poek as pk


model = pk.model()

x = model.variable(index=[1,2], value=2.0)
z = model.variable(value=2.0)


model.add( z )

model.add( z+5*x[1]-x[2] >= 0 )

model.add( z-4*x[2]-x[1]**2-x[2]**2 >= 0 )

model.add( z-5*x[1]-x[2] >= 0 )

model.add( x[1]+x[2]+10.0 <= 0 )

model.add( 2*x[1]**2-x[2]**2+4.0 <= 0 )
