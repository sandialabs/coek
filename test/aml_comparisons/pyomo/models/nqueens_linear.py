import sys
import pyomo.environ as pe
from pyomo.core.expr.numeric_expr import LinearExpression


def nqueens_linear(N):
    onesN = [1] * N

    model = pe.ConcreteModel()

    model.Rows = pe.RangeSet(0, N - 1)
    model.Cols = pe.RangeSet(0, N - 1)

    model.x = pe.Var(model.Rows, model.Cols, within=pe.Boolean)

    # obj
    model.obj = pe.Objective(
        expr=LinearExpression(
            linear_coefs=[1] * (N * N),
            linear_vars=[model.x[i, j] for i in model.Rows for j in model.Cols],
        )
    )

    # one per row
    def row_rule(model, i):
        return (
            LinearExpression(linear_coefs=onesN, linear_vars=[model.x[i, j] for j in range(N)]) == 1
        )

    model.row_rule = pe.Constraint(model.Rows, rule=row_rule)

    def col_rule(model, j):
        return (
            LinearExpression(linear_coefs=onesN, linear_vars=[model.x[i, j] for i in range(N)]) == 1
        )

    model.col_rule = pe.Constraint(model.Cols, rule=col_rule)

    # \diagonals_col
    def ldiag_cols_rule(model, i):
        return (
            model.x[0, i]
            + LinearExpression(
                linear_coefs=[1] * (N - i - 1),
                linear_vars=[model.x[j, i + j] for j in range(1, N - i)],
            )
            <= 1
        )

    model.ldiag_cols = pe.Constraint(range(N - 1), rule=ldiag_cols_rule)

    # \diagonals_row
    def ldiag_rows_rule(model, i):
        return (
            model.x[i, 0]
            + LinearExpression(
                linear_coefs=[1] * (N - i - 1),
                linear_vars=[model.x[i + j, j] for j in range(1, N - i)],
            )
            <= 1
        )

    model.ldiag_rows = pe.Constraint(range(1, N - 1), rule=ldiag_rows_rule)

    # /diagonals_col
    def rdiag_cols_rule(model, i):
        return (
            model.x[0, i]
            + LinearExpression(
                linear_coefs=[1] * i,
                linear_vars=[model.x[j, i - j] for j in range(1, i + 1)],
            )
            <= 1
        )

    model.rdiag_cols = pe.Constraint(range(1, N), rule=rdiag_cols_rule)

    # /diagonals_row
    def rdiag_rows_rule(model, i):
        return (
            model.x[i, N - 1]
            + LinearExpression(
                linear_coefs=[1] * (N - i - 1),
                linear_vars=[model.x[i + j, N - 1 - j] for j in range(1, N - i)],
            )
            <= 1
        )

    model.rdiag_rows = pe.Constraint(range(1, N - 1), rule=rdiag_rows_rule)

    return model
