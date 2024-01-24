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
from pyomo.core.base.set import Binary, Reals, Integers, Any, GlobalSetBase
from pyomo.common.collections import ComponentMap
from weakref import ref as weakref_ref
from pyomo.common.deprecation import deprecation_warning
from pyomo.core.expr.numvalue import native_types
import types
from pyomo.core.base.misc import apply_indexed_rule, apply_parameterized_indexed_rule
from pyomo.core.base.global_set import UnindexedComponent_index


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

    __slots__ = ("_stale", "_pe")

    def __init__(self, component=None):
        super().__init__(component=component)
        self._stale = 0  # True
        self._pe = pk.variable_single()

    @classmethod
    def copy(cls, src, poek_var, index):
        self = cls.__new__(cls)
        self._component = src._component
        self._pe = poek_var
        self._stale = src._stale
        self._index = index
        return self

    def has_lb(self):
        return self._pe.lb > -pk.inf

    def has_ub(self):
        return self._pe.ub < pk.inf

    def setlb(self, val):
        self._pe.lb = val

    def setub(self, val):
        self._pe.ub = val

    @property
    def lb(self):
        if self.has_lb():
            return self._pe.lb
        else:
            return None

    @lb.setter
    def lb(self, val):
        if val is None:
            self._pe.lb = -pk.inf
        else:
            self._pe.lb = val

    @property
    def ub(self):
        if self.has_ub():
            return self._pe.ub
        else:
            return None

    @ub.setter
    def ub(self, val):
        if val is None:
            self._pe.ub = pk.inf
        else:
            self._pe.ub = val

    @property
    def bounds(self):
        return self.lb, self.ub

    @bounds.setter
    def bounds(self, val):
        self.lb, self.ub = val

    def is_integer(self):
        return self._pe.is_integer()

    def is_binary(self):
        return self._pe.is_binary()

    def is_continuous(self):
        return self._pe.is_continuous()

    def is_fixed(self):
        return self._pe.fixed

    def is_constant(self):
        return False

    def is_parameter_type(self):
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
        self._pe.value = 0

    def __call__(self, exception=True):
        return self.value

    def set_value(self, val, skip_validation=False):
        self._pe.value = val
        self._stale = StaleFlagManager.get_flag(self._stale)

    @property
    def value(self):
        return self._pe.value

    @value.setter
    def value(self, val):
        self.set_value(val)

    @property
    def domain(self):
        return _rev_domain_map[self._pe.within]

    @domain.setter
    def domain(self, val):
        self._pe.within = _domain_map[val]

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
        return self._pe.fixed

    @fixed.setter
    def fixed(self, val):
        self._pe.fixed = val

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
        return self._pe + _other_operand_map[type(other)](other)

    def __radd__(self, other):
        return _other_operand_map[type(other)](other) + self._pe

    def __sub__(self, other):
        return self._pe - _other_operand_map[type(other)](other)

    def __rsub__(self, other):
        return _other_operand_map[type(other)](other) - self._pe

    def __mul__(self, other):
        return self._pe * _other_operand_map[type(other)](other)

    def __rmul__(self, other):
        return _other_operand_map[type(other)](other) * self._pe

    def __neg__(self):
        return -self._pe

    def __truediv__(self, other):
        return self._pe / _other_operand_map[type(other)](other)

    def __rtruediv__(self, other):
        return _other_operand_map[type(other)](other) / self._pe

    def __div__(self, other):
        return self._pe / _other_operand_map[type(other)](other)

    def __rdiv__(self, other):
        return _other_operand_map[type(other)](other) / self._pe

    def __pow__(self, other):
        func, other = _var_pow_map[type(other)](other)
        return func(self._pe, other)
        # return self._pe ** _other_operand_map[type(other)](other)

    def __rpow__(self, other):
        return _other_operand_map[type(other)](other) ** self._pe

    def __pos__(self):
        return self

    def __ge__(self, other):
        return self._pe >= _other_operand_map[type(other)](other)

    def __le__(self, other):
        return self._pe <= _other_operand_map[type(other)](other)

    def __eq__(self, other):
        func, other = _var_eq_map[type(other)](other)
        return func(self._pe, other)
        # return self._pe == _other_operand_map[type(other)](other)

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
            self._pop_from_kwargs("Var", kwargs, ("rule", "initialize"), 0)
        )
        self._rule_domain = Initializer(
            self._pop_from_kwargs("Var", kwargs, ("domain", "within"), Reals)
        )
        _bounds_arg = kwargs.pop("bounds", None)
        self._dense = kwargs.pop("dense", True)
        self._units = kwargs.pop("units", None)
        if self._units is not None:
            self._units = units.get_units(self._units)

        kwargs.setdefault("ctype", Var)
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
            _bounds_arg, treat_sequences_as_mappings=treat_bounds_sequences_as_mappings
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
        self._constructed = True

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
                    "'dense=False'" % (self.name,)
                )
                self._dense = False

            if self._rule_init is not None and self._rule_init.contains_indices():
                # Historically we have allowed Vars to be initialized by
                # a sparse map (i.e., a dict containing only some of the
                # keys).  We will wrap the incoming initializer to map
                # KeyErrors to None
                self._rule_init = DefaultInitializer(self._rule_init, None, KeyError)
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
                    ref = self._getitem_when_not_present(next(iter(self.index_set())))
                except StopIteration:
                    # Empty index!
                    return
                call_domain_rule = not self._rule_domain.constant()
                call_bounds_rule = self._rule_bounds is not None and (
                    not self._rule_bounds.constant()
                )
                call_init_rule = self._rule_init is not None and (
                    not self._rule_init.constant()
                    # If either the domain or bounds change, then we
                    # need to re-verify the initial value, even if it is
                    # constant:
                    or call_domain_rule
                    or call_bounds_rule
                )
                # Initialize all the component datas with the common data
                poek_var_list = pk.copy_var(ref._pe, len(self.index_set()))
                for pk_var, index in zip(poek_var_list, self.index_set()):
                    self._data[index] = self._ComponentDataClass.copy(ref, pk_var, index)
                    # NOTE: This is a special case where a key, value pair is
                    # added to the _data dictionary without calling
                    # _getitem_when_not_present, which is why we need to set the
                    # index here.
                    # self._data[index]._index = index
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
                "Var %s with index %s:\n%s: %s" % (self.name, str(index), type(err).__name__, err)
            )
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
            headers.append(("Units", str(self._units)))
        return (
            headers,
            self._data.items(),
            ("Lower", "Value", "Upper", "Fixed", "Stale", "Domain"),
            lambda k, v: [v.lb, v.value, v.ub, v.fixed, v.stale, v.domain],
        )


