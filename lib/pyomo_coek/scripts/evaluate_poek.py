import pyomo.environ as pe
import pyomo_coek.components_only as pc
from pyomo.common.timing import TicTocTimer
import poek as pk
from pyomo.core.expr.numeric_expr import LinearExpression
from pyomo.repn.standard_repn import generate_standard_repn
from pyomo.core.expr.visitor import identify_variables


def create_indexed_var():
    m = pe.ConcreteModel()
    m.x = pc.Var([1, 2, 3])
    e = 2 * m.x[1] + m.x[2] - m.x[3]
    print(e.to_list())


def main():
    m = pe.ConcreteModel()
    # pm = pk.model()

    n_vars = 10

    m.x = pe.Var(list(range(n_vars)), initialize=2)
    m.cx = pc.Var(list(range(n_vars)), initialize=2)
    # x = pm.add_variable(n_vars)

    N = 50000

    timer = TicTocTimer()
    timer.tic("start")

    for ndx in range(N):
        # e = LinearExpression(linear_coefs=[i for i in range(n_vars)],
        #                      linear_vars=[m.x[i] for i in range(n_vars)])
        e = sum(i * m.x[i] for i in range(n_vars))
    timer.toc("constructed pyomo expressions")

    # pr = enable_c_profiler()
    for ndx in range(N):
        ce = sum(i * m.cx[i] for i in range(n_vars))
    timer.toc("constructed pyomo-poek expressions")
    # disable_c_profiler(pr)

    # for ndx in range(N):
    #     pk_e = pk.construct_linear_expression([i for i in range(n_vars)], [x[i] for i in range(n_vars)])
    # timer.toc('construced raw poek expressions')

    print(pe.value(e))
    print(pe.value(ce))
    timer.toc("printed values")

    for ndx in range(N):
        e_val = e()
    timer.toc("done pyomo eval")

    for ndx in range(N):
        e_val = ce(False)
    timer.toc("done pyomo-poek eval")

    for ndx in range(N):
        repn = generate_standard_repn(e)
    timer.toc("pyomo generate_standard_repn")

    for ndx in range(N):
        repn = pk.generate_standard_repn(ce)
    timer.toc("pyomo-poek generate_standard_repn")


if __name__ == "__main__":
    main()
    # create_indexed_var()
