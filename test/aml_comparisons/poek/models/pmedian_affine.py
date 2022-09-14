import sys
import itertools
import poek as pk


def pmedian_affine(N, P):

    model = pk.model()

    # N - Locations
    M = N  # Customers
    # P - Facilities

    d = {}
    for n in range(N):
        for m in range(M):
            d[n, m] = 1.0 + 1.0 / (n + m + 1)

    x = model.add_variable((N, M), lb=0.0, ub=1.0, initial=0.0)
    y = model.add_variable(N, lb=0.0, ub=1.0, initial=0.0)

    # obj
    model.add_objective(
        pk.affine_expression(
            [d[n, m] for n in range(N) for m in range(M)],
            [x[n, m] for n in range(N) for m in range(M)],
        )
    )

    # single_x
    for m in range(M):
        model.add_constraint(pk.affine_expression([x[n, m] for n in range(N)]) == 1)

    # bound_y
    for n, m in itertools.product(range(N), range(M)):
        model.add_constraint(pk.affine_expression((1, -1), (x[n, m], y[n])) <= 0)

    # num_facilities
    model.add_constraint(pk.affine_expression([y[n] for n in range(N)]) == P)

    return model