class ScalarVar(_GeneralVarData, Var):
    def __init__(self, *args, **kwd):
        _GeneralVarData.__init__(self, component=self)
        Var.__init__(self, *args, **kwd)
        self._index = UnindexedComponent_set

    @property
    def _name(self):
        return self._pe.get_name()

    @_name.setter
    def _name(self, val):
        self._pe.set_name(val)


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
            "but must be accessed for individual variables in this container."
        )

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


def _raise_modifying_immutable_error(obj, index):
    if obj.is_indexed():
        name = "%s[%s]" % (obj.name, index)
    else:
        name = obj.name
    raise TypeError(
        "Attempting to set the value of the immutable parameter "
        "%s after the parameter has been constructed.  If you intend "
        "to change the value of this parameter dynamically, please "
        "declare the parameter as mutable [i.e., Param(mutable=True)]" % (name,)
    )


class _ImplicitAny(Any.__class__):
    """An Any that issues a deprecation warning for non-Real values.

    This is a helper class to implement the deprecation warnings for the
    change of Param's implicit domain from Any to Reals.

    """

    def __new__(cls, **kwargs):
        # Strip off owner / kwargs before calling base __new__
        return super().__new__(cls)

    def __init__(self, owner, **kwargs):
        self._owner = weakref_ref(owner)
        super().__init__(**kwargs)
        self._component = weakref_ref(self)
        self.construct()

    def __getstate__(self):
        state = super().__getstate__()
        state["_owner"] = None if self._owner is None else self._owner()
        return state

    def __setstate__(self, state):
        _owner = state.pop("_owner")
        super().__setstate__(state)
        self._owner = None if _owner is None else weakref_ref(_owner)

    def __deepcopy__(self, memo):
        # Note: we need to start super() at GlobalSetBase to actually
        # copy this object
        return super(GlobalSetBase, self).__deepcopy__(memo)

    def __contains__(self, val):
        if val not in Reals:
            if self._owner is None or self._owner() is None:
                name = "Unknown"
            else:
                name = self._owner().name
            deprecation_warning(
                f"Param '{name}' declared with an implicit domain of 'Any'. "
                "The default domain for Param objects is 'Any'.  However, "
                "we will be changing that default to 'Reals' in the "
                "future.  If you really intend the domain of this Param"
                "to be 'Any', you can suppress this warning by explicitly "
                "specifying 'within=Any' to the Param constructor.",
                version="5.6.9",
                remove_in="6.0",
            )
        return True

    # This should "mock up" a global set, so the "name" should always be
    # the local name (without block scope)
    def getname(self, fully_qualified=False, name_buffer=None, relative_to=None):
        return super().getname(False, name_buffer, relative_to)

    # The parent tracks the parent of the owner.  We can't set it
    # directly here because the owner has not been assigned to a block
    # when we create the _ImplicitAny
    @property
    def _parent(self):
        if self._owner is None or self._owner() is None:
            return None
        return self._owner()._parent

    # This is not settable.  However the base classes assume that it is,
    # so we need to define the setter and just ignore the incoming value
    @_parent.setter
    def _parent(self, val):
        pass


