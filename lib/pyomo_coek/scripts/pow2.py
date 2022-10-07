import pyomo.environ as pe
from pyomo.common.timing import TicTocTimer
from pyomo.core.expr.numeric_expr import LinearExpression


Nv = 10
m = pe.ConcreteModel()
m.x = pe.Var()
m.y = pe.Var(list(range(10)))

timer = TicTocTimer()
N = 1000000

timer.tic('start')
for i in range(N):
    e = 2*m.x
timer.toc('done 2*x')

for i in range(N):
    e = m.x**2
timer.toc('done x**2')

for i in range(N):
    e = m.x + 2
timer.toc('done x+2')

for i in range(int(N/5)):
    e = sum(m.y.values())
timer.toc('done sum(m.y.values())')

for i in range(int(N/5)):
    e = pe.quicksum(m.y.values())
timer.toc('done quicksum(m.y.values())')

for i in range(int(N/5)):
    e = sum((i+1)*m.y[i] for i in range(Nv))
timer.toc('done sum((i+1)*m.y[i])')

for i in range(int(N/5)):
    e = LinearExpression(
        linear_coefs=[i+1 for i in range(Nv)],
        linear_vars=[m.y[i] for i in range(Nv)]
    )
timer.toc('done construct_linear_expression')
