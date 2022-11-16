from pyomo.contrib.appsi.base import (
    TerminationCondition,
    SolverConfig,
    MIPSolverConfig,
    SolutionLoader,
    Results,
    Solver,
    SolverFactory,
)
import poek as pk
from typing import Tuple, Dict
from pyomo.core.base.block import _BlockData
from pyomo.common.timing import HierarchicalTimer
import pyomo.environ as pe
from pyomo_coek.full_integration.objective import Objective, minimize, maximize
from pyomo_coek.full_integration.constraint import Constraint
from pyomo_coek.full_integration.expression import Var


_nlp_solvers = {
    "ipopt",
}


class HybridSolver(Solver):
    def __init__(self, solver_name):
        self._solver_name = solver_name
        self._opt = None
        self._solver_options = dict()
        self._poek_model = None
        self._poek_nlp_model = None

    def available(self):
        if self._opt.available:
            return self.Availability.FullLicense
        else:
            return self.Availability.NotFound

    def version(self) -> Tuple:
        raise NotImplementedError("ToDo")

    @property
    def options(self):
        return self._solver_options

    @options.setter
    def options(self, val: Dict):
        self._solver_options = val

    @property
    def symbol_map(self):
        raise NotImplementedError("ToDo")

    def _construct_poek_model(self, model: _BlockData, timer: HierarchicalTimer) -> pk.model:
        timer.start("construct poek model")
        pm = pk.model()
        for v in model.component_data_objects(Var, descend_into=True):
            pm.add_variable_(v._pe)
        for c in model.component_data_objects(Constraint, active=True, descend_into=True):
            pm.add_constraint(c.expr._pe)
        for obj in model.component_data_objects(Objective, active=True, descend_into=True):
            if obj.sense == minimize:
                obj_expr = obj.expr._pe
            else:
                obj_expr = -obj.expr._pe
            pm.add_objective(obj_expr, True)
        timer.stop("construct poek model")

        self._poek_model = pm
        return pm

    def load(self, model: _BlockData, timer: HierarchicalTimer = None):
        if timer is None:
            timer = HierarchicalTimer()
        pm = self._construct_poek_model(model, timer)
        timer.start("coek load")

        if self._solver_name in _nlp_solvers:
            self._opt = pk.nlp_solver(self._solver_name)
            nlp = pk.nlp_model(pm, "cppad")
            self._poek_nlp_model = nlp
            self._opt.load(nlp)
        else:
            self._opt = pk.solver(self._solver_name)
            self._opt.load(pm)
        timer.stop("coek load")

    def _set_options(self):
        for key, option in self.options.items():
            self._opt.set_option(key, option)

    def resolve(self, timer: HierarchicalTimer = None):
        if timer is None:
            timer = HierarchicalTimer()

        self._set_options()

        timer.start("coek resolve")
        self._opt.resolve()
        timer.stop("coek resolve")

        res = Results()
        return res

    def solve(self, model: _BlockData, timer: HierarchicalTimer = None) -> Results:
        if timer is None:
            timer = HierarchicalTimer()

        pm = self._construct_poek_model(model, timer)

        timer.start("coek solve")
        if self._solver_name in _nlp_solvers:
            self._opt = pk.nlp_solver(self._solver_name)
            nlp = pk.nlp_model(pm, "cppad")
            self._poek_nlp_model = nlp
            self._set_options()
            _res = self._opt.solve(nlp)
        else:
            self._opt = pk.solver(self._solver_name)
            self._set_options()
            _res = self._opt.solve(pm)
        timer.stop("coek solve")

        res = Results()
        return res


class Gurobi(HybridSolver):
    def __init__(self):
        super().__init__("gurobi")
        self._config = MIPSolverConfig()

    @property
    def config(self):
        return self._config

    @config.setter
    def config(self, val):
        self._config = val

    @property
    def gurobi_options(self):
        return self._solver_options

    @gurobi_options.setter
    def gurobi_options(self, val: Dict):
        self._solver_options = val

    def _set_options(self):
        if self.config.stream_solver:
            self._opt.set_option("OutputFlag", 1)
        else:
            self._opt.set_option("OutputFlag", 0)
        super()._set_options()


class Ipopt(HybridSolver):
    def __init__(self):
        super().__init__("ipopt")
        self._config = SolverConfig()

    @property
    def config(self):
        return self._config

    @config.setter
    def config(self, val):
        self._config = val

    @property
    def ipopt_options(self):
        return self._solver_options

    @ipopt_options.setter
    def ipopt_options(self, val: Dict):
        self._solver_options = val

    def _set_options(self):
        if self.config.stream_solver:
            self._opt.set_option("print_level", 5)
        else:
            self._opt.set_option("print_level", 0)
        super()._set_options()


SolverFactory.register(name="coek_gurobi", doc="Coek interface to Gurobi")(Gurobi)
SolverFactory.register(name="coek_ipopt", doc="Coek interface to Gurobi")(Ipopt)