class _ParamData(ComponentData):

    __slots__ = ("_pe",)

    def __init__(self, component):
        super().__init__(component=component)
        self._pe = pk.parameter_single()

    def clear(self):
        self.value = 0

    def set_value(self, value, idx=NOTSET):
        self._pe.value = value

    def __call__(self, exception=True):
        return self._pe.value

    @property
    def value(self):
        return self()

    @value.setter
    def value(self, val):
        self.set_value(val)

    @property
    def fixed(self):
        return True

    def is_fixed(self):
        return True

    def is_constant(self):
        return False

    def is_parameter_type(self):
        return True

    def is_variable_type(self):
        return False

    def is_potentially_variable(self):
        return False

    def is_numeric_type(self):
        return True

    def _compute_polynomial_degree(self, result):
        return 0

    def __add__(self, other):
        return self._pe + _other_operand_map[type(other)](other)

    def __radd__(self, other):
        return _other_operand_map[type(other)](other) + self._pe

    def __sub__(self, other):
        return self._pe - _other_operand_map[type(other)](other)

    def __rsub__(self, other):
        return _other_operand_map[type(other)](other) - self._pe

    def __mul__(self, other):
        return self._pe * _other_operand_map[type(other)](other)

    def __rmul__(self, other):
        return _other_operand_map[type(other)](other) * self._pe

    def __neg__(self):
        return -self._pe

    def __truediv__(self, other):
        return self._pe / _other_operand_map[type(other)](other)

    def __rtruediv__(self, other):
        return _other_operand_map[type(other)](other) / self._pe

    def __div__(self, other):
        return self._pe / _other_operand_map[type(other)](other)

    def __rdiv__(self, other):
        return _other_operand_map[type(other)](other) / self._pe

    def __pow__(self, other):
        return self._pe ** _other_operand_map[type(other)](other)

    def __rpow__(self, other):
        return _other_operand_map[type(other)](other) ** self._pe

    def __pos__(self):
        return self

    def __ge__(self, other):
        return self._pe >= _other_operand_map[type(other)](other)

    def __le__(self, other):
        return self._pe <= _other_operand_map[type(other)](other)

    def __eq__(self, other):
        return self._pe == _other_operand_map[type(other)](other)


