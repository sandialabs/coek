# TODO
# Formulated in pyomo by Logan Barnes. Taken from:

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

#   Source: An inverse problem from astronomy,
#   reformulated as a convex quadratic program by
#   S. P. Hestis, SIAM Review 34 (1992) pp. 642-647.

#   SIF input: Nick Gould, January 1993.
#   improvements by: Ruediger Franke (Ruediger.Franke@RZ.TU-Ilmenau.DE)

#   classification QLR2-MN-V-V

import poek as pk


model = pk.model()

K = 10000
Range = 1.0
deltax = Range/K

N = list(range(1,K+1))

M = model.add_variable(index=N, lb=0, value=1)

model.add_objective(sum((M[i]**2)/K for i in N))
model.add_constraint(sum(((i**3)-((i-1)**3))*(deltax**3)*M[i]/3 for i in N) - 1835.2 == 0)
model.add_constraint(sum(((i**5)-((i-1)**5))*(deltax**5)*M[i]/5 for i in N) - 909.8 == 0)
