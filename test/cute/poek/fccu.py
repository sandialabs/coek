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

#   Source:
#   W. J. Korchinski, Profimatics, Inc,
#   325 Rolling Oaks Drive, Thousand Oaks, California, USA 91361-1200
#   Telephone: 1-805 496 6661, Fax: 1-805 373 5108

#   SIF input: W. Korchinski, Spring 1993.

#   classification SLR2-MN-19-8

import poek as pk


model = pk.model()

data = pk.util.load_data("fccu.json")
w, m = data.unpack("w", "m")

Feed = model.add_variable(value=1)
Effluent = model.add_variable(value=1)
MF_ohd = model.add_variable(value=1)
HCN = model.add_variable(value=1)
LCO = model.add_variable(value=1)
HCO = model.add_variable(value=1)
MF_btms = model.add_variable(value=1)
Decant = model.add_variable(value=1)
Dec_recy = model.add_variable(value=1)
Off_gas = model.add_variable(value=1)
DC4_feed = model.add_variable(value=1)
DC3_feed = model.add_variable(value=1)
DC4_btms = model.add_variable(value=1)
Lean_oil = model.add_variable(value=1)
Propane = model.add_variable(value=1)
Butane = model.add_variable(value=1)
C8spl_fd = model.add_variable(value=1)
LCN = model.add_variable(value=1)
MCN = model.add_variable(value=1)

model.add_objective(
    (Feed - m[1]) ** 2 / w[1]
    + (Effluent - m[2]) ** 2 / w[2]
    + (MF_ohd - m[3]) ** 2 / w[3]
    + (HCN - m[4]) ** 2 / w[4]
    + (LCO - m[5]) ** 2 / w[5]
    + (HCO - m[6]) ** 2 / w[6]
    + (MF_btms - m[7]) ** 2 / w[7]
    + (Decant - m[8]) ** 2 / w[8]
    + (Dec_recy - m[9]) ** 2 / w[9]
    + (Off_gas - m[10]) ** 2 / w[10]
    + (DC4_feed - m[11]) ** 2 / w[11]
    + (DC3_feed - m[12]) ** 2 / w[12]
    + (DC4_btms - m[13]) ** 2 / w[13]
    + (Lean_oil - m[14]) ** 2 / w[14]
    + (Propane - m[15]) ** 2 / w[15]
    + (Butane - m[16]) ** 2 / w[16]
    + (C8spl_fd - m[17]) ** 2 / w[17]
    + (LCN - m[18]) ** 2 / w[18]
    + (MCN - m[19]) ** 2 / w[19]
)

model.add_constraint(Feed + Dec_recy - Effluent == 0)
model.add_constraint(Effluent - MF_ohd - HCN - LCO - HCO - MF_btms == 0)
model.add_constraint(MF_btms - Decant - Dec_recy == 0)
model.add_constraint(MF_ohd + Lean_oil - Off_gas - DC4_feed == 0)
model.add_constraint(DC4_feed - DC3_feed - DC4_btms == 0)
model.add_constraint(DC4_btms - Lean_oil - C8spl_fd == 0)
model.add_constraint(DC3_feed - Propane - Butane == 0)
model.add_constraint(C8spl_fd - LCN - MCN == 0)
