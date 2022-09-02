# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
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

#   Source: problem 31 in
#   Ph.L. Toint,
#   "Test problems for partially separable optimization and results
#   for the routine PSPMIN",
#   Report 83/4, Department of Mathematics, FUNDP (Namur, B), 1983.

#   See also Buckley#172 (p. 52)
#   SIF input: Ph. Toint and N. Gould, Dec 1989.

#   classification OUR2-AN-V-0

import poek as pk


model = pk.model()

N = 5000

x = model.add_variable(index=range(1,N+1), value=2.0)

sumexp1 = sum(-4*x[i]+3.0 for i in range(1,N))
sumexp2 = sum((x[i]**2+x[i+1]**2)**2 for i in range(1,N))
model.add_objective( sumexp1 + sumexp2 )
