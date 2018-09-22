from _expr.lib import *
from _expr import ffi, lib


NULL = misc_getnull()
BUFFER = ffi.new("char []", 64)


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def is_expression(self):
        return False

    def is_constraint(self):
        return False

    def get_descr(self):
        return get_numval_str(self.ptr, BUFFER, 64)

    def size(self):
        return expr_size(self.ptr)

    def value(self, compute=True):
        if compute:
            return compute_numval_value(self.ptr)
        else:
            return get_numval_value(self.ptr)

    def diff(self, var):
        if self.__class__ is parameter:
            return ZeroParameter
        if self.__class__ is variable:
            if id(self) == id(var):
                return OneParameter
            else:
                return ZeroParameter
        if var.__class__ is variable_single:
            tmp = expr_diff(self.ptr, var.ptr)
            if tmp == NULL:
                return ZeroParameter
            return expression(tmp)
        if var.__class__ is list or var.__class__ is tuple:
            tmp = []
            for v in var:
                ptr = expr_diff(self.ptr, v.ptr)
                if ptr == NULL:
                    tmp.append( ZeroParameter )
                else:
                    tmp.append( expression(expr_diff(self.ptr, v.ptr)) )
            return tmp
        raise RuntimeError("Badly formated argument")

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

    def show(self):
        print_parameter(self.ptr)

ZeroParameter = parameter(0)
OneParameter = parameter(1)

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)


class variable_single(NumericValue):

    __slots__ = ('ptr', 'name', 'index')

    def __init__(self, name=None):
        self.name = name
        self.ptr = create_variable(0,0)   # TODO: add 'within' argument
        self.index = get_variable_index(self.ptr)

    def set_value(self, value):
        set_variable_value(self.ptr, value)

    def get_value(self):
        return get_variable_value(self.ptr)

    def __str__(self):
        if self.name is None:
            self.name = 'x%d' % self.index
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

    __slots__ = ('ptr','x','nx','c','nc')

    def __init__(self):
        self.ptr = create_model()
        self.nx = None
        self.x = None
        self.nc = 0
        self.c = None

    def add(self, obj):
        if self.x is not None:
            self.x = None
        if obj.is_expression():
            add_objective(self.ptr, obj.ptr)
        elif obj.is_constraint():
            if obj.is_inequality():
                self.nc += 1
                add_inequality(self.ptr, obj.ptr)
            elif obj.is_equality():
                self.nc += 1
                add_equality(self.ptr, obj.ptr)
            else:
                raise RuntimeError("Cannot add constraint to model: "+str(type(obj)))
        else:
            raise RuntimeError("Cannot add object to model: "+str(type(obj)))

    def compute_f(self, i=0):
        return compute_objective_f(self.ptr, i)

    def compute_df(self, i=0):
        if self.nx is None:
            self.nx = get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        compute_objective_df(self.ptr, self.x, self.nx, i)
        tmp = []
        for i in range(self.nx):
            tmp.append( self.x[i] )
        return tmp

    def compute_c(self):
        if self.c is None:
            self.c = ffi.new("double []", self.nc)
        compute_constraint_f(self.ptr, self.c, self.nc)
        tmp = []
        for i in range(self.nc):
            tmp.append( self.c[i] )
        return tmp

    def compute_dc(self, i):
        if self.nx is None:
            self.nx = get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        compute_constraint_df(self.ptr, self.x, self.nx, i)
        tmp = []
        for i in range(self.nx):
            tmp.append( self.x[i] )
        return tmp

    def show(self, df=0):
        print_model(self.ptr, df)

    def build(self):
        # TODO: customize build based on AD needs f/g/h
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



class Visitor(object):

    def walk(self, expr):
        visitor = ffi.new_handle(self)
        self._visitor = visitor       # keep this object alive
        lib.visitor_walk(expr.ptr, lib.visitor_callback, visitor)

    def visit(self, ptr, parent):
        """
        This method needs to be defined by a subclass.  It is
        called when a node in an expression tree is visited.
        """
        pass
        
@ffi.def_extern()
def visitor_callback(ptr, parent, visitor):
    self = ffi.from_handle(visitor)
    self.visit(ptr, parent)


class ValueVisitor(Visitor):

    def visit(self, ptr, parent):
        if parent == NULL:
            self.values = []
        #self.values.append(get_numval_value(ptr))
        get_numval_str(ptr, BUFFER, 64)
        self.values.append(ffi.string(BUFFER).decode('utf-8'))


