from pyomo.environ import *

import random
random.seed(1000)

N = 640
M = 640
P = 1
#N = 7
#M = 5
#P = 1

model = ConcreteModel()

model.x = Var(range(N), range(M), bounds=(0,1), initialize=0)
#x = {}
#for n in range(N):
#    for m in range(M):
#        x[n,m] = variable(lb=0, ub=1, initialize=0)

model.y = Var(range(N), bounds=(0,1), initialize=0)
#y = {}
#for n in range(N):
#    y[n] = variable(lb=0, ub=1, initialize=0)

model.d = Param(range(N), range(M), initialize=lambda  n, m, model : random.uniform(1.0,2.0))
#d = {}
#for n in range(N):
#    for m in range(M):
#        d[n,m] = random.uniform(0.0,1.0)

#pmedian = model()

# objective
#pmedian.add( quicksum(d[n,m]*x[n,m] for n in range(N) for m in range(M)) )

# single_x
#for m in range(M):
#    pmedian.add( quicksum(x[n,m] for n in range(N)) == 1 )

# bound_y
#for n in range(N):
#    for m in range(M):
#        pmedian.add( x[n,m] - y[n] <= 0 )

# num_facilities
#pmedian.add( quicksum(y[n] for n in range(N)) == P )


def rule(model):
    return quicksum(model.d[n,m]*model.x[n,m] for n in range(N) for m in range(M))
model.obj = Objective(rule=rule)

def rule(model, m):
    return quicksum(model.x[n,m] for n in range(N)) ==  1.0
model.single_x = Constraint(range(M), rule=rule)

def rule(model, n,m):
    return model.x[n,m] - model.y[n] <= 0.0
model.bound_y = Constraint(range(N), range(M), rule=rule)

def rule(model):
    return quicksum(model.y[n] for n in range(N)) == P
model.num_facilities = Constraint(rule=rule)



#pmedian.show()
#pmedian.build()

pmedian = model
