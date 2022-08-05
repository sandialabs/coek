import sys
import itertools
import poek as pk

quicksum = pk.quicksum


def nqueens(N):
    model = pk.model()

    x = model.add_variable((N,N), lb=0.0, ub=1.0, binary=True)

    # obj
    model.add_objective( quicksum(x[i,j] for i in range(N) for j in range(N)) )

    # one per row
    for i in range(N):
        model.add_constraint( quicksum(x[i,j] for j in range(N)) == 1 )

    # one per column
    for j in range(N):
        model.add_constraint( quicksum(x[i,j] for i in range(N)) == 1 )

    # \diagonals_col
    for i in range(N-1):
        model.add_constraint( x[0, i] + quicksum(x[j, i+j] for j in range(1, N-i)) <= 1 )
    # \diagonals_row
    for i in range(1, N-1):
        model.add_constraint( x[i, 0] + quicksum(x[i+j, j] for j in range(1, N-i)) <= 1 )

    # /diagonals_col
    for i in range(1,N):
        model.add_constraint( x[0, i] + quicksum(x[j, i-j] for j in range(1, i+1)) <= 1 )
    # /diagonals_row
    for i in range(1,N-1):
        model.add_constraint( x[i, N-1] + quicksum(x[i+j, N-1-j] for j in range(1, N-i)) <= 1)

    return model

