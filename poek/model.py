from poek._expr import ffi, lib


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
        if obj.is_constraint():
            if obj.is_inequality():
                self.nc += 1
                lib.add_inequality(self.ptr, obj.ptr)
            else:  
                self.nc += 1
                lib.add_equality(self.ptr, obj.ptr)
        elif obj.is_expression() or obj.is_variable() or obj.is_parameter():
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

