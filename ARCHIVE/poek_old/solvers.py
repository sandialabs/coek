from poek.coek_cffi import ffi, lib

__all__ = ['Solver', 'GurobiSolver']


class Solver(object):

    __slots__ = ('ptr')

    def __init__(self, name):
        self.ptr = lib.get_solver(str.encode(name))

    def solve(self, model):
        lib.set_solver_model(self.ptr, model.ptr)
        lib.solver_solve(self.ptr)



class GurobiSolver(object):

    __slots__ = ('env', 'modelptr', 'x', 'nx')

    def __init__(self):
        self.modelptr = None
        self.x = None
        self.nx = None

    def solve(self, model):
        solver = lib.get_solver(str.encode('gurobi'))
        lib.set_solver_model(solver, model.ptr)
        lib.solver_solve(solver)
        
