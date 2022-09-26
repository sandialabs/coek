import poek as pk
from pyomo.common.timing import TicTocTimer


Nv = 10
m = pk.model()
x = m.add_variable('x')
y = m.add_variable(Nv)

timer = TicTocTimer()
N = 1000000

timer.tic('start')
for i in range(N):
    e = 2*x
timer.toc('done 2*x')

for i in range(N):
    e = x*2
timer.toc('done x*2')

for i in range(N):
    e = x**2
timer.toc('done x**2')

for i in range(int(N/5)):
    e = sum(y[i] for i in range(Nv))
timer.toc('done sum(y_i)')
