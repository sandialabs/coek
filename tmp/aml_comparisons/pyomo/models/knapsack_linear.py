import sys
import random
import pyomo.environ as pe
from pyomo.core.expr.numeric_expr import LinearExpression


def knapsack_linear(N):
    random.seed(1000)

    N = N*1000
    W = N/10.0


    model = pe.ConcreteModel()

    model.INDEX = pe.RangeSet(1,N)

    model.w = pe.Param(model.INDEX, initialize=lambda model, i : random.uniform(0.0,1.0), within=pe.Reals)

    model.v = pe.Param(model.INDEX, initialize=lambda model, i : random.uniform(0.0,1.0), within=pe.Reals)

    model.x = pe.Var(model.INDEX, bounds=(0.0,1.0))

    model.o = pe.Objective(expr=LinearExpression(linear_coefs=[model.v[i] for i in model.INDEX], 
                                                    linear_vars= [model.x[i] for i in model.INDEX]))

    model.c = pe.Constraint(expr=LinearExpression(linear_coefs=[model.w[i] for i in model.INDEX],
                                                     linear_vars= [model.x[i] for i in model.INDEX]) <= W)

    return model

