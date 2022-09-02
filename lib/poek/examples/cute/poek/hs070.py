# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk


model = pk.model()

N = list(range(1,5))
M = list(range(1,20))

data = pk.util.load_data('hs070.json')
c = data.unpack("c", index=("i"))           # c(M)
y_obs = data.unpack("y_obs", index=("i"))   # y_obs(M)

x = model.add_variable(index=N, lb=0.00001, ub=100)
x[3].ub = 1.0
x[1].value = 2.0
x[2].value = 4.0
x[3].value = 0.04
x[4].value = 2.0

b = x[3] + (1-x[3])*x[4]

y_cal = {}
for i in M:
    y_cal[i] = (1 + 1.0/(12*x[2])) \
           * \
           ( \
               x[3]*b**x[2]*(x[2]/6.2832)**0.5 * (c[i]/7.685)**(x[2]-1) \
               * pk.exp(x[2] - b*c[i]*x[2]/7.658) \
           ) \
           + \
           (1 + 1.0/(12*x[1])) \
           * \
           ( \
               (1-x[3])*(b/x[4])**x[1]*(x[1]/6.2832)**0.5 * (c[i]/7.658)**(x[1]-1) \
               * pk.exp(x[1] - b*c[i]*x[1]/(7.658*x[4])) \
           )

model.add_objective( sum((y_cal[i] - y_obs[i])**2 for i in M) )
    
model.add_constraint( x[3] + (1-x[3])*x[4] >= 0 )