@ModelComponentFactory.register("Poek parameter")
class Param(IndexedComponent):

    DefaultMutable = False

    def __new__(cls, *args, **kwds):
        if cls != Param:
            return super(Param, cls).__new__(cls)
        if not args or (args[0] is UnindexedComponent_set and len(args) == 1):
            return super(Param, cls).__new__(ScalarParam)
        else:
            return super(Param, cls).__new__(IndexedParam)

    def __init__(self, *args, **kwd):
        _init = self._pop_from_kwargs("Param", kwd, ("rule", "initialize"), NOTSET)
        self.domain = self._pop_from_kwargs("Param", kwd, ("domain", "within"))
        self._validate = kwd.pop("validate", None)
        self._mutable = kwd.pop("mutable", Param.DefaultMutable)
        self._default_val = kwd.pop("default", 0)
        self._dense_initialize = kwd.pop("initialize_as_dense", False)
        self._units = kwd.pop("units", None)
        if self._units is not None:
            self._units = units.get_units(self._units)
            self._mutable = True

        kwd.setdefault("ctype", Param)
        IndexedComponent.__init__(self, *args, **kwd)

        if self.domain is None:
            self.domain = _ImplicitAny(owner=self, name="Any")
        # After IndexedComponent.__init__ so we can call is_indexed().
        self._rule = Initializer(
            _init, treat_sequences_as_mappings=self.is_indexed(), arg_not_specified=NOTSET
        )

    def __len__(self):
        """
        Return the number of component data objects stored by this
        component.  If a default value is specified, then the
        length equals the number of items in the component index.
        """
        return len(self._index_set)

    def __contains__(self, idx):
        """
        Return true if the index is in the dictionary.  If the default value
        is specified, then all members of the component index are valid.
        """
        return idx in self._index_set

    # We do not need to override keys(), as the __len__ override will
    # cause the base class keys() to correctly correctly handle default
    # values
    # def keys(self, ordered=False):

    @property
    def mutable(self):
        return self._mutable

    def get_units(self):
        """Return the units for this ParamData"""
        return self._units

    #
    # These are "sparse equivalent" access / iteration methods that
    # only loop over the defined data.
    #

    def sparse_keys(self):
        """Return a list of keys in the defined parameters"""
        return list(self._data.keys())

    def sparse_values(self):
        """Return a list of the defined param data objects"""
        return list(self._data.values())

    def sparse_items(self):
        """Return a list (index,data) tuples for defined parameters"""
        return list(self._data.items())

    def sparse_iterkeys(self):
        """Return an iterator for the keys in the defined parameters"""
        return self._data.keys()

    def sparse_itervalues(self):
        """Return an iterator for the defined param data objects"""
        return self._data.values()

    def sparse_iteritems(self):
        """Return an iterator of (index,data) tuples for defined parameters"""
        return self._data.items()

    def extract_values(self):
        """
        A utility to extract all index-value pairs defined for this
        parameter, returned as a dictionary.

        This method is useful in contexts where key iteration and
        repeated __getitem__ calls are too expensive to extract
        the contents of a parameter.
        """
        if self._mutable:
            #
            # The parameter is mutable, parameter data are ParamData types.
            # Thus, we need to create a temporary dictionary that contains the
            # values from the ParamData objects.
            #
            return {key: param_value() for key, param_value in self.items()}
        elif not self.is_indexed():
            #
            # The parameter is a scalar, so we need to create a temporary
            # dictionary using the value for this parameter.
            #
            return {None: self()}
        else:
            #
            # The parameter is not mutable, so iteritems() can be
            # converted into a dictionary containing parameter values.
            #
            return dict(self.items())

    def extract_values_sparse(self):
        """
        A utility to extract all index-value pairs defined with non-default
        values, returned as a dictionary.

        This method is useful in contexts where key iteration and
        repeated __getitem__ calls are too expensive to extract
        the contents of a parameter.
        """
        if self._mutable:
            #
            # The parameter is mutable, parameter data are ParamData types.
            # Thus, we need to create a temporary dictionary that contains the
            # values from the ParamData objects.
            #
            ans = {}
            for key, param_value in self.sparse_iteritems():
                ans[key] = param_value()
            return ans
        elif not self.is_indexed():
            #
            # The parameter is a scalar, so we need to create a temporary
            # dictionary using the value for this parameter.
            #
            return {None: self()}
        else:
            #
            # The parameter is not mutable, so sparse_iteritems() can be
            # converted into a dictionary containing parameter values.
            #
            return dict(self.sparse_iteritems())

    def store_values(self, new_values, check=True):
        """
        A utility to update a Param with a dictionary or scalar.

        If check=True, then both the index and value
        are checked through the __getitem__ method.  Using check=False
        should only be used by developers!
        """
        if not self._mutable:
            _raise_modifying_immutable_error(self, "*")
        #
        _srcType = type(new_values)
        _isDict = False
        if _srcType is dict:
            _isDict = True
        elif hasattr(new_values, "__getitem__"):
            if isinstance(new_values, IndexedComponent):
                if new_values.is_indexed():
                    _isDict = True
            else:
                _isDict = True

        #
        if check:
            if _isDict:
                for index, new_value in new_values.items():
                    self[index] = new_value
            else:
                for index in self._index_set:
                    self[index] = new_values
            return
        #
        # The argument check is False, so we bypass almost all of the
        # Param logic for ensuring data integrity.
        #
        if self.is_indexed():
            if _isDict:
                # It is possible that the Param is sparse and that the
                # index is not already in the _data dict.  As these
                # cases are rare, we will recover from the exception
                # instead of incurring the penalty of checking.
                for index, new_value in new_values.items():
                    if index not in self._data:
                        self._data[index] = _ParamData(self)
                    self._data[index].value = new_value
            else:
                # For scalars, we will choose an approach based on
                # how "dense" the Param is
                if not self._data:  # empty
                    for index in self._index_set:
                        p = self._data[index] = _ParamData(self)
                        p.value = new_values
                elif len(self._data) == len(self._index_set):
                    for index in self._index_set:
                        self._data[index].value = new_values
                else:
                    for index in self._index_set:
                        if index not in self._data:
                            self._data[index] = _ParamData(self)
                        self._data[index].value = new_values
        else:
            #
            # Initialize a scalar
            #
            if _isDict:
                if None not in new_values:
                    raise RuntimeError(
                        "Cannot store value for scalar Param %s:\n\tNo value "
                        "with index None in the new values dict." % (self.name,)
                    )
                new_values = new_values[None]
            # scalars have to be handled differently
            self[None] = new_values

    def set_default(self, val):
        """
        Perform error checks and then set the default value for this parameter.

        NOTE: this test will not validate the value of function return values.
        """
        if self._constructed and type(val) in native_types and val not in self.domain:
            raise ValueError(
                "Default value (%s) is not valid for Param %s domain %s"
                % (str(val), self.name, self.domain.name)
            )
        self._default_val = val

    def default(self):
        """
        Return the value of the parameter default.

        Possible values:
            Numeric
                A constant value that is the default value for all undefined
                parameters.
            Function
                f(model, i) returns the value for the default value for
                parameter i
        """
        return self._default_val

    def _getitem_when_not_present(self, index):
        """
        Returns the default component data value
        """
        #
        # Local values
        #
        val = self._default_val

        _default_type = type(val)
        _check_value_domain = True

        _isDict = False
        if _default_type is dict:
            _isDict = True
        elif hasattr(val, "__getitem__"):
            if isinstance(val, IndexedComponent):
                if val.is_indexed():
                    _isDict = True
            else:
                _isDict = True

        if _default_type in native_types:
            #
            # The set_default() method validates the domain of native types, so
            # we can skip the check on the value domain.
            #
            _check_value_domain = False
        elif _default_type is types.FunctionType:
            val = apply_indexed_rule(self, val, self.parent_block(), index)
        elif _isDict:
            # Things that look like Dictionaries should be allowable.  This
            # includes other IndexedComponent objects.
            val = val[index]
        else:
            # this is something simple like a non-indexed component
            pass

        #
        # If the user wants to validate values, we need to validate the
        # default value as well. For Mutable Params, this is easy:
        # _setitem_impl will inject the value into _data and
        # then call validate.
        #
        if self._mutable:
            return self._setitem_when_not_present(index, val)
        #
        # For immutable params, we never inject the default into the data
        # dictionary.  This will break validation, as the validation rule is
        # allowed to assume the data is already present (actually, it will
        # die on infinite recursion, as Param.__getitem__() will re-call
        # _getitem_when_not_present).
        #
        # So, we will do something very inefficient: if we are
        # validating, we will inject the value into the dictionary,
        # call validate, and remove it.
        #
        if _check_value_domain or self._validate:
            try:
                self._data[index] = val
                self._validate_value(index, val, _check_value_domain)
            finally:
                del self._data[index]

        return val

    def _setitem_impl(self, index, obj, value):
        """The __setitem__ method performs significant validation around the
        input indices, particularly when the index value is new.  In
        various contexts, we don't need to incur this overhead
        (e.g. during initialization).  The _setitem_impl
        assumes the input value is in the set native_types

        """
        #
        # We need to ensure that users don't override the value for immutable
        # parameters.
        #
        if self._constructed and not self._mutable:
            _raise_modifying_immutable_error(self, index)
        #
        # Params should contain *values*.  Note that if we just call
        # value(), then that forces the value to be a numeric value.
        # Notably, we allow Params with domain==Any to hold strings, tuples,
        # etc.  The following lets us use NumericValues to initialize
        # Params, but is optimized to check for "known" native types to
        # bypass a potentially expensive isinstance()==False call.
        #
        if value.__class__ not in native_types:
            # if isinstance(value, NumericValue):
            value = value()
        #
        # Set the value depending on the type of param value.
        #
        if self._mutable:
            obj.set_value(value, index)
            return obj
        else:
            old_value, self._data[index] = self._data[index], value
            # Because we do not have a _ParamData, we cannot rely on the
            # validation that occurs in _ParamData.set_value()
            try:
                self._validate_value(index, value)
                return value
            except:
                self._data[index] = old_value
                raise

    def _setitem_when_not_present(self, index, value, _check_domain=True):
        #
        # We need to ensure that users don't override the value for immutable
        # parameters.
        #
        if self._constructed and not self._mutable:
            _raise_modifying_immutable_error(self, index)
        #
        # Params should contain *values*.  Note that if we just call
        # value(), then that forces the value to be a numeric value.
        # Notably, we allow Params with domain==Any to hold strings, tuples,
        # etc.  The following lets us use NumericValues to initialize
        # Params, but is optimized to check for "known" native types to
        # bypass a potentially expensive isinstance()==False call.
        #
        if value.__class__ not in native_types:
            # if isinstance(value, NumericValue):
            value = value()

        #
        # Set the value depending on the type of param value.
        #
        try:
            if index is None and not self.is_indexed():
                self._data[None] = self
                self.set_value(value, index)
                self._index = UnindexedComponent_index
                return self
            elif self._mutable:
                obj = self._data[index] = _ParamData(self)
                obj.set_value(value, index)
                obj._index = index
                return obj
            else:
                self._data[index] = value
                # Because we do not have a _ParamData, we cannot rely on the
                # validation that occurs in _ParamData.set_value()
                self._validate_value(index, value, _check_domain)
                return value
        except:
            del self._data[index]
            raise

    def _validate_value(self, index, value, validate_domain=True, data=None):
        """
        Validate a given input/value pair.
        """
        #
        # Check if the value is valid within the current domain
        #
        if validate_domain and not value in self.domain:
            if index is NOTSET:
                index = data.index()
            raise ValueError(
                "Invalid parameter value: %s[%s] = '%s', value type=%s.\n"
                "\tValue not in parameter domain %s"
                % (self.name, index, value, type(value), self.domain.name)
            )
        if self._validate:
            if index is NOTSET:
                index = data.index()
            valid = apply_parameterized_indexed_rule(
                self, self._validate, self.parent_block(), value, index
            )
            if not valid:
                raise ValueError(
                    "Invalid parameter value: %s[%s] = '%s', value type=%s.\n"
                    "\tValue failed parameter validation rule"
                    % (self.name, index, value, type(value))
                )

    def construct(self, data=None):
        """
        Initialize this component.

        A parameter is constructed using the initial data or
        the data loaded from an external source.  We first
        set all the values based on self._rule, and then
        allow the data dictionary to overwrite anything.

        Note that we allow an undefined Param value to be
        constructed.  We throw an exception if a user tries
        to use an uninitialized Param.
        """
        if self._constructed:
            return

        timer = ConstructionTimer(self)
        if is_debug_set(logger):  # pragma:nocover
            logger.debug("Constructing Param, name=%s, from data=%s" % (self.name, str(data)))

        try:
            #
            # If the default value is a simple type, we check it versus
            # the domain.
            #
            val = self._default_val
            if type(val) in native_types and val not in self.domain:
                raise ValueError(
                    "Default value (%s) is not valid for Param %s domain %s"
                    % (str(val), self.name, self.domain.name)
                )
            #
            # Flag that we are in the "during construction" phase
            #
            self._constructed = None
            #
            # Step #1: initialize data from rule value
            #
            self._construct_from_rule_using_setitem()
            #
            # Step #2: allow any user-specified (external) data to override
            # the initialization
            #
            if data is not None:
                try:
                    data_items = data.items()
                except AttributeError:
                    raise ValueError(
                        "Attempting to initialize parameter=%s with data=%s.\n"
                        "\tData type is not a mapping type, and a Mapping is "
                        "expected." % (self.name, str(data))
                    )
            else:
                data_items = iter(())

            try:
                for key, val in data_items:
                    self._setitem_when_not_present(self._validate_index(key), val)
            except:
                msg = sys.exc_info()[1]
                raise RuntimeError(
                    "Failed to set value for param=%s, index=%s, value=%s.\n"
                    "\tsource error message=%s" % (self.name, str(key), str(val), str(msg))
                )
            #
            # Flag that things are fully constructed now (and changing an
            # immutable Param is now an exception).
            #
            self._constructed = True

            # populate all other indices with default data
            # (avoids calling _set_contains on self._index_set at runtime)
            if self._dense_initialize:
                self.to_dense_data()
        finally:
            timer.report()

    def _pprint(self):
        """
        Return data that will be printed for this component.
        """
        if type(self._default_val) is types.FunctionType:
            default = "(function)"
        else:
            default = str(self._default_val)
        if self._mutable or not self.is_indexed():
            dataGen = lambda k, v: [
                v.value,
            ]
        else:
            dataGen = lambda k, v: [
                v,
            ]
        headers = [
            ("Size", len(self)),
            ("Index", self._index_set if self.is_indexed() else None),
            ("Domain", self.domain.name),
            ("Default", default),
            ("Mutable", self._mutable),
        ]
        if self._units is not None:
            headers.append(("Units", str(self._units)))
        return (
            headers,
            self.sparse_iteritems(),
            ("Value",),
            dataGen,
        )


