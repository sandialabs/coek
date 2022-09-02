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

#   Source: problem 11 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#27
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-MN-3-0

import poek as pk
from math import log


model = pk.model()

N=3
M=99

t = {i: i/100.0 for i in range(1,M+1)}
y = {i: 25+ (-50*log(t[i]))**(2.0/3.0) for i in range(1,M+1)}

x = model.add_variable(index=[1,2,3])
x[1].value = 5.0;
x[2].value = 2.5;
x[3].value = 0.15;

model.add_objective( sum((pk.exp(abs(y[i]-x[2])**x[3]/(-x[1]))-t[i])**2 for i in range(1,M+1)) )
