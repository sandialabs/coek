import time
import timeit
import pyomo.environ as pe
import poek.expr as PE

N = 1000000

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

    elif i == 3:
        e = sum(i*model.x[i] for i in model.x)

from _expr import lib

def quicksum(args):
    # A hack.  This assumes that each term is an expression or variable or parameter
    try:
        first = next(args, None)
    except:
        try:
            args = args.__iter__()
            first = next(args, None)
        except:
            raise RuntimeError("The argument to quicksum() is not iterable!")
    if first is None:
        return 0

    const = []
    try:
        e = first.ptr
        for arg in args:
            e = lib.add_expr_expression(e, arg.ptr)
    except AttributeError:
        e = first
    return PE.expression(e)


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

    elif i == 200:
        e = lib.mul_expr_int(v[0].ptr, 0)
        for i in range(1,N):
            e = lib.add_expr_expression(e, lib.mul_expr_int(v[i].ptr,i))
        e = PE.expression(e)

    elif i == 3:
        e = sum(i*v[i] for i in range(N))

#for i in [0,1,2,20,3]:
#    print((i,timeit.timeit('linear_pe(%d)' % i, "from __main__ import linear_pe", number=1)))
#for i in [0,1,2,3]:
#    print((i,timeit.timeit('linear_PE(%d)' % i, "from __main__ import linear_PE", number=1)))


import cProfile
import pstats

cProfile.run('linear_pe(2)','pe.stats')
pe_stats = pstats.Stats('pe.stats')
pe_stats.sort_stats('cumulative')
pe_stats.print_stats()

cProfile.run('linear_PE(2)','PE.stats')
PE_stats = pstats.Stats('PE.stats')
PE_stats.sort_stats('cumulative')
PE_stats.print_stats()

cProfile.run('linear_PE(200)','PE.stats')
PE_stats = pstats.Stats('PE.stats')
PE_stats.sort_stats('cumulative')
PE_stats.print_stats()


