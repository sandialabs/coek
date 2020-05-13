# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes. Taken from:

#
#**************************
# SET UP THE INITIAL DATA *
#**************************
#   Problem :
#   *********
#   A minimum energy problem in quantum physics.
#   Source:
#   Leonor Cruzeiro-Hasson (private communication).
#   SIF input: Nick Gould, Feb 1991.
#   classification OOR2-MY-V-1
#   N is the number of discretization points.
#IE N                   10
#IE N                   50
#IE N                   100
#   problem constants
#   Define useful parameters
#   first contributions to the objective function
#   first contributions to the nonlinear constraint
#   general contributions to the objective function
#   general contributions to the nonlinear constraint
#   last contributions to the objective function
#   last contributions to the nonlinear constraint
#   Solution
#LO SOLTN(10)          -3.7565286
#LO SOLTN(50)          -4.2295940
#LO SOLTN(100)         -4.2887306

import poek as pk

model = pk.model()

pr1 = model.variable(value=0.05)
pi1 = model.variable(value=0.05)
u1 = model.variable(value=0.1)
pr2 = model.variable(value=0.05)
pi2 = model.variable(value=0.05)
u2 = model.variable(value=0.2)
pr3 = model.variable(value=0.05)
pi3 = model.variable(value=0.05)
u3 = model.variable(value=0.3)
pr4 = model.variable(value=0.05)
pi4 = model.variable(value=0.05)
u4 = model.variable(value=0.4)
pr5 = model.variable(value=0.05)
pi5 = model.variable(value=0.05)
u5 = model.variable(value=0.5)
pr6 = model.variable(value=0.05)
pi6 = model.variable(value=0.05)
u6 = model.variable(value=0.6)
pr7 = model.variable(value=0.05)
pi7 = model.variable(value=0.05)
u7 = model.variable(value=0.7)
pr8 = model.variable(value=0.05)
pi8 = model.variable(value=0.05)
u8 = model.variable(value=0.7999999999999999)
pr9 = model.variable(value=0.05)
pi9 = model.variable(value=0.05)
u9 = model.variable(value=0.8999999999999999)
pr10 = model.variable(value=0.05)
pi10 = model.variable(value=0.05)
u10 = model.variable(value=0.9999999999999999)
pr11 = model.variable(value=0.05)
pi11 = model.variable(value=0.05)
u11 = model.variable(value=1.0999999999999999)
pr12 = model.variable(value=0.05)
pi12 = model.variable(value=0.05)
u12 = model.variable(value=1.2)
pr13 = model.variable(value=0.05)
pi13 = model.variable(value=0.05)
u13 = model.variable(value=1.3)
pr14 = model.variable(value=0.05)
pi14 = model.variable(value=0.05)
u14 = model.variable(value=1.4000000000000001)
pr15 = model.variable(value=0.05)
pi15 = model.variable(value=0.05)
u15 = model.variable(value=1.5000000000000002)
pr16 = model.variable(value=0.05)
pi16 = model.variable(value=0.05)
u16 = model.variable(value=1.6000000000000003)
pr17 = model.variable(value=0.05)
pi17 = model.variable(value=0.05)
u17 = model.variable(value=1.7000000000000004)
pr18 = model.variable(value=0.05)
pi18 = model.variable(value=0.05)
u18 = model.variable(value=1.8000000000000005)
pr19 = model.variable(value=0.05)
pi19 = model.variable(value=0.05)
u19 = model.variable(value=1.9000000000000006)
pr20 = model.variable(value=0.05)
pi20 = model.variable(value=0.05)
u20 = model.variable(value=2.0000000000000004)
pr21 = model.variable(value=0.05)
pi21 = model.variable(value=0.05)
u21 = model.variable(value=2.1000000000000005)
pr22 = model.variable(value=0.05)
pi22 = model.variable(value=0.05)
u22 = model.variable(value=2.2000000000000006)
pr23 = model.variable(value=0.05)
pi23 = model.variable(value=0.05)
u23 = model.variable(value=2.3000000000000007)
pr24 = model.variable(value=0.05)
pi24 = model.variable(value=0.05)
u24 = model.variable(value=2.400000000000001)
pr25 = model.variable(value=0.05)
pi25 = model.variable(value=0.05)
u25 = model.variable(value=2.500000000000001)
pr26 = model.variable(value=0.05)
pi26 = model.variable(value=0.05)
u26 = model.variable(value=2.600000000000001)
pr27 = model.variable(value=0.05)
pi27 = model.variable(value=0.05)
u27 = model.variable(value=2.700000000000001)
pr28 = model.variable(value=0.05)
pi28 = model.variable(value=0.05)
u28 = model.variable(value=2.800000000000001)
pr29 = model.variable(value=0.05)
pi29 = model.variable(value=0.05)
u29 = model.variable(value=2.9000000000000012)
pr30 = model.variable(value=0.05)
pi30 = model.variable(value=0.05)
u30 = model.variable(value=3.0000000000000013)
pr31 = model.variable(value=0.05)
pi31 = model.variable(value=0.05)
u31 = model.variable(value=3.1000000000000014)
pr32 = model.variable(value=0.05)
pi32 = model.variable(value=0.05)
u32 = model.variable(value=3.2000000000000015)
pr33 = model.variable(value=0.05)
pi33 = model.variable(value=0.05)
u33 = model.variable(value=3.3000000000000016)
pr34 = model.variable(value=0.05)
pi34 = model.variable(value=0.05)
u34 = model.variable(value=3.4000000000000017)
pr35 = model.variable(value=0.05)
pi35 = model.variable(value=0.05)
u35 = model.variable(value=3.5000000000000018)
pr36 = model.variable(value=0.05)
pi36 = model.variable(value=0.05)
u36 = model.variable(value=3.600000000000002)
pr37 = model.variable(value=0.05)
pi37 = model.variable(value=0.05)
u37 = model.variable(value=3.700000000000002)
pr38 = model.variable(value=0.05)
pi38 = model.variable(value=0.05)
u38 = model.variable(value=3.800000000000002)
pr39 = model.variable(value=0.05)
pi39 = model.variable(value=0.05)
u39 = model.variable(value=3.900000000000002)
pr40 = model.variable(value=0.05)
pi40 = model.variable(value=0.05)
u40 = model.variable(value=4.000000000000002)
pr41 = model.variable(value=0.05)
pi41 = model.variable(value=0.05)
u41 = model.variable(value=4.100000000000001)
pr42 = model.variable(value=0.05)
pi42 = model.variable(value=0.05)
u42 = model.variable(value=4.200000000000001)
pr43 = model.variable(value=0.05)
pi43 = model.variable(value=0.05)
u43 = model.variable(value=4.300000000000001)
pr44 = model.variable(value=0.05)
pi44 = model.variable(value=0.05)
u44 = model.variable(value=4.4)
pr45 = model.variable(value=0.05)
pi45 = model.variable(value=0.05)
u45 = model.variable(value=4.5)
pr46 = model.variable(value=0.05)
pi46 = model.variable(value=0.05)
u46 = model.variable(value=4.6)
pr47 = model.variable(value=0.05)
pi47 = model.variable(value=0.05)
u47 = model.variable(value=4.699999999999999)
pr48 = model.variable(value=0.05)
pi48 = model.variable(value=0.05)
u48 = model.variable(value=4.799999999999999)
pr49 = model.variable(value=0.05)
pi49 = model.variable(value=0.05)
u49 = model.variable(value=4.899999999999999)
pr50 = model.variable(value=0.05)
pi50 = model.variable(value=0.05)
u50 = model.variable(value=4.999999999999998)
pr51 = model.variable(value=0.05)
pi51 = model.variable(value=0.05)
u51 = model.variable(value=5.099999999999998)
pr52 = model.variable(value=0.05)
pi52 = model.variable(value=0.05)
u52 = model.variable(value=5.1999999999999975)
pr53 = model.variable(value=0.05)
pi53 = model.variable(value=0.05)
u53 = model.variable(value=5.299999999999997)
pr54 = model.variable(value=0.05)
pi54 = model.variable(value=0.05)
u54 = model.variable(value=5.399999999999997)
pr55 = model.variable(value=0.05)
pi55 = model.variable(value=0.05)
u55 = model.variable(value=5.4999999999999964)
pr56 = model.variable(value=0.05)
pi56 = model.variable(value=0.05)
u56 = model.variable(value=5.599999999999996)
pr57 = model.variable(value=0.05)
pi57 = model.variable(value=0.05)
u57 = model.variable(value=5.699999999999996)
pr58 = model.variable(value=0.05)
pi58 = model.variable(value=0.05)
u58 = model.variable(value=5.799999999999995)
pr59 = model.variable(value=0.05)
pi59 = model.variable(value=0.05)
u59 = model.variable(value=5.899999999999995)
pr60 = model.variable(value=0.05)
pi60 = model.variable(value=0.05)
u60 = model.variable(value=5.999999999999995)
pr61 = model.variable(value=0.05)
pi61 = model.variable(value=0.05)
u61 = model.variable(value=6.099999999999994)
pr62 = model.variable(value=0.05)
pi62 = model.variable(value=0.05)
u62 = model.variable(value=6.199999999999994)
pr63 = model.variable(value=0.05)
pi63 = model.variable(value=0.05)
u63 = model.variable(value=6.299999999999994)
pr64 = model.variable(value=0.05)
pi64 = model.variable(value=0.05)
u64 = model.variable(value=6.399999999999993)
pr65 = model.variable(value=0.05)
pi65 = model.variable(value=0.05)
u65 = model.variable(value=6.499999999999993)
pr66 = model.variable(value=0.05)
pi66 = model.variable(value=0.05)
u66 = model.variable(value=6.5999999999999925)
pr67 = model.variable(value=0.05)
pi67 = model.variable(value=0.05)
u67 = model.variable(value=6.699999999999992)
pr68 = model.variable(value=0.05)
pi68 = model.variable(value=0.05)
u68 = model.variable(value=6.799999999999992)
pr69 = model.variable(value=0.05)
pi69 = model.variable(value=0.05)
u69 = model.variable(value=6.8999999999999915)
pr70 = model.variable(value=0.05)
pi70 = model.variable(value=0.05)
u70 = model.variable(value=6.999999999999991)
pr71 = model.variable(value=0.05)
pi71 = model.variable(value=0.05)
u71 = model.variable(value=7.099999999999991)
pr72 = model.variable(value=0.05)
pi72 = model.variable(value=0.05)
u72 = model.variable(value=7.19999999999999)
pr73 = model.variable(value=0.05)
pi73 = model.variable(value=0.05)
u73 = model.variable(value=7.29999999999999)
pr74 = model.variable(value=0.05)
pi74 = model.variable(value=0.05)
u74 = model.variable(value=7.39999999999999)
pr75 = model.variable(value=0.05)
pi75 = model.variable(value=0.05)
u75 = model.variable(value=7.499999999999989)
pr76 = model.variable(value=0.05)
pi76 = model.variable(value=0.05)
u76 = model.variable(value=7.599999999999989)
pr77 = model.variable(value=0.05)
pi77 = model.variable(value=0.05)
u77 = model.variable(value=7.699999999999989)
pr78 = model.variable(value=0.05)
pi78 = model.variable(value=0.05)
u78 = model.variable(value=7.799999999999988)
pr79 = model.variable(value=0.05)
pi79 = model.variable(value=0.05)
u79 = model.variable(value=7.899999999999988)
pr80 = model.variable(value=0.05)
pi80 = model.variable(value=0.05)
u80 = model.variable(value=7.999999999999988)
pr81 = model.variable(value=0.05)
pi81 = model.variable(value=0.05)
u81 = model.variable(value=8.099999999999987)
pr82 = model.variable(value=0.05)
pi82 = model.variable(value=0.05)
u82 = model.variable(value=8.199999999999987)
pr83 = model.variable(value=0.05)
pi83 = model.variable(value=0.05)
u83 = model.variable(value=8.299999999999986)
pr84 = model.variable(value=0.05)
pi84 = model.variable(value=0.05)
u84 = model.variable(value=8.399999999999986)
pr85 = model.variable(value=0.05)
pi85 = model.variable(value=0.05)
u85 = model.variable(value=8.499999999999986)
pr86 = model.variable(value=0.05)
pi86 = model.variable(value=0.05)
u86 = model.variable(value=8.599999999999985)
pr87 = model.variable(value=0.05)
pi87 = model.variable(value=0.05)
u87 = model.variable(value=8.699999999999985)
pr88 = model.variable(value=0.05)
pi88 = model.variable(value=0.05)
u88 = model.variable(value=8.799999999999985)
pr89 = model.variable(value=0.05)
pi89 = model.variable(value=0.05)
u89 = model.variable(value=8.899999999999984)
pr90 = model.variable(value=0.05)
pi90 = model.variable(value=0.05)
u90 = model.variable(value=8.999999999999984)
pr91 = model.variable(value=0.05)
pi91 = model.variable(value=0.05)
u91 = model.variable(value=9.099999999999984)
pr92 = model.variable(value=0.05)
pi92 = model.variable(value=0.05)
u92 = model.variable(value=9.199999999999983)
pr93 = model.variable(value=0.05)
pi93 = model.variable(value=0.05)
u93 = model.variable(value=9.299999999999983)
pr94 = model.variable(value=0.05)
pi94 = model.variable(value=0.05)
u94 = model.variable(value=9.399999999999983)
pr95 = model.variable(value=0.05)
pi95 = model.variable(value=0.05)
u95 = model.variable(value=9.499999999999982)
pr96 = model.variable(value=0.05)
pi96 = model.variable(value=0.05)
u96 = model.variable(value=9.599999999999982)
pr97 = model.variable(value=0.05)
pi97 = model.variable(value=0.05)
u97 = model.variable(value=9.699999999999982)
pr98 = model.variable(value=0.05)
pi98 = model.variable(value=0.05)
u98 = model.variable(value=9.799999999999981)
pr99 = model.variable(value=0.05)
pi99 = model.variable(value=0.05)
u99 = model.variable(value=9.89999999999998)
pr100 = model.variable(value=0.05)
pi100 = model.variable(value=0.05)
u100 = model.variable(value=9.99999999999998)
pr101 = model.variable(value=0.05)
pi101 = model.variable(value=0.05)
u101 = model.variable(value=10.09999999999998)
pr102 = model.variable(value=0.05)
pi102 = model.variable(value=0.05)
u102 = model.variable(value=10.19999999999998)
pr103 = model.variable(value=0.05)
pi103 = model.variable(value=0.05)
u103 = model.variable(value=10.29999999999998)
pr104 = model.variable(value=0.05)
pi104 = model.variable(value=0.05)
u104 = model.variable(value=10.399999999999979)
pr105 = model.variable(value=0.05)
pi105 = model.variable(value=0.05)
u105 = model.variable(value=10.499999999999979)
pr106 = model.variable(value=0.05)
pi106 = model.variable(value=0.05)
u106 = model.variable(value=10.599999999999978)
pr107 = model.variable(value=0.05)
pi107 = model.variable(value=0.05)
u107 = model.variable(value=10.699999999999978)
pr108 = model.variable(value=0.05)
pi108 = model.variable(value=0.05)
u108 = model.variable(value=10.799999999999978)
pr109 = model.variable(value=0.05)
pi109 = model.variable(value=0.05)
u109 = model.variable(value=10.899999999999977)
pr110 = model.variable(value=0.05)
pi110 = model.variable(value=0.05)
u110 = model.variable(value=10.999999999999977)
pr111 = model.variable(value=0.05)
pi111 = model.variable(value=0.05)
u111 = model.variable(value=11.099999999999977)
pr112 = model.variable(value=0.05)
pi112 = model.variable(value=0.05)
u112 = model.variable(value=11.199999999999976)
pr113 = model.variable(value=0.05)
pi113 = model.variable(value=0.05)
u113 = model.variable(value=11.299999999999976)
pr114 = model.variable(value=0.05)
pi114 = model.variable(value=0.05)
u114 = model.variable(value=11.399999999999975)
pr115 = model.variable(value=0.05)
pi115 = model.variable(value=0.05)
u115 = model.variable(value=11.499999999999975)
pr116 = model.variable(value=0.05)
pi116 = model.variable(value=0.05)
u116 = model.variable(value=11.599999999999975)
pr117 = model.variable(value=0.05)
pi117 = model.variable(value=0.05)
u117 = model.variable(value=11.699999999999974)
pr118 = model.variable(value=0.05)
pi118 = model.variable(value=0.05)
u118 = model.variable(value=11.799999999999974)
pr119 = model.variable(value=0.05)
pi119 = model.variable(value=0.05)
u119 = model.variable(value=11.899999999999974)
pr120 = model.variable(value=0.05)
pi120 = model.variable(value=0.05)
u120 = model.variable(value=11.999999999999973)
pr121 = model.variable(value=0.05)
pi121 = model.variable(value=0.05)
u121 = model.variable(value=12.099999999999973)
pr122 = model.variable(value=0.05)
pi122 = model.variable(value=0.05)
u122 = model.variable(value=12.199999999999973)
pr123 = model.variable(value=0.05)
pi123 = model.variable(value=0.05)
u123 = model.variable(value=12.299999999999972)
pr124 = model.variable(value=0.05)
pi124 = model.variable(value=0.05)
u124 = model.variable(value=12.399999999999972)
pr125 = model.variable(value=0.05)
pi125 = model.variable(value=0.05)
u125 = model.variable(value=12.499999999999972)
pr126 = model.variable(value=0.05)
pi126 = model.variable(value=0.05)
u126 = model.variable(value=12.599999999999971)
pr127 = model.variable(value=0.05)
pi127 = model.variable(value=0.05)
u127 = model.variable(value=12.69999999999997)
pr128 = model.variable(value=0.05)
pi128 = model.variable(value=0.05)
u128 = model.variable(value=12.79999999999997)
pr129 = model.variable(value=0.05)
pi129 = model.variable(value=0.05)
u129 = model.variable(value=12.89999999999997)
pr130 = model.variable(value=0.05)
pi130 = model.variable(value=0.05)
u130 = model.variable(value=12.99999999999997)
pr131 = model.variable(value=0.05)
pi131 = model.variable(value=0.05)
u131 = model.variable(value=13.09999999999997)
pr132 = model.variable(value=0.05)
pi132 = model.variable(value=0.05)
u132 = model.variable(value=13.199999999999969)
pr133 = model.variable(value=0.05)
pi133 = model.variable(value=0.05)
u133 = model.variable(value=13.299999999999969)
pr134 = model.variable(value=0.05)
pi134 = model.variable(value=0.05)
u134 = model.variable(value=13.399999999999968)
pr135 = model.variable(value=0.05)
pi135 = model.variable(value=0.05)
u135 = model.variable(value=13.499999999999968)
pr136 = model.variable(value=0.05)
pi136 = model.variable(value=0.05)
u136 = model.variable(value=13.599999999999968)
pr137 = model.variable(value=0.05)
pi137 = model.variable(value=0.05)
u137 = model.variable(value=13.699999999999967)
pr138 = model.variable(value=0.05)
pi138 = model.variable(value=0.05)
u138 = model.variable(value=13.799999999999967)
pr139 = model.variable(value=0.05)
pi139 = model.variable(value=0.05)
u139 = model.variable(value=13.899999999999967)
pr140 = model.variable(value=0.05)
pi140 = model.variable(value=0.05)
u140 = model.variable(value=13.999999999999966)
pr141 = model.variable(value=0.05)
pi141 = model.variable(value=0.05)
u141 = model.variable(value=14.099999999999966)
pr142 = model.variable(value=0.05)
pi142 = model.variable(value=0.05)
u142 = model.variable(value=14.199999999999966)
pr143 = model.variable(value=0.05)
pi143 = model.variable(value=0.05)
u143 = model.variable(value=14.299999999999965)
pr144 = model.variable(value=0.05)
pi144 = model.variable(value=0.05)
u144 = model.variable(value=14.399999999999965)
pr145 = model.variable(value=0.05)
pi145 = model.variable(value=0.05)
u145 = model.variable(value=14.499999999999964)
pr146 = model.variable(value=0.05)
pi146 = model.variable(value=0.05)
u146 = model.variable(value=14.599999999999964)
pr147 = model.variable(value=0.05)
pi147 = model.variable(value=0.05)
u147 = model.variable(value=14.699999999999964)
pr148 = model.variable(value=0.05)
pi148 = model.variable(value=0.05)
u148 = model.variable(value=14.799999999999963)
pr149 = model.variable(value=0.05)
pi149 = model.variable(value=0.05)
u149 = model.variable(value=14.899999999999963)
pr150 = model.variable(value=0.05)
pi150 = model.variable(value=0.05)
u150 = model.variable(value=14.999999999999963)
pr151 = model.variable(value=0.05)
pi151 = model.variable(value=0.05)
u151 = model.variable(value=15.099999999999962)
pr152 = model.variable(value=0.05)
pi152 = model.variable(value=0.05)
u152 = model.variable(value=15.199999999999962)
pr153 = model.variable(value=0.05)
pi153 = model.variable(value=0.05)
u153 = model.variable(value=15.299999999999962)
pr154 = model.variable(value=0.05)
pi154 = model.variable(value=0.05)
u154 = model.variable(value=15.399999999999961)
pr155 = model.variable(value=0.05)
pi155 = model.variable(value=0.05)
u155 = model.variable(value=15.499999999999961)
pr156 = model.variable(value=0.05)
pi156 = model.variable(value=0.05)
u156 = model.variable(value=15.59999999999996)
pr157 = model.variable(value=0.05)
pi157 = model.variable(value=0.05)
u157 = model.variable(value=15.69999999999996)
pr158 = model.variable(value=0.05)
pi158 = model.variable(value=0.05)
u158 = model.variable(value=15.79999999999996)
pr159 = model.variable(value=0.05)
pi159 = model.variable(value=0.05)
u159 = model.variable(value=15.89999999999996)
pr160 = model.variable(value=0.05)
pi160 = model.variable(value=0.05)
u160 = model.variable(value=15.99999999999996)
pr161 = model.variable(value=0.05)
pi161 = model.variable(value=0.05)
u161 = model.variable(value=16.09999999999996)
pr162 = model.variable(value=0.05)
pi162 = model.variable(value=0.05)
u162 = model.variable(value=16.19999999999996)
pr163 = model.variable(value=0.05)
pi163 = model.variable(value=0.05)
u163 = model.variable(value=16.29999999999996)
pr164 = model.variable(value=0.05)
pi164 = model.variable(value=0.05)
u164 = model.variable(value=16.399999999999963)
pr165 = model.variable(value=0.05)
pi165 = model.variable(value=0.05)
u165 = model.variable(value=16.499999999999964)
pr166 = model.variable(value=0.05)
pi166 = model.variable(value=0.05)
u166 = model.variable(value=16.599999999999966)
pr167 = model.variable(value=0.05)
pi167 = model.variable(value=0.05)
u167 = model.variable(value=16.699999999999967)
pr168 = model.variable(value=0.05)
pi168 = model.variable(value=0.05)
u168 = model.variable(value=16.79999999999997)
pr169 = model.variable(value=0.05)
pi169 = model.variable(value=0.05)
u169 = model.variable(value=16.89999999999997)
pr170 = model.variable(value=0.05)
pi170 = model.variable(value=0.05)
u170 = model.variable(value=16.99999999999997)
pr171 = model.variable(value=0.05)
pi171 = model.variable(value=0.05)
u171 = model.variable(value=17.099999999999973)
pr172 = model.variable(value=0.05)
pi172 = model.variable(value=0.05)
u172 = model.variable(value=17.199999999999974)
pr173 = model.variable(value=0.05)
pi173 = model.variable(value=0.05)
u173 = model.variable(value=17.299999999999976)
pr174 = model.variable(value=0.05)
pi174 = model.variable(value=0.05)
u174 = model.variable(value=17.399999999999977)
pr175 = model.variable(value=0.05)
pi175 = model.variable(value=0.05)
u175 = model.variable(value=17.49999999999998)
pr176 = model.variable(value=0.05)
pi176 = model.variable(value=0.05)
u176 = model.variable(value=17.59999999999998)
pr177 = model.variable(value=0.05)
pi177 = model.variable(value=0.05)
u177 = model.variable(value=17.69999999999998)
pr178 = model.variable(value=0.05)
pi178 = model.variable(value=0.05)
u178 = model.variable(value=17.799999999999983)
pr179 = model.variable(value=0.05)
pi179 = model.variable(value=0.05)
u179 = model.variable(value=17.899999999999984)
pr180 = model.variable(value=0.05)
pi180 = model.variable(value=0.05)
u180 = model.variable(value=17.999999999999986)
pr181 = model.variable(value=0.05)
pi181 = model.variable(value=0.05)
u181 = model.variable(value=18.099999999999987)
pr182 = model.variable(value=0.05)
pi182 = model.variable(value=0.05)
u182 = model.variable(value=18.19999999999999)
pr183 = model.variable(value=0.05)
pi183 = model.variable(value=0.05)
u183 = model.variable(value=18.29999999999999)
pr184 = model.variable(value=0.05)
pi184 = model.variable(value=0.05)
u184 = model.variable(value=18.39999999999999)
pr185 = model.variable(value=0.05)
pi185 = model.variable(value=0.05)
u185 = model.variable(value=18.499999999999993)
pr186 = model.variable(value=0.05)
pi186 = model.variable(value=0.05)
u186 = model.variable(value=18.599999999999994)
pr187 = model.variable(value=0.05)
pi187 = model.variable(value=0.05)
u187 = model.variable(value=18.699999999999996)
pr188 = model.variable(value=0.05)
pi188 = model.variable(value=0.05)
u188 = model.variable(value=18.799999999999997)
pr189 = model.variable(value=0.05)
pi189 = model.variable(value=0.05)
u189 = model.variable(value=18.9)
pr190 = model.variable(value=0.05)
pi190 = model.variable(value=0.05)
u190 = model.variable(value=19.0)
pr191 = model.variable(value=0.05)
pi191 = model.variable(value=0.05)
u191 = model.variable(value=19.1)
pr192 = model.variable(value=0.05)
pi192 = model.variable(value=0.05)
u192 = model.variable(value=19.200000000000003)
pr193 = model.variable(value=0.05)
pi193 = model.variable(value=0.05)
u193 = model.variable(value=19.300000000000004)
pr194 = model.variable(value=0.05)
pi194 = model.variable(value=0.05)
u194 = model.variable(value=19.400000000000006)
pr195 = model.variable(value=0.05)
pi195 = model.variable(value=0.05)
u195 = model.variable(value=19.500000000000007)
pr196 = model.variable(value=0.05)
pi196 = model.variable(value=0.05)
u196 = model.variable(value=19.60000000000001)
pr197 = model.variable(value=0.05)
pi197 = model.variable(value=0.05)
u197 = model.variable(value=19.70000000000001)
pr198 = model.variable(value=0.05)
pi198 = model.variable(value=0.05)
u198 = model.variable(value=19.80000000000001)
pr199 = model.variable(value=0.05)
pi199 = model.variable(value=0.05)
u199 = model.variable(value=19.900000000000013)
pr200 = model.variable(value=0.05)
pi200 = model.variable(value=0.05)
u200 = model.variable(value=20.000000000000014)

