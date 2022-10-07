# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
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

#   Source: problem 3 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.

#   SIF input: A.R. Conn March 1990

#   classification QBR2-AN-2-0

import poek as pk


model = pk.model()

N = [1, 2]

x = model.add_variable(index=N)
x[1].value = 10
x[2].value = 1

model.add_objective(x[2] + (-x[1] + x[2]) ** 2)

model.add_constraint(x[2] >= 0)
