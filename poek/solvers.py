
__all__ = ['GurobiSolver']


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
        
