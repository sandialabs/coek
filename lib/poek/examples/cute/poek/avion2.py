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

#   classification OLR2-RN-49-15

import poek as pk
sqrt = pk.sqrt


model = pk.model()

SR = model.add_variable(value=27.452, lb=10, ub=150)

LR = model.add_variable(value=1.5000, lb=0, ub=10)

PK = model.add_variable(value=10.000, lb=0, ub=10)

EF = model.add_variable(value=0.000, lb=0, ub=5)

SX = model.add_variable(value=19.217, lb=7, ub=120)

LX = model.add_variable(value=1.5000, lb=1.5, ub=8)

SD = model.add_variable(value=3.5688, lb=2, ub=20)

SK = model.add_variable(value=4.0696, lb=2, ub=30)

ST = model.add_variable(value=34.315, lb=30, ub=500)

SF = model.add_variable(value=88.025, lb=20, ub=200)

LF = model.add_variable(value=5.1306, lb=0.01, ub=20)

AM = model.add_variable(value=0.0000, lb=0, ub=10)

CA = model.add_variable(value=-0.14809, lb=-0.2, ub=-0.001)

CB = model.add_variable(value=0.75980, lb=0.1, ub=2)

SO = model.add_variable(value=0.0000, lb=0, ub=1)

SS = model.add_variable(value=0.0000, lb=0, ub=2)

IMPDER = model.add_variable(value=114.7, lb=100, ub=1000)

IMPK = model.add_variable(value=500.00, lb=500, ub=5000)

IMPFUS = model.add_variable(value=1760.5, lb=500, ub=5000)

QI = model.add_variable(value=2325.6, lb=1000, ub=20000)

PT = model.add_variable(value=5.6788, lb=2, ub=30)

MV = model.add_variable(value=14197.0, lb=2000, ub=20000)

MC = model.add_variable(value=12589.0, lb=3000, ub=30000)

MD = model.add_variable(value=28394.0, lb=5000, ub=50000)

PD = model.add_variable(value=0.2000, lb=0.2, ub=0.8)

NS = model.add_variable(value=1.0000, lb=1, ub=5)

VS = model.add_variable(value=0.0000, lb=0, ub=20)

CR = model.add_variable(value=100.00, lb=100, ub=400)

PM = model.add_variable(value=15.000, lb=4, ub=15)

DV = model.add_variable(value=0.0000, lb=0, ub=10)

MZ = model.add_variable(value=500.00, lb=500, ub=10000)

VN = model.add_variable(value=10.000, lb=10, ub=50)

QV = model.add_variable(value=814.90, lb=250, ub=5000)

QF = model.add_variable(value=3140.5, lb=750, ub=15000)

IMPTRAIN = model.add_variable(value=1945.0, lb=250, ub=3000)

IMPMOT = model.add_variable(value=190.85, lb=10, ub=5000)

IMPNMOT = model.add_variable(value=35.000, lb=35, ub=70)

IMPPET = model.add_variable(value=100.00, lb=100, ub=3000)

IMPPIL = model.add_variable(value=200.00, lb=200, ub=400)

IMPCAN = model.add_variable(value=120.00, lb=120, ub=240)

IMPSNA = model.add_variable(value=700.00, lb=700, ub=1900)

MS = model.add_variable(value=1000.0, lb=100, ub=1000)

EL = model.add_variable(value=4.9367, lb=2, ub=20)

DE = model.add_variable(value=0.0000, lb=0, ub=1)

DS = model.add_variable(value=0.0000, lb=0, ub=2)

IMPVOIL = model.add_variable(value=5000.0, lb=500, ub=5000)

NM = model.add_variable(value=1.0, lb=1, ub=2)

NP = model.add_variable(value=1.0, lb=1, ub=2)

NG = model.add_variable(value=1.0, lb=1, ub=2)

expr = (SK - 0.01*PK*SR)**2 \
+ (CA - (SS-SO-CB*LF)/(LF**2) )**2 \
+ (-2*AM+SO+SS + 0.01*EF/LF)**2 \
+ (AM - 0.025*SO*CB**2/CA)**2 \
+ (IMPDER - 27.5*SD - 1.3*SD**2)**2 \
+ (IMPK - 70*SK + 8.6*SK**2)**2 \
+ (QI - 1000 + MV**2/24000)**2 \
+ (1000*PT - MD*PD)**2 \
+ (VN + VS +QF/790 + 2 - MZ/CR +DV*PT)**2 \
+ (IMPMOT - 1000*PT/(PM+20) - 12*sqrt(PT))**2 \
+ (ST - 1.25*SR*NM)**2 \
+ (SR - MD/MS)**2 \
+ (QV - 2.4*SX*sqrt(SX)*EL/sqrt(LX))**2 \
+ (SO - 0.785*DE**2*PT)**2 \
+ (SS - 0.785*DS**2*PT)**2 \
+ (CB - 2*(VN-CA*LF**3)/(LF**2*(3-SO*LF)))**2 \
+ (IMPVOIL - 1.15*SX*(15+0.15*SX)*(8+(MC*LX/(50*SR*EL))**1.5))**2
model.add_objective( expr )

model.add_constraint( 0 == SD-0.13*SR) 

model.add_constraint( SX-0.7*SR == 0 )

model.add_constraint( LX-LR == 0 )

model.add_constraint( SF - ST - 2*SD - 2*SX - 2*SK == 0 )

model.add_constraint( IMPFUS - 20*SF == 0 )

model.add_constraint( MD - 2*MV == 0 )

model.add_constraint( QF - QI - QV == 0 )

model.add_constraint( IMPTRAIN - 0.137*MV == 0 )

model.add_constraint( IMPNMOT - 35*NM == 0 )

model.add_constraint( IMPPET - 0.043*QI == 0 )

model.add_constraint( IMPPIL - 200*NP == 0 )

model.add_constraint( IMPCAN - 120*NG == 0 )

model.add_constraint( IMPSNA - 300*NS -400 == 0 )

model.add_constraint( MC - MV + 95*NP + 70*NG + 660*NM + 0.5*QI -380 == 0 )

model.add_constraint( MZ - IMPTRAIN + IMPNMOT + IMPPET + IMPPIL + IMPCAN + IMPSNA + 290 == 0 )