class ScalarParam(_ParamData, Param):
    def __init__(self, *args, **kwds):
        _ParamData.__init__(self, component=self)
        Param.__init__(self, *args, **kwds)
        self._index = UnindexedComponent_index

    #
    # Since this class derives from Component and Component.__getstate__
    # just packs up the entire __dict__ into the state dict, there s
    # nothng special that we need to do here.  We will just defer to the
    # super() get/set state.  Since all of our get/set state methods
    # rely on super() to traverse the MRO, this will automatically pick
    # up both the Component and Data base classes.
    #

    def __call__(self, exception=True):
        """
        Return the value of this parameter.
        """
        if self._constructed:
            if not self._data:
                if self._mutable:
                    # This will trigger populating the _data dict and setting
                    # the _default, if applicable
                    self[None]
                else:
                    # Immutable Param defaults never get added to the
                    # _data dict
                    return self[None]
            return super(ScalarParam, self).__call__(exception=exception)
        if exception:
            raise ValueError(
                "Evaluating the numeric value of parameter '%s' before\n\t"
                "the Param has been constructed (there is currently no "
                "value to return)." % (self.name,)
            )

    @property
    def _name(self):
        return self._pe.get_name()

    @_name.setter
    def _name(self, val):
        self._pe.set_name(val)

    def set_value(self, value, index=NOTSET):
        if index is NOTSET:
            index = None
        if self._constructed and not self._mutable:
            _raise_modifying_immutable_error(self, index)
        if not self._data:
            self._data[index] = self
        super(ScalarParam, self).set_value(value, index)

    def is_constant(self):
        """Determine if this ScalarParam is constant (and can be eliminated)

        Returns False if either unconstructed or mutable, as it must be kept
        in expressions (as it either doesn't have a value yet or the value
        can change later.
        """
        return self._constructed and not self._mutable


