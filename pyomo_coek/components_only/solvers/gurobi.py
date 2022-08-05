from pyomo.contrib.appsi.base import (
TerminationCondition,
MIPSolverConfig,
SolutionLoader,
Results,
Solver,
SolverFactory
)
import poek as pk
from typing import Tuple, Dict
from pyomo.core.base.block import _BlockData
from pyomo.common.timing import HierarchicalTimer
import pyomo.environ as pe
from pyomo_coek.components_only.objective import Objective
from pyomo_coek.components_only.constraint import Constraint
from pyomo_coek.components_only.variable import Var


class Gurobi(Solver):
    def __init__(self):
        self._opt = pk.solver('gurobi')
        self._config = MIPSolverConfig()
        self._solver_options = dict()

    def available(self):
        if self._opt.available:
            return self.Availability.FullLicense
        else:
            return self.Availability.NotFound

    def version(self) -> Tuple:
        raise NotImplementedError('ToDo')

    @property
    def config(self):
        return self._config

    @config.setter
    def config(self, val):
        self._config = val

    @property
    def symbol_map(self):
        raise NotImplementedError('ToDo')

    @property
    def gurobi_options(self):
        return self._solver_options

    @gurobi_options.setter
    def gurobi_options(self, val: Dict):
        self._solver_options = val

    def solve(self, model: _BlockData, timer: HierarchicalTimer = None) -> Results:
        if timer is None:
            timer = HierarchicalTimer()

        timer.start('construct poek model')
        pm = pk.model()
        for v in model.component_data_objects(Var, descend_into=True):
            pm.add_variable_(v._pe)
        for c in model.component_data_objects(Constraint, active=True, descend_into=True):
            pm.add_constraint(c._pe)
        for obj in model.component_data_objects(Objective, active=True, descend_into=True):
            pm.add_objective(obj._pe)
        timer.stop('construct poek model')

        timer.start('coek solve')
        _res = self._opt.solve(pm)
        timer.stop('coek solve')

        res = Results()
        return res


SolverFactory.register(name='coek_gurobi',
                       doc='Coek interface to Gurobi')(Gurobi)
