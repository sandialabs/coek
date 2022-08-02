import poek as pk
from pyomo.core.base.component import ComponentData, ModelComponentFactory
from pyomo.common.modeling import NOTSET
from pyomo.core.staleflag import StaleFlagManager
from pyomo.core.base.indexed_component import IndexedComponent, UnindexedComponent_set
from pyomo.core.base.disable_methods import disable_methods
from pyomo.core.base.var import _VARDATA_API
from pyomo.core.base.initializer import Initializer, DefaultInitializer
from pyomo.core.base.set import SetInitializer
from pyomo.core.base.units_container import units
from pyomo.common.collections import Sequence
import logging
from pyomo.common.timing import ConstructionTimer
from pyomo.common.log import is_debug_set
import sys
from pyomo.core.base.set import Binary, Reals, Integers
from pyomo.common.collections import ComponentMap


logger = logging.getLogger(__name__)


_domain_map = ComponentMap()
_domain_map[Binary] = pk.VariableTypes.Binary
_domain_map[Reals] = pk.VariableTypes.Reals
_domain_map[Integers] = pk.VariableTypes.Integers

_rev_domain_map = dict()
_rev_domain_map[pk.VariableTypes.Binary] = Binary
_rev_domain_map[pk.VariableTypes.Reals] = Reals
_rev_domain_map[pk.VariableTypes.Integers] = Integers


class _GeneralVarData(ComponentData):

    __slots__ = ('_stale', '_pv')

    def __init__(self, component=None):
        super().__init__(component=component)
        self._stale = 0  # True
        self._pv = pk.variable_single()

    @classmethod
    def copy(cls, src):
        self = cls.__new__(cls)
        self._component = src._component
        self._pv = pk.variable_single()
        self._pv.value = src._pv.value
        self._pv.lb = src._pv.lb
        self._pv.ub = src._pv.ub
        self._pv.within = src._pv.within
        self._pv.fixed = src._pv.fixed
        self._stale = src._stale
        self._index = src._index
        return self

    def has_lb(self):
        return self._pv.lb > -pk.inf

    def has_ub(self):
        return self._pv.ub < pk.inf

    def setlb(self, val):
        self._pv.lb = val

    def setub(self, val):
        self._pv.ub = val

    @property
    def lb(self):
        if self.has_lb():
            return self._pv.lb
        else:
            return None

    @lb.setter
    def lb(self, val):
        if val is None:
            self._pv.lb = -pk.inf
        else:
            self._pv.lb = val

    @property
    def ub(self):
        if self.has_ub():
            return self._pv.ub
        else:
            return None

    @ub.setter
    def ub(self, val):
        if val is None:
            self._pv.ub = pk.inf
        else:
            self._pv.ub = val

    @property
    def bounds(self):
        return self.lb, self.ub

    @bounds.setter
    def bounds(self, val):
        self.lb, self.ub = val

    def is_integer(self):
        return self._pv.is_integer()

    def is_binary(self):
        return self._pv.is_binary()

    def is_continuous(self):
        return self._pv.is_continuous()

    def is_fixed(self):
        return self._pv.fixed

    def is_constant(self):
        return False

    def is_variable_type(self):
        return True

    def is_potentially_variable(self):
        return True

    def _compute_polynomial_degree(self, resut):
        if self.fixed:
            return 0
        return 1

    def clear(self):
        self._pv.value = 0

    def __call__(self, exception=True):
        return self.value

    def set_value(self, val, skip_validation=False):
        self._pv.value = val
        self._stale = StaleFlagManager.get_flag(self._stale)

    @property
    def value(self):
        return self._pv.value

    @value.setter
    def value(self, val):
        self.set_value(val)

    @property
    def domain(self):
        return _rev_domain_map[self._pv.within]

    @domain.setter
    def domain(self, val):
        self._pv.within = _domain_map[val]

    @property
    def lower(self):
        return self.lb

    @lower.setter
    def lower(self, val):
        self.lb = val

    @property
    def upper(self):
        return self.ub

    @upper.setter
    def upper(self, val):
        self.ub = val

    @property
    def fixed(self):
        return self._pv.fixed

    @fixed.setter
    def fixed(self, val):
        self._pv.fixed = val

    @property
    def stale(self):
        return StaleFlagManager.is_stale(self._stale)

    @stale.setter
    def stale(self, val):
        if val:
            self._stale = 0  # True
        else:
            self._stale = StaleFlagManager.get_flag(0)

    def fix(self, value=NOTSET, skip_validation=False):
        self.fixed = True
        if value is not NOTSET:
            self.set_value(value, skip_validation)

    def unfix(self):
        self.fixed = False

    def free(self):
        self.unfix()

    def __add__(self, other):
        return self._pv + _other_operand_map[type(other)](other)

    def __radd__(self, other):
        return _other_operand_map[type(other)](other) + self._pv

    def __sub__(self, other):
        return self._pv - _other_operand_map[type(other)](other)

    def __rsub__(self, other):
        return _other_operand_map[type(other)](other) - self._pv

    def __mul__(self, other):
        return self._pv * _other_operand_map[type(other)](other)

    def __rmul__(self, other):
        return _other_operand_map[type(other)](other) * self._pv

    def __neg__(self):
        return -self._pv

    def __truediv__(self, other):
        return self._pv / _other_operand_map[type(other)](other)

    def __rtruediv__(self, other):
        return _other_operand_map[type(other)](other) / self._pv

    def __div__(self, other):
        return self._pv / _other_operand_map[type(other)](other)

    def __rdiv__(self, other):
        return _other_operand_map[type(other)](other) / self._pv

    def __pow__(self, other):
        return self._pv ** _other_operand_map[type(other)](other)

    def __rpow__(self, other):
        return _other_operand_map[type(other)](other) ** self._pv

    def __pos__(self):
        return self

    def __ge__(self, other):
        return self._pv >= _other_operand_map[type(other)](other)

    def __le__(self, other):
        return self._pv <= _other_operand_map[type(other)](other)

    def is_numeric_type(self):
        return True