model.add( - 1.549429*(pr1) * (pr200+pr2) - 1.549429*(pi1) * (pi200+pi2) + (-62.0*u200+0.0*u1+62.0*u2) * (pr1*pr1+pi1*pi1) + 650.0*(u200-u1) * (u200-u1) - 1.549429*(pr2) * (pr1+pr3) - 1.549429*(pi2) * (pi1+pi3) + (-62.0*u1+0.0*u2+62.0*u3) * (pr2*pr2+pi2*pi2) + 650.0*(u1-u2) * (u1-u2) - 1.549429*(pr3) * (pr2+pr4) - 1.549429*(pi3) * (pi2+pi4) + (-62.0*u2+0.0*u3+62.0*u4) * (pr3*pr3+pi3*pi3) + 650.0*(u2-u3) * (u2-u3) - 1.549429*(pr4) * (pr3+pr5) - 1.549429*(pi4) * (pi3+pi5) + (-62.0*u3+0.0*u4+62.0*u5) * (pr4*pr4+pi4*pi4) + 650.0*(u3-u4) * (u3-u4) - 1.549429*(pr5) * (pr4+pr6) - 1.549429*(pi5) * (pi4+pi6) + (-62.0*u4+0.0*u5+62.0*u6) * (pr5*pr5+pi5*pi5) + 650.0*(u4-u5) * (u4-u5) - 1.549429*(pr6) * (pr5+pr7) - 1.549429*(pi6) * (pi5+pi7) + (-62.0*u5+0.0*u6+62.0*u7) * (pr6*pr6+pi6*pi6) + 650.0*(u5-u6) * (u5-u6) - 1.549429*(pr7) * (pr6+pr8) - 1.549429*(pi7) * (pi6+pi8) + 
    (-62.0*u6+0.0*u7+62.0*u8) * (pr7*pr7+pi7*pi7) + 650.0*(u6-u7) * (u6-u7) - 
    1.549429*(pr8) * (pr7+pr9) - 1.549429*(pi8) * (pi7+pi9) + 
    (-62.0*u7+0.0*u8+62.0*u9) * (pr8*pr8+pi8*pi8) + 650.0*(u7-u8) * (u7-u8) - 
    1.549429*(pr9) * (pr8+pr10) - 1.549429*(pi9) * (pi8+pi10) + 
    (-62.0*u8+0.0*u9+62.0*u10) * (pr9*pr9+pi9*pi9) + 650.0*(u8-u9) * (u8-u9) - 
    1.549429*(pr10) * (pr9+pr11) - 1.549429*(pi10) * (pi9+pi11) + 
    (-62.0*u9+0.0*u10+62.0*u11) * (pr10*pr10+pi10*pi10) + 650.0*(u9-u10) * (u9-u10) 
    - 1.549429*(pr11) * (pr10+pr12) - 1.549429*(pi11) * (pi10+pi12) + 
    (-62.0*u10+0.0*u11+62.0*u12) * (pr11*pr11+pi11*pi11) + 650.0*(u10-u11) * 
    (u10-u11) - 1.549429*(pr12) * (pr11+pr13) - 1.549429*(pi12) * (pi11+pi13) + 
    (-62.0*u11+0.0*u12+62.0*u13) * (pr12*pr12+pi12*pi12) + 650.0*(u11-u12) * 
    (u11-u12) - 1.549429*(pr13) * (pr12+pr14) - 1.549429*(pi13) * (pi12+pi14) + 
    (-62.0*u12+0.0*u13+62.0*u14) * (pr13*pr13+pi13*pi13) + 650.0*(u12-u13) * 
    (u12-u13) - 1.549429*(pr14) * (pr13+pr15) - 1.549429*(pi14) * (pi13+pi15) + 
    (-62.0*u13+0.0*u14+62.0*u15) * (pr14*pr14+pi14*pi14) + 650.0*(u13-u14) * 
    (u13-u14) - 1.549429*(pr15) * (pr14+pr16) - 1.549429*(pi15) * (pi14+pi16) + 
    (-62.0*u14+0.0*u15+62.0*u16) * (pr15*pr15+pi15*pi15) + 650.0*(u14-u15) * 
    (u14-u15) - 1.549429*(pr16) * (pr15+pr17) - 1.549429*(pi16) * (pi15+pi17) + 
    (-62.0*u15+0.0*u16+62.0*u17) * (pr16*pr16+pi16*pi16) + 650.0*(u15-u16) * 
    (u15-u16) - 1.549429*(pr17) * (pr16+pr18) - 1.549429*(pi17) * (pi16+pi18) + 
    (-62.0*u16+0.0*u17+62.0*u18) * (pr17*pr17+pi17*pi17) + 650.0*(u16-u17) * 
    (u16-u17) - 1.549429*(pr18) * (pr17+pr19) - 1.549429*(pi18) * (pi17+pi19) + 
    (-62.0*u17+0.0*u18+62.0*u19) * (pr18*pr18+pi18*pi18) + 650.0*(u17-u18) * 
    (u17-u18) - 1.549429*(pr19) * (pr18+pr20) - 1.549429*(pi19) * (pi18+pi20) + 
    (-62.0*u18+0.0*u19+62.0*u20) * (pr19*pr19+pi19*pi19) + 650.0*(u18-u19) * 
    (u18-u19) - 1.549429*(pr20) * (pr19+pr21) - 1.549429*(pi20) * (pi19+pi21) + 
    (-62.0*u19+0.0*u20+62.0*u21) * (pr20*pr20+pi20*pi20) + 650.0*(u19-u20) * 
    (u19-u20) - 1.549429*(pr21) * (pr20+pr22) - 1.549429*(pi21) * (pi20+pi22) + 
    (-62.0*u20+0.0*u21+62.0*u22) * (pr21*pr21+pi21*pi21) + 650.0*(u20-u21) * 
    (u20-u21) - 1.549429*(pr22) * (pr21+pr23) - 1.549429*(pi22) * (pi21+pi23) + 
    (-62.0*u21+0.0*u22+62.0*u23) * (pr22*pr22+pi22*pi22) + 650.0*(u21-u22) * 
    (u21-u22) - 1.549429*(pr23) * (pr22+pr24) - 1.549429*(pi23) * (pi22+pi24) + 
    (-62.0*u22+0.0*u23+62.0*u24) * (pr23*pr23+pi23*pi23) + 650.0*(u22-u23) * 
    (u22-u23) - 1.549429*(pr24) * (pr23+pr25) - 1.549429*(pi24) * (pi23+pi25) + 
    (-62.0*u23+0.0*u24+62.0*u25) * (pr24*pr24+pi24*pi24) + 650.0*(u23-u24) * 
    (u23-u24) - 1.549429*(pr25) * (pr24+pr26) - 1.549429*(pi25) * (pi24+pi26) + 
    (-62.0*u24+0.0*u25+62.0*u26) * (pr25*pr25+pi25*pi25) + 650.0*(u24-u25) * 
    (u24-u25) - 1.549429*(pr26) * (pr25+pr27) - 1.549429*(pi26) * (pi25+pi27) + 
    (-62.0*u25+0.0*u26+62.0*u27) * (pr26*pr26+pi26*pi26) + 650.0*(u25-u26) * 
    (u25-u26) - 1.549429*(pr27) * (pr26+pr28) - 1.549429*(pi27) * (pi26+pi28) + 
    (-62.0*u26+0.0*u27+62.0*u28) * (pr27*pr27+pi27*pi27) + 650.0*(u26-u27) * 
    (u26-u27) - 1.549429*(pr28) * (pr27+pr29) - 1.549429*(pi28) * (pi27+pi29) + 
    (-62.0*u27+0.0*u28+62.0*u29) * (pr28*pr28+pi28*pi28) + 650.0*(u27-u28) * 
    (u27-u28) - 1.549429*(pr29) * (pr28+pr30) - 1.549429*(pi29) * (pi28+pi30) + 
    (-62.0*u28+0.0*u29+62.0*u30) * (pr29*pr29+pi29*pi29) + 650.0*(u28-u29) * 
    (u28-u29) - 1.549429*(pr30) * (pr29+pr31) - 1.549429*(pi30) * (pi29+pi31) + 
    (-62.0*u29+0.0*u30+62.0*u31) * (pr30*pr30+pi30*pi30) + 650.0*(u29-u30) * 
    (u29-u30) - 1.549429*(pr31) * (pr30+pr32) - 1.549429*(pi31) * (pi30+pi32) + 
    (-62.0*u30+0.0*u31+62.0*u32) * (pr31*pr31+pi31*pi31) + 650.0*(u30-u31) * 
    (u30-u31) - 1.549429*(pr32) * (pr31+pr33) - 1.549429*(pi32) * (pi31+pi33) + 
    (-62.0*u31+0.0*u32+62.0*u33) * (pr32*pr32+pi32*pi32) + 650.0*(u31-u32) * 
    (u31-u32) - 1.549429*(pr33) * (pr32+pr34) - 1.549429*(pi33) * (pi32+pi34) + 
    (-62.0*u32+0.0*u33+62.0*u34) * (pr33*pr33+pi33*pi33) + 650.0*(u32-u33) * 
    (u32-u33) - 1.549429*(pr34) * (pr33+pr35) - 1.549429*(pi34) * (pi33+pi35) + 
    (-62.0*u33+0.0*u34+62.0*u35) * (pr34*pr34+pi34*pi34) + 650.0*(u33-u34) * 
    (u33-u34) - 1.549429*(pr35) * (pr34+pr36) - 1.549429*(pi35) * (pi34+pi36) + 
    (-62.0*u34+0.0*u35+62.0*u36) * (pr35*pr35+pi35*pi35) + 650.0*(u34-u35) * 
    (u34-u35) - 1.549429*(pr36) * (pr35+pr37) - 1.549429*(pi36) * (pi35+pi37) + 
    (-62.0*u35+0.0*u36+62.0*u37) * (pr36*pr36+pi36*pi36) + 650.0*(u35-u36) * 
    (u35-u36) - 1.549429*(pr37) * (pr36+pr38) - 1.549429*(pi37) * (pi36+pi38) + 
    (-62.0*u36+0.0*u37+62.0*u38) * (pr37*pr37+pi37*pi37) + 650.0*(u36-u37) * 
    (u36-u37) - 1.549429*(pr38) * (pr37+pr39) - 1.549429*(pi38) * (pi37+pi39) + 
    (-62.0*u37+0.0*u38+62.0*u39) * (pr38*pr38+pi38*pi38) + 650.0*(u37-u38) * 
    (u37-u38) - 1.549429*(pr39) * (pr38+pr40) - 1.549429*(pi39) * (pi38+pi40) + 
    (-62.0*u38+0.0*u39+62.0*u40) * (pr39*pr39+pi39*pi39) + 650.0*(u38-u39) * 
    (u38-u39) - 1.549429*(pr40) * (pr39+pr41) - 1.549429*(pi40) * (pi39+pi41) + 
    (-62.0*u39+0.0*u40+62.0*u41) * (pr40*pr40+pi40*pi40) + 650.0*(u39-u40) * 
    (u39-u40) - 1.549429*(pr41) * (pr40+pr42) - 1.549429*(pi41) * (pi40+pi42) + 
    (-62.0*u40+0.0*u41+62.0*u42) * (pr41*pr41+pi41*pi41) + 650.0*(u40-u41) * 
    (u40-u41) - 1.549429*(pr42) * (pr41+pr43) - 1.549429*(pi42) * (pi41+pi43) + 
    (-62.0*u41+0.0*u42+62.0*u43) * (pr42*pr42+pi42*pi42) + 650.0*(u41-u42) * 
    (u41-u42) - 1.549429*(pr43) * (pr42+pr44) - 1.549429*(pi43) * (pi42+pi44) + 
    (-62.0*u42+0.0*u43+62.0*u44) * (pr43*pr43+pi43*pi43) + 650.0*(u42-u43) * 
    (u42-u43) - 1.549429*(pr44) * (pr43+pr45) - 1.549429*(pi44) * (pi43+pi45) + 
    (-62.0*u43+0.0*u44+62.0*u45) * (pr44*pr44+pi44*pi44) + 650.0*(u43-u44) * 
    (u43-u44) - 1.549429*(pr45) * (pr44+pr46) - 1.549429*(pi45) * (pi44+pi46) + 
    (-62.0*u44+0.0*u45+62.0*u46) * (pr45*pr45+pi45*pi45) + 650.0*(u44-u45) * 
    (u44-u45) - 1.549429*(pr46) * (pr45+pr47) - 1.549429*(pi46) * (pi45+pi47) + 
    (-62.0*u45+0.0*u46+62.0*u47) * (pr46*pr46+pi46*pi46) + 650.0*(u45-u46) * 
    (u45-u46) - 1.549429*(pr47) * (pr46+pr48) - 1.549429*(pi47) * (pi46+pi48) + 
    (-62.0*u46+0.0*u47+62.0*u48) * (pr47*pr47+pi47*pi47) + 650.0*(u46-u47) * 
    (u46-u47) - 1.549429*(pr48) * (pr47+pr49) - 1.549429*(pi48) * (pi47+pi49) + 
    (-62.0*u47+0.0*u48+62.0*u49) * (pr48*pr48+pi48*pi48) + 650.0*(u47-u48) * 
    (u47-u48) - 1.549429*(pr49) * (pr48+pr50) - 1.549429*(pi49) * (pi48+pi50) + 
    (-62.0*u48+0.0*u49+62.0*u50) * (pr49*pr49+pi49*pi49) + 650.0*(u48-u49) * 
    (u48-u49) - 1.549429*(pr50) * (pr49+pr51) - 1.549429*(pi50) * (pi49+pi51) + 
    (-62.0*u49+0.0*u50+62.0*u51) * (pr50*pr50+pi50*pi50) + 650.0*(u49-u50) * 
    (u49-u50) - 1.549429*(pr51) * (pr50+pr52) - 1.549429*(pi51) * (pi50+pi52) + 
    (-62.0*u50+0.0*u51+62.0*u52) * (pr51*pr51+pi51*pi51) + 650.0*(u50-u51) * 
    (u50-u51) - 1.549429*(pr52) * (pr51+pr53) - 1.549429*(pi52) * (pi51+pi53) + 
    (-62.0*u51+0.0*u52+62.0*u53) * (pr52*pr52+pi52*pi52) + 650.0*(u51-u52) * 
    (u51-u52) - 1.549429*(pr53) * (pr52+pr54) - 1.549429*(pi53) * (pi52+pi54) + 
    (-62.0*u52+0.0*u53+62.0*u54) * (pr53*pr53+pi53*pi53) + 650.0*(u52-u53) * 
    (u52-u53) - 1.549429*(pr54) * (pr53+pr55) - 1.549429*(pi54) * (pi53+pi55) + 
    (-62.0*u53+0.0*u54+62.0*u55) * (pr54*pr54+pi54*pi54) + 650.0*(u53-u54) * 
    (u53-u54) - 1.549429*(pr55) * (pr54+pr56) - 1.549429*(pi55) * (pi54+pi56) + 
    (-62.0*u54+0.0*u55+62.0*u56) * (pr55*pr55+pi55*pi55) + 650.0*(u54-u55) * 
    (u54-u55) - 1.549429*(pr56) * (pr55+pr57) - 1.549429*(pi56) * (pi55+pi57) + 
    (-62.0*u55+0.0*u56+62.0*u57) * (pr56*pr56+pi56*pi56) + 650.0*(u55-u56) * 
    (u55-u56) - 1.549429*(pr57) * (pr56+pr58) - 1.549429*(pi57) * (pi56+pi58) + 
    (-62.0*u56+0.0*u57+62.0*u58) * (pr57*pr57+pi57*pi57) + 650.0*(u56-u57) * 
    (u56-u57) - 1.549429*(pr58) * (pr57+pr59) - 1.549429*(pi58) * (pi57+pi59) + 
    (-62.0*u57+0.0*u58+62.0*u59) * (pr58*pr58+pi58*pi58) + 650.0*(u57-u58) * 
    (u57-u58) - 1.549429*(pr59) * (pr58+pr60) - 1.549429*(pi59) * (pi58+pi60) + 
    (-62.0*u58+0.0*u59+62.0*u60) * (pr59*pr59+pi59*pi59) + 650.0*(u58-u59) * 
    (u58-u59) - 1.549429*(pr60) * (pr59+pr61) - 1.549429*(pi60) * (pi59+pi61) + 
    (-62.0*u59+0.0*u60+62.0*u61) * (pr60*pr60+pi60*pi60) + 650.0*(u59-u60) * 
    (u59-u60) - 1.549429*(pr61) * (pr60+pr62) - 1.549429*(pi61) * (pi60+pi62) + 
    (-62.0*u60+0.0*u61+62.0*u62) * (pr61*pr61+pi61*pi61) + 650.0*(u60-u61) * 
    (u60-u61) - 1.549429*(pr62) * (pr61+pr63) - 1.549429*(pi62) * (pi61+pi63) + 
    (-62.0*u61+0.0*u62+62.0*u63) * (pr62*pr62+pi62*pi62) + 650.0*(u61-u62) * 
    (u61-u62) - 1.549429*(pr63) * (pr62+pr64) - 1.549429*(pi63) * (pi62+pi64) + 
    (-62.0*u62+0.0*u63+62.0*u64) * (pr63*pr63+pi63*pi63) + 650.0*(u62-u63) * 
    (u62-u63) - 1.549429*(pr64) * (pr63+pr65) - 1.549429*(pi64) * (pi63+pi65) + 
    (-62.0*u63+0.0*u64+62.0*u65) * (pr64*pr64+pi64*pi64) + 650.0*(u63-u64) * 
    (u63-u64) - 1.549429*(pr65) * (pr64+pr66) - 1.549429*(pi65) * (pi64+pi66) + 
    (-62.0*u64+0.0*u65+62.0*u66) * (pr65*pr65+pi65*pi65) + 650.0*(u64-u65) * 
    (u64-u65) - 1.549429*(pr66) * (pr65+pr67) - 1.549429*(pi66) * (pi65+pi67) + 
    (-62.0*u65+0.0*u66+62.0*u67) * (pr66*pr66+pi66*pi66) + 650.0*(u65-u66) * 
    (u65-u66) - 1.549429*(pr67) * (pr66+pr68) - 1.549429*(pi67) * (pi66+pi68) + 
    (-62.0*u66+0.0*u67+62.0*u68) * (pr67*pr67+pi67*pi67) + 650.0*(u66-u67) * 
    (u66-u67) - 1.549429*(pr68) * (pr67+pr69) - 1.549429*(pi68) * (pi67+pi69) + 
    (-62.0*u67+0.0*u68+62.0*u69) * (pr68*pr68+pi68*pi68) + 650.0*(u67-u68) * 
    (u67-u68) - 1.549429*(pr69) * (pr68+pr70) - 1.549429*(pi69) * (pi68+pi70) + 
    (-62.0*u68+0.0*u69+62.0*u70) * (pr69*pr69+pi69*pi69) + 650.0*(u68-u69) * 
    (u68-u69) - 1.549429*(pr70) * (pr69+pr71) - 1.549429*(pi70) * (pi69+pi71) + 
    (-62.0*u69+0.0*u70+62.0*u71) * (pr70*pr70+pi70*pi70) + 650.0*(u69-u70) * 
    (u69-u70) - 1.549429*(pr71) * (pr70+pr72) - 1.549429*(pi71) * (pi70+pi72) + 
    (-62.0*u70+0.0*u71+62.0*u72) * (pr71*pr71+pi71*pi71) + 650.0*(u70-u71) * 
    (u70-u71) - 1.549429*(pr72) * (pr71+pr73) - 1.549429*(pi72) * (pi71+pi73) + 
    (-62.0*u71+0.0*u72+62.0*u73) * (pr72*pr72+pi72*pi72) + 650.0*(u71-u72) * 
    (u71-u72) - 1.549429*(pr73) * (pr72+pr74) - 1.549429*(pi73) * (pi72+pi74) + 
    (-62.0*u72+0.0*u73+62.0*u74) * (pr73*pr73+pi73*pi73) + 650.0*(u72-u73) * 
    (u72-u73) - 1.549429*(pr74) * (pr73+pr75) - 1.549429*(pi74) * (pi73+pi75) + 
    (-62.0*u73+0.0*u74+62.0*u75) * (pr74*pr74+pi74*pi74) + 650.0*(u73-u74) * 
    (u73-u74) - 1.549429*(pr75) * (pr74+pr76) - 1.549429*(pi75) * (pi74+pi76) + 
    (-62.0*u74+0.0*u75+62.0*u76) * (pr75*pr75+pi75*pi75) + 650.0*(u74-u75) * 
    (u74-u75) - 1.549429*(pr76) * (pr75+pr77) - 1.549429*(pi76) * (pi75+pi77) + 
    (-62.0*u75+0.0*u76+62.0*u77) * (pr76*pr76+pi76*pi76) + 650.0*(u75-u76) * 
    (u75-u76) - 1.549429*(pr77) * (pr76+pr78) - 1.549429*(pi77) * (pi76+pi78) + 
    (-62.0*u76+0.0*u77+62.0*u78) * (pr77*pr77+pi77*pi77) + 650.0*(u76-u77) * 
    (u76-u77) - 1.549429*(pr78) * (pr77+pr79) - 1.549429*(pi78) * (pi77+pi79) + 
    (-62.0*u77+0.0*u78+62.0*u79) * (pr78*pr78+pi78*pi78) + 650.0*(u77-u78) * 
    (u77-u78) - 1.549429*(pr79) * (pr78+pr80) - 1.549429*(pi79) * (pi78+pi80) + 
    (-62.0*u78+0.0*u79+62.0*u80) * (pr79*pr79+pi79*pi79) + 650.0*(u78-u79) * 
    (u78-u79) - 1.549429*(pr80) * (pr79+pr81) - 1.549429*(pi80) * (pi79+pi81) + 
    (-62.0*u79+0.0*u80+62.0*u81) * (pr80*pr80+pi80*pi80) + 650.0*(u79-u80) * 
    (u79-u80) - 1.549429*(pr81) * (pr80+pr82) - 1.549429*(pi81) * (pi80+pi82) + 
    (-62.0*u80+0.0*u81+62.0*u82) * (pr81*pr81+pi81*pi81) + 650.0*(u80-u81) * 
    (u80-u81) - 1.549429*(pr82) * (pr81+pr83) - 1.549429*(pi82) * (pi81+pi83) + 
    (-62.0*u81+0.0*u82+62.0*u83) * (pr82*pr82+pi82*pi82) + 650.0*(u81-u82) * 
    (u81-u82) - 1.549429*(pr83) * (pr82+pr84) - 1.549429*(pi83) * (pi82+pi84) + 
    (-62.0*u82+0.0*u83+62.0*u84) * (pr83*pr83+pi83*pi83) + 650.0*(u82-u83) * 
    (u82-u83) - 1.549429*(pr84) * (pr83+pr85) - 1.549429*(pi84) * (pi83+pi85) + 
    (-62.0*u83+0.0*u84+62.0*u85) * (pr84*pr84+pi84*pi84) + 650.0*(u83-u84) * 
    (u83-u84) - 1.549429*(pr85) * (pr84+pr86) - 1.549429*(pi85) * (pi84+pi86) + 
    (-62.0*u84+0.0*u85+62.0*u86) * (pr85*pr85+pi85*pi85) + 650.0*(u84-u85) * 
    (u84-u85) - 1.549429*(pr86) * (pr85+pr87) - 1.549429*(pi86) * (pi85+pi87) + 
    (-62.0*u85+0.0*u86+62.0*u87) * (pr86*pr86+pi86*pi86) + 650.0*(u85-u86) * 
    (u85-u86) - 1.549429*(pr87) * (pr86+pr88) - 1.549429*(pi87) * (pi86+pi88) + 
    (-62.0*u86+0.0*u87+62.0*u88) * (pr87*pr87+pi87*pi87) + 650.0*(u86-u87) * 
    (u86-u87) - 1.549429*(pr88) * (pr87+pr89) - 1.549429*(pi88) * (pi87+pi89) + 
    (-62.0*u87+0.0*u88+62.0*u89) * (pr88*pr88+pi88*pi88) + 650.0*(u87-u88) * 
    (u87-u88) - 1.549429*(pr89) * (pr88+pr90) - 1.549429*(pi89) * (pi88+pi90) + 
    (-62.0*u88+0.0*u89+62.0*u90) * (pr89*pr89+pi89*pi89) + 650.0*(u88-u89) * 
    (u88-u89) - 1.549429*(pr90) * (pr89+pr91) - 1.549429*(pi90) * (pi89+pi91) + 
    (-62.0*u89+0.0*u90+62.0*u91) * (pr90*pr90+pi90*pi90) + 650.0*(u89-u90) * 
    (u89-u90) - 1.549429*(pr91) * (pr90+pr92) - 1.549429*(pi91) * (pi90+pi92) + 
    (-62.0*u90+0.0*u91+62.0*u92) * (pr91*pr91+pi91*pi91) + 650.0*(u90-u91) * 
    (u90-u91) - 1.549429*(pr92) * (pr91+pr93) - 1.549429*(pi92) * (pi91+pi93) + 
    (-62.0*u91+0.0*u92+62.0*u93) * (pr92*pr92+pi92*pi92) + 650.0*(u91-u92) * 
    (u91-u92) - 1.549429*(pr93) * (pr92+pr94) - 1.549429*(pi93) * (pi92+pi94) + 
    (-62.0*u92+0.0*u93+62.0*u94) * (pr93*pr93+pi93*pi93) + 650.0*(u92-u93) * 
    (u92-u93) - 1.549429*(pr94) * (pr93+pr95) - 1.549429*(pi94) * (pi93+pi95) + 
    (-62.0*u93+0.0*u94+62.0*u95) * (pr94*pr94+pi94*pi94) + 650.0*(u93-u94) * 
    (u93-u94) - 1.549429*(pr95) * (pr94+pr96) - 1.549429*(pi95) * (pi94+pi96) + 
    (-62.0*u94+0.0*u95+62.0*u96) * (pr95*pr95+pi95*pi95) + 650.0*(u94-u95) * 
    (u94-u95) - 1.549429*(pr96) * (pr95+pr97) - 1.549429*(pi96) * (pi95+pi97) + 
    (-62.0*u95+0.0*u96+62.0*u97) * (pr96*pr96+pi96*pi96) + 650.0*(u95-u96) * 
    (u95-u96) - 1.549429*(pr97) * (pr96+pr98) - 1.549429*(pi97) * (pi96+pi98) + 
    (-62.0*u96+0.0*u97+62.0*u98) * (pr97*pr97+pi97*pi97) + 650.0*(u96-u97) * 
    (u96-u97) - 1.549429*(pr98) * (pr97+pr99) - 1.549429*(pi98) * (pi97+pi99) + 
    (-62.0*u97+0.0*u98+62.0*u99) * (pr98*pr98+pi98*pi98) + 650.0*(u97-u98) * 
    (u97-u98) - 1.549429*(pr99) * (pr98+pr100) - 1.549429*(pi99) * (pi98+pi100) + 
    (-62.0*u98+0.0*u99+62.0*u100) * (pr99*pr99+pi99*pi99) + 650.0*(u98-u99) * 
    (u98-u99) - 1.549429*(pr100) * (pr99+pr101) - 1.549429*(pi100) * (pi99+pi101) + 
    (-62.0*u99+0.0*u100+62.0*u101) * (pr100*pr100+pi100*pi100) + 650.0*(u99-u100) * 
    (u99-u100) - 1.549429*(pr101) * (pr100+pr102) - 1.549429*(pi101) * 
    (pi100+pi102) + (-62.0*u100+0.0*u101+62.0*u102) * (pr101*pr101+pi101*pi101) + 
    650.0*(u100-u101) * (u100-u101) - 1.549429*(pr102) * (pr101+pr103) - 
    1.549429*(pi102) * (pi101+pi103) + (-62.0*u101+0.0*u102+62.0*u103) * 
    (pr102*pr102+pi102*pi102) + 650.0*(u101-u102) * (u101-u102) - 1.549429*(pr103) 
    * (pr102+pr104) - 1.549429*(pi103) * (pi102+pi104) + 
    (-62.0*u102+0.0*u103+62.0*u104) * (pr103*pr103+pi103*pi103) + 650.0*(u102-u103) 
    * (u102-u103) - 1.549429*(pr104) * (pr103+pr105) - 1.549429*(pi104) * 
    (pi103+pi105) + (-62.0*u103+0.0*u104+62.0*u105) * (pr104*pr104+pi104*pi104) + 
    650.0*(u103-u104) * (u103-u104) - 1.549429*(pr105) * (pr104+pr106) - 
    1.549429*(pi105) * (pi104+pi106) + (-62.0*u104+0.0*u105+62.0*u106) * 
    (pr105*pr105+pi105*pi105) + 650.0*(u104-u105) * (u104-u105) - 1.549429*(pr106) 
    * (pr105+pr107) - 1.549429*(pi106) * (pi105+pi107) + 
    (-62.0*u105+0.0*u106+62.0*u107) * (pr106*pr106+pi106*pi106) + 650.0*(u105-u106) 
    * (u105-u106) - 1.549429*(pr107) * (pr106+pr108) - 1.549429*(pi107) * 
    (pi106+pi108) + (-62.0*u106+0.0*u107+62.0*u108) * (pr107*pr107+pi107*pi107) + 
    650.0*(u106-u107) * (u106-u107) - 1.549429*(pr108) * (pr107+pr109) - 
    1.549429*(pi108) * (pi107+pi109) + (-62.0*u107+0.0*u108+62.0*u109) * 
    (pr108*pr108+pi108*pi108) + 650.0*(u107-u108) * (u107-u108) - 1.549429*(pr109) 
    * (pr108+pr110) - 1.549429*(pi109) * (pi108+pi110) + 
    (-62.0*u108+0.0*u109+62.0*u110) * (pr109*pr109+pi109*pi109) + 650.0*(u108-u109) 
    * (u108-u109) - 1.549429*(pr110) * (pr109+pr111) - 1.549429*(pi110) * 
    (pi109+pi111) + (-62.0*u109+0.0*u110+62.0*u111) * (pr110*pr110+pi110*pi110) + 
    650.0*(u109-u110) * (u109-u110) - 1.549429*(pr111) * (pr110+pr112) - 
    1.549429*(pi111) * (pi110+pi112) + (-62.0*u110+0.0*u111+62.0*u112) * 
    (pr111*pr111+pi111*pi111) + 650.0*(u110-u111) * (u110-u111) - 1.549429*(pr112) 
    * (pr111+pr113) - 1.549429*(pi112) * (pi111+pi113) + 
    (-62.0*u111+0.0*u112+62.0*u113) * (pr112*pr112+pi112*pi112) + 650.0*(u111-u112) 
    * (u111-u112) - 1.549429*(pr113) * (pr112+pr114) - 1.549429*(pi113) * 
    (pi112+pi114) + (-62.0*u112+0.0*u113+62.0*u114) * (pr113*pr113+pi113*pi113) + 
    650.0*(u112-u113) * (u112-u113) - 1.549429*(pr114) * (pr113+pr115) - 
    1.549429*(pi114) * (pi113+pi115) + (-62.0*u113+0.0*u114+62.0*u115) * 
    (pr114*pr114+pi114*pi114) + 650.0*(u113-u114) * (u113-u114) - 1.549429*(pr115) 
    * (pr114+pr116) - 1.549429*(pi115) * (pi114+pi116) + 
    (-62.0*u114+0.0*u115+62.0*u116) * (pr115*pr115+pi115*pi115) + 650.0*(u114-u115) 
    * (u114-u115) - 1.549429*(pr116) * (pr115+pr117) - 1.549429*(pi116) * 
    (pi115+pi117) + (-62.0*u115+0.0*u116+62.0*u117) * (pr116*pr116+pi116*pi116) + 
    650.0*(u115-u116) * (u115-u116) - 1.549429*(pr117) * (pr116+pr118) - 
    1.549429*(pi117) * (pi116+pi118) + (-62.0*u116+0.0*u117+62.0*u118) * 
    (pr117*pr117+pi117*pi117) + 650.0*(u116-u117) * (u116-u117) - 1.549429*(pr118) 
    * (pr117+pr119) - 1.549429*(pi118) * (pi117+pi119) + 
    (-62.0*u117+0.0*u118+62.0*u119) * (pr118*pr118+pi118*pi118) + 650.0*(u117-u118) 
    * (u117-u118) - 1.549429*(pr119) * (pr118+pr120) - 1.549429*(pi119) * 
    (pi118+pi120) + (-62.0*u118+0.0*u119+62.0*u120) * (pr119*pr119+pi119*pi119) + 
    650.0*(u118-u119) * (u118-u119) - 1.549429*(pr120) * (pr119+pr121) - 
    1.549429*(pi120) * (pi119+pi121) + (-62.0*u119+0.0*u120+62.0*u121) * 
    (pr120*pr120+pi120*pi120) + 650.0*(u119-u120) * (u119-u120) - 1.549429*(pr121) 
    * (pr120+pr122) - 1.549429*(pi121) * (pi120+pi122) + 
    (-62.0*u120+0.0*u121+62.0*u122) * (pr121*pr121+pi121*pi121) + 650.0*(u120-u121) 
    * (u120-u121) - 1.549429*(pr122) * (pr121+pr123) - 1.549429*(pi122) * 
    (pi121+pi123) + (-62.0*u121+0.0*u122+62.0*u123) * (pr122*pr122+pi122*pi122) + 
    650.0*(u121-u122) * (u121-u122) - 1.549429*(pr123) * (pr122+pr124) - 
    1.549429*(pi123) * (pi122+pi124) + (-62.0*u122+0.0*u123+62.0*u124) * 
    (pr123*pr123+pi123*pi123) + 650.0*(u122-u123) * (u122-u123) - 1.549429*(pr124) 
    * (pr123+pr125) - 1.549429*(pi124) * (pi123+pi125) + 
    (-62.0*u123+0.0*u124+62.0*u125) * (pr124*pr124+pi124*pi124) + 650.0*(u123-u124) 
    * (u123-u124) - 1.549429*(pr125) * (pr124+pr126) - 1.549429*(pi125) * 
    (pi124+pi126) + (-62.0*u124+0.0*u125+62.0*u126) * (pr125*pr125+pi125*pi125) + 
    650.0*(u124-u125) * (u124-u125) - 1.549429*(pr126) * (pr125+pr127) - 
    1.549429*(pi126) * (pi125+pi127) + (-62.0*u125+0.0*u126+62.0*u127) * 
    (pr126*pr126+pi126*pi126) + 650.0*(u125-u126) * (u125-u126) - 1.549429*(pr127) 
    * (pr126+pr128) - 1.549429*(pi127) * (pi126+pi128) + 
    (-62.0*u126+0.0*u127+62.0*u128) * (pr127*pr127+pi127*pi127) + 650.0*(u126-u127) 
    * (u126-u127) - 1.549429*(pr128) * (pr127+pr129) - 1.549429*(pi128) * 
    (pi127+pi129) + (-62.0*u127+0.0*u128+62.0*u129) * (pr128*pr128+pi128*pi128) + 
    650.0*(u127-u128) * (u127-u128) - 1.549429*(pr129) * (pr128+pr130) - 
    1.549429*(pi129) * (pi128+pi130) + (-62.0*u128+0.0*u129+62.0*u130) * 
    (pr129*pr129+pi129*pi129) + 650.0*(u128-u129) * (u128-u129) - 1.549429*(pr130) 
    * (pr129+pr131) - 1.549429*(pi130) * (pi129+pi131) + 
    (-62.0*u129+0.0*u130+62.0*u131) * (pr130*pr130+pi130*pi130) + 650.0*(u129-u130) 
    * (u129-u130) - 1.549429*(pr131) * (pr130+pr132) - 1.549429*(pi131) * 
    (pi130+pi132) + (-62.0*u130+0.0*u131+62.0*u132) * (pr131*pr131+pi131*pi131) + 
    650.0*(u130-u131) * (u130-u131) - 1.549429*(pr132) * (pr131+pr133) - 
    1.549429*(pi132) * (pi131+pi133) + (-62.0*u131+0.0*u132+62.0*u133) * 
    (pr132*pr132+pi132*pi132) + 650.0*(u131-u132) * (u131-u132) - 1.549429*(pr133) 
    * (pr132+pr134) - 1.549429*(pi133) * (pi132+pi134) + 
    (-62.0*u132+0.0*u133+62.0*u134) * (pr133*pr133+pi133*pi133) + 650.0*(u132-u133) 
    * (u132-u133) - 1.549429*(pr134) * (pr133+pr135) - 1.549429*(pi134) * 
    (pi133+pi135) + (-62.0*u133+0.0*u134+62.0*u135) * (pr134*pr134+pi134*pi134) + 
    650.0*(u133-u134) * (u133-u134) - 1.549429*(pr135) * (pr134+pr136) - 
    1.549429*(pi135) * (pi134+pi136) + (-62.0*u134+0.0*u135+62.0*u136) * 
    (pr135*pr135+pi135*pi135) + 650.0*(u134-u135) * (u134-u135) - 1.549429*(pr136) 
    * (pr135+pr137) - 1.549429*(pi136) * (pi135+pi137) + 
    (-62.0*u135+0.0*u136+62.0*u137) * (pr136*pr136+pi136*pi136) + 650.0*(u135-u136) 
    * (u135-u136) - 1.549429*(pr137) * (pr136+pr138) - 1.549429*(pi137) * 
    (pi136+pi138) + (-62.0*u136+0.0*u137+62.0*u138) * (pr137*pr137+pi137*pi137) + 
    650.0*(u136-u137) * (u136-u137) - 1.549429*(pr138) * (pr137+pr139) - 
    1.549429*(pi138) * (pi137+pi139) + (-62.0*u137+0.0*u138+62.0*u139) * 
    (pr138*pr138+pi138*pi138) + 650.0*(u137-u138) * (u137-u138) - 1.549429*(pr139) 
    * (pr138+pr140) - 1.549429*(pi139) * (pi138+pi140) + 
    (-62.0*u138+0.0*u139+62.0*u140) * (pr139*pr139+pi139*pi139) + 650.0*(u138-u139) 
    * (u138-u139) - 1.549429*(pr140) * (pr139+pr141) - 1.549429*(pi140) * 
    (pi139+pi141) + (-62.0*u139+0.0*u140+62.0*u141) * (pr140*pr140+pi140*pi140) + 
    650.0*(u139-u140) * (u139-u140) - 1.549429*(pr141) * (pr140+pr142) - 
    1.549429*(pi141) * (pi140+pi142) + (-62.0*u140+0.0*u141+62.0*u142) * 
    (pr141*pr141+pi141*pi141) + 650.0*(u140-u141) * (u140-u141) - 1.549429*(pr142) 
    * (pr141+pr143) - 1.549429*(pi142) * (pi141+pi143) + 
    (-62.0*u141+0.0*u142+62.0*u143) * (pr142*pr142+pi142*pi142) + 650.0*(u141-u142) 
    * (u141-u142) - 1.549429*(pr143) * (pr142+pr144) - 1.549429*(pi143) * 
    (pi142+pi144) + (-62.0*u142+0.0*u143+62.0*u144) * (pr143*pr143+pi143*pi143) + 
    650.0*(u142-u143) * (u142-u143) - 1.549429*(pr144) * (pr143+pr145) - 
    1.549429*(pi144) * (pi143+pi145) + (-62.0*u143+0.0*u144+62.0*u145) * 
    (pr144*pr144+pi144*pi144) + 650.0*(u143-u144) * (u143-u144) - 1.549429*(pr145) 
    * (pr144+pr146) - 1.549429*(pi145) * (pi144+pi146) + 
    (-62.0*u144+0.0*u145+62.0*u146) * (pr145*pr145+pi145*pi145) + 650.0*(u144-u145) 
    * (u144-u145) - 1.549429*(pr146) * (pr145+pr147) - 1.549429*(pi146) * 
    (pi145+pi147) + (-62.0*u145+0.0*u146+62.0*u147) * (pr146*pr146+pi146*pi146) + 
    650.0*(u145-u146) * (u145-u146) - 1.549429*(pr147) * (pr146+pr148) - 
    1.549429*(pi147) * (pi146+pi148) + (-62.0*u146+0.0*u147+62.0*u148) * 
    (pr147*pr147+pi147*pi147) + 650.0*(u146-u147) * (u146-u147) - 1.549429*(pr148) 
    * (pr147+pr149) - 1.549429*(pi148) * (pi147+pi149) + 
    (-62.0*u147+0.0*u148+62.0*u149) * (pr148*pr148+pi148*pi148) + 650.0*(u147-u148) 
    * (u147-u148) - 1.549429*(pr149) * (pr148+pr150) - 1.549429*(pi149) * 
    (pi148+pi150) + (-62.0*u148+0.0*u149+62.0*u150) * (pr149*pr149+pi149*pi149) + 
    650.0*(u148-u149) * (u148-u149) - 1.549429*(pr150) * (pr149+pr151) - 
    1.549429*(pi150) * (pi149+pi151) + (-62.0*u149+0.0*u150+62.0*u151) * 
    (pr150*pr150+pi150*pi150) + 650.0*(u149-u150) * (u149-u150) - 1.549429*(pr151) 
    * (pr150+pr152) - 1.549429*(pi151) * (pi150+pi152) + 
    (-62.0*u150+0.0*u151+62.0*u152) * (pr151*pr151+pi151*pi151) + 650.0*(u150-u151) 
    * (u150-u151) - 1.549429*(pr152) * (pr151+pr153) - 1.549429*(pi152) * 
    (pi151+pi153) + (-62.0*u151+0.0*u152+62.0*u153) * (pr152*pr152+pi152*pi152) + 
    650.0*(u151-u152) * (u151-u152) - 1.549429*(pr153) * (pr152+pr154) - 
    1.549429*(pi153) * (pi152+pi154) + (-62.0*u152+0.0*u153+62.0*u154) * 
    (pr153*pr153+pi153*pi153) + 650.0*(u152-u153) * (u152-u153) - 1.549429*(pr154) 
    * (pr153+pr155) - 1.549429*(pi154) * (pi153+pi155) + 
    (-62.0*u153+0.0*u154+62.0*u155) * (pr154*pr154+pi154*pi154) + 650.0*(u153-u154) 
    * (u153-u154) - 1.549429*(pr155) * (pr154+pr156) - 1.549429*(pi155) * 
    (pi154+pi156) + (-62.0*u154+0.0*u155+62.0*u156) * (pr155*pr155+pi155*pi155) + 
    650.0*(u154-u155) * (u154-u155) - 1.549429*(pr156) * (pr155+pr157) - 
    1.549429*(pi156) * (pi155+pi157) + (-62.0*u155+0.0*u156+62.0*u157) * 
    (pr156*pr156+pi156*pi156) + 650.0*(u155-u156) * (u155-u156) - 1.549429*(pr157) 
    * (pr156+pr158) - 1.549429*(pi157) * (pi156+pi158) + 
    (-62.0*u156+0.0*u157+62.0*u158) * (pr157*pr157+pi157*pi157) + 650.0*(u156-u157) 
    * (u156-u157) - 1.549429*(pr158) * (pr157+pr159) - 1.549429*(pi158) * 
    (pi157+pi159) + (-62.0*u157+0.0*u158+62.0*u159) * (pr158*pr158+pi158*pi158) + 
    650.0*(u157-u158) * (u157-u158) - 1.549429*(pr159) * (pr158+pr160) - 
    1.549429*(pi159) * (pi158+pi160) + (-62.0*u158+0.0*u159+62.0*u160) * 
    (pr159*pr159+pi159*pi159) + 650.0*(u158-u159) * (u158-u159) - 1.549429*(pr160) 
    * (pr159+pr161) - 1.549429*(pi160) * (pi159+pi161) + 
    (-62.0*u159+0.0*u160+62.0*u161) * (pr160*pr160+pi160*pi160) + 650.0*(u159-u160) 
    * (u159-u160) - 1.549429*(pr161) * (pr160+pr162) - 1.549429*(pi161) * 
    (pi160+pi162) + (-62.0*u160+0.0*u161+62.0*u162) * (pr161*pr161+pi161*pi161) + 
    650.0*(u160-u161) * (u160-u161) - 1.549429*(pr162) * (pr161+pr163) - 
    1.549429*(pi162) * (pi161+pi163) + (-62.0*u161+0.0*u162+62.0*u163) * 
    (pr162*pr162+pi162*pi162) + 650.0*(u161-u162) * (u161-u162) - 1.549429*(pr163) 
    * (pr162+pr164) - 1.549429*(pi163) * (pi162+pi164) + 
    (-62.0*u162+0.0*u163+62.0*u164) * (pr163*pr163+pi163*pi163) + 650.0*(u162-u163) 
    * (u162-u163) - 1.549429*(pr164) * (pr163+pr165) - 1.549429*(pi164) * 
    (pi163+pi165) + (-62.0*u163+0.0*u164+62.0*u165) * (pr164*pr164+pi164*pi164) + 
    650.0*(u163-u164) * (u163-u164) - 1.549429*(pr165) * (pr164+pr166) - 
    1.549429*(pi165) * (pi164+pi166) + (-62.0*u164+0.0*u165+62.0*u166) * 
    (pr165*pr165+pi165*pi165) + 650.0*(u164-u165) * (u164-u165) - 1.549429*(pr166) 
    * (pr165+pr167) - 1.549429*(pi166) * (pi165+pi167) + 
    (-62.0*u165+0.0*u166+62.0*u167) * (pr166*pr166+pi166*pi166) + 650.0*(u165-u166) 
    * (u165-u166) - 1.549429*(pr167) * (pr166+pr168) - 1.549429*(pi167) * 
    (pi166+pi168) + (-62.0*u166+0.0*u167+62.0*u168) * (pr167*pr167+pi167*pi167) + 
    650.0*(u166-u167) * (u166-u167) - 1.549429*(pr168) * (pr167+pr169) - 
    1.549429*(pi168) * (pi167+pi169) + (-62.0*u167+0.0*u168+62.0*u169) * 
    (pr168*pr168+pi168*pi168) + 650.0*(u167-u168) * (u167-u168) - 1.549429*(pr169) 
    * (pr168+pr170) - 1.549429*(pi169) * (pi168+pi170) + 
    (-62.0*u168+0.0*u169+62.0*u170) * (pr169*pr169+pi169*pi169) + 650.0*(u168-u169) 
    * (u168-u169) - 1.549429*(pr170) * (pr169+pr171) - 1.549429*(pi170) * 
    (pi169+pi171) + (-62.0*u169+0.0*u170+62.0*u171) * (pr170*pr170+pi170*pi170) + 
    650.0*(u169-u170) * (u169-u170) - 1.549429*(pr171) * (pr170+pr172) - 
    1.549429*(pi171) * (pi170+pi172) + (-62.0*u170+0.0*u171+62.0*u172) * 
    (pr171*pr171+pi171*pi171) + 650.0*(u170-u171) * (u170-u171) - 1.549429*(pr172) 
    * (pr171+pr173) - 1.549429*(pi172) * (pi171+pi173) + 
    (-62.0*u171+0.0*u172+62.0*u173) * (pr172*pr172+pi172*pi172) + 650.0*(u171-u172) 
    * (u171-u172) - 1.549429*(pr173) * (pr172+pr174) - 1.549429*(pi173) * 
    (pi172+pi174) + (-62.0*u172+0.0*u173+62.0*u174) * (pr173*pr173+pi173*pi173) + 
    650.0*(u172-u173) * (u172-u173) - 1.549429*(pr174) * (pr173+pr175) - 
    1.549429*(pi174) * (pi173+pi175) + (-62.0*u173+0.0*u174+62.0*u175) * 
    (pr174*pr174+pi174*pi174) + 650.0*(u173-u174) * (u173-u174) - 1.549429*(pr175) 
    * (pr174+pr176) - 1.549429*(pi175) * (pi174+pi176) + 
    (-62.0*u174+0.0*u175+62.0*u176) * (pr175*pr175+pi175*pi175) + 650.0*(u174-u175) 
    * (u174-u175) - 1.549429*(pr176) * (pr175+pr177) - 1.549429*(pi176) * 
    (pi175+pi177) + (-62.0*u175+0.0*u176+62.0*u177) * (pr176*pr176+pi176*pi176) + 
    650.0*(u175-u176) * (u175-u176) - 1.549429*(pr177) * (pr176+pr178) - 
    1.549429*(pi177) * (pi176+pi178) + (-62.0*u176+0.0*u177+62.0*u178) * 
    (pr177*pr177+pi177*pi177) + 650.0*(u176-u177) * (u176-u177) - 1.549429*(pr178) 
    * (pr177+pr179) - 1.549429*(pi178) * (pi177+pi179) + 
    (-62.0*u177+0.0*u178+62.0*u179) * (pr178*pr178+pi178*pi178) + 650.0*(u177-u178) 
    * (u177-u178) - 1.549429*(pr179) * (pr178+pr180) - 1.549429*(pi179) * 
    (pi178+pi180) + (-62.0*u178+0.0*u179+62.0*u180) * (pr179*pr179+pi179*pi179) + 
    650.0*(u178-u179) * (u178-u179) - 1.549429*(pr180) * (pr179+pr181) - 
    1.549429*(pi180) * (pi179+pi181) + (-62.0*u179+0.0*u180+62.0*u181) * 
    (pr180*pr180+pi180*pi180) + 650.0*(u179-u180) * (u179-u180) - 1.549429*(pr181) 
    * (pr180+pr182) - 1.549429*(pi181) * (pi180+pi182) + 
    (-62.0*u180+0.0*u181+62.0*u182) * (pr181*pr181+pi181*pi181) + 650.0*(u180-u181) 
    * (u180-u181) - 1.549429*(pr182) * (pr181+pr183) - 1.549429*(pi182) * 
    (pi181+pi183) + (-62.0*u181+0.0*u182+62.0*u183) * (pr182*pr182+pi182*pi182) + 
    650.0*(u181-u182) * (u181-u182) - 1.549429*(pr183) * (pr182+pr184) - 
    1.549429*(pi183) * (pi182+pi184) + (-62.0*u182+0.0*u183+62.0*u184) * 
    (pr183*pr183+pi183*pi183) + 650.0*(u182-u183) * (u182-u183) - 1.549429*(pr184) 
    * (pr183+pr185) - 1.549429*(pi184) * (pi183+pi185) + 
    (-62.0*u183+0.0*u184+62.0*u185) * (pr184*pr184+pi184*pi184) + 650.0*(u183-u184) 
    * (u183-u184) - 1.549429*(pr185) * (pr184+pr186) - 1.549429*(pi185) * 
    (pi184+pi186) + (-62.0*u184+0.0*u185+62.0*u186) * (pr185*pr185+pi185*pi185) + 
    650.0*(u184-u185) * (u184-u185) - 1.549429*(pr186) * (pr185+pr187) - 
    1.549429*(pi186) * (pi185+pi187) + (-62.0*u185+0.0*u186+62.0*u187) * 
    (pr186*pr186+pi186*pi186) + 650.0*(u185-u186) * (u185-u186) - 1.549429*(pr187) 
    * (pr186+pr188) - 1.549429*(pi187) * (pi186+pi188) + 
    (-62.0*u186+0.0*u187+62.0*u188) * (pr187*pr187+pi187*pi187) + 650.0*(u186-u187) 
    * (u186-u187) - 1.549429*(pr188) * (pr187+pr189) - 1.549429*(pi188) * 
    (pi187+pi189) + (-62.0*u187+0.0*u188+62.0*u189) * (pr188*pr188+pi188*pi188) + 
    650.0*(u187-u188) * (u187-u188) - 1.549429*(pr189) * (pr188+pr190) - 
    1.549429*(pi189) * (pi188+pi190) + (-62.0*u188+0.0*u189+62.0*u190) * 
    (pr189*pr189+pi189*pi189) + 650.0*(u188-u189) * (u188-u189) - 1.549429*(pr190) 
    * (pr189+pr191) - 1.549429*(pi190) * (pi189+pi191) + 
    (-62.0*u189+0.0*u190+62.0*u191) * (pr190*pr190+pi190*pi190) + 650.0*(u189-u190) 
    * (u189-u190) - 1.549429*(pr191) * (pr190+pr192) - 1.549429*(pi191) * 
    (pi190+pi192) + (-62.0*u190+0.0*u191+62.0*u192) * (pr191*pr191+pi191*pi191) + 
    650.0*(u190-u191) * (u190-u191) - 1.549429*(pr192) * (pr191+pr193) - 
    1.549429*(pi192) * (pi191+pi193) + (-62.0*u191+0.0*u192+62.0*u193) * 
    (pr192*pr192+pi192*pi192) + 650.0*(u191-u192) * (u191-u192) - 1.549429*(pr193) 
    * (pr192+pr194) - 1.549429*(pi193) * (pi192+pi194) + 
    (-62.0*u192+0.0*u193+62.0*u194) * (pr193*pr193+pi193*pi193) + 650.0*(u192-u193) 
    * (u192-u193) - 1.549429*(pr194) * (pr193+pr195) - 1.549429*(pi194) * 
    (pi193+pi195) + (-62.0*u193+0.0*u194+62.0*u195) * (pr194*pr194+pi194*pi194) + 
    650.0*(u193-u194) * (u193-u194) - 1.549429*(pr195) * (pr194+pr196) - 
    1.549429*(pi195) * (pi194+pi196) + (-62.0*u194+0.0*u195+62.0*u196) * 
    (pr195*pr195+pi195*pi195) + 650.0*(u194-u195) * (u194-u195) - 1.549429*(pr196) 
    * (pr195+pr197) - 1.549429*(pi196) * (pi195+pi197) + 
    (-62.0*u195+0.0*u196+62.0*u197) * (pr196*pr196+pi196*pi196) + 650.0*(u195-u196) 
    * (u195-u196) - 1.549429*(pr197) * (pr196+pr198) - 1.549429*(pi197) * 
    (pi196+pi198) + (-62.0*u196+0.0*u197+62.0*u198) * (pr197*pr197+pi197*pi197) + 
    650.0*(u196-u197) * (u196-u197) - 1.549429*(pr198) * (pr197+pr199) - 
    1.549429*(pi198) * (pi197+pi199) + (-62.0*u197+0.0*u198+62.0*u199) * 
    (pr198*pr198+pi198*pi198) + 650.0*(u197-u198) * (u197-u198) - 1.549429*(pr199) 
    * (pr198+pr200) - 1.549429*(pi199) * (pi198+pi200) + 
    (-62.0*u198+0.0*u199+62.0*u200) * (pr199*pr199+pi199*pi199) + 650.0*(u198-u199) 
    * (u198-u199) - 1.549429*(pr200) * (pr199+pr1) - 1.549429*(pi200) * (pi199+pi1) 
    + (-62.0*u199+0.0*u200+62.0*u1) * (pr200*pr200+pi200*pi200) + 650.0*(u199-u200) 
    * (u199-u200) )

