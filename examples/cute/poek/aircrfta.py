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

#   Source: Problem 9 in
#   J.J. More',"A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer Seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-RN-8-5

import poek as pk


model = pk.model()

rollrate = model.variable(value=0.0)
pitchrat = model.variable(value=0.0)
yawrate = model.variable(value=0.0)
attckang = model.variable(value=0.0)
sslipang = model.variable(value=0.0)
elevator = model.variable(value=0.0)
aileron = model.variable(value=0.0)
rudderdf = model.variable(value=0.0)

elevator.value = 0.1
elevator.fixed=True
aileron.value = 0
aileron.fixed=True
rudderdf.value = 0
rudderdf.fixed=True

model.add( pk.expression(0) )

exp1 = -3.933*rollrate+0.107*pitchrat+0.126*yawrate-9.99*sslipang\
    - 45.83*aileron-7.64*rudderdf-0.727*pitchrat*yawrate+8.39*yawrate*attckang\
    - 684.4*attckang*sslipang+63.5*pitchrat*attckang
model.add( 0 == exp1 )

exp2 = -0.987*pitchrat-22.95*attckang-28.37*elevator+0.949*rollrate*yawrate\
    + 0.173*rollrate*sslipang
model.add( 0 == exp2 )

exp3 = 0.002*rollrate-0.235*yawrate+5.67*sslipang-0.921*aileron-6.51*rudderdf\
    - 0.716*rollrate*pitchrat-1.578*rollrate*attckang+1.132*pitchrat*attckang
model.add( 0 == exp3 )

exp4 = pitchrat- attckang-1.168*elevator-rollrate*sslipang
model.add( 0 == exp4 )

exp5 = -yawrate-0.196*sslipang-0.0071*aileron+rollrate*attckang
model.add( 0 == exp5 )