@ModelComponentFactory.register("Poek decision variables")
class Var(IndexedComponent):

    _ComponentDataClass = _GeneralVarData

    def __new__(cls, *args, **kwargs):
        if cls is not Var:
            return super(Var, cls).__new__(cls)
        if not args or (args[0] is UnindexedComponent_set and len(args) == 1):
            return super(Var, cls).__new__(AbstractScalarVar)
        else:
            return super(Var, cls).__new__(IndexedVar)

    def __init__(self, *args, **kwargs):
        self._rule_init = Initializer(
            self._pop_from_kwargs(
                'Var', kwargs, ('rule', 'initialize'), 0
            )
        )
        self._rule_domain = Initializer(
            self._pop_from_kwargs(
                'Var', kwargs, ('domain', 'within'), Reals
            )
        )
        _bounds_arg = kwargs.pop('bounds', None)
        self._dense = kwargs.pop('dense', True)
        self._units = kwargs.pop('units', None)
        if self._units is not None:
            self._units = units.get_units(self._units)

        kwargs.setdefault('ctype', Var)
        IndexedComponent.__init__(self, *args, **kwargs)

        if self.is_indexed():
            treat_bounds_sequences_as_mappings = not (
                isinstance(_bounds_arg, Sequence)
                and len(_bounds_arg) == 2
                and not isinstance(_bounds_arg[0], Sequence)
            )
        else:
            treat_bounds_sequences_as_mappings = False
            if not self._dense:
                logger.warning(
                    f"ScalarVar object {self.name}: dense=False is not allowed for "
                    f"scalar variables; converting to dense=True"
                )
                self._dense = True
        self._rule_bounds = Initializer(
            _bounds_arg,
            treat_sequences_as_mappings=treat_bounds_sequences_as_mappings
        )

    def flag_as_stale(self):
        for v in self._data.values():
            v.stale = True

    def get_values(self, include_fixed_values=True):
        if include_fixed_values:
            return {idx: v.value for idx, v in self._data.items()}
        return {idx: v.value for idx, v in self._data.items() if not v.fixed}

    extract_values = get_values

    def set_values(self, new_values, skip_validation=False):
        for ndx, val in new_values.items():
            self[ndx].set_value(val, skip_validation)

    def get_units(self):
        return self._units

    def add(self, index):
        return self[index]

    def construct(self, data=None):
        """
        Construct the _VarData objects for this variable
        """
        if self._constructed:
            return
        self._constructed=True

        timer = ConstructionTimer(self)
        if is_debug_set(logger):
            logger.debug("Constructing Variable %s" % (self.name,))

        # Note: define 'index' to avoid 'variable referenced before
        # assignment' in the error message generated in the 'except:'
        # block below.
        index = None
        try:
            # We do not (currently) accept data for constructing Variables
            assert data is None

            if not self.index_set().isfinite() and self._dense:
                # Note: if the index is not finite, then we cannot
                # iterate over it.  This used to be fatal; now we
                # just warn
                logger.warning(
                    "Var '%s' indexed by a non-finite set, but declared "
                    "with 'dense=True'.  Reverting to 'dense=False' as "
                    "it is not possible to make this variable dense.  "
                    "This warning can be suppressed by specifying "
                    "'dense=False'" % (self.name,))
                self._dense = False

            if ( self._rule_init is not None and
                 self._rule_init.contains_indices() ):
                # Historically we have allowed Vars to be initialized by
                # a sparse map (i.e., a dict containing only some of the
                # keys).  We will wrap the incoming initializer to map
                # KeyErrors to None
                self._rule_init = DefaultInitializer(
                    self._rule_init, None, KeyError)
                # The index is coming in externally; we need to validate it
                for index in self._rule_init.indices():
                    self[index]
                # If this is a dense object, we need to ensure that it
                # has been filled in.
                if self._dense:
                    for index in self.index_set():
                        if index not in self._data:
                            self._getitem_when_not_present(index)
            elif not self.is_indexed():
                # As there is only a single VarData to populate, just do
                # so and bypass all special-case testing below
                self._getitem_when_not_present(None)
            elif self._dense:
                # Special case: initialize every VarData.  For the
                # initializers that are constant, we can avoid
                # re-calling (and re-validating) the inputs in certain
                # cases.  To support this, we will create the first
                # _VarData and then use it as a template to initialize
                # (constant portions of) every VarData so as to not
                # repeat all the domain/bounds validation.
                try:
                    ref = self._getitem_when_not_present(
                        next(iter(self.index_set())))
                except StopIteration:
                    # Empty index!
                    return
                call_domain_rule = not self._rule_domain.constant()
                call_bounds_rule = self._rule_bounds is not None and (
                        not self._rule_bounds.constant())
                call_init_rule = self._rule_init is not None and (
                    not self._rule_init.constant()
                    # If either the domain or bounds change, then we
                    # need to re-verify the initial value, even if it is
                    # constant:
                    or call_domain_rule or call_bounds_rule
                )
                # Initialize all the component datas with the common data
                for index in self.index_set():
                    self._data[index] = self._ComponentDataClass.copy(ref)
                    # NOTE: This is a special case where a key, value pair is
                    # added to the _data dictionary without calling
                    # _getitem_when_not_present, which is why we need to set the
                    # index here.
                    self._data[index]._index = index
                # Now go back and initialize any index-specific data
                block = self.parent_block()
                if call_domain_rule:
                    for index, obj in self._data.items():
                        # We can directly set the attribute (not the
                        # property) because the SetInitializer ensures
                        # that the value is a proper Set.
                        obj.domain = self._rule_domain(block, index)
                if call_bounds_rule:
                    for index, obj in self._data.items():
                        obj.lower, obj.upper = self._rule_bounds(block, index)
                if call_init_rule:
                    for index, obj in self._data.items():
                        obj.set_value(self._rule_init(block, index))
            else:
                # non-dense indexed var with generic
                # (non-index-containing) initializer: nothing to do
                pass

        except Exception:
            err = sys.exc_info()[1]
            logger.error(
                "Rule failed when initializing variable for "
                "Var %s with index %s:\n%s: %s"
                % (self.name,
                   str(index),
                   type(err).__name__,
                   err))
            raise
        finally:
            timer.report()

    #
    # This method must be defined on subclasses of
    # IndexedComponent that support implicit definition
    #
    def _getitem_when_not_present(self, index):
        """Returns the default component data value."""
        if index is None and not self.is_indexed():
            obj = self._data[index] = self
        else:
            obj = self._data[index] = self._ComponentDataClass(component=self)
        parent = self.parent_block()
        obj._index = index
        # We can directly set the attribute (not the property) because
        # the SetInitializer ensures that the value is a proper Set.
        obj.domain = self._rule_domain(parent, index)
        if self._rule_bounds is not None:
            obj.lower, obj.upper = self._rule_bounds(parent, index)
        if self._rule_init is not None:
            obj.set_value(self._rule_init(parent, index))
        return obj

    #
    # Because we need to do more initialization than simply calling
    # set_value(), we need to override _setitem_when_not_present
    #
    def _setitem_when_not_present(self, index, value=NOTSET):
        if value is self.Skip:
            return None
        try:
            obj = self._getitem_when_not_present(index)
            if value is not NOTSET:
                obj.set_value(value)
        except:
            self._data.pop(index, None)
            raise
        return obj

    def _pprint(self):
        """Print component information."""
        headers = [
            ("Size", len(self)),
            ("Index", self._index_set if self.is_indexed() else None),
        ]
        if self._units is not None:
            headers.append(('Units', str(self._units)))
        return ( headers,
                 self._data.items(),
                 ( "Lower","Value","Upper","Fixed","Stale","Domain"),
                 lambda k, v: [ v.lb,
                                v.value,
                                v.ub,
                                v.fixed,
                                v.stale,
                                v.domain
                                ]
                 )


