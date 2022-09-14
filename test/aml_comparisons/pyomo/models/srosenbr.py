#
# Source:  problem 21 in
# J.J. More', B.S. Garbow and K.E. Hillstrom,
# "Testing Unconstrained Optimization Software",
# ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.
#

import pyomo.environ as pe


def srosenbr(N):

    model = pe.ConcreteModel()

    model.A = pe.RangeSet(1, N)

    def init_fn(model, i):
        if i % 2 == 1:
            return -1.2
        else:
            return 1

    model.x = pe.Var(model.A, initialize=init_fn)

    model.o = pe.Objective(
        expr=sum(
            100 * (model.x[2 * i] - model.x[2 * i - 1] ** 2) ** 2
            + (model.x[2 * i - 1] - 1) ** 2
            for i in range(1, N // 2)
        )
    )

    return model
