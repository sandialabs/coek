__all__ = ['model']

from poek.coek_cffi import ffi, lib
import poek.expr 


class model(object):

    __slots__ = ('ptr','x','nx','c','nc', 'v')

    def __init__(self):
        self.ptr = lib.create_model()
        self.nx = None
        self.v = None
        self.x = None
        self.nc = 0
        self.c = None
        #
        # We should not normally change data in a class like this, but
        # the *Parameter objects are treated as global constants.  Since they
        # may have been imported, we need to change their class values.
        #
        poek.expr.ZeroParameter.ptr = lib.get_parameter_zero(poek.expr.NULL)
        poek.expr.OneParameter.ptr = lib.get_parameter_one(poek.expr.NULL)
        poek.expr.NegativeOneParameter = lib.get_parameter_negative_one(poek.expr.NULL)

    def add(self, obj):
        if obj.is_constraint():
            if obj.is_inequality():
                self.nc += 1
                lib.add_inequality(self.ptr, obj.ptr)
            else:  
                self.nc += 1
                lib.add_equality(self.ptr, obj.ptr)
        elif obj.is_expression() or obj.is_variable() or obj.is_parameter():
            lib.add_objective(self.ptr, obj.ptr)

    def num_objectives(self):
        return lib.get_nobjectives(self.ptr)

    def num_constraints(self):
        return lib.get_nconstraints(self.ptr)

    def get_objective(self, i=0):
        return poek.expr.expression( lib.get_objective(self.ptr, i) )

    def get_constraint(self, i):
        if i < lib.get_niconstraints(self.ptr):
            return poek.expr.inequality_constraint( lib.get_constraint(self.ptr, i) )
        return poek.expr.equality_constraint( lib.get_constraint(self.ptr, i) )

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

    def write(self, filename):
        lib.write_model(self.ptr, str.encode(filename))


