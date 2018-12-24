import time
import timeit
import pyomo.environ as pe
import poek.expr as PE
import poek.utilities
import sys

N = 1000000
#N = 5

start = time.time()
model = pe.ConcreteModel()
model.x = pe.Var(pe.RangeSet(0,N-1), initialize=2)
print("Time to create Pyomo variables: "+str(time.time() - start))

if '-v' in sys.argv:
    start = time.time()
    v = PE.variable(N)
    print("Time to create Poek variable vector: "+str(time.time() - start))
else:
    start = time.time()
    v = []
    for i in range(N):
        v.append(PE.variable())
    print("Time to create Poek variable objects: "+str(time.time() - start))
    

def linear_pe(i):
    if i == 0:
        e = 0
        for i in model.x:
            e += model.x[i]
        #print(e)

    elif i == 1:
        e = 0
        for i in model.x:
            e += i*model.x[i]
        #print(e)

    elif i == 2:
        e = 0
        for i in model.x:
            e += model.x[i]*i
        #print(e)

    elif i == 3:
        e = sum(i*model.x[i] for i in model.x)
        #print(e)

    elif i == 300:
        e = pe.quicksum(i*model.x[i] for i in model.x)
        #print(e)

    elif i == 1000:
        e = sum(model.x[i] for i in model.x)
        e += sum(model.x[i] for i in model.x)
        #print(e)

from poek.coek_cffi import lib


def linear_PE(i):
    if i == 0:
        e = 0
        for i in range(N):
            e += v[i]
        #e.show()

    elif i == 1:
        e = 0
        for i in range(N):
            e += i*v[i]
        #e.show()

    elif i == 2:
        e = 0
        for i in range(N):
            e += v[i]*i
        #e.show()

    elif i == 200:
        e = lib.expr_times_int(v[0].ptr, 0)
        for i in range(1,N):
            e = lib.expr_plus_expression(e, lib.expr_times_int(v[i].ptr,i))
        e = PE.expression(e)
        #e.show()

    elif i == 3:
        e = sum(i*v[i] for i in range(N))
        #e.show()

    elif i == 300:
        e = poek.utilities.quicksum(i*v[i] for i in range(N))
        #e.show()

    elif i == 1000:
        e = sum(v[i] for i in range(N))
        e += sum(v[i] for i in range(N))
        #print(e)

for i in [0,1,2,3,300,1000]:
    print((i,timeit.timeit('linear_pe(%d)' % i, "from __main__ import linear_pe", number=1)))
for i in [0,1,2,200,3,300,1000]:
    print((i,timeit.timeit('linear_PE(%d)' % i, "from __main__ import linear_PE", number=1)))

sys.exit(0)

import cProfile
import pstats

#cProfile.run('linear_pe(2)','pe.stats')
#pe_stats = pstats.Stats('pe.stats')
#pe_stats.sort_stats('cumulative')
#pe_stats.print_stats()

print("linear_PE(0)")
cProfile.run('linear_PE(0)','PE.stats')
PE_stats = pstats.Stats('PE.stats')
PE_stats.sort_stats('cumulative')
PE_stats.print_stats()

print("linear_PE(3)")
cProfile.run('linear_PE(3)','PE.stats')
PE_stats = pstats.Stats('PE.stats')
PE_stats.sort_stats('cumulative')
PE_stats.print_stats()

print("linear_PE(1000)")
cProfile.run('linear_PE(1000)','PE.stats')
PE_stats = pstats.Stats('PE.stats')
PE_stats.sort_stats('cumulative')
PE_stats.print_stats()


