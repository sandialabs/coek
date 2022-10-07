import sys
import random
import itertools
import poek as pk

random.seed(1000)


def baseline_test(N):
    model = pk.model()

    c = [random.uniform(0.0, 1.0) for i in range(N)]
    A = {}
    for n in range(N):
        for m in range(N):
            A[n, m] = random.uniform(0.0, 1.0)
    b = [random.uniform(0.0, 1.0) for i in range(N)]

    x = model.add_variable(N, lb=0.0, initial=0.0)

    # obj
    model.add_objective(pk.quicksum(c[m] * x[m] for m in range(N)), False)

    # Ax <= b
    for n in range(N):
        model.add_constraint(pk.quicksum(A[n, m] * x[m] for m in range(N)) <= b[n])

    opt = pk.solver("gurobi")
    if opt.available:
        opt.solve(model)
    print("Solve: 0  Objective: " + str(model.get_objective().value))


def obj_test(N, nsolves):
    model = pk.model()

    c = [pk.parameter(random.uniform(0.0, 1.0)) for i in range(N)]
    A = {}
    for n in range(N):
        for m in range(N):
            A[n, m] = random.uniform(0.0, 1.0)
    b = [random.uniform(0.0, 1.0) for i in range(N)]

    x = model.add_variable(N, lb=0.0, initial=0.0)

    # obj
    model.add_objective(pk.quicksum(c[m] * x[m] for m in range(N)), False)

    # Ax <= b
    for n in range(N):
        model.add_constraint(pk.quicksum(A[n, m] * x[m] for m in range(N)) <= b[n])

    opt = pk.solver("gurobi")
    if opt.available:
        opt.load(model)

        for i in range(nsolves):
            opt.resolve()
            print(
                "Solve: " + str(i) + "  Objective: " + str(model.get_objective().value),
                flush=True,
            )
            for i in range(N):
                c[i].value = random.uniform(0.0, 1.0)
            print("")


def row_test(N, nsolves):
    model = pk.model()

    k = min(10, N - 1)

    c = [random.uniform(0.0, 1.0) for i in range(N)]
    A_ = [pk.parameter(random.uniform(0.0, 1.0)) for i in range(N)]
    A = {}
    for n in range(N):
        for m in range(N):
            A[n, m] = random.uniform(0.0, 1.0)
    b = [random.uniform(0.0, 1.0) for i in range(N)]

    x = model.add_variable(N, lb=0.0, initial=0.0)

    # obj
    model.add_objective(pk.quicksum(c[m] * x[m] for m in range(N)), False)

    # Ax <= b
    for n in range(N):
        if n != k:
            model.add_constraint(pk.quicksum(A[n, m] * x[m] for m in range(N)) <= b[n])
        else:
            model.add_constraint(pk.quicksum(A_[m] * x[m] for m in range(N)) <= b[n])

    opt = pk.solver("gurobi")
    if opt.available:
        opt.load(model)

        for i in range(nsolves):
            opt.resolve()
            print(
                "Solve: " + str(i) + "  Objective: " + str(model.get_objective().value),
                flush=True,
            )
            for j in range(N):
                A_[j].value = random.uniform(0.0, 1.0)
            print("")


def col_test(N, nsolves):
    model = pk.model()

    k = min(10, N - 1)

    c = [random.uniform(0.0, 1.0) for i in range(N)]
    A_ = [pk.parameter(random.uniform(0.0, 1.0)) for i in range(N)]
    A = {}
    for n in range(N):
        for m in range(N):
            A[n, m] = random.uniform(0.0, 1.0)
    b = [random.uniform(0.0, 1.0) for i in range(N)]

    x = model.add_variable(N, lb=0.0, initial=0.0)

    # obj
    model.add_objective(pk.quicksum(c[m] * x[m] for m in range(N)), False)

    # Ax <= b
    for n in range(N):
        model.add_constraint(
            pk.quicksum(A[n, m] * x[m] if m != k else A_[n] * x[m] for m in range(N)) <= b[n]
        )

    opt = pk.solver("gurobi")
    opt.load(model)

    for i in range(nsolves):
        opt.resolve()
        # for j in range(N):
        #    print(("CON",j, model.get_constraint(j).to_list()))
        # for j in range(N):
        #    print(("X",j,x[j].value))
        print(
            "Solve: " + str(i) + "  Objective: " + str(model.get_objective().value),
            flush=True,
        )
        for j in range(N):
            A_[j].value = random.uniform(0.0, 1.0)

        print("")


def rhs_test(N, nsolves):
    model = pk.model()

    c = [random.uniform(0.0, 1.0) for i in range(N)]
    A = {}
    for n in range(N):
        for m in range(N):
            A[n, m] = random.uniform(0.0, 1.0)
    b = [pk.parameter(random.uniform(0.0, 1.0)) for i in range(N)]

    x = model.add_variable(N, lb=0.0, initial=0.0)

    # obj
    model.add_objective(pk.quicksum(c[m] * x[m] for m in range(N)), False)

    # Ax <= b
    for n in range(N):
        model.add_constraint(pk.quicksum(A[n, m] * x[m] for m in range(N)) <= b[n])

    opt = pk.solver("gurobi")
    if opt.available:
        opt.load(model)

        for i in range(nsolves):
            opt.resolve()
            print(
                "Solve: " + str(i) + "  Objective: " + str(model.get_objective().value),
                flush=True,
            )
            for i in range(N):
                b[i].value = random.uniform(0.0, 1.0)
            print("")


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("resolves.py <N> <n-solves> <test>")
        print("    Test: baseline, obj, row, col, rhs")
        sys.exit(0)
    N = int(sys.argv[1])
    nsolves = int(sys.argv[2])
    test = sys.argv[3]
    if test == "baseline":
        baseline_test(N)
    elif test == "obj":
        obj_test(N, nsolves)
    elif test == "row":
        row_test(N, nsolves)
    elif test == "col":
        col_test(N, nsolves)
    elif test == "rhs":
        rhs_test(N, nsolves)