class IndexedParam(Param):
    def __call__(self, exception=True):
        """Compute the value of the parameter"""
        if exception:
            raise TypeError("Cannot compute the value of an indexed Param (%s)" % (self.name,))

    # Because IndexedParam can use a non-standard data store (i.e., the
    # values in the _data dict may not be ComponentData objects), we
    # need to override the normal scheme for pre-allocating
    # ComponentData objects during deepcopy.
    def _create_objects_for_deepcopy(self, memo, component_list):
        _id = id(self)
        if _id not in memo:
            component_list.append(self)
            memo[_id] = self.__class__.__new__(self.__class__)
        if self.mutable:
            for obj in self._data.values():
                _id = id(obj)
                if _id in memo:
                    continue
                component_list.append(obj)
                memo[id(obj)] = obj.__class__.__new__(obj.__class__)


def _get_other_operand_float(operand):
    return operand


def _get_other_operand_poek_expr(operand):
    return operand


def _get_other_operand_var(operand):
    return operand._pe


_other_operand_map = dict()
_other_operand_map[float] = _get_other_operand_float
_other_operand_map[int] = _get_other_operand_float
_other_operand_map[_GeneralVarData] = _get_other_operand_var
_other_operand_map[ScalarVar] = _get_other_operand_var
_other_operand_map[_ParamData] = _get_other_operand_var
_other_operand_map[ScalarParam] = _get_other_operand_var
_other_operand_map[pk.expression] = _get_other_operand_poek_expr


