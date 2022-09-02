# TODO
# Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,11))
M = list(range(1,6))
L = list(range(1,16))

data = pk.util.load_data("hs117.json")
a, b, c, d, e = data.unpack("a", "b", "c", "d", "e")

x = model.add_variable(index=L, lb=0)
x[1].value =   .001
x[2].value =   .001
x[3].value =   .001
x[4].value =   .001
x[5].value =   .001
x[6].value =   .001
x[7].value = 60.0
x[8].value =   .001
x[9].value =   .001
x[10].value=   .001
x[11].value=   .001
x[12].value=   .001
x[13].value=   .001
x[14].value=   .001
x[15].value=   .001

model.add_objective( sum(-b[j]*x[j] for j in N) + sum(c[k,j]*x[10+k]*x[10+j] for j in M for k in M) + sum(2*d[j]*x[10+j]**3 for j in M) )

for j in M:
    model.add_constraint( sum(2*c[k,j]*x[10+k] for k in M) + 3*d[j]*x[10+j]**2+e[j] - sum(a[k,j]*x[k] for k in N) >= 0 )