class ScalarVar(_GeneralVarData, Var):
    def __init__(self, *args, **kwd):
        _GeneralVarData.__init__(self, component=self)
        Var.__init__(self, *args, **kwd)
        self._index = UnindexedComponent_set


@disable_methods(_VARDATA_API)
class AbstractScalarVar(ScalarVar):
    pass


class IndexedVar(Var):
    """An array of variables."""

    def setlb(self, val):
        """
        Set the lower bound for this variable.
        """
        for vardata in self.values():
            vardata.lower = val

    def setub(self, val):
        """
        Set the upper bound for this variable.
        """
        for vardata in self.values():
            vardata.upper = val

    def fix(self, value=NOTSET, skip_validation=False):
        """Fix all variables in this :class:`IndexedVar` (treat as nonvariable)

        This sets the :attr:`fixed` indicator to True for every variable
        in this IndexedVar.  If ``value`` is provided, the value (and
        the ``skip_validation`` flag) are first passed to
        :meth:`set_value`.

        """
        for vardata in self.values():
            vardata.fix(value, skip_validation)

    def unfix(self):
        """Unfix all variables in this :class:`IndexedVar` (treat as variable)

        This sets the :attr:`_VarData.fixed` indicator to False for
        every variable in this :class:`IndexedVar`.

        """
        for vardata in self.values():
            vardata.unfix()

    def free(self):
        """Alias for :meth:`unfix`"""
        return self.unfix()

    @property
    def domain(self):
        raise AttributeError(
            "The domain is not an attribute for IndexedVar. It "
            "can be set for all indices using this property setter, "
            "but must be accessed for individual variables in this container.")
    @domain.setter
    def domain(self, domain):
        """Sets the domain for all variables in this container."""
        # TODO: Ideally we would pass valid arguments to the initializer
        # that we just created.  However at the moment, getting the
        # index() is expensive (see #1228).  As a result, for the moment
        # we will only support constant initializers
        domain = SetInitializer(domain)(None, None)
        for vardata in self.values():
            vardata.domain = domain


def _get_other_operand_float(operand):
    return operand


def _get_other_operand_poek_expr(operand):
    return operand


def _get_other_operand_var(operand):
    return operand._pv


_other_operand_map = dict()
_other_operand_map[float] = _get_other_operand_float
_other_operand_map[int] = _get_other_operand_float
_other_operand_map[_GeneralVarData] = _get_other_operand_var
_other_operand_map[ScalarVar] = _get_other_operand_var
_other_operand_map[pk.expression] = _get_other_operand_poek_expr
