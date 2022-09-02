import sys
import itertools
import poek as pk
quicksum = pk.quicksum


def lqcp(n):

    m = n
    dx = 1.0/n
    T = 1.58
    dt = T/n
    h2 = dx**2
    a = 0.001

    model = pk.model()

    y = model.add_variable((n+1,m+1), lb=0.0, ub=1.0)
    u = model.add_variable(n+1, lb=-1.0, ub=1.0)

    def yt(j, dx):
        return 0.5*(1 - (j*dx)*(j*dx))

    #obj
    model.add_objective(0.25*dx*(
            (y[m, 0] - yt(0, dx))**2 +
            2*quicksum((y[m, j] - yt(j, dx))**2 for j in range(1, n)) +
            (y[m, n] - yt(n, dx))**2
        ) + 0.25*a*dt*(
            2 * quicksum(u[i]**2 for i in range(1, m)) +
            u[m]**2
        ))

    #pde
    pde_coef = T*0.5*n      # == dt*0.5/h2
    for i, j in itertools.product(range(n), range(1, m)):
        model.add_constraint(y[i+1, j] - y[i, j] == pde_coef*(y[i, j-1] - 2*y[i, j] + y[i, j+1] + y[i+1, j-1] - 2*y[i+1, j] + y[i+1, j+1]))

    #ic
    for j in range(m+1):
        model.add_constraint(y[0, j] == 0)

    #bc1
    for i in range(1, n+1):
        model.add_constraint(y[i, 2] - 4*y[i, 1] + 3*y[i, 0] == 0)

    #bc2
    for i in range(1, n+1):
        model.add_constraint(y[i, n-2] - 4*y[i, n-1] + 3*y[i, n-0] == (2*dx)*(u[i] - y[i, n-0]))

    return model
