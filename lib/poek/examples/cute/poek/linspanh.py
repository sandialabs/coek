# Adapted from Pyomo model by William E. Hart
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

#   Source:
#   A partial specification of problem SPANHYD.

#   SIF input: Ph. Toint, Sept 1990.

#   classification LNR2-MN-97-33

import json
import poek as pk


model = pk.model()

N = list(range(1,98))

with open('linspanh.json', 'r') as INPUT:
    data = json.load(INPUT)
l = {int(key):value for key,value in data["l"].items()}
u = {int(key):value for key,value in data["u"].items()}
x0 = {int(key):value for key,value in data["x0"].items()}

x = model.add_variable(index=N)
for i in N:
    x[i].lb = l[i]
    x[i].ub = u[i]
    x[i].value = x0[i]


model.add_objective( -x[1] )

model.add_constraint( x[1] - x[11] - x[21] - x[30] + 51.38 == 0 )
model.add_constraint( x[2] + x[11] - x[12] + x[21] - x[22] - x[31] + 13.84 == 0 )
model.add_constraint( x[3] + x[12] - x[13] + x[22] - x[23] - x[32] + 2.58 == 0 )
model.add_constraint( x[4] - x[14] - x[24] - x[33] + 21.91 == 0 )
model.add_constraint( x[5] + x[13] + x[14] - x[15] - x[34] == 0 )
model.add_constraint( x[6] - x[16] - x[25] - x[35] + 12.97 == 0 )
model.add_constraint( x[7] + x[16] - x[17] + x[25] - x[26] - x[36] == 0 )
model.add_constraint( x[8] + x[17] - x[18] + x[26] - x[27] - x[37] + 2.89 == 0 )
model.add_constraint( x[9] + x[15] + x[18] - x[19] + x[23] + x[24] + x[27] - x[28] - x[38] + 20.84 == 0 )
model.add_constraint( x[10] + x[19] - x[20] + x[28] - x[29] - x[39] + 17.14 == 0 )
model.add_constraint( x[30] - x[40] - x[50] - x[59] + 32.06 == 0 )
model.add_constraint( x[31] + x[40] - x[41] + x[50] - x[51] - x[60] + 0.28 == 0 )
model.add_constraint( x[32] + x[41] - x[42] + x[51] - x[52] - x[61] + 4.2 == 0 )
model.add_constraint( x[33] - x[43] - x[53] - x[62] + 48.37 == 0 )
model.add_constraint( x[34] + x[42] + x[43] - x[44] - x[63] == 0 )
model.add_constraint( x[35] - x[45] - x[54] - x[64] + 18.13 == 0 )
model.add_constraint( x[36] + x[45] - x[46] + x[54] - x[55] - x[65] == 0 )
model.add_constraint( x[37] + x[46] - x[47] + x[55] - x[56] - x[66] - 1.61 == 0 )
model.add_constraint( x[38] + x[44] + x[47] - x[48] + x[52] + x[53] + x[56] - x[57] - x[67] + 26.6 == 0 )
model.add_constraint( x[39] + x[48] - x[49] + x[57] - x[58] - x[68] + 18.76 == 0 )
model.add_constraint( x[59] - x[69] - x[79] - x[88] + 18.13 == 0 )
model.add_constraint( x[60] + x[69] - x[70] + x[79] - x[80] - x[89] == 0 )
model.add_constraint( x[61] + x[70] - x[71] + x[80] - x[81] - x[90] == 0 )
model.add_constraint( x[62] - x[72] - x[82] - x[91] + 18.13 == 0 )
model.add_constraint( x[63] + x[71] + x[72] - x[73] - x[92] == 0 )
model.add_constraint( x[64] - x[74] - x[83] - x[93] + 9.1 == 0 )
model.add_constraint( x[65] + x[74] - x[75] + x[83] - x[84] - x[94] == 0 )
model.add_constraint( x[66] + x[75] - x[76] + x[84] - x[85] - x[95] - 5.81 == 0 )
model.add_constraint( x[67] + x[73] + x[76] - x[77] + x[81] + x[82] + x[85] - x[86] - x[96] + 9.1 == 0 )
model.add_constraint( x[68] + x[77] - x[78] + x[86] - x[87] - x[97] + 6.02 == 0 )
model.add_constraint( x[20] + x[29] + x[49] + x[58] + x[78] + x[87] - 608.35 == 0 )
model.add_constraint( -x[1] - x[2] - x[3] - x[4] - x[5] - x[6] - x[7] - x[8] - x[9] - x[10] + 4626.34 == 0 )
model.add_constraint( x[88] + x[89] + x[90] + x[91] + x[92] + x[93] + x[94] + x[95] + x[96] + x[97] - 4363.0 == 0 )
