from poek._expr import ffi, lib
from poek.globals import NAN, NULL, BUFFER

__all__ = ['NumericValue', 'parameter', 'ZeroParameter', 'OneParameter',
    'variable', 'variable_single', 'variable_array', 'expression',
    'constraint', 'inequality_constraint', 'equality_constraint']


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def is_parameter(self):             #pragma:nocover
        return False

    def is_expression(self):            #pragma:nocover
        return False

    def is_constraint(self):            #pragma:nocover
        return False

    def is_variable(self):              #pragma:nocover
        return False

    def size(self):
        return lib.expr_size(self.ptr)

    def eval(self):
        return lib.compute_numval_value(self.ptr)

    #
    # Currently, the core subclasses define these functions.  Maybe we'll need 
    # default values for some future subclass.  But for now I can't see a use case.
    #
    #@property
    #def value(self):
    #    return lib.get_numval_value(self.ptr)
    #@value.setter
    #def value(self, value):
    #    lib.set_variable_value(self.ptr, value)

    #
    # TODO: test this logic
    #
    # It's convenient to return immutable parameters here rather than constants,
    # since the user expecting to get an expression back.
    #
    def diff(self, var):
        if self.__class__ is parameter:
            return ZeroParameter
        elif var.__class__ is variable_single:
            if self.__class__ is variable_single:
                if id(self) == id(var):
                    return OneParameter
                else:
                    return ZeroParameter
            tmp = lib.expr_diff(self.ptr, var.ptr)
            if tmp == NULL:
                return ZeroParameter
            return expression(tmp)
        elif var.__class__ is list or var.__class__ is tuple:
            tmp = []
            for v in var:
                ptr = lib.expr_diff(self.ptr, v.ptr)
                if ptr == NULL:
                    tmp.append( ZeroParameter )
                else:
                    tmp.append( expression(lib.expr_diff(self.ptr, v.ptr)) )
            return tmp
        else:
            raise TypeError("Badly formated argument")

    def __float__(self):
        """
        Coerce the value to a floating point

        Raises:
            TypeError
        """
        raise TypeError("Implicit conversion of NumericValue type to a float is disabled.")

    def __int__(self):
        """
        Coerce the value to a integer

        Raises:
            TypeError
        """
        raise TypeError("Implicit conversion of NumericValue type to an integer is disabled.")

    def __bool__(self):
        """
        Coerce the value to a boolean

        Raises:
            TypeError
        """
        raise TypeError("Implicit conversion of NumericValue type to a boolean is disabled.")

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::
        
            self < other
            other > self
        """
        e = self - other
        return inequality_constraint(lib.create_inequality(e.ptr, 1))

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::
        
            self > other
            other < self
        """
        e = other - self
        return inequality_constraint(lib.create_inequality(e.ptr, 1))

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::
        
            self <= other
            other >= self
        """
        e = self - other
        return inequality_constraint(lib.create_inequality(e.ptr, 0))

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::
        
            self >= other
            other <= self
        """
        e = other - self
        return inequality_constraint(lib.create_inequality(e.ptr, 0))

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::
        
            self == other
        """
        e = self - other
        return equality_constraint(lib.create_equality(e.ptr))

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self + other
        """
        if other.__class__ is int:
            if other == 0:
                return self
            return expression( lib.add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return self
            return expression( lib.add_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        try:
            return expression( lib.add_expr_expression(self.ptr, other.ptr) )
        except AttributeError as e:
            raise TypeError("Cannot treat argument as a single numeric value: "+str(e))

    def __sub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self - other
        """
        if other.__class__ is int:
            if other == 0:
                return self
            return expression( lib.sub_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return self
            return expression( lib.sub_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        try:
            return expression( lib.sub_expr_expression(self.ptr, other.ptr) )
        except AttributeError as e:
            raise TypeError("Cannot treat argument as a single numeric value: "+str(e))

    def __mul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::
        
            self * other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            elif other == 1:
                return self
            return expression( lib.mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( lib.mul_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        try:
            tmp = lib.mul_expr_expression(self.ptr, other.ptr)
            if tmp == NULL:
                return ZeroParameter
            elif tmp == self.ptr:
                return self
            elif tmp == other.ptr:
                return other
            return expression(tmp)
        except AttributeError as e:
            raise TypeError("Cannot treat argument as a single numeric value: "+str(e))

    def __div__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::
        
            self / other
        """
        if other.__class__ is int:
            if other == 0:
                raise ZeroDivisionError
            elif other == 1:
                return self
            return expression( lib.div_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                raise ZeroDivisionError
            elif other == 1.0:
                return self
            return expression( lib.div_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        try:
            tmp = lib.div_expr_expression(self.ptr, other.ptr)
            if tmp == NULL:
                return ZeroParameter
            elif tmp == self.ptr:
                return self
            elif tmp == other.ptr:
                raise ZeroDivisionError
            return expression(tmp)
        except AttributeError as e:
            raise TypeError("Cannot treat argument as a single numeric value: "+str(e))

    __truediv__ = __div__

    def __pow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::
        
            self ** other
        """
        if other.__class__ is int:
            if other == 0:
                return 1
            elif other == 1:
                return self
            return expression( lib.pow_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 1.0
            elif other == 1.0:
                return self
            return expression( lib.pow_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        try:
            tmp = lib.pow_expr_expression(self.ptr, other.ptr)
            if tmp == NULL:
                return ZeroParameter
            elif tmp == self.ptr:
                return self
            return expression(tmp)
        except AttributeError as e:
            raise TypeError("Cannot treat argument as a single numeric value: "+str(e))

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            other + self
        """
        if other.__class__ is int:
            if other == 0:
                return self
            return expression( lib.radd_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return self
            return expression( lib.radd_expr_double(other, self.ptr) )
        else:
            raise TypeError("Unexpected type (LHS) when adding: %s" % str(type(other)))

    def __rsub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            other - self
        """
        if other.__class__ is int:
            return expression( lib.rsub_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            return expression( lib.rsub_expr_double(other, self.ptr) )
        else:
            raise TypeError("Unexpected type (LHS) when subtracting: %s" % str(type(other)))

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
            elif other == 1:
                return self
            return expression( lib.rmul_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( lib.rmul_expr_double(other, self.ptr) )
        else:
            raise TypeError("Unexpected type (LHS) when multiplying: %s" % str(type(other)))

    def __rdiv__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::

            other * self

        when other is not a :class:`NumericValue <pyomo.core.expr.numvalue.NumericValue>` object.
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            tmp = lib.rdiv_expr_int(other, self.ptr)
            if tmp == self.ptr:         # This is a hack to catch division by zero errors
                raise ZeroDivisionError
            return expression( tmp )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            tmp = lib.rdiv_expr_double(other, self.ptr)
            if tmp == self.ptr:
                raise ZeroDivisionError
            return expression( tmp )
        else:
            raise TypeError("Unexpected type (LHS) when dividing: %s" % str(type(other)))

    __rtruediv__ = __rdiv__

    def __rpow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::

            other ** self

        when other is not a :class:`NumericValue <pyomo.core.expr.numvalue.NumericValue>` object.
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            elif other == 1:
                return 1
            return expression( lib.rpow_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return 1.0
            return expression( lib.rpow_expr_double(other, self.ptr) )
        else:
            raise TypeError("Unexpected type (LHS) in a power expression: %s" % str(type(other)))

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self += other
        """
        if other.__class__ is int:
            if other == 0:
                return self
            return expression( lib.add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return self
            return expression( lib.add_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.add_expr_expression(self.ptr, other.ptr) )

    def __isub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self -= other
        """
        if other.__class__ is int:
            if other == 0:
                return self
            return expression( lib.sub_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return self
            return expression( lib.sub_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.sub_expr_expression(self.ptr, other.ptr) )

    def __imul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self *= other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            elif other == 1:
                return self
            return expression( lib.mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( lib.mul_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.mul_expr_expression(self.ptr, other.ptr) )

    def __idiv__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::

            self /= other
        """
        if other.__class__ is int:
            if other == 0:
                raise ZeroDivisionError
            elif other == 1:
                return self
            return expression( lib.div_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                raise ZeroDivisionError
            elif other == 1.0:
                return self
            return expression( lib.div_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.div_expr_expression(self.ptr, other.ptr) )

    __itruediv__ = __idiv__

    def __ipow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::

            self **= other
        """
        if other.__class__ is int:
            if other == 0:
                return 1
            elif other == 1:
                return self
            return expression( lib.pow_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 1.0
            elif other == 1.0:
                return self
            return expression( lib.pow_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.pow_expr_expression(self.ptr, other.ptr) )

    def __neg__(self):
        """
        Negation

        This method is called when Python processes the statement::

            - self
        """
        return expression( lib.neg_expr(self.ptr) )

    def __pos__(self):
        """
        Positive expression

        This method is called when Python processes the statement::

            + self
        """
        return self

    def __abs__(self):
        """ Absolute value

        This method is called when Python processes the statement::
        
            abs(self)
        """
        return expression( lib.intrinsic_abs(self.ptr) )


class parameter(NumericValue):

    __slots__ = ('ptr','mutable')

    def __init__(self, value, mutable=True, name=None):
        self.mutable = 0 if not mutable else 1
        if name is None:
            tmp = str.encode("")
        else:
            tmp = str.encode(name)
        if value.__class__ is int:
            self.ptr = lib.create_parameter_int(value, self.mutable, tmp)
        else:
            self.ptr = lib.create_parameter_double(value, self.mutable, tmp)

    def show(self):             #pragma:nocover
        lib.print_parameter(self.ptr)

    @property
    def value(self):
        return lib.get_numval_value(self.ptr)
    @value.setter
    def value(self, value):
        if self.mutable:
            lib.set_variable_value(self.ptr, value)
        else:
            raise TypeError("Cannot set the value of an immutable parameter")

    def is_parameter(self):  #pragma:nocover
        return True


ZeroParameter = parameter(0, False)
OneParameter = parameter(1, False)


class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)


class variable_single(NumericValue):

    __slots__ = ('ptr', 'name', 'index')

    def __init__(self, name=None, initialize=NAN, lb=NAN, ub=NAN):
        self.name = name
        if name is None:
            self.ptr = lib.create_variable(0,0,lb,ub,initialize,str.encode(""))   # TODO: add 'within' argument
        else:
            self.ptr = lib.create_variable(0,0,lb,ub,initialize,str.encode(name))   # TODO: add 'within' argument
        self.index = lib.get_variable_index(self.ptr)

    @property
    def value(self):
        return lib.get_numval_value(self.ptr)
    @value.setter
    def value(self, value):
        lib.set_variable_value(self.ptr, value)

    @property
    def lb(self):
        return lib.get_variable_lb(self.ptr)
    @lb.setter
    def lb(self, value):
        lib.set_variable_lb(self.ptr, value)

    @property
    def ub(self):
        return lib.get_variable_ub(self.ptr)
    @ub.setter
    def ub(self, value):
        lib.set_variable_ub(self.ptr, value)

    def __str__(self):      #pragma:nocover
        if self.name is None:
            self.name = 'x%d' % self.index
        return self.name

    def show(self):         #pragma:nocover
        print(str(self))

    def is_variable(self):  #pragma:nocover
        return True


class variable_view(variable_single):

    def __init__(self, name=None, ptr=None):
        self.ptr = ptr
        if name is None:
            self.index = lib.get_variable_index(self.ptr)
            self.name = 'x%d' % self.index
        else:
            self.name = name

    @property
    def value(self):
        return lib.get_numval_value(self.ptr)
    @value.setter
    def value(self, value):
        lib.set_variable_value(self.ptr, value)

    @property
    def lb(self):
        return lib.get_variable_lb(self.ptr)
    @lb.setter
    def lb(self, value):
        lib.set_variable_lb(self.ptr, value)

    @property
    def ub(self):
        return lib.get_variable_ub(self.ptr)
    @ub.setter
    def ub(self, value):
        lib.set_variable_ub(self.ptr, value)



class variable_array(object):

    __slots__ = ('ptrs', 'name', 'num', 'views', 'initialize')

    def __init__(self, num, name=None, initialize=NAN, lb=NAN, ub=NAN):
        self.num = num 
        self.name = name
        self.initialize = initialize
        prefix = str.encode("") if name is None else str.encode(name)
        ptrs = ffi.new("void* []", num)
        lib.create_variable_array(ptrs,num,0,0,lb,ub,initialize,prefix)
        self.ptrs = ptrs
        self.views = {}

    def is_expression(self):        #pragma:nocover
        return False

    def is_constraint(self):        #pragma:nocover
        return False

    def __getitem__(self, key):
        views = self.views
        if key in views:
            return views[key]
        if self.name is None:
            views[key] = variable_view(ptr=self.ptrs[key])
        else:
            views[key] = variable_view(name=self.name + str(key), ptr=self.ptrs[key])
        return views[key]

    def __iter__(self):
        for i in range(self.num):
            yield i #self.__getitem__(i)


class expression(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    @property
    def value(self):
        return self.eval()
    @value.setter
    def value(self, value):
        raise TypeError("Cannot set the value of an expression")

    def is_expression(self):        #pragma:nocover
        return True

    def show(self):                 #pragma:nocover
        lib.print_expr(self.ptr)


class constraint(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    #
    # TODO: Should the default be body=False?
    #
    def eval(self, body=True):
        return lib.compute_constraint_value(self.ptr, body)

    def __bool__(self):
        """
        Coerce the value to a boolean

        Returns:
            Boolean value
        """
        return lib.compute_constraint_value(self.ptr, False) == 1

    @property
    def value(self):
        return self.eval()
    @value.setter
    def value(self, value):
        raise TypeError("Cannot set the value of a constraint")

    #
    # TODO: Reassess whether this should be TRUE
    #
    def is_expression(self):    #pragma:nocover
        return True

    def is_constraint(self):    #pragma:nocover
        return True

    def is_inequality(self):    #pragma:nocover
        return False

    def is_equality(self):      #pragma:nocover
        return False

    def show(self):             #pragma:nocover
        lib.print_expr(self.ptr)

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::

            self < other
            other > self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::

            self > other
            other < self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::

            self <= other
            other >= self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::

            self >= other
            other <= self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::

            self == other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self + other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __sub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self - other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __mul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self * other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __div__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::
        
            self / other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    __truediv__ = __div__

    def __pow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::
        
            self ** other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            other + self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __rsub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            other - self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __rmul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            other * self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __rdiv__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::

            other * self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    __rtruediv__ = __rdiv__

    def __rpow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::

            other ** self
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self += other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __isub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self -= other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __imul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self *= other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    def __idiv__(self,other):
        """
        Binary division

        This method is called when Python processes the statement::

            self /= other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")

    __itruediv__ = __idiv__

    def __ipow__(self,other):
        """
        Binary power

        This method is called when Python processes the statement::

            self **= other
        """
        raise TypeError("Cannot create a constraint with a relational subexpression.")


class inequality_constraint(constraint):

    __slots__ = ()

    def is_inequality(self):
        return True


class equality_constraint(constraint):

    __slots__ = ()

    def is_equality(self):  #pragma:nocover
        return True


