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

#   classification QLR2-AN-V-V

import poek as pk


model = pk.model()

n = 1000
b = 5

x = model.variable(index=range(1,n+1), lb=-1, ub=1, value=0.99)
y = model.variable(index=range(1,n+1), lb=-1, ub=1, value=-0.99)
z = model.variable(index=range(1,b+1), lb=0, ub=1, value=0.5)

sum_expr_1 = sum((i/n)*x[i]*y[i] for i in x)
sum_expr_2 = sum(0.5*z[j]**2 for j in z)
model.add( sum_expr_1 + sum_expr_2 )

sum_cexpr_1 = sum(x[i] + y[i] for i in x)
sum_cexpr_2 = sum(z[j] for j in z)
cexp = sum_cexpr_1 + sum_cexpr_2
model.add( b+1 <= cexp )

for i in x:
    csum = 0
    for j in z:
        csum += z[j]
    model.add( x[i] + y[i] + csum == b )
