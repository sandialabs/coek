#
# Source:  problem 21 in
# J.J. More', B.S. Garbow and K.E. Hillstrom,
# "Testing Unconstrained Optimization Software",
# ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.
#
# TODO - Double-check example
#

import poek as pk


def srosenbr(N):

    model = pk.model()

    x = model.add_variable(N)
    for i in range(N):
        if i%2 == 0:
            x[i].value = -1.2
        else:
            x[i].value = 1

    model.add_objective( sum(100*(x[2*i-1] - x[2*i-2]**2)**2 + (x[2*i-2] - 1)**2 for i in range(1,N//2)) )

    return model