model.add( pr1 * pr1 + pi1 * pi1 + pr2 * pr2 + pi2 * pi2 + pr3 * pr3 + pi3 * pi3 + pr4 * 
    pr4 + pi4 * pi4 + pr5 * pr5 + pi5 * pi5 + pr6 * pr6 + pi6 * pi6 + pr7 * pr7 + 
    pi7 * pi7 + pr8 * pr8 + pi8 * pi8 + pr9 * pr9 + pi9 * pi9 + pr10 * pr10 + pi10 
    * pi10 + pr11 * pr11 + pi11 * pi11 + pr12 * pr12 + pi12 * pi12 + pr13 * pr13 + 
    pi13 * pi13 + pr14 * pr14 + pi14 * pi14 + pr15 * pr15 + pi15 * pi15 + pr16 * 
    pr16 + pi16 * pi16 + pr17 * pr17 + pi17 * pi17 + pr18 * pr18 + pi18 * pi18 + 
    pr19 * pr19 + pi19 * pi19 + pr20 * pr20 + pi20 * pi20 + pr21 * pr21 + pi21 * 
    pi21 + pr22 * pr22 + pi22 * pi22 + pr23 * pr23 + pi23 * pi23 + pr24 * pr24 + 
    pi24 * pi24 + pr25 * pr25 + pi25 * pi25 + pr26 * pr26 + pi26 * pi26 + pr27 * 
    pr27 + pi27 * pi27 + pr28 * pr28 + pi28 * pi28 + pr29 * pr29 + pi29 * pi29 + 
    pr30 * pr30 + pi30 * pi30 + pr31 * pr31 + pi31 * pi31 + pr32 * pr32 + pi32 * 
    pi32 + pr33 * pr33 + pi33 * pi33 + pr34 * pr34 + pi34 * pi34 + pr35 * pr35 + 
    pi35 * pi35 + pr36 * pr36 + pi36 * pi36 + pr37 * pr37 + pi37 * pi37 + pr38 * 
    pr38 + pi38 * pi38 + pr39 * pr39 + pi39 * pi39 + pr40 * pr40 + pi40 * pi40 + 
    pr41 * pr41 + pi41 * pi41 + pr42 * pr42 + pi42 * pi42 + pr43 * pr43 + pi43 * 
    pi43 + pr44 * pr44 + pi44 * pi44 + pr45 * pr45 + pi45 * pi45 + pr46 * pr46 + 
    pi46 * pi46 + pr47 * pr47 + pi47 * pi47 + pr48 * pr48 + pi48 * pi48 + pr49 * 
    pr49 + pi49 * pi49 + pr50 * pr50 + pi50 * pi50 + pr51 * pr51 + pi51 * pi51 + 
    pr52 * pr52 + pi52 * pi52 + pr53 * pr53 + pi53 * pi53 + pr54 * pr54 + pi54 * 
    pi54 + pr55 * pr55 + pi55 * pi55 + pr56 * pr56 + pi56 * pi56 + pr57 * pr57 + 
    pi57 * pi57 + pr58 * pr58 + pi58 * pi58 + pr59 * pr59 + pi59 * pi59 + pr60 * 
    pr60 + pi60 * pi60 + pr61 * pr61 + pi61 * pi61 + pr62 * pr62 + pi62 * pi62 + 
    pr63 * pr63 + pi63 * pi63 + pr64 * pr64 + pi64 * pi64 + pr65 * pr65 + pi65 * 
    pi65 + pr66 * pr66 + pi66 * pi66 + pr67 * pr67 + pi67 * pi67 + pr68 * pr68 + 
    pi68 * pi68 + pr69 * pr69 + pi69 * pi69 + pr70 * pr70 + pi70 * pi70 + pr71 * 
    pr71 + pi71 * pi71 + pr72 * pr72 + pi72 * pi72 + pr73 * pr73 + pi73 * pi73 + 
    pr74 * pr74 + pi74 * pi74 + pr75 * pr75 + pi75 * pi75 + pr76 * pr76 + pi76 * 
    pi76 + pr77 * pr77 + pi77 * pi77 + pr78 * pr78 + pi78 * pi78 + pr79 * pr79 + 
    pi79 * pi79 + pr80 * pr80 + pi80 * pi80 + pr81 * pr81 + pi81 * pi81 + pr82 * 
    pr82 + pi82 * pi82 + pr83 * pr83 + pi83 * pi83 + pr84 * pr84 + pi84 * pi84 + 
    pr85 * pr85 + pi85 * pi85 + pr86 * pr86 + pi86 * pi86 + pr87 * pr87 + pi87 * 
    pi87 + pr88 * pr88 + pi88 * pi88 + pr89 * pr89 + pi89 * pi89 + pr90 * pr90 + 
    pi90 * pi90 + pr91 * pr91 + pi91 * pi91 + pr92 * pr92 + pi92 * pi92 + pr93 * 
    pr93 + pi93 * pi93 + pr94 * pr94 + pi94 * pi94 + pr95 * pr95 + pi95 * pi95 + 
    pr96 * pr96 + pi96 * pi96 + pr97 * pr97 + pi97 * pi97 + pr98 * pr98 + pi98 * 
    pi98 + pr99 * pr99 + pi99 * pi99 + pr100 * pr100 + pi100 * pi100 + pr101 * 
    pr101 + pi101 * pi101 + pr102 * pr102 + pi102 * pi102 + pr103 * pr103 + pi103 * 
    pi103 + pr104 * pr104 + pi104 * pi104 + pr105 * pr105 + pi105 * pi105 + pr106 * 
    pr106 + pi106 * pi106 + pr107 * pr107 + pi107 * pi107 + pr108 * pr108 + pi108 * 
    pi108 + pr109 * pr109 + pi109 * pi109 + pr110 * pr110 + pi110 * pi110 + pr111 * 
    pr111 + pi111 * pi111 + pr112 * pr112 + pi112 * pi112 + pr113 * pr113 + pi113 * 
    pi113 + pr114 * pr114 + pi114 * pi114 + pr115 * pr115 + pi115 * pi115 + pr116 * 
    pr116 + pi116 * pi116 + pr117 * pr117 + pi117 * pi117 + pr118 * pr118 + pi118 * 
    pi118 + pr119 * pr119 + pi119 * pi119 + pr120 * pr120 + pi120 * pi120 + pr121 * 
    pr121 + pi121 * pi121 + pr122 * pr122 + pi122 * pi122 + pr123 * pr123 + pi123 * 
    pi123 + pr124 * pr124 + pi124 * pi124 + pr125 * pr125 + pi125 * pi125 + pr126 * 
    pr126 + pi126 * pi126 + pr127 * pr127 + pi127 * pi127 + pr128 * pr128 + pi128 * 
    pi128 + pr129 * pr129 + pi129 * pi129 + pr130 * pr130 + pi130 * pi130 + pr131 * 
    pr131 + pi131 * pi131 + pr132 * pr132 + pi132 * pi132 + pr133 * pr133 + pi133 * 
    pi133 + pr134 * pr134 + pi134 * pi134 + pr135 * pr135 + pi135 * pi135 + pr136 * 
    pr136 + pi136 * pi136 + pr137 * pr137 + pi137 * pi137 + pr138 * pr138 + pi138 * 
    pi138 + pr139 * pr139 + pi139 * pi139 + pr140 * pr140 + pi140 * pi140 + pr141 * 
    pr141 + pi141 * pi141 + pr142 * pr142 + pi142 * pi142 + pr143 * pr143 + pi143 * 
    pi143 + pr144 * pr144 + pi144 * pi144 + pr145 * pr145 + pi145 * pi145 + pr146 * 
    pr146 + pi146 * pi146 + pr147 * pr147 + pi147 * pi147 + pr148 * pr148 + pi148 * 
    pi148 + pr149 * pr149 + pi149 * pi149 + pr150 * pr150 + pi150 * pi150 + pr151 * 
    pr151 + pi151 * pi151 + pr152 * pr152 + pi152 * pi152 + pr153 * pr153 + pi153 * 
    pi153 + pr154 * pr154 + pi154 * pi154 + pr155 * pr155 + pi155 * pi155 + pr156 * 
    pr156 + pi156 * pi156 + pr157 * pr157 + pi157 * pi157 + pr158 * pr158 + pi158 * 
    pi158 + pr159 * pr159 + pi159 * pi159 + pr160 * pr160 + pi160 * pi160 + pr161 * 
    pr161 + pi161 * pi161 + pr162 * pr162 + pi162 * pi162 + pr163 * pr163 + pi163 * 
    pi163 + pr164 * pr164 + pi164 * pi164 + pr165 * pr165 + pi165 * pi165 + pr166 * 
    pr166 + pi166 * pi166 + pr167 * pr167 + pi167 * pi167 + pr168 * pr168 + pi168 * 
    pi168 + pr169 * pr169 + pi169 * pi169 + pr170 * pr170 + pi170 * pi170 + pr171 * 
    pr171 + pi171 * pi171 + pr172 * pr172 + pi172 * pi172 + pr173 * pr173 + pi173 * 
    pi173 + pr174 * pr174 + pi174 * pi174 + pr175 * pr175 + pi175 * pi175 + pr176 * 
    pr176 + pi176 * pi176 + pr177 * pr177 + pi177 * pi177 + pr178 * pr178 + pi178 * 
    pi178 + pr179 * pr179 + pi179 * pi179 + pr180 * pr180 + pi180 * pi180 + pr181 * 
    pr181 + pi181 * pi181 + pr182 * pr182 + pi182 * pi182 + pr183 * pr183 + pi183 * 
    pi183 + pr184 * pr184 + pi184 * pi184 + pr185 * pr185 + pi185 * pi185 + pr186 * 
    pr186 + pi186 * pi186 + pr187 * pr187 + pi187 * pi187 + pr188 * pr188 + pi188 * 
    pi188 + pr189 * pr189 + pi189 * pi189 + pr190 * pr190 + pi190 * pi190 + pr191 * 
    pr191 + pi191 * pi191 + pr192 * pr192 + pi192 * pi192 + pr193 * pr193 + pi193 * 
    pi193 + pr194 * pr194 + pi194 * pi194 + pr195 * pr195 + pi195 * pi195 + pr196 * 
    pr196 + pi196 * pi196 + pr197 * pr197 + pi197 * pi197 + pr198 * pr198 + pi198 * 
    pi198 + pr199 * pr199 + pi199 * pi199 + pr200 * pr200 + pi200 * pi200 - 1.0 == 0 )
