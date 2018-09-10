import time
import timeit
import pyomo.environ as pe
import poek.expr as PE

N = 100000

start = time.time()
model = pe.ConcreteModel()
model.x = pe.Var(pe.RangeSet(N), initialize=2)
print("Time to create Pyomo: "+str(time.time() - start))

start = time.time()
v = []
for i in range(N):
    v.append(PE.variable())
print("Time to create Poek: "+str(time.time() - start))


def linear_pe(i):
    if i == 0:
        e = 0
        for i in model.x:
            e += model.x[i]

    elif i == 1:
        e = 0
        for i in model.x:
            e += i*model.x[i]

    elif i == 2:
        e = 0
        for i in model.x:
            e += model.x[i]*i

    elif i == 20:
        e = 0
        x = model.x
        for i in x:
            e += x[i]*i

    elif i == 3:
        e = sum(i*model.x[i] for i in model.x)

def linear_PE(i):
    if i == 0:
        e = 0
        for i in range(N):
            e += v[i]

    elif i == 1:
        e = 0
        for i in range(N):
            e += i*v[i]

    elif i == 2:
        e = 0
        for i in range(N):
            e += v[i]*i

    elif i == 3:
        e = sum(i*v[i] for i in range(N))

for i in [0,1,2,20,3]:
    print((i,timeit.timeit('linear_pe(%d)' % i, "from __main__ import linear_pe", number=1)))
for i in [0,1,2,3]:
    print((i,timeit.timeit('linear_PE(%d)' % i, "from __main__ import linear_PE", number=1)))

