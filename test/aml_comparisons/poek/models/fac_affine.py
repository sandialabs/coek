import sys
import itertools
import poek as pk


def fac_affine(F):

    G = F
    G_ = G + 1

    model = pk.model()

    d = model.add_variable()
    y = model.add_variable((F, 2), lb=0.0, ub=1.0)
    z = model.add_variable((G_, G_, F), binary=True)
    s = model.add_variable((G_, G_, F), lb=0.0)
    r = model.add_variable((G_, G_, F, 2))

    # obj
    model.add_objective(d)

    # assmt
    for i, j in itertools.product(range(G_), range(G_)):
        model.add_constraint(pk.affine_expression([z[i, j, f] for f in range(F)]) == 1)

    M = 2 * 1.414

    # quadrhs
    for i, j, f in itertools.product(range(G_), range(G_), range(F)):
        model.add_constraint(s[i, j, f] == d + M * (1 - z[i, j, f]))

    # quaddistk1
    for i, j, f in itertools.product(range(G_), range(G_), range(F)):
        model.add_constraint(r[i, j, f, 0] == i / G - y[f, 0])

    # quaddistk2
    for i, j, f in itertools.product(range(G_), range(G_), range(F)):
        model.add_constraint(r[i, j, f, 1] == j / G - y[f, 1])

    # quaddist
    for i, j, f in itertools.product(range(G_), range(G_), range(F)):
        model.add_constraint(r[i, j, f, 0] ** 2 + r[i, j, f, 1] ** 2 <= s[i, j, f] ** 2)

    return model
