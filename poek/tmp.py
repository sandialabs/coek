from poek.expr import *

v1 = variable()

v2 = variable()

print(v1)
print(v2)

e1 = v1 + 1
e2 = 2 + v2
e1.print()
e2.print()

e3 = e1 + e2
e3.print()
e4 = e1 + 3
e4.print()
e5 = 4 + e1
e5.print()
e6 = e1 + v1
e6.print()
e7 = v2 + e2
e7.print()
e8 = e2 + 0
e8.print()
e9 = 0 + e2
e9.print()


e1 = v1 * 1
e2 = 2 * v2
e1.print()
e2.print()

e3 = e1 * e2
e3.print()
e4 = e1 * 3
e4.print()
e5 = 4 * e1
e5.print()
e6 = e1 * v1
e6.print()
e7 = v2 * e2
e7.print()
e8 = e2 * 0
print(e8)
e9 = 0 * e2
print(e9)

e1 = 2*v1 + 2*v2 + 3*(v1+2*v2)
e1.print()
print(e1.size())
