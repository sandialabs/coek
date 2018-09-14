from poek.expr import *

v1 = variable()

v2 = variable()

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
m.add( 2*(v1+3*v2+1) )
m.add( v1*v1+1 <= 3*v2 )
m.add( v1 == 3*v2-3 )
m.show()
