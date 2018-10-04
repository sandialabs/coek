from poek import *

v1 = variable(name='v1')

v2 = variable()

v3 = variable(name='v3')

print(v1)
print(v2)

e1 = v1 + 1
e2 = 2 + v2
e1.show()
e2.show()

e3 = e1 + e2
e3.show()
e4 = e1 + 3
e4.show()
e5 = 4 + e1
e5.show()
e6 = e1 + v1
e6.show()
e7 = v2 + e2
e7.show()
e8 = e2 + 0
e8.show()
e9 = 0 + e2
e9.show()


e1 = v1 - 1
e2 = 2 - v2
e1.show()
e2.show()

e3 = e1 - e2
e3.show()
e4 = e1 - 3
e4.show()
e5 = 4 - e1
e5.show()
e6 = e1 - v1
e6.show()
e7 = v2 - e2
e7.show()
e8 = e2 - 0
e8.show()
e9 = 0 - e2
e9.show()


e1 = v1 * 1
e2 = 2 * v2
e1.show()
e2.show()

e3 = e1 * e2
e3.show()
e4 = e1 * 3
e4.show()
e5 = 4 * e1
e5.show()
e6 = e1 * v1
e6.show()
e7 = v2 * e2
e7.show()
e8 = e2 * 0
print(e8)
e9 = 0 * e2
print(e9)

e1 = 2*v1 + 2*v2 + 3*(v1+2*v2)
e1.show()
print(e1.size())



e = v1 <= v2
e.show()
e = v1 <= 1
e.show()
e = v1 >= 0
e.show()
e = v1 > 0
e.show()
e = v1 < 0
e.show()
e = v1 == 0
e.show()
e = v1 == 2
e.show()
e = v1 == 2 + v2
e.show()


m = model()
e = v1+v2
m.add( (1-e) + 3*e + e*e + v3)
m.add( v1*v1+1 <= 3*v2 )
m.add( v1 == 3*v2-3 )
m.show(1)
m.build()
m.show(1)

v1.value = 5
v2.value = 7
v3.value = 0
print(v1.value)
print(v2.value)
print(v3.value)

print(m.compute_f())
print(m.compute_df())
print(m.compute_c())
for i in range(m.nc):
    print((i, m.compute_dc(i)))
print()

print("HESSIAN")
print(m.compute_Hv([1,0,0]))
print(m.compute_Hv([0,1,0]))
print(m.compute_Hv([0,0,1]))

print("VALUES")
E = v1*v1+1-3*v2*v1
print(E.eval())
print(E.value)

print("DIFF")
dEdv1 = E.diff(v1)
dEdv1.show()
dEdv2 = E.diff(v2)
dEdv2.show()
dEdv3 = E.diff(v3)
dEdv3.show()

E.show()
visitor = ValueVisitor()
print(visitor.walk(E))
print(visitor.walk(E, show=True))

Y = variable(10, name='Y', initialize=3)
for var in Y:
  print((var.name, var.value))
Z = variable(10, initialize=3)
for var in Z:
  print((var.name, var.value))
