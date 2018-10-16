from poek._expr import ffi, lib
__all__ = ['Solver']


class Solver(object):

    __slots__ = ('ptr')

    def __init__(self, name):
        self.ptr = lib.get_solver(str.encode(name))

    def solve(self, model):
        lib.set_solver_model(self.ptr, model.ptr)
        lib.solver_solve(self.ptr)



class XGurobiSolver(object):

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
        