def _get_var_pow_func_float(operand):
    return pk.var_pow_float, operand


def _get_var_pow_func_param(operand):
    return pk.var_pow_param, operand._pe


def _get_var_pow_func_var(operand):
    return pk.var_pow_var, operand._pe


def _get_var_pow_func_expr(operand):
    return pk.var_pow_expression, operand


_var_pow_map = dict()
_var_pow_map[float] = _get_var_pow_func_float
_var_pow_map[int] = _get_var_pow_func_float
_var_pow_map[_GeneralVarData] = _get_var_pow_func_var
_var_pow_map[ScalarVar] = _get_var_pow_func_var
_var_pow_map[_ParamData] = _get_var_pow_func_param
_var_pow_map[ScalarParam] = _get_var_pow_func_param
_var_pow_map[pk.expression] = _get_var_pow_func_expr


def _get_var_eq_func_float(operand):
    return pk.var_eq_float, operand


def _get_var_eq_func_param(operand):
    return pk.var_eq_param, operand._pe


def _get_var_eq_func_var(operand):
    return pk.var_eq_var, operand._pe


def _get_var_eq_func_expr(operand):
    return pk.var_eq_expression, operand


_var_eq_map = dict()
_var_eq_map[float] = _get_var_eq_func_float
_var_eq_map[int] = _get_var_eq_func_float
_var_eq_map[_GeneralVarData] = _get_var_eq_func_var
_var_eq_map[ScalarVar] = _get_var_eq_func_var
_var_eq_map[_ParamData] = _get_var_eq_func_param
_var_eq_map[ScalarParam] = _get_var_eq_func_param
_var_eq_map[pk.expression] = _get_var_eq_func_expr
