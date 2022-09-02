import sys
import itertools
import poek as pk

model = pk.compact_model()

n = int(sys.argv[1])
m = n
dx = 1.0/n
T = 1.58
dt = T/n
h2 = dx**2
a = 0.001

M = pk.RangeSet(0,m)
N = pk.RangeSet(0,n)
y = pk.variable(M*N, 0, 1, 0, "y")
u = pk.variable(M, -1, 1, 0, "u")
model.use(y)
model.use(u)

i = pk.index("i")
j = pk.index("j")

def yt(j, dx):
    return 0.5*(1 - (j*dx)*(j*dx))

#obj
model.add(0.25*dx*(
        (y[m, 0] - yt(0, dx))**2 +
        2*pk.Sum(((y[m, j] - yt(j, dx))**2) .Forall(j).In(pk.RangeSet(1, n-1))) + 
        (y[m, n] - yt(n, dx))**2
    ) + 0.25*a*dt*(
        2 * pk.Sum( (u[i]**2) .Forall(i).In(pk.RangeSet(1,m-1))) +
        u[m]**2
    ))

#pde
model.add( ((y[i+1, j] - y[i, j])/dt == 0.5*(y[i, j-1] - 2*y[i, j] + y[i, j+1] + y[i+1, j-1] - 2*y[i+1, j] + y[i+1, j+1])/h2)
        .Forall(i,j).In(pk.RangeSet(0,m-1) * pk.RangeSet(1,n-1)) )

#ic
model.add( (y[0, i] == 0)
        .Forall(i).In(M) )

#bc1
model.add( (y[i, 2] - 4*y[i, 1] + 3*y[i, 0] == 0)
        .Forall(i).In(pk.RangeSet(1,m)) )

#bc2
model.add( (y[i, n-2] - 4*y[i, n-1] + 3*y[i, n-0] == (2*dx)*(u[i] - y[i, n-0]))
        .Forall(i).In(pk.RangeSet(1,m)) )


model.write("lqcp2.lp")
