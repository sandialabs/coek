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

S = RangeSet(1,19)
w = Param(S)
m = Param(S)

# LOAD DATA
Feed = Var(initialize=1)
Effluent = Var(initialize=1)
MF_ohd = Var(initialize=1)
HCN  = Var(initialize=1)
LCO = Var(initialize=1)
HCO = Var(initialize=1)
MF_btms = Var(initialize=1)
Decant = Var(initialize=1)
Dec_recy = Var(initialize=1)
Off_gas = Var(initialize=1)
DC4_feed = Var(initialize=1)
DC3_feed = Var(initialize=1)
DC4_btms = Var(initialize=1)
Lean_oil = Var(initialize=1)
Propane = Var(initialize=1)
Butane = Var(initialize=1)
C8spl_fd = Var(initialize=1)
LCN = Var(initialize=1)
MCN  = Var(initialize=1)

    return (Feed-value(m[1]))**2/value(w[1])\
    +(Effluent-value(m[2]))**2/value(w[2])\
    +(MF_ohd-value(m[3]))**2/value(w[3])\
    +(HCN-value(m[4]))**2/value(w[4])\
    +(LCO-value(m[5]))**2/value(w[5])\
    +(HCO-value(m[6]))**2/value(w[6])\
    +(MF_btms-value(m[7]))**2/value(w[7])\
    +(Decant-value(m[8]))**2/value(w[8])\
    +(Dec_recy-value(m[9]))**2/value(w[9])\
    +(Off_gas-value(m[10]))**2/value(w[10])\
    +(DC4_feed-value(m[11]))**2/value(w[11])\
    +(DC3_feed-value(m[12]))**2/value(w[12])\
    +(DC4_btms-value(m[13]))**2/value(w[13])\
    +(Lean_oil-value(m[14]))**2/value(w[14])\
    +(Propane-value(m[15]))**2/value(w[15])\
    +(Butane-value(m[16]))**2/value(w[16])\
    +(C8spl_fd-value(m[17]))**2/value(w[17])\
    +(LCN-value(m[18]))**2/value(w[18])\
    +(MCN-value(m[19]))**2/value(w[19])

    return Feed + Dec_recy - Effluent == 0
    return Effluent - MF_ohd - HCN - LCO - HCO - MF_btms == 0
    return MF_btms - Decant - Dec_recy == 0
    return MF_ohd + Lean_oil - Off_gas - DC4_feed == 0
    return DC4_feed - DC3_feed - DC4_btms == 0
    return DC4_btms - Lean_oil - C8spl_fd == 0
    return DC3_feed - Propane - Butane == 0
    return C8spl_fd - LCN - MCN == 0
