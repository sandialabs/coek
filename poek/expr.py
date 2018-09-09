#
# E -> float
# E -> int
# E -> param(float/int)
# E -> var(binary/int/float)
# E -> E
#
# expr_add(float, E*)
# expr_add(int, E*)
# expr_add(V*, E*)

# expr_add(E*, float)
# expr_add(E*, int)
# expr_add(E*, V*)
# expr_add(E*, E*)

from _expr import lib


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def is_expression(self):
        return False

    def is_parameter(self):
        return False

    def is_variable(self):
        return False

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::
        
            self < other
            other > self
        """
        return _generate_relational_expression(_lt, self, other)

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::
        
            self > other
            other < self
        """
        return _generate_relational_expression(_lt, other, self)

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::
        
            self <= other
            other >= self
        """
        return _generate_relational_expression(_le, self, other)

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::
        
            self >= other
            other <= self
        """
        return _generate_relational_expression(_le, other, self)

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::
        
            self == other
        """
        return _generate_relational_expression(_eq, self, other)

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self + other
        """
        if other.__class__ is int:
            return expression( lib.add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( lib.add_expr_float(self.ptr, other) )
        else:
            return expression( lib.add_expr_expression(self.ptr, other.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))

    def __mul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::
        
            self * other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( lib.mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( lib.mul_expr_float(self.ptr, other) )
        else:
            return expression( lib.mul_expr_expression(self.ptr, other.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            other + self
        """
        if other.__class__ is int:
            return expression( lib.radd_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            return expression( lib.radd_expr_float(other, self.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))

    def __rmul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::
        
            other * self

        when other is not a :class:`NumericValue <pyomo.core.expr.numvalue.NumericValue>` object.
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( lib.rmul_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( lib.rmul_expr_float(other, self.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self += other
        """
        if other.__class__ is int:
            return expression( lib.add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( lib.add_expr_float(self.ptr, other) )
        else:
            return expression( lib.add_expr_expression(self.ptr, other.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))

    def __imul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self *= other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( lib.mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( lib.mul_expr_float(self.ptr, other) )
        else:
            return expression( lib.mul_expr_expression(self.ptr, other.ptr) )
        raise RuntimeError("Bad expression argument: '%s'" % str(type(other)))


class parameter(NumericValue):

    __slots__ = ('ptr', 'name', 'value')

    def is_parameter(self):
        return True

    def __str__(self):
        return self.name


class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)
            

class variable_single(NumericValue):

    __slots__ = ('ptr', 'name', 'value')

    def __init__(self, name=None):
        self.name = name
        self.ptr = lib.create_variable(0,0)   # TODO: add 'within' argument
        self.value = None

    def is_variable(self):
        return True

    def __str__(self):
        if self.name is None:
            index = lib.get_variable_index(self.ptr)
            self.name = 'x%d' % index
        return self.name


class variable_array(object):

    __slots__ = ('ptrs', 'name', 'num')

    def __init__(self, num, name=None):
        self.num = num 
        self.name = name
        self.ptr = lib.create_variable_array(0,0,num)   # TODO: add 'within' argument


class expression(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    def is_expression(self):
        return True

    def print(self):
        lib.print_expr(self.ptr)

