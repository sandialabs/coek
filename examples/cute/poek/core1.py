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

#   classification LQI2-RN-65-59

import poek as pk


model = pk.model()

data = pk.util.load_data('core1.json')
Region, loflows, hiflows, hisupply, loprods, hiprods, lopi, hipi, Flow_init = \
    data.unpack('Region', 'loflows', 'hiflows', 'hisupply', 'loprods', 'hiprods', 'lopi', 'hipi', 'Flow_init')

SF = list(range(1,25))
SP = list(range(1,7))
SD = list(range(1,10))
SPi = list(range(1,21))

Flow = model.add_variable(index=SF)
for i in SF:
    Flow[i].value = Flow_init[i]
Prod = model.add_variable(index=SP)
Supply = model.add_variable(index=SP)
Demand = model.add_variable(index=SD)
Pi = model.add_variable(index=SPi)

model.add_objective( 2.28*Prod[1] + 2.28*Prod[2] + 2.28*Prod[3] + 1.68*Prod[4] + 1.68*Prod[5] + 1.68*Prod[6] )

model.add_constraint( Flow[1] + Flow[2] - Supply[1] == 0 )
model.add_constraint( -1*Flow[1] - Flow[2] + Flow[3] + Flow[4] - Supply[2] == 0 )
model.add_constraint(  -1*Flow[3] - Flow[4] + Flow[5] + Demand[1] == 0 )
model.add_constraint( -1*Flow[5] - Flow[8] + Flow[9] == 0 )
model.add_constraint( Flow[6] - Supply[3] == 0 )
model.add_constraint( -1*Flow[6] + Flow[7] + Demand[2] == 0 )
model.add_constraint( -1*Flow[7] + Flow[8] + Demand[3] == 0 )
model.add_constraint( Flow[10] + Flow[11] - Supply[4] == 0 )
model.add_constraint( -1*Flow[10] - Flow[11] + Flow[12] + Flow[13] == 0 )
model.add_constraint( -1*Flow[12] - Flow[13] + Flow[14] + Flow[15] + Demand[4] == 0 )
model.add_constraint( -1*Flow[14] - Flow[15] + Flow[16] + Flow[21] == 0 )
model.add_constraint( -1*Flow[16] + Flow[17] + Demand[5] == 0 )
model.add_constraint( -1*Flow[17] + Flow[18] - Supply[5] == 0 )
model.add_constraint( -1*Flow[9] - Flow[18] + Flow[19] - Supply[6] == 0 )
model.add_constraint( -1*Flow[19] + Flow[20] + Demand[6] == 0 )
model.add_constraint( -1*Flow[20] + Demand[7] == 0 )
model.add_constraint( -1*Flow[21] + Flow[22] == 0 )
model.add_constraint( -1*Flow[22] + Flow[23] == 0 )
model.add_constraint( -1*Flow[23] + Flow[24] + Demand[8] == 0 )
model.add_constraint( -1*Flow[24] + Demand[9] == 0 )
model.add_constraint( -1*Demand[1] <= Region[1] )
model.add_constraint( -1*Demand[2] <= Region[2] )
model.add_constraint( -1*Demand[3] <= Region[3] )
model.add_constraint( -1*Demand[4] <= Region[4] )
model.add_constraint( -1*Demand[5] <= Region[5] )
model.add_constraint( -1*Demand[6] <= Region[6] )
model.add_constraint( -1*Demand[7] <= Region[7] )
model.add_constraint( -1*Demand[8] <= Region[8] )
model.add_constraint( -1*Demand[9] <= Region[9] )
model.add_constraint( Supply[1] <= Prod[1] )
model.add_constraint( Supply[2] <= Prod[2] )
model.add_constraint( Supply[3] <= Prod[3] )
model.add_constraint( Supply[4] <= Prod[4] )
model.add_constraint( Supply[5] <= Prod[5] )
model.add_constraint( Supply[6] <= Prod[6] )
model.add_constraint( (Flow[1]**2) -9.07027*(Pi[1] - Pi[2]) == 0 )
model.add_constraint( (Flow[2]**2) -9.07027*(Pi[1] - Pi[2]) == 0 )
model.add_constraint( (Flow[3]**2) -6.04685*(Pi[2] - Pi[3])  == 0 )
model.add_constraint( (Flow[4]**2) -6.04685*(Pi[2] - Pi[3]) == 0 )
model.add_constraint( (Flow[5]**2) -1.39543*(Pi[3] - Pi[4]) == 0 )
model.add_constraint( (Flow[6]**2) -0.100256*(Pi[5] - Pi[6]) == 0 )
model.add_constraint( (Flow[7]**2) -0.148655*(Pi[6] - Pi[7]) == 0 )
model.add_constraint( (Flow[8]**2) +0.226895*(Pi[4] - Pi[7]) == 0 )
model.add_constraint( (Flow[9]**2) -0.659656*(Pi[4] - Pi[14]) == 0 )
model.add_constraint( (Flow[10]**2) -7.25622*(Pi[8] - Pi[9]) >= 0 )
model.add_constraint( (Flow[11]**2) -0.10803*(Pi[8] - Pi[9]) >= 0 )
model.add_constraint( (Flow[12]**2) -1.81405*(Pi[9] - Pi[10]) == 0 )
model.add_constraint( (Flow[13]**2) -0.0270084*(Pi[9] - Pi[10]) == 0 )
model.add_constraint( (Flow[14]**2) -1.45124*(Pi[10] - Pi[11]) == 0 )
model.add_constraint( (Flow[15]**2) -0.0216067*(Pi[10] - Pi[11]) == 0 )
model.add_constraint( (Flow[16]**2) -0.863836*(Pi[11] - Pi[12]) == 0 )
model.add_constraint( (Flow[17]**2) -0.907027*(Pi[12] - Pi[13]) == 0 )
model.add_constraint( (Flow[18]**2) -7.25622*(Pi[13] - Pi[14]) == 0 )
model.add_constraint( (Flow[19]**2) -3.62811*(Pi[14] - Pi[15]) == 0 )
model.add_constraint( (Flow[20]**2) -1.45124*(Pi[15] - Pi[16]) == 0 )
model.add_constraint( (Flow[21]**2) -0.0514445*(Pi[11] - Pi[17]) == 0 )
model.add_constraint( (Flow[22]**2) -0.00641977*(Pi[17] - Pi[18]) >= 0 )
model.add_constraint( (Flow[23]**2) -0.00170320*(Pi[18] - Pi[19]) == 0 )
model.add_constraint( (Flow[24]**2) -0.0278190*(Pi[19] - Pi[20]) == 0 )

for i in SF:
    model.add_constraint( pk.inequality(loflows[i], Flow[i], hiflows) )

for i in SP:
    model.add_constraint( Supply[i] <= hisupply[i] )

for i in SP:
    model.add_constraint( pk.inequality(loprods[i], Prod[i], hiprods[i]) )

for i in SPi:
    model.add_constraint( pk.inequality(lopi[i], Pi[i], hipi[i]) )
