from poek._expr import ffi, lib


NAN = float('nan')
NULL = lib.misc_getnull()
BUFFER = ffi.new("char []", 64)


def value(obj):
    if isinstance(obj, NumericValue):
        return obj.eval()
    return obj


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def is_expression(self):
        return False

    def is_constraint(self):
        return False

    #def get_descr(self):
    #    return lib.get_numval_str(self.ptr, BUFFER, 64)

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
        if self.__class__ is variable:
            if id(self) == id(var):
                return OneParameter
            else:
                return ZeroParameter
        if var.__class__ is variable_single:
            tmp = lib.expr_diff(self.ptr, var.ptr)
            if tmp == NULL:
                return ZeroParameter
            return expression(tmp)
        if var.__class__ is list or var.__class__ is tuple:
            tmp = []
            for v in var:
                ptr = lib.expr_diff(self.ptr, v.ptr)
                if ptr == NULL:
                    tmp.append( ZeroParameter )
                else:
                    tmp.append( expression(lib.expr_diff(self.ptr, v.ptr)) )
            return tmp
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
        else:
            tmp = lib.mul_expr_expression(self.ptr, other.ptr)
            if tmp == NULL:
                return ZeroParameter
            elif tmp == self.ptr:
                return self
            elif tmp == other.ptr:
                return other
            return expression(tmp)

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
        else:
            tmp = lib.div_expr_expression(self.ptr, other.ptr)
            if tmp == NULL:
                return ZeroParameter
            elif tmp == self.ptr:
                return self
            elif tmp == other.ptr:
                raise ZeroDivisionError
            return expression(tmp)

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
        tmp = lib.pow_expr_expression(self.ptr, other.ptr)
        if tmp == NULL:
            return ZeroParameter
        elif tmp == self.ptr:
            return self
        return expression(tmp)

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
            raise TypeError("Unexpected type for LHS of rmul: %s" % str(type(other)))

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
            raise TypeError("Unexpected type for LHS of rdiv: %s" % str(type(other)))

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
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            raise TypeError("Unexpected type for LHS of rpow: %s" % str(type(other)))

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
                return NAN
            elif other == 1:
                return self
            return expression( lib.idiv_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return NAN
            elif other == 1.0:
                return self
            return expression( lib.idiv_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.idiv_expr_expression(self.ptr, other.ptr) )

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
            return expression( lib.ipow_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 1.0
            elif other == 1.0:
                return self
            return expression( lib.ipow_expr_double(self.ptr, other) )
        elif other.is_constraint():
            raise TypeError("Cannot create a constraint with a relational subexpression.")
        else:
            return expression( lib.ipow_expr_expression(self.ptr, other.ptr) )

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

    def show(self):
        lib.print_parameter(self.ptr)

    @property
    def value(self):
        return lib.get_numval_value(self.ptr)
    @value.setter
    def value(self, value):
        if self.mutable:
            lib.set_variable_value(self.ptr, value)
        else:
            raise RuntimeError("Cannot set the value of an immutable parameter")


ZeroParameter = parameter(0, False)
OneParameter = parameter(1, False)

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)


class variable_single(NumericValue):

    __slots__ = ('ptr', 'name', 'index')

    def __init__(self, name=None, initialize=None):
        self.name = name
        if name is None:
            self.ptr = lib.create_variable(0,0,str.encode(""))   # TODO: add 'within' argument
        else:
            self.ptr = lib.create_variable(0,0, str.encode(name))   # TODO: add 'within' argument
        if not initialize is None:
            self.value = initialize
        self.index = lib.get_variable_index(self.ptr)

    @property
    def value(self):
        return lib.get_numval_value(self.ptr)
    @value.setter
    def value(self, value):
        lib.set_variable_value(self.ptr, value)

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
        prefix = str.encode("") if name is None else str.encode(name)
        tmp = ffi.new("void* []", num)
        self.ptrs = lib.create_variable_array(tmp,num,0,0,prefix)

    def is_expression(self):
        return False

    def is_constraint(self):
        return False


class expression(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    @property
    def value(self):
        return self.eval()
    @value.setter
    def value(self, value):
        raise RuntimeError("Cannot set value of an expression")

    def is_expression(self):
        return True

    def show(self):
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
        raise RuntimeError("Cannot set value of a constraint")

    #
    # TODO: Reassess whether this should be TRUE
    #
    def is_expression(self):
        return True

    def is_constraint(self):
        return True

    def is_inequality(self):
        return False

    def is_equality(self):
        return False

    def show(self):
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


class inequality_constraint(constraint):

    __slots__ = ()

    def is_inequality(self):
        return True


class equality_constraint(constraint):

    __slots__ = ()

    def is_equality(self):
        return True


class model(object):

    __slots__ = ('ptr','x','nx','c','nc', 'v')

    def __init__(self):
        self.ptr = lib.create_model()
        self.nx = None
        self.v = None
        self.x = None
        self.nc = 0
        self.c = None

    def add(self, obj):
        if self.x is not None:
            self.x = None
        if obj.is_constraint():
            if obj.is_inequality():
                self.nc += 1
                lib.add_inequality(self.ptr, obj.ptr)
            elif obj.is_equality():
                self.nc += 1
                lib.add_equality(self.ptr, obj.ptr)
            else:
                raise TypeError("Cannot add constraint to model: "+str(type(obj)))
        elif obj.is_expression():
            lib.add_objective(self.ptr, obj.ptr)
        else:
            lib.add_objective(self.ptr, obj.ptr)

    def compute_f(self, i=0):
        return lib.compute_objective_f(self.ptr, i)

    def compute_df(self, i=0):
        if self.nx is None:
            self.nx = lib.get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        lib.compute_objective_df(self.ptr, self.x, self.nx, i)
        tmp = []
        for i in range(self.nx):
            tmp.append( self.x[i] )
        return tmp

    def compute_c(self):
        if self.c is None:
            self.c = ffi.new("double []", self.nc)
        lib.compute_constraint_f(self.ptr, self.c, self.nc)
        tmp = []
        for i in range(self.nc):
            tmp.append( self.c[i] )
        return tmp

    def compute_dc(self, i):
        if self.nx is None:
            self.nx = lib.get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        lib.compute_constraint_df(self.ptr, self.x, self.nx, i)
        tmp = []
        for i in range(self.nx):
            tmp.append( self.x[i] )
        return tmp

    def compute_Hv(self, v, i=0):
        if self.nx is None:
            self.nx = lib.get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        if self.v is None:
            self.v = ffi.new("double []", self.nx)
        for j in range(self.nx):
            self.v[j] = v[j]
        lib.compute_Hv(self.ptr, self.v, self.x, self.nx, i)
        tmp = []
        for j in range(self.nx):
            tmp.append( self.x[j] )
        return tmp

    def show(self, df=0):
        lib.print_model(self.ptr, df)

    def build(self):
        # TODO: customize build based on AD needs f/g/h
        lib.build_model(self.ptr)


class GurobiSolver(object):

    __slots__ = ('env','model', 'x', 'nx')

    def __init__(self):
        self.envptr = lib.solver_gurobi_env()
        self.modelptr = None
        self.x = None
        self.nx = None

    def solve(self, model):
        self.modelptr = lib.solver_gurobi_model(model.ptr)
        if self.nx is None:
            self.nx = lib.get_nvariables(self.ptr)
        if self.x is None:
            self.x = ffi.new("double []", self.nx)
        lib.gurobi_solve(self.modelptr, self.x, self.nx)
        tmp = []
        for i in range(self.nx):
            tmp.append( self.x[i] )
        

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
            raise TypeError("The argument to quicksum() is not iterable!")
    if first is None:
        return 0

    start = ZeroParameter
    if first is 0:
        ptr = start.ptr
    else:
        estart = start + first
        ptr = estart.ptr
    const = []
    for arg in args:
        try:
            ptr = lib.add_expr_expression(ptr, arg.ptr)
        except AttributeError:
            const.append(arg)
    return expression(ptr) + sum(const)



class Visitor(object):

    def walk(self, expr):
        visitor = ffi.new_handle(self)
        self._visitor = visitor       # keep this object alive
        lib.visitor_walk(expr.ptr, lib.visitor_enter_callback, lib.visitor_exit_callback, visitor)
        return self._return()

    def visit_enter(self, ptr, parent):
        """
        This method needs to be defined by a subclass.  It is
        called when a node in an expression tree is visited.
        """
        pass
        
    def visit_exit(self, ptr, parent):
        """
        This method needs to be defined by a subclass.  It is
        called after all children of a node have been visited.
        """
        pass
        
@ffi.def_extern()
def visitor_enter_callback(ptr, parent, visitor):
    self = ffi.from_handle(visitor)
    self.visit_enter(ptr, parent)

@ffi.def_extern()
def visitor_exit_callback(ptr, parent, visitor):
    self = ffi.from_handle(visitor)
    self.visit_exit(ptr, parent)


class ValueVisitor(Visitor):

    def walk(self, expr, show=False):
        self.show = show
        return Visitor.walk(self, expr)

    def _return(self):
        return self.values[0]

    def visit_enter(self, ptr, parent):
        if parent == NULL:
            self.values = [ ]
            self.curr = [self.values]
        lib.get_numval_str(ptr, BUFFER, 64)
        tmp = [ffi.string(BUFFER).decode('utf-8') ]
        self.curr[-1].append( tmp )
        self.curr.append( tmp )

    def visit_exit(self, ptr, parent):
        tmp = self.curr.pop()
        if type(tmp) is list and len(tmp) == 1:
            self.curr[-1].pop()
            if self.show:
                self.curr[-1].append(tmp[0])
            else:
                self.curr[-1].append( tmp[0].split('{')[0] )
        #
        # This is a hack for inequality and equalities.  The RHS is implicit in these classe,
        # but we make it explicit here.
        #
        if self.curr[-1][0] == '<=':
            self.curr[1].append( '0' )
        elif self.curr[-1][0] == '<':
            self.curr[1].append( '0' )
        elif self.curr[-1][0] == '==':
            self.curr[1].append( '0' )


