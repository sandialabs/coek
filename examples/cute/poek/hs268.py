# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes. 
#
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
#   K. Schittkowski
#   "More Test Examples for Nonlinear Programming Codes"
#   Springer Verlag, Berlin, Lecture notes in economics and 
#   mathematical systems, volume 282, 1987

#   SIF input: Michel Bierlaire and Annick Sartenaer, October 1992.
#              minor correction by Ph. Shott, Jan 1995.

#   classification QLR2-AN-5-5

import poek as pk


model = pk.model()

N = [1,2,3,4,5]

B = {1:-9170, 2:17099, 3:-2271, 4:-4336, 5:-43}
D = {
(1,1):10197,
(1,2):-12454,
(1,3):-1013,
(1,4):1948,
(1,5):329,
(2,1):-12454,
(2,2):-20909,
(2,3):-1733,
(2,4):-4914,
(2,5):-186,
(3,1):-1013,
(3,2):-1733,
(3,3):1755,
(3,4):1089,
(3,5):-174,
(4,1):1948,
(4,2):-4914,
(4,3):1089,
(4,4):1515,
(4,5):-22,
(5,1):329,
(5,2):-186,
(5,3):-174,
(5,4):-22,
(5,5):27
}

x = model.variable(index=N,value=1.0)

model.add( 14463.0 + sum(D[i,j]*x[i]*x[j] for i in N for j in N) + -2*sum(B[i]*x[i] for i in N) )

model.add( -sum(x[i] for i in N) + 5 >= 0 )
model.add( 10*x[1]+10*x[2]-3*x[3]+5*x[4]+4*x[5]-20 >= 0 )
model.add( -8*x[1]+x[2]-2*x[3]-5*x[4]+3*x[5]+40 >= 0 )
model.add( 8*x[1]-x[2]+2*x[3]+5*x[4]-3*x[5]-11 >= 0 )
model.add( -4*x[1]-2*x[2]+3*x[3]-5*x[4]+x[5]+30 >= 0 )
