from _expr.lib import *


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def is_expression(self):
        return False

    def is_constraint(self):
        return False

    def size(self):
        return expr_size(self.ptr)

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::
        
            self < other
            other > self
        """
        e = self - other
        return inequality_constraint(create_inequality(e.ptr))

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::
        
            self > other
            other < self
        """
        e = other - self
        return inequality_constraint(create_inequality(e.ptr))

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::
        
            self <= other
            other >= self
        """
        e = self - other
        return inequality_constraint(create_inequality(e.ptr))

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::
        
            self >= other
            other <= self
        """
        e = other - self
        return inequality_constraint(create_inequality(e.ptr))

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::
        
            self == other
        """
        e = self - other
        return equality_constraint(create_equality(e.ptr))

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self + other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise RuntimeError("Cannot create a constraint from another constraint.")
        else:
            return expression( add_expr_expression(self.ptr, other.ptr) )

    def __sub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self - other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, - other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, - other) )
        elif other.is_constraint():
            raise RuntimeError("Cannot create a constraint from another constraint.")
        else:
            return expression( add_expr_expression(self.ptr, rmul_expr_int(-1, other.ptr)) )

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
            return expression( mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( mul_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise RuntimeError("Cannot create a constraint from another constraint.")
        else:
            return expression( mul_expr_expression(self.ptr, other.ptr) )

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            other + self
        """
        if other.__class__ is int:
            return expression( radd_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            return expression( radd_expr_double(other, self.ptr) )

    def __rsub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            other - self
        """
        if other.__class__ is int:
            return expression( radd_expr_int(other, rmul_expr_int(-1, self.ptr)) )
        elif other.__class__ is float:
            return expression( radd_expr_double(other, rmul_expr_int(-1, self.ptr)) )

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
            return expression( rmul_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( rmul_expr_double(other, self.ptr) )

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self += other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, other) )
        else:
            return expression( add_expr_expression(self.ptr, other.ptr) )

    def __isub__(self,other):
        """
        Binary subtraction

        This method is called when Python processes the statement::

            self -= other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, - other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, - other) )
        else:
            return expression( add_expr_expression(self.ptr, rmul_expr_int(-1, other.ptr)) )

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
            return expression( mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            elif other == 1.0:
                return self
            return expression( mul_expr_double(self.ptr, other) )
        else:
            return expression( mul_expr_expression(self.ptr, other.ptr) )

    def __neg__(self):
        """
        Negation

        This method is called when Python processes the statement::

            - self
        """
        return expression( rmul_expr_int(-1, self.ptr) )

    def __pos__(self):
        """
        Positive expression

        This method is called when Python processes the statement::

            + self
        """
        return self




class parameter(NumericValue):

    __slots__ = ('ptr')

    def __init__(self, value):
        if value.__class__ is int:
            self.ptr = create_parameter_int(value, 0)
        else:
            self.ptr = create_parameter_double(value, 0)


class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)


class variable_single(NumericValue):

    __slots__ = ('ptr', 'name')

    def __init__(self, name=None):
        self.name = name
        self.ptr = create_variable(0,0)   # TODO: add 'within' argument

    def set_value(self, value):
        set_variable_value(self.ptr, value)

    def get_value(self):
        return get_variable_value(self.ptr)

    def __str__(self):
        if self.name is None:
            index = get_variable_index(self.ptr)
            self.name = 'x%d' % index
        return self.name

    def show(self):
        print(str(self))


class variable_array(object):

    __slots__ = ('ptrs', 'name', 'num')

    def __init__(self, num, name=None):
        self.num = num 
        self.name = name
        self.ptr = create_variable_array(0,0,num)   # TODO: add 'within' argument


class expression(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    def is_expression(self):
        return True

    def show(self):
        print_expr(self.ptr)


class constraint(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    def is_constraint(self):
        return True

    def is_inequality(self):
        return False

    def is_equality(self):
        return False

    def show(self):
        print_expr(self.ptr)

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::

            self < other
            other > self
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::

            self > other
            other < self
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::

            self <= other
            other >= self
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::

            self >= other
            other <= self
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::

            self == other
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self + other
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __mul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self * other
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            other + self
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __rmul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            other * self

        when other is not a :class:`NumericValue <pyomo.core.expr.numvalue.NumericValue>` object.
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::

            self += other
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")

    def __imul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self *= other
        """
        raise RuntimeError("Cannot create a constraint from another constraint.")


class inequality_constraint(constraint):

    __slots__ = ()

    def is_inequality(self):
        return True


class equality_constraint(constraint):

    __slots__ = ()

    def is_equality(self):
        return True


class model(object):

    __slots__ = ('ptr',)

    def __init__(self):
        self.ptr = create_model()

    def add(self, obj):
        if obj.is_expression():
            add_objective(self.ptr, obj.ptr)
        elif obj.is_constraint():
            if obj.is_inequality():
                add_inequality(self.ptr, obj.ptr)
            elif obj.is_equality():
                add_equality(self.ptr, obj.ptr)
            else:
                raise RuntimeError("Cannot add constraint to model: "+str(type(obj)))
        else:
            raise RuntimeError("Cannot add object to model: "+str(type(obj)))

    def compute_f(self, i=0):
        return compute_objective_f(self.ptr, i)

    def compute_df(self, i=0):
        return compute_objective_df(self.ptr, i)

    def show(self):
        print_model(self.ptr)

    def build(self):
        build_model(self.ptr)


def quicksum(args):
    # NOTE:  We could simplify this logic by having the summation object
    #   maintain a list of things being summed.
    try:
        first = next(args, None)
    except:
        try:
            args = args.__iter__()
            first = next(args, None)
        except:
            raise RuntimeError("The argument to quicksum() is not iterable!")
    if first is None:
        return 0

    start = parameter(0)
    if first is 0:
        ptr = start.ptr
    else:
        estart = start + first
        ptr = estart.ptr
    const = []
    for arg in args:
        try:
            ptr = add_expr_expression(ptr, arg.ptr)
        except AttributeError:
            const.append(arg)
    return expression(ptr) + sum(const)

