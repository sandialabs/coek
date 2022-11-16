import operator
from pyomo.core.expr import numeric_expr
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
from pyomo.core.expr.numvalue import nonpyomo_leaf_types, native_types
from pyomo.core.expr import logical_expr
from pyomo.common.deprecation import deprecation_warning
from weakref import ref as weakref_ref
from pyomo.common.deprecation import deprecation_warning
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


_var_term_to_var_map = dict()


class _GeneralVarData(ComponentData):

    __slots__ = ("_stale", "_pe")

    def __init__(self, component=None):
        super().__init__(component=component)
        self._stale = 0  # True
        self._pe = pk.variable_single()
        _var_term_to_var_map[self._pe.repn] = self

    def __del__(self):
        del _var_term_to_var_map[self._pe.repn]

    @classmethod
    def copy(cls, src, poek_var, index):
        self = cls.__new__(cls)
        self._component = src._component
        self._pe = poek_var
        _var_term_to_var_map[self._pe.repn] = self
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
        if type(other) is SumExpression:
            return other + self
        else:
            return SumExpression([self, other])

    def __radd__(self, other):
        if type(other) in nonpyomo_leaf_types and other == 0:
            return self
        else:
            return SumExpression([other, self])

    def __sub__(self, other):
        return SumExpression([self, -other])

    def __rsub__(self, other):
        return SumExpression([other, NegationExpression((self,))])

    def __mul__(self, other):
        if type(other) in nonpyomo_leaf_types:
            if other == 0:
                return 0
            else:
                return MonomialTermExpression((other, self))
        else:
            return ProductExpression((self, other))

    def __rmul__(self, other):
        if type(other) in nonpyomo_leaf_types:
            if other == 0:
                return 0
            else:
                return MonomialTermExpression((other, self))
        else:
            return ProductExpression((self, other))

    def __neg__(self):
        return NegationExpression((self,))

    def __truediv__(self, other):
        return DivisionExpression((self, other))

    def __rtruediv__(self, other):
        return DivisionExpression((other, self))

    def __div__(self, other):
        return DivisionExpression((self, other))

    def __rdiv__(self, other):
        return DivisionExpression((other, self))

    def __pow__(self, other):
        return PowExpression((self, other))

    def __rpow__(self, other):
        return PowExpression((other, self))

    def __pos__(self):
        return self

    def __le__(self, other):
        return InequalityExpression((self, other))

    def __ge__(self, other):
        return InequalityExpression((other, self))

    def __eq__(self, other):
        return EqualityExpression((self, other))

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
        if type(other) is SumExpression:
            return other + self
        else:
            return SumExpression([self, other])

    def __radd__(self, other):
        if type(other) in nonpyomo_leaf_types and other == 0:
            return self
        else:
            return SumExpression([other, self])

    def __sub__(self, other):
        return SumExpression([self, -other])

    def __rsub__(self, other):
        return SumExpression([other, NegationExpression((self,))])

    def __mul__(self, other):
        if type(other) in nonpyomo_leaf_types:
            if other == 0:
                return 0
            else:
                return ProductExpression((self, other))
        else:
            return ProductExpression((self, other))

    def __rmul__(self, other):
        if type(other) in nonpyomo_leaf_types:
            if other == 0:
                return 0
            else:
                return ProductExpression((self, other))
        else:
            return ProductExpression((self, other))

    def __neg__(self):
        return NegationExpression((self,))

    def __truediv__(self, other):
        return DivisionExpression((self, other))

    def __rtruediv__(self, other):
        return DivisionExpression((other, self))

    def __div__(self, other):
        return DivisionExpression((self, other))

    def __rdiv__(self, other):
        return DivisionExpression((other, self))

    def __pow__(self, other):
        return PowExpression((self, other))

    def __rpow__(self, other):
        return PowExpression((other, self))

    def __pos__(self):
        return self

    def __le__(self, other):
        return InequalityExpression((self, other))

    def __ge__(self, other):
        return InequalityExpression((other, self))

    def __eq__(self, other):
        return EqualityExpression((self, other))


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
        Param.__init__(self, *args, **kwds)
        _ParamData.__init__(self, component=self)
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


class ExpressionBase(object):
    __slots__ = tuple()

    def __call__(self, exception=False):
        return self._pe.value

    def __mul__(self, other):
        return ProductExpression((self, other))

    def __rmul__(self, other):
        return ProductExpression((other, self))

    def __add__(self, other):
        if type(other) is SumExpression:
            return other + self
        else:
            return SumExpression([self, other])

    def __radd__(self, other):
        if type(other) in nonpyomo_leaf_types and other == 0:
            return self
        else:
            return SumExpression([other, self])

    def __sub__(self, other):
        return SumExpression([self, -other])

    def __rsub__(self, other):
        return SummExpression([other, NegationExpression((self,))])

    def __neg__(self):
        return NegationExpression((self,))

    def __truediv__(self, other):
        return DivisionExpression((self, other))

    def __rtruediv__(self, other):
        return DivisionExpression((other, self))

    def __div__(self, other):
        return DivisionExpression((self, other))

    def __rdiv__(self, other):
        return DivisionExpression((other, self))

    def __pow__(self, other):
        return PowExpression((self, other))

    def __rpow__(self, other):
        return PowExpression((other, self))

    def __pos__(self):
        return self

    def __le__(self, other):
        return InequalityExpression((self, other))

    def __ge__(self, other):
        return InequalityExpression((other, self))

    def __eq__(self, other):
        return EqualityExpression((self, other))

    def __iadd__(self, other):
        return self + other

    def __isub__(self, other):
        return self - other

    def __imul__(self, other):
        return self * other

    def __idiv__(self, other):
        return self / other

    def __itruediv__(self, other):
        return self / other

    def __ipow__(self, other):
        return self**other


class BinaryExpression(ExpressionBase):
    __slots__ = tuple()
    func = None

    def __init__(self, args):
        self._args_ = args
        arg0, arg1 = args
        func, arg0, arg1 = _binary_map[type(arg0), type(arg1), self.func](arg0, arg1)
        self._pe = func(arg0, arg1)


class UnaryExpression(ExpressionBase):
    __slots__ = tuple()
    func = None

    def __init__(self, args):
        self._args_ = args
        arg = _operand_map[type(args[0])](args[0])
        self._pe = self.func(arg)


class NegationExpression(UnaryExpression, numeric_expr.NegationExpression):
    __slots__ = "_pe"
    func = operator.neg


class ProductExpression(BinaryExpression, numeric_expr.ProductExpression):
    __slots__ = ("_pe",)
    func = operator.mul


class MonomialTermExpression(ProductExpression, numeric_expr.MonomialTermExpression):
    __slots__ = tuple()
    func = operator.mul


class DivisionExpression(BinaryExpression, numeric_expr.DivisionExpression):
    __slots__ = ("_pe",)
    func = operator.truediv


class PowExpression(BinaryExpression, numeric_expr.PowExpression):
    __slots__ = ("_pe",)
    func = operator.pow


class SumExpression(ExpressionBase, numeric_expr.SumExpression):
    __slots__ = ("_pe",)

    def __init__(self, args):
        self._shared_args = False
        self._args_ = args
        self._nargs = len(self._args_)
        if len(args) == 2:
            arg0, arg1 = args
            func, arg0, arg1 = _binary_map[type(arg0), type(arg1), operator.add](arg0, arg1)
            self._pe = func(arg0, arg1)
        else:
            self._pe = pk.sum([_sum_operand_map[type(arg)](arg) for arg in args])

    def __add__(self, other):
        res = SumExpression([self, other])
        if self._shared_args:
            new_args = self.args
        else:
            new_args = self._args_
            self._shared_args = True
        new_args.append(other)
        res._args_ = new_args
        res._nargs = len(new_args)
        return res


class LinearExpression(ExpressionBase, numeric_expr.LinearExpression):
    __slots__ = ("_pe",)

    def __init__(self, args=None, constant=None, linear_coefs=None, linear_vars=None):
        """A linear expression of the form `const + sum_i(c_i*x_i).

        You can specify args OR (constant, linear_coefs, and
        linear_vars).  If args is provided, it should be a list that
        contains the constant, followed by a series of
        :py:class:`MonomialTermExpression` objects. Alternatively, you
        can specify the constant, the list of linear_coeffs and the list
        of linear_vars separately. Note that these lists are NOT copied.

        """
        # I am not sure why LinearExpression allows omitting args, but
        # it does.  If they are provided, they should be the (non-zero)
        # constant followed by MonomialTermExpressions.
        if args:
            if any(arg is not None for arg in (constant, linear_coefs, linear_vars)):
                raise ValueError(
                    "Cannot specify both args and any of "
                    "{constant, linear_coeffs, or linear_vars}"
                )
            if len(args) > 1 and (
                args[1].__class__ in native_types or not args[1].is_potentially_variable()
            ):
                deprecation_warning(
                    "LinearExpression has been updated to expect args= to "
                    "be a constant followed by MonomialTermExpressions.  "
                    "The older format (`[const, coefficient_1, ..., "
                    "variable_1, ...]`) is deprecated.",
                    version="6.2",
                )
                args = args[:1] + list(
                    map(
                        MonomialTermExpression,
                        zip(args[1 : 1 + len(args) // 2], args[1 + len(args) // 2 :]),
                    )
                )
            self._args_ = args
        else:
            self.constant = constant if constant is not None else 0
            self.linear_coefs = linear_coefs if linear_coefs else []
            self.linear_vars = linear_vars if linear_vars else []
            self._args_cache_ = []

        self._pe = pk.construct_linear_expression(
            self.linear_coefs, [v._pe for v in self.linear_vars], self.constant
        )


class InequalityExpression(BinaryExpression, logical_expr.InequalityExpression):
    __slots__ = ("_pe",)
    func = operator.le

    def __init__(self, args, strict=False):
        super().__init__(args)
        self._strict = strict


class EqualityExpression(BinaryExpression, logical_expr.EqualityExpression):
    __slots__ = ("_pe",)
    func = operator.eq


def quicksum(args):
    return SumExpression(list(args))


def _get_float_mul_var(arg0, arg1):
    return pk.float_mul_var, arg0, arg1._pe


def _get_float_mul_param(arg0, arg1):
    return pk.float_mul_param, arg0, arg1._pe


def _get_float_mul_expression(arg0, arg1):
    return pk.float_mul_expression, arg0, arg1._pe


def _get_float_add_var(arg0, arg1):
    return pk.float_add_var, arg0, arg1._pe


def _get_float_add_param(arg0, arg1):
    return pk.float_add_param, arg0, arg1._pe


def _get_float_add_expression(arg0, arg1):
    return pk.float_add_expression, arg0, arg1._pe


def _get_float_sub_var(arg0, arg1):
    return pk.float_sub_var, arg0, arg1._pe


def _get_float_sub_param(arg0, arg1):
    return pk.float_sub_param, arg0, arg1._pe


def _get_float_sub_expression(arg0, arg1):
    return pk.float_sub_expression, arg0, arg1._pe


def _get_float_div_var(arg0, arg1):
    return pk.float_div_var, arg0, arg1._pe


def _get_float_div_param(arg0, arg1):
    return pk.float_div_param, arg0, arg1._pe


def _get_float_div_expression(arg0, arg1):
    return pk.float_div_expression, arg0, arg1._pe


def _get_float_pow_var(arg0, arg1):
    return pk.float_pow_var, arg0, arg1._pe


def _get_float_pow_param(arg0, arg1):
    return pk.float_pow_param, arg0, arg1._pe


def _get_float_pow_expression(arg0, arg1):
    return pk.float_pow_expression, arg0, arg1._pe


def _get_float_le_var(arg0, arg1):
    return pk.float_le_var, arg0, arg1._pe


def _get_float_le_param(arg0, arg1):
    return pk.float_le_param, arg0, arg1._pe


def _get_float_le_expression(arg0, arg1):
    return pk.float_le_expression, arg0, arg1._pe


def _get_float_ge_var(arg0, arg1):
    return pk.float_ge_var, arg0, arg1._pe


def _get_float_ge_param(arg0, arg1):
    return pk.float_ge_param, arg0, arg1._pe


def _get_float_ge_expression(arg0, arg1):
    return pk.float_ge_expression, arg0, arg1._pe


def _get_float_eq_var(arg0, arg1):
    return pk.float_eq_var, arg0, arg1._pe


def _get_float_eq_param(arg0, arg1):
    return pk.float_eq_param, arg0, arg1._pe


def _get_float_eq_expression(arg0, arg1):
    return pk.float_eq_expression, arg0, arg1._pe


def _get_param_mul_float(arg0, arg1):
    return pk.param_mul_float, arg0._pe, arg1


def _get_param_mul_var(arg0, arg1):
    return pk.param_mul_var, arg0._pe, arg1._pe


def _get_param_mul_param(arg0, arg1):
    return pk.param_mul_param, arg0._pe, arg1._pe


def _get_param_mul_expression(arg0, arg1):
    return pk.param_mul_expression, arg0._pe, arg1._pe


def _get_param_add_float(arg0, arg1):
    return pk.param_add_float, arg0._pe, arg1


def _get_param_add_var(arg0, arg1):
    return pk.param_add_var, arg0._pe, arg1._pe


def _get_param_add_param(arg0, arg1):
    return pk.param_add_param, arg0._pe, arg1._pe


def _get_param_add_expression(arg0, arg1):
    return pk.param_add_expression, arg0._pe, arg1._pe


def _get_param_sub_float(arg0, arg1):
    return pk.param_sub_float, arg0._pe, arg1


def _get_param_sub_var(arg0, arg1):
    return pk.param_sub_var, arg0._pe, arg1._pe


def _get_param_sub_param(arg0, arg1):
    return pk.param_sub_param, arg0._pe, arg1._pe


def _get_param_sub_expression(arg0, arg1):
    return pk.param_sub_expression, arg0._pe, arg1._pe


def _get_param_div_float(arg0, arg1):
    return pk.param_div_float, arg0._pe, arg1


def _get_param_div_var(arg0, arg1):
    return pk.param_div_var, arg0._pe, arg1._pe


def _get_param_div_param(arg0, arg1):
    return pk.param_div_param, arg0._pe, arg1._pe


def _get_param_div_expression(arg0, arg1):
    return pk.param_div_expression, arg0._pe, arg1._pe


def _get_param_pow_float(arg0, arg1):
    return pk.param_pow_float, arg0._pe, arg1


def _get_param_pow_var(arg0, arg1):
    return pk.param_pow_var, arg0._pe, arg1._pe


def _get_param_pow_param(arg0, arg1):
    return pk.param_pow_param, arg0._pe, arg1._pe


def _get_param_pow_expression(arg0, arg1):
    return pk.param_pow_expression, arg0._pe, arg1._pe


def _get_param_le_float(arg0, arg1):
    return pk.param_le_float, arg0._pe, arg1


def _get_param_le_var(arg0, arg1):
    return pk.param_le_var, arg0._pe, arg1._pe


def _get_param_le_param(arg0, arg1):
    return pk.param_le_param, arg0._pe, arg1._pe


def _get_param_le_expression(arg0, arg1):
    return pk.param_le_expression, arg0._pe, arg1._pe


def _get_param_ge_float(arg0, arg1):
    return pk.param_ge_float, arg0._pe, arg1


def _get_param_ge_var(arg0, arg1):
    return pk.param_ge_var, arg0._pe, arg1._pe


def _get_param_ge_param(arg0, arg1):
    return pk.param_ge_param, arg0._pe, arg1._pe


def _get_param_ge_expression(arg0, arg1):
    return pk.param_ge_expression, arg0._pe, arg1._pe


def _get_param_eq_float(arg0, arg1):
    return pk.param_eq_float, arg0._pe, arg1


def _get_param_eq_var(arg0, arg1):
    return pk.param_eq_var, arg0._pe, arg1._pe


def _get_param_eq_param(arg0, arg1):
    return pk.param_eq_param, arg0._pe, arg1._pe


def _get_param_eq_expression(arg0, arg1):
    return pk.param_eq_expression, arg0._pe, arg1._pe


def _get_var_mul_float(arg0, arg1):
    return pk.var_mul_float, arg0._pe, arg1


def _get_var_mul_var(arg0, arg1):
    return pk.var_mul_var, arg0._pe, arg1._pe


def _get_var_mul_param(arg0, arg1):
    return pk.var_mul_param, arg0._pe, arg1._pe


def _get_var_mul_expression(arg0, arg1):
    return pk.var_mul_expression, arg0._pe, arg1._pe


def _get_var_add_float(arg0, arg1):
    return pk.var_add_float, arg0._pe, arg1


def _get_var_add_var(arg0, arg1):
    return pk.var_add_var, arg0._pe, arg1._pe


def _get_var_add_param(arg0, arg1):
    return pk.var_add_param, arg0._pe, arg1._pe


def _get_var_add_expression(arg0, arg1):
    return pk.var_add_expression, arg0._pe, arg1._pe


def _get_var_sub_float(arg0, arg1):
    return pk.var_sub_float, arg0._pe, arg1


def _get_var_sub_var(arg0, arg1):
    return pk.var_sub_var, arg0._pe, arg1._pe


def _get_var_sub_param(arg0, arg1):
    return pk.var_sub_param, arg0._pe, arg1._pe


def _get_var_sub_expression(arg0, arg1):
    return pk.var_sub_expression, arg0._pe, arg1._pe


def _get_var_div_float(arg0, arg1):
    return pk.var_div_float, arg0._pe, arg1


def _get_var_div_var(arg0, arg1):
    return pk.var_div_var, arg0._pe, arg1._pe


def _get_var_div_param(arg0, arg1):
    return pk.var_div_param, arg0._pe, arg1._pe


def _get_var_div_expression(arg0, arg1):
    return pk.var_div_expression, arg0._pe, arg1._pe


def _get_var_pow_float(arg0, arg1):
    return pk.var_pow_float, arg0._pe, arg1


def _get_var_pow_var(arg0, arg1):
    return pk.var_pow_var, arg0._pe, arg1._pe


def _get_var_pow_param(arg0, arg1):
    return pk.var_pow_param, arg0._pe, arg1._pe


def _get_var_pow_expression(arg0, arg1):
    return pk.var_pow_expression, arg0._pe, arg1._pe


def _get_var_le_float(arg0, arg1):
    return pk.var_le_float, arg0._pe, arg1


def _get_var_le_var(arg0, arg1):
    return pk.var_le_var, arg0._pe, arg1._pe


def _get_var_le_param(arg0, arg1):
    return pk.var_le_param, arg0._pe, arg1._pe


def _get_var_le_expression(arg0, arg1):
    return pk.var_le_expression, arg0._pe, arg1._pe


def _get_var_ge_float(arg0, arg1):
    return pk.var_ge_float, arg0._pe, arg1


def _get_var_ge_var(arg0, arg1):
    return pk.var_ge_var, arg0._pe, arg1._pe


def _get_var_ge_param(arg0, arg1):
    return pk.var_ge_param, arg0._pe, arg1._pe


def _get_var_ge_expression(arg0, arg1):
    return pk.var_ge_expression, arg0._pe, arg1._pe


def _get_var_eq_float(arg0, arg1):
    return pk.var_eq_float, arg0._pe, arg1


def _get_var_eq_var(arg0, arg1):
    return pk.var_eq_var, arg0._pe, arg1._pe


def _get_var_eq_param(arg0, arg1):
    return pk.var_eq_param, arg0._pe, arg1._pe


def _get_var_eq_expression(arg0, arg1):
    return pk.var_eq_expression, arg0._pe, arg1._pe


def _get_expression_mul_float(arg0, arg1):
    return pk.expression_mul_float, arg0._pe, arg1


def _get_expression_mul_var(arg0, arg1):
    return pk.expression_mul_var, arg0._pe, arg1._pe


def _get_expression_mul_param(arg0, arg1):
    return pk.expression_mul_param, arg0._pe, arg1._pe


def _get_expression_mul_expression(arg0, arg1):
    return pk.expression_mul_expression, arg0._pe, arg1._pe


def _get_expression_add_float(arg0, arg1):
    return pk.expression_add_float, arg0._pe, arg1


def _get_expression_add_var(arg0, arg1):
    return pk.expression_add_var, arg0._pe, arg1._pe


def _get_expression_add_param(arg0, arg1):
    return pk.expression_add_param, arg0._pe, arg1._pe


def _get_expression_add_expression(arg0, arg1):
    return pk.expression_add_expression, arg0._pe, arg1._pe


def _get_expression_sub_float(arg0, arg1):
    return pk.expression_sub_float, arg0._pe, arg1


def _get_expression_sub_var(arg0, arg1):
    return pk.expression_sub_var, arg0._pe, arg1._pe


def _get_expression_sub_param(arg0, arg1):
    return pk.expression_sub_param, arg0._pe, arg1._pe


def _get_expression_sub_expression(arg0, arg1):
    return pk.expression_sub_expression, arg0._pe, arg1._pe


def _get_expression_div_float(arg0, arg1):
    return pk.expression_div_float, arg0._pe, arg1


def _get_expression_div_var(arg0, arg1):
    return pk.expression_div_var, arg0._pe, arg1._pe


def _get_expression_div_param(arg0, arg1):
    return pk.expression_div_param, arg0._pe, arg1._pe


def _get_expression_div_expression(arg0, arg1):
    return pk.expression_div_expression, arg0._pe, arg1._pe


def _get_expression_pow_float(arg0, arg1):
    return pk.expression_pow_float, arg0._pe, arg1


def _get_expression_pow_var(arg0, arg1):
    return pk.expression_pow_var, arg0._pe, arg1._pe


def _get_expression_pow_param(arg0, arg1):
    return pk.expression_pow_param, arg0._pe, arg1._pe


def _get_expression_pow_expression(arg0, arg1):
    return pk.expression_pow_expression, arg0._pe, arg1._pe


def _get_expression_le_float(arg0, arg1):
    return pk.expression_le_float, arg0._pe, arg1


def _get_expression_le_var(arg0, arg1):
    return pk.expression_le_var, arg0._pe, arg1._pe


def _get_expression_le_param(arg0, arg1):
    return pk.expression_le_param, arg0._pe, arg1._pe


def _get_expression_le_expression(arg0, arg1):
    return pk.expression_le_expression, arg0._pe, arg1._pe


def _get_expression_ge_float(arg0, arg1):
    return pk.expression_ge_float, arg0._pe, arg1


def _get_expression_ge_var(arg0, arg1):
    return pk.expression_ge_var, arg0._pe, arg1._pe


def _get_expression_ge_param(arg0, arg1):
    return pk.expression_ge_param, arg0._pe, arg1._pe


def _get_expression_ge_expression(arg0, arg1):
    return pk.expression_ge_expression, arg0._pe, arg1._pe


def _get_expression_eq_float(arg0, arg1):
    return pk.expression_eq_float, arg0._pe, arg1


def _get_expression_eq_var(arg0, arg1):
    return pk.expression_eq_var, arg0._pe, arg1._pe


def _get_expression_eq_param(arg0, arg1):
    return pk.expression_eq_param, arg0._pe, arg1._pe


def _get_expression_eq_expression(arg0, arg1):
    return pk.expression_eq_expression, arg0._pe, arg1._pe


_binary_map = dict()
_binary_map[float, _GeneralVarData, operator.mul] = _get_float_mul_var
_binary_map[int, _GeneralVarData, operator.mul] = _get_float_mul_var
_binary_map[float, _ParamData, operator.mul] = _get_float_mul_param
_binary_map[int, _ParamData, operator.mul] = _get_float_mul_param
_binary_map[float, ScalarVar, operator.mul] = _get_float_mul_var
_binary_map[int, ScalarVar, operator.mul] = _get_float_mul_var
_binary_map[float, ScalarParam, operator.mul] = _get_float_mul_param
_binary_map[int, ScalarParam, operator.mul] = _get_float_mul_param
_binary_map[float, ProductExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, ProductExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, MonomialTermExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, MonomialTermExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, SumExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, SumExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, LinearExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, LinearExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, NegationExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, NegationExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, DivisionExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, DivisionExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, PowExpression, operator.mul] = _get_float_mul_expression
_binary_map[int, PowExpression, operator.mul] = _get_float_mul_expression
_binary_map[float, _GeneralVarData, operator.add] = _get_float_add_var
_binary_map[int, _GeneralVarData, operator.add] = _get_float_add_var
_binary_map[float, _ParamData, operator.add] = _get_float_add_param
_binary_map[int, _ParamData, operator.add] = _get_float_add_param
_binary_map[float, ScalarVar, operator.add] = _get_float_add_var
_binary_map[int, ScalarVar, operator.add] = _get_float_add_var
_binary_map[float, ScalarParam, operator.add] = _get_float_add_param
_binary_map[int, ScalarParam, operator.add] = _get_float_add_param
_binary_map[float, ProductExpression, operator.add] = _get_float_add_expression
_binary_map[int, ProductExpression, operator.add] = _get_float_add_expression
_binary_map[float, MonomialTermExpression, operator.add] = _get_float_add_expression
_binary_map[int, MonomialTermExpression, operator.add] = _get_float_add_expression
_binary_map[float, SumExpression, operator.add] = _get_float_add_expression
_binary_map[int, SumExpression, operator.add] = _get_float_add_expression
_binary_map[float, LinearExpression, operator.add] = _get_float_add_expression
_binary_map[int, LinearExpression, operator.add] = _get_float_add_expression
_binary_map[float, NegationExpression, operator.add] = _get_float_add_expression
_binary_map[int, NegationExpression, operator.add] = _get_float_add_expression
_binary_map[float, DivisionExpression, operator.add] = _get_float_add_expression
_binary_map[int, DivisionExpression, operator.add] = _get_float_add_expression
_binary_map[float, PowExpression, operator.add] = _get_float_add_expression
_binary_map[int, PowExpression, operator.add] = _get_float_add_expression
_binary_map[float, _GeneralVarData, operator.sub] = _get_float_sub_var
_binary_map[int, _GeneralVarData, operator.sub] = _get_float_sub_var
_binary_map[float, _ParamData, operator.sub] = _get_float_sub_param
_binary_map[int, _ParamData, operator.sub] = _get_float_sub_param
_binary_map[float, ScalarVar, operator.sub] = _get_float_sub_var
_binary_map[int, ScalarVar, operator.sub] = _get_float_sub_var
_binary_map[float, ScalarParam, operator.sub] = _get_float_sub_param
_binary_map[int, ScalarParam, operator.sub] = _get_float_sub_param
_binary_map[float, ProductExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, ProductExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, MonomialTermExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, MonomialTermExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, SumExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, SumExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, LinearExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, LinearExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, NegationExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, NegationExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, DivisionExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, DivisionExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, PowExpression, operator.sub] = _get_float_sub_expression
_binary_map[int, PowExpression, operator.sub] = _get_float_sub_expression
_binary_map[float, _GeneralVarData, operator.truediv] = _get_float_div_var
_binary_map[int, _GeneralVarData, operator.truediv] = _get_float_div_var
_binary_map[float, _ParamData, operator.truediv] = _get_float_div_param
_binary_map[int, _ParamData, operator.truediv] = _get_float_div_param
_binary_map[float, ScalarVar, operator.truediv] = _get_float_div_var
_binary_map[int, ScalarVar, operator.truediv] = _get_float_div_var
_binary_map[float, ScalarParam, operator.truediv] = _get_float_div_param
_binary_map[int, ScalarParam, operator.truediv] = _get_float_div_param
_binary_map[float, ProductExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, ProductExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, MonomialTermExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, MonomialTermExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, SumExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, SumExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, LinearExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, LinearExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, NegationExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, NegationExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, DivisionExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, DivisionExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, PowExpression, operator.truediv] = _get_float_div_expression
_binary_map[int, PowExpression, operator.truediv] = _get_float_div_expression
_binary_map[float, _GeneralVarData, operator.pow] = _get_float_pow_var
_binary_map[int, _GeneralVarData, operator.pow] = _get_float_pow_var
_binary_map[float, _ParamData, operator.pow] = _get_float_pow_param
_binary_map[int, _ParamData, operator.pow] = _get_float_pow_param
_binary_map[float, ScalarVar, operator.pow] = _get_float_pow_var
_binary_map[int, ScalarVar, operator.pow] = _get_float_pow_var
_binary_map[float, ScalarParam, operator.pow] = _get_float_pow_param
_binary_map[int, ScalarParam, operator.pow] = _get_float_pow_param
_binary_map[float, ProductExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, ProductExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, MonomialTermExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, MonomialTermExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, SumExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, SumExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, LinearExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, LinearExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, NegationExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, NegationExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, DivisionExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, DivisionExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, PowExpression, operator.pow] = _get_float_pow_expression
_binary_map[int, PowExpression, operator.pow] = _get_float_pow_expression
_binary_map[float, _GeneralVarData, operator.le] = _get_float_le_var
_binary_map[int, _GeneralVarData, operator.le] = _get_float_le_var
_binary_map[float, _ParamData, operator.le] = _get_float_le_param
_binary_map[int, _ParamData, operator.le] = _get_float_le_param
_binary_map[float, ScalarVar, operator.le] = _get_float_le_var
_binary_map[int, ScalarVar, operator.le] = _get_float_le_var
_binary_map[float, ScalarParam, operator.le] = _get_float_le_param
_binary_map[int, ScalarParam, operator.le] = _get_float_le_param
_binary_map[float, ProductExpression, operator.le] = _get_float_le_expression
_binary_map[int, ProductExpression, operator.le] = _get_float_le_expression
_binary_map[float, MonomialTermExpression, operator.le] = _get_float_le_expression
_binary_map[int, MonomialTermExpression, operator.le] = _get_float_le_expression
_binary_map[float, SumExpression, operator.le] = _get_float_le_expression
_binary_map[int, SumExpression, operator.le] = _get_float_le_expression
_binary_map[float, LinearExpression, operator.le] = _get_float_le_expression
_binary_map[int, LinearExpression, operator.le] = _get_float_le_expression
_binary_map[float, NegationExpression, operator.le] = _get_float_le_expression
_binary_map[int, NegationExpression, operator.le] = _get_float_le_expression
_binary_map[float, DivisionExpression, operator.le] = _get_float_le_expression
_binary_map[int, DivisionExpression, operator.le] = _get_float_le_expression
_binary_map[float, PowExpression, operator.le] = _get_float_le_expression
_binary_map[int, PowExpression, operator.le] = _get_float_le_expression
_binary_map[float, _GeneralVarData, operator.ge] = _get_float_ge_var
_binary_map[int, _GeneralVarData, operator.ge] = _get_float_ge_var
_binary_map[float, _ParamData, operator.ge] = _get_float_ge_param
_binary_map[int, _ParamData, operator.ge] = _get_float_ge_param
_binary_map[float, ScalarVar, operator.ge] = _get_float_ge_var
_binary_map[int, ScalarVar, operator.ge] = _get_float_ge_var
_binary_map[float, ScalarParam, operator.ge] = _get_float_ge_param
_binary_map[int, ScalarParam, operator.ge] = _get_float_ge_param
_binary_map[float, ProductExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, ProductExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, MonomialTermExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, MonomialTermExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, SumExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, SumExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, LinearExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, LinearExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, NegationExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, NegationExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, DivisionExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, DivisionExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, PowExpression, operator.ge] = _get_float_ge_expression
_binary_map[int, PowExpression, operator.ge] = _get_float_ge_expression
_binary_map[float, _GeneralVarData, operator.eq] = _get_float_eq_var
_binary_map[int, _GeneralVarData, operator.eq] = _get_float_eq_var
_binary_map[float, _ParamData, operator.eq] = _get_float_eq_param
_binary_map[int, _ParamData, operator.eq] = _get_float_eq_param
_binary_map[float, ScalarVar, operator.eq] = _get_float_eq_var
_binary_map[int, ScalarVar, operator.eq] = _get_float_eq_var
_binary_map[float, ScalarParam, operator.eq] = _get_float_eq_param
_binary_map[int, ScalarParam, operator.eq] = _get_float_eq_param
_binary_map[float, ProductExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, ProductExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, MonomialTermExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, MonomialTermExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, SumExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, SumExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, LinearExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, LinearExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, NegationExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, NegationExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, DivisionExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, DivisionExpression, operator.eq] = _get_float_eq_expression
_binary_map[float, PowExpression, operator.eq] = _get_float_eq_expression
_binary_map[int, PowExpression, operator.eq] = _get_float_eq_expression

_binary_map[_ParamData, float, operator.mul] = _get_var_mul_float
_binary_map[_ParamData, int, operator.mul] = _get_var_mul_float
_binary_map[ScalarParam, float, operator.mul] = _get_var_mul_float
_binary_map[ScalarParam, int, operator.mul] = _get_var_mul_float
_binary_map[_ParamData, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[_ParamData, _ParamData, operator.mul] = _get_var_mul_param
_binary_map[_ParamData, ScalarVar, operator.mul] = _get_var_mul_var
_binary_map[_ParamData, ScalarParam, operator.mul] = _get_var_mul_param
_binary_map[ScalarParam, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[ScalarParam, _ParamData, operator.mul] = _get_var_mul_param
_binary_map[ScalarParam, ScalarVar, operator.mul] = _get_var_mul_var
_binary_map[ScalarParam, ScalarParam, operator.mul] = _get_var_mul_param
_binary_map[_ParamData, ProductExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, ProductExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, MonomialTermExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, SumExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, SumExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, LinearExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, LinearExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, NegationExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, NegationExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, DivisionExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, DivisionExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, PowExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarParam, PowExpression, operator.mul] = _get_var_mul_expression
_binary_map[_ParamData, float, operator.add] = _get_var_add_float
_binary_map[_ParamData, int, operator.add] = _get_var_add_float
_binary_map[ScalarParam, float, operator.add] = _get_var_add_float
_binary_map[ScalarParam, int, operator.add] = _get_var_add_float
_binary_map[_ParamData, _GeneralVarData, operator.add] = _get_var_add_var
_binary_map[_ParamData, _ParamData, operator.add] = _get_var_add_param
_binary_map[_ParamData, ScalarVar, operator.add] = _get_var_add_var
_binary_map[_ParamData, ScalarParam, operator.add] = _get_var_add_param
_binary_map[ScalarParam, _GeneralVarData, operator.add] = _get_var_add_var
_binary_map[ScalarParam, _ParamData, operator.add] = _get_var_add_param
_binary_map[ScalarParam, ScalarVar, operator.add] = _get_var_add_var
_binary_map[ScalarParam, ScalarParam, operator.add] = _get_var_add_param
_binary_map[_ParamData, ProductExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, ProductExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, MonomialTermExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, SumExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, SumExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, LinearExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, LinearExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, NegationExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, NegationExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, DivisionExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, DivisionExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, PowExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarParam, PowExpression, operator.add] = _get_var_add_expression
_binary_map[_ParamData, float, operator.sub] = _get_var_sub_float
_binary_map[_ParamData, int, operator.sub] = _get_var_sub_float
_binary_map[ScalarParam, float, operator.sub] = _get_var_sub_float
_binary_map[ScalarParam, int, operator.sub] = _get_var_sub_float
_binary_map[_ParamData, _GeneralVarData, operator.sub] = _get_var_sub_var
_binary_map[_ParamData, _ParamData, operator.sub] = _get_var_sub_param
_binary_map[_ParamData, ScalarVar, operator.sub] = _get_var_sub_var
_binary_map[_ParamData, ScalarParam, operator.sub] = _get_var_sub_param
_binary_map[ScalarParam, _GeneralVarData, operator.sub] = _get_var_sub_var
_binary_map[ScalarParam, _ParamData, operator.sub] = _get_var_sub_param
_binary_map[ScalarParam, ScalarVar, operator.sub] = _get_var_sub_var
_binary_map[ScalarParam, ScalarParam, operator.sub] = _get_var_sub_param
_binary_map[_ParamData, ProductExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, ProductExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, MonomialTermExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, SumExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, SumExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, LinearExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, LinearExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, NegationExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, NegationExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, DivisionExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, DivisionExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, PowExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarParam, PowExpression, operator.sub] = _get_var_sub_expression
_binary_map[_ParamData, float, operator.truediv] = _get_var_div_float
_binary_map[_ParamData, int, operator.truediv] = _get_var_div_float
_binary_map[ScalarParam, float, operator.truediv] = _get_var_div_float
_binary_map[ScalarParam, int, operator.truediv] = _get_var_div_float
_binary_map[_ParamData, _GeneralVarData, operator.truediv] = _get_var_div_var
_binary_map[_ParamData, _ParamData, operator.truediv] = _get_var_div_param
_binary_map[_ParamData, ScalarVar, operator.truediv] = _get_var_div_var
_binary_map[_ParamData, ScalarParam, operator.truediv] = _get_var_div_param
_binary_map[ScalarParam, _GeneralVarData, operator.truediv] = _get_var_div_var
_binary_map[ScalarParam, _ParamData, operator.truediv] = _get_var_div_param
_binary_map[ScalarParam, ScalarVar, operator.truediv] = _get_var_div_var
_binary_map[ScalarParam, ScalarParam, operator.truediv] = _get_var_div_param
_binary_map[_ParamData, ProductExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, ProductExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, MonomialTermExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, SumExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, SumExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, LinearExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, LinearExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, NegationExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, NegationExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, DivisionExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, DivisionExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, PowExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarParam, PowExpression, operator.truediv] = _get_var_div_expression
_binary_map[_ParamData, float, operator.pow] = _get_var_pow_float
_binary_map[_ParamData, int, operator.pow] = _get_var_pow_float
_binary_map[ScalarParam, float, operator.pow] = _get_var_pow_float
_binary_map[ScalarParam, int, operator.pow] = _get_var_pow_float
_binary_map[_ParamData, _GeneralVarData, operator.pow] = _get_var_pow_var
_binary_map[_ParamData, _ParamData, operator.pow] = _get_var_pow_param
_binary_map[_ParamData, ScalarVar, operator.pow] = _get_var_pow_var
_binary_map[_ParamData, ScalarParam, operator.pow] = _get_var_pow_param
_binary_map[ScalarParam, _GeneralVarData, operator.pow] = _get_var_pow_var
_binary_map[ScalarParam, _ParamData, operator.pow] = _get_var_pow_param
_binary_map[ScalarParam, ScalarVar, operator.pow] = _get_var_pow_var
_binary_map[ScalarParam, ScalarParam, operator.pow] = _get_var_pow_param
_binary_map[_ParamData, ProductExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, ProductExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, MonomialTermExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, SumExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, SumExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, LinearExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, LinearExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, NegationExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, NegationExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, DivisionExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, DivisionExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, PowExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarParam, PowExpression, operator.pow] = _get_var_pow_expression
_binary_map[_ParamData, float, operator.le] = _get_var_le_float
_binary_map[_ParamData, int, operator.le] = _get_var_le_float
_binary_map[ScalarParam, float, operator.le] = _get_var_le_float
_binary_map[ScalarParam, int, operator.le] = _get_var_le_float
_binary_map[_ParamData, _GeneralVarData, operator.le] = _get_var_le_var
_binary_map[_ParamData, _ParamData, operator.le] = _get_var_le_param
_binary_map[_ParamData, ScalarVar, operator.le] = _get_var_le_var
_binary_map[_ParamData, ScalarParam, operator.le] = _get_var_le_param
_binary_map[ScalarParam, _GeneralVarData, operator.le] = _get_var_le_var
_binary_map[ScalarParam, _ParamData, operator.le] = _get_var_le_param
_binary_map[ScalarParam, ScalarVar, operator.le] = _get_var_le_var
_binary_map[ScalarParam, ScalarParam, operator.le] = _get_var_le_param
_binary_map[_ParamData, ProductExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, ProductExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, MonomialTermExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, SumExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, SumExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, LinearExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, LinearExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, NegationExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, NegationExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, DivisionExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, DivisionExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, PowExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarParam, PowExpression, operator.le] = _get_var_le_expression
_binary_map[_ParamData, float, operator.ge] = _get_var_ge_float
_binary_map[_ParamData, int, operator.ge] = _get_var_ge_float
_binary_map[ScalarParam, float, operator.ge] = _get_var_ge_float
_binary_map[ScalarParam, int, operator.ge] = _get_var_ge_float
_binary_map[_ParamData, _GeneralVarData, operator.ge] = _get_var_ge_var
_binary_map[_ParamData, _ParamData, operator.ge] = _get_var_ge_param
_binary_map[_ParamData, ScalarVar, operator.ge] = _get_var_ge_var
_binary_map[_ParamData, ScalarParam, operator.ge] = _get_var_ge_param
_binary_map[ScalarParam, _GeneralVarData, operator.ge] = _get_var_ge_var
_binary_map[ScalarParam, _ParamData, operator.ge] = _get_var_ge_param
_binary_map[ScalarParam, ScalarVar, operator.ge] = _get_var_ge_var
_binary_map[ScalarParam, ScalarParam, operator.ge] = _get_var_ge_param
_binary_map[_ParamData, ProductExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, ProductExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, MonomialTermExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, SumExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, SumExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, LinearExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, LinearExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, NegationExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, NegationExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, DivisionExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, DivisionExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, PowExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarParam, PowExpression, operator.ge] = _get_var_ge_expression
_binary_map[_ParamData, float, operator.eq] = _get_var_eq_float
_binary_map[_ParamData, int, operator.eq] = _get_var_eq_float
_binary_map[ScalarParam, float, operator.eq] = _get_var_eq_float
_binary_map[ScalarParam, int, operator.eq] = _get_var_eq_float
_binary_map[_ParamData, _GeneralVarData, operator.eq] = _get_var_eq_var
_binary_map[_ParamData, _ParamData, operator.eq] = _get_var_eq_param
_binary_map[_ParamData, ScalarVar, operator.eq] = _get_var_eq_var
_binary_map[_ParamData, ScalarParam, operator.eq] = _get_var_eq_param
_binary_map[ScalarParam, _GeneralVarData, operator.eq] = _get_var_eq_var
_binary_map[ScalarParam, _ParamData, operator.eq] = _get_var_eq_param
_binary_map[ScalarParam, ScalarVar, operator.eq] = _get_var_eq_var
_binary_map[ScalarParam, ScalarParam, operator.eq] = _get_var_eq_param
_binary_map[_ParamData, ProductExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, ProductExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, MonomialTermExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, MonomialTermExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, SumExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, SumExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, LinearExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, LinearExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, NegationExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, NegationExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, DivisionExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, DivisionExpression, operator.eq] = _get_var_eq_expression
_binary_map[_ParamData, PowExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarParam, PowExpression, operator.eq] = _get_var_eq_expression

_binary_map[_GeneralVarData, float, operator.mul] = _get_var_mul_float
_binary_map[_GeneralVarData, int, operator.mul] = _get_var_mul_float
_binary_map[ScalarVar, float, operator.mul] = _get_var_mul_float
_binary_map[ScalarVar, int, operator.mul] = _get_var_mul_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[_GeneralVarData, _ParamData, operator.mul] = _get_var_mul_param
_binary_map[_GeneralVarData, ScalarVar, operator.mul] = _get_var_mul_var
_binary_map[_GeneralVarData, ScalarParam, operator.mul] = _get_var_mul_param
_binary_map[ScalarVar, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[ScalarVar, _ParamData, operator.mul] = _get_var_mul_param
_binary_map[ScalarVar, ScalarVar, operator.mul] = _get_var_mul_var
_binary_map[ScalarVar, ScalarParam, operator.mul] = _get_var_mul_param
_binary_map[_GeneralVarData, ProductExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, ProductExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, SumExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, SumExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, LinearExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, LinearExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, NegationExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, NegationExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, DivisionExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, PowExpression, operator.mul] = _get_var_mul_expression
_binary_map[ScalarVar, PowExpression, operator.mul] = _get_var_mul_expression
_binary_map[_GeneralVarData, float, operator.add] = _get_var_add_float
_binary_map[_GeneralVarData, int, operator.add] = _get_var_add_float
_binary_map[ScalarVar, float, operator.add] = _get_var_add_float
_binary_map[ScalarVar, int, operator.add] = _get_var_add_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.add] = _get_var_add_var
_binary_map[_GeneralVarData, _ParamData, operator.add] = _get_var_add_param
_binary_map[_GeneralVarData, ScalarVar, operator.add] = _get_var_add_var
_binary_map[_GeneralVarData, ScalarParam, operator.add] = _get_var_add_param
_binary_map[ScalarVar, _GeneralVarData, operator.add] = _get_var_add_var
_binary_map[ScalarVar, _ParamData, operator.add] = _get_var_add_param
_binary_map[ScalarVar, ScalarVar, operator.add] = _get_var_add_var
_binary_map[ScalarVar, ScalarParam, operator.add] = _get_var_add_param
_binary_map[_GeneralVarData, ProductExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, ProductExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, SumExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, SumExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, LinearExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, LinearExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, NegationExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, NegationExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, DivisionExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, PowExpression, operator.add] = _get_var_add_expression
_binary_map[ScalarVar, PowExpression, operator.add] = _get_var_add_expression
_binary_map[_GeneralVarData, float, operator.sub] = _get_var_sub_float
_binary_map[_GeneralVarData, int, operator.sub] = _get_var_sub_float
_binary_map[ScalarVar, float, operator.sub] = _get_var_sub_float
_binary_map[ScalarVar, int, operator.sub] = _get_var_sub_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.sub] = _get_var_sub_var
_binary_map[_GeneralVarData, _ParamData, operator.sub] = _get_var_sub_param
_binary_map[_GeneralVarData, ScalarVar, operator.sub] = _get_var_sub_var
_binary_map[_GeneralVarData, ScalarParam, operator.sub] = _get_var_sub_param
_binary_map[ScalarVar, _GeneralVarData, operator.sub] = _get_var_sub_var
_binary_map[ScalarVar, _ParamData, operator.sub] = _get_var_sub_param
_binary_map[ScalarVar, ScalarVar, operator.sub] = _get_var_sub_var
_binary_map[ScalarVar, ScalarParam, operator.sub] = _get_var_sub_param
_binary_map[_GeneralVarData, ProductExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, ProductExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, SumExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, SumExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, LinearExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, LinearExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, NegationExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, NegationExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, DivisionExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, PowExpression, operator.sub] = _get_var_sub_expression
_binary_map[ScalarVar, PowExpression, operator.sub] = _get_var_sub_expression
_binary_map[_GeneralVarData, float, operator.truediv] = _get_var_div_float
_binary_map[_GeneralVarData, int, operator.truediv] = _get_var_div_float
_binary_map[ScalarVar, float, operator.truediv] = _get_var_div_float
_binary_map[ScalarVar, int, operator.truediv] = _get_var_div_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.truediv] = _get_var_div_var
_binary_map[_GeneralVarData, _ParamData, operator.truediv] = _get_var_div_param
_binary_map[_GeneralVarData, ScalarVar, operator.truediv] = _get_var_div_var
_binary_map[_GeneralVarData, ScalarParam, operator.truediv] = _get_var_div_param
_binary_map[ScalarVar, _GeneralVarData, operator.truediv] = _get_var_div_var
_binary_map[ScalarVar, _ParamData, operator.truediv] = _get_var_div_param
_binary_map[ScalarVar, ScalarVar, operator.truediv] = _get_var_div_var
_binary_map[ScalarVar, ScalarParam, operator.truediv] = _get_var_div_param
_binary_map[_GeneralVarData, ProductExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, ProductExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, SumExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, SumExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, LinearExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, LinearExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, NegationExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, NegationExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, DivisionExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, PowExpression, operator.truediv] = _get_var_div_expression
_binary_map[ScalarVar, PowExpression, operator.truediv] = _get_var_div_expression
_binary_map[_GeneralVarData, float, operator.pow] = _get_var_pow_float
_binary_map[_GeneralVarData, int, operator.pow] = _get_var_pow_float
_binary_map[ScalarVar, float, operator.pow] = _get_var_pow_float
_binary_map[ScalarVar, int, operator.pow] = _get_var_pow_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.pow] = _get_var_pow_var
_binary_map[_GeneralVarData, _ParamData, operator.pow] = _get_var_pow_param
_binary_map[_GeneralVarData, ScalarVar, operator.pow] = _get_var_pow_var
_binary_map[_GeneralVarData, ScalarParam, operator.pow] = _get_var_pow_param
_binary_map[ScalarVar, _GeneralVarData, operator.pow] = _get_var_pow_var
_binary_map[ScalarVar, _ParamData, operator.pow] = _get_var_pow_param
_binary_map[ScalarVar, ScalarVar, operator.pow] = _get_var_pow_var
_binary_map[ScalarVar, ScalarParam, operator.pow] = _get_var_pow_param
_binary_map[_GeneralVarData, ProductExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, ProductExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, SumExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, SumExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, LinearExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, LinearExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, NegationExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, NegationExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, DivisionExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, PowExpression, operator.pow] = _get_var_pow_expression
_binary_map[ScalarVar, PowExpression, operator.pow] = _get_var_pow_expression
_binary_map[_GeneralVarData, float, operator.le] = _get_var_le_float
_binary_map[_GeneralVarData, int, operator.le] = _get_var_le_float
_binary_map[ScalarVar, float, operator.le] = _get_var_le_float
_binary_map[ScalarVar, int, operator.le] = _get_var_le_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.le] = _get_var_le_var
_binary_map[_GeneralVarData, _ParamData, operator.le] = _get_var_le_param
_binary_map[_GeneralVarData, ScalarVar, operator.le] = _get_var_le_var
_binary_map[_GeneralVarData, ScalarParam, operator.le] = _get_var_le_param
_binary_map[ScalarVar, _GeneralVarData, operator.le] = _get_var_le_var
_binary_map[ScalarVar, _ParamData, operator.le] = _get_var_le_param
_binary_map[ScalarVar, ScalarVar, operator.le] = _get_var_le_var
_binary_map[ScalarVar, ScalarParam, operator.le] = _get_var_le_param
_binary_map[_GeneralVarData, ProductExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, ProductExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, SumExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, SumExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, LinearExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, LinearExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, NegationExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, NegationExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, DivisionExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, PowExpression, operator.le] = _get_var_le_expression
_binary_map[ScalarVar, PowExpression, operator.le] = _get_var_le_expression
_binary_map[_GeneralVarData, float, operator.ge] = _get_var_ge_float
_binary_map[_GeneralVarData, int, operator.ge] = _get_var_ge_float
_binary_map[ScalarVar, float, operator.ge] = _get_var_ge_float
_binary_map[ScalarVar, int, operator.ge] = _get_var_ge_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.ge] = _get_var_ge_var
_binary_map[_GeneralVarData, _ParamData, operator.ge] = _get_var_ge_param
_binary_map[_GeneralVarData, ScalarVar, operator.ge] = _get_var_ge_var
_binary_map[_GeneralVarData, ScalarParam, operator.ge] = _get_var_ge_param
_binary_map[ScalarVar, _GeneralVarData, operator.ge] = _get_var_ge_var
_binary_map[ScalarVar, _ParamData, operator.ge] = _get_var_ge_param
_binary_map[ScalarVar, ScalarVar, operator.ge] = _get_var_ge_var
_binary_map[ScalarVar, ScalarParam, operator.ge] = _get_var_ge_param
_binary_map[_GeneralVarData, ProductExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, ProductExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, SumExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, SumExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, LinearExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, LinearExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, NegationExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, NegationExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, DivisionExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, PowExpression, operator.ge] = _get_var_ge_expression
_binary_map[ScalarVar, PowExpression, operator.ge] = _get_var_ge_expression
_binary_map[_GeneralVarData, float, operator.eq] = _get_var_eq_float
_binary_map[_GeneralVarData, int, operator.eq] = _get_var_eq_float
_binary_map[ScalarVar, float, operator.eq] = _get_var_eq_float
_binary_map[ScalarVar, int, operator.eq] = _get_var_eq_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.eq] = _get_var_eq_var
_binary_map[_GeneralVarData, _ParamData, operator.eq] = _get_var_eq_param
_binary_map[_GeneralVarData, ScalarVar, operator.eq] = _get_var_eq_var
_binary_map[_GeneralVarData, ScalarParam, operator.eq] = _get_var_eq_param
_binary_map[ScalarVar, _GeneralVarData, operator.eq] = _get_var_eq_var
_binary_map[ScalarVar, _ParamData, operator.eq] = _get_var_eq_param
_binary_map[ScalarVar, ScalarVar, operator.eq] = _get_var_eq_var
_binary_map[ScalarVar, ScalarParam, operator.eq] = _get_var_eq_param
_binary_map[_GeneralVarData, ProductExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, ProductExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, MonomialTermExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, MonomialTermExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, SumExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, SumExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, LinearExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, LinearExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, NegationExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, NegationExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, DivisionExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, DivisionExpression, operator.eq] = _get_var_eq_expression
_binary_map[_GeneralVarData, PowExpression, operator.eq] = _get_var_eq_expression
_binary_map[ScalarVar, PowExpression, operator.eq] = _get_var_eq_expression

_binary_map[ProductExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[ProductExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[ProductExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[ProductExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[ProductExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[ProductExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[ProductExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[
    ProductExpression, MonomialTermExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[ProductExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, float, operator.add] = _get_expression_add_float
_binary_map[ProductExpression, int, operator.add] = _get_expression_add_float
_binary_map[ProductExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[ProductExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[ProductExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[ProductExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[ProductExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[
    ProductExpression, MonomialTermExpression, operator.add
] = _get_expression_add_expression
_binary_map[ProductExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[ProductExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[ProductExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[ProductExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[ProductExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[ProductExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[ProductExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[
    ProductExpression, MonomialTermExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[ProductExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[ProductExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[ProductExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[ProductExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[ProductExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[ProductExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[ProductExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    ProductExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[ProductExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    ProductExpression, NegationExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    ProductExpression, DivisionExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[ProductExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[ProductExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[ProductExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[ProductExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[ProductExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[ProductExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[ProductExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[
    ProductExpression, MonomialTermExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[ProductExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, float, operator.le] = _get_expression_le_float
_binary_map[ProductExpression, int, operator.le] = _get_expression_le_float
_binary_map[ProductExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[ProductExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[ProductExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[ProductExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[ProductExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[ProductExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[ProductExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[ProductExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[ProductExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[ProductExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[ProductExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[ProductExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[ProductExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[ProductExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[ProductExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[ProductExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[ProductExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[ProductExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[ProductExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[ProductExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[MonomialTermExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[MonomialTermExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[MonomialTermExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[MonomialTermExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[MonomialTermExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[
    MonomialTermExpression, ProductExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[
    MonomialTermExpression, NegationExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[
    MonomialTermExpression, DivisionExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, float, operator.add] = _get_expression_add_float
_binary_map[MonomialTermExpression, int, operator.add] = _get_expression_add_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[MonomialTermExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[MonomialTermExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[MonomialTermExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[
    MonomialTermExpression, ProductExpression, operator.add
] = _get_expression_add_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.add
] = _get_expression_add_expression
_binary_map[MonomialTermExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[
    MonomialTermExpression, NegationExpression, operator.add
] = _get_expression_add_expression
_binary_map[
    MonomialTermExpression, DivisionExpression, operator.add
] = _get_expression_add_expression
_binary_map[MonomialTermExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[MonomialTermExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[MonomialTermExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[MonomialTermExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[MonomialTermExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[
    MonomialTermExpression, ProductExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[
    MonomialTermExpression, NegationExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[
    MonomialTermExpression, DivisionExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[MonomialTermExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[MonomialTermExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[MonomialTermExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[MonomialTermExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[
    MonomialTermExpression, ProductExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, SumExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, LinearExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, NegationExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, DivisionExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    MonomialTermExpression, PowExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[MonomialTermExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[MonomialTermExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[MonomialTermExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[MonomialTermExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[MonomialTermExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[
    MonomialTermExpression, ProductExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[
    MonomialTermExpression, NegationExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[
    MonomialTermExpression, DivisionExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, float, operator.le] = _get_expression_le_float
_binary_map[MonomialTermExpression, int, operator.le] = _get_expression_le_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[MonomialTermExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[MonomialTermExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[MonomialTermExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[MonomialTermExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.le
] = _get_expression_le_expression
_binary_map[MonomialTermExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[MonomialTermExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[MonomialTermExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[MonomialTermExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[MonomialTermExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[MonomialTermExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.ge
] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[MonomialTermExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[MonomialTermExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[MonomialTermExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[MonomialTermExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[MonomialTermExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[
    MonomialTermExpression, MonomialTermExpression, operator.eq
] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[SumExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[SumExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[SumExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[SumExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[SumExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[SumExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[SumExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[SumExpression, float, operator.add] = _get_expression_add_float
_binary_map[SumExpression, int, operator.add] = _get_expression_add_float
_binary_map[SumExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[SumExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[SumExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[SumExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[SumExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[SumExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[SumExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[SumExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[SumExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[SumExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[SumExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[SumExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[SumExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[SumExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[SumExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[SumExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[SumExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[SumExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[SumExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    SumExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[SumExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[SumExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[SumExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[SumExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[SumExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[SumExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[SumExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[SumExpression, float, operator.le] = _get_expression_le_float
_binary_map[SumExpression, int, operator.le] = _get_expression_le_float
_binary_map[SumExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[SumExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[SumExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[SumExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[SumExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[SumExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[SumExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[SumExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[SumExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[SumExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[SumExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[SumExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[SumExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[SumExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[SumExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[SumExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[SumExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[SumExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[SumExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[SumExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[LinearExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[LinearExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[LinearExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[LinearExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[LinearExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[LinearExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[LinearExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[LinearExpression, float, operator.add] = _get_expression_add_float
_binary_map[LinearExpression, int, operator.add] = _get_expression_add_float
_binary_map[LinearExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[LinearExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[LinearExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[LinearExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[LinearExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[LinearExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[LinearExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[LinearExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[LinearExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[LinearExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[LinearExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[LinearExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[LinearExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[LinearExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[LinearExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[LinearExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[LinearExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[LinearExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[LinearExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    LinearExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[LinearExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[LinearExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[LinearExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[LinearExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[LinearExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[LinearExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[LinearExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[LinearExpression, float, operator.le] = _get_expression_le_float
_binary_map[LinearExpression, int, operator.le] = _get_expression_le_float
_binary_map[LinearExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[LinearExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[LinearExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[LinearExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[LinearExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[LinearExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[LinearExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[LinearExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[LinearExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[LinearExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[LinearExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[LinearExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[LinearExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[LinearExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[LinearExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[LinearExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[LinearExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[LinearExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[LinearExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[LinearExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[NegationExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[NegationExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[NegationExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[NegationExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[NegationExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[NegationExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[NegationExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[
    NegationExpression, MonomialTermExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[NegationExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, float, operator.add] = _get_expression_add_float
_binary_map[NegationExpression, int, operator.add] = _get_expression_add_float
_binary_map[NegationExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[NegationExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[NegationExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[NegationExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[NegationExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[
    NegationExpression, MonomialTermExpression, operator.add
] = _get_expression_add_expression
_binary_map[NegationExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[NegationExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[NegationExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[NegationExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[NegationExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[NegationExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[NegationExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[
    NegationExpression, MonomialTermExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[NegationExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[NegationExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[NegationExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[NegationExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[NegationExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[NegationExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[
    NegationExpression, ProductExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    NegationExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[NegationExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    NegationExpression, NegationExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    NegationExpression, DivisionExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[NegationExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[NegationExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[NegationExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[NegationExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[NegationExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[NegationExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[NegationExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[
    NegationExpression, MonomialTermExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[NegationExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, float, operator.le] = _get_expression_le_float
_binary_map[NegationExpression, int, operator.le] = _get_expression_le_float
_binary_map[NegationExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[NegationExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[NegationExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[NegationExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[NegationExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[NegationExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[NegationExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[NegationExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[NegationExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[NegationExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[NegationExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[NegationExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[NegationExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[NegationExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[NegationExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[NegationExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[NegationExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[NegationExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[NegationExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[NegationExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[DivisionExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[DivisionExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[DivisionExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[DivisionExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[DivisionExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[DivisionExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[DivisionExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[
    DivisionExpression, MonomialTermExpression, operator.mul
] = _get_expression_mul_expression
_binary_map[DivisionExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, float, operator.add] = _get_expression_add_float
_binary_map[DivisionExpression, int, operator.add] = _get_expression_add_float
_binary_map[DivisionExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[DivisionExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[DivisionExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[DivisionExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[DivisionExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[
    DivisionExpression, MonomialTermExpression, operator.add
] = _get_expression_add_expression
_binary_map[DivisionExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[DivisionExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[DivisionExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[DivisionExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[DivisionExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[DivisionExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[DivisionExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[
    DivisionExpression, MonomialTermExpression, operator.sub
] = _get_expression_sub_expression
_binary_map[DivisionExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[DivisionExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[DivisionExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[DivisionExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[DivisionExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[DivisionExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[
    DivisionExpression, ProductExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    DivisionExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[DivisionExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    DivisionExpression, NegationExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[
    DivisionExpression, DivisionExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[DivisionExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[DivisionExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[DivisionExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[DivisionExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[DivisionExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[DivisionExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[DivisionExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[
    DivisionExpression, MonomialTermExpression, operator.pow
] = _get_expression_pow_expression
_binary_map[DivisionExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, float, operator.le] = _get_expression_le_float
_binary_map[DivisionExpression, int, operator.le] = _get_expression_le_float
_binary_map[DivisionExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[DivisionExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[DivisionExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[DivisionExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[DivisionExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[DivisionExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[DivisionExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[DivisionExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[DivisionExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[DivisionExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[DivisionExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[DivisionExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[DivisionExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[DivisionExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[DivisionExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[DivisionExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[DivisionExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[DivisionExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[DivisionExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[DivisionExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[PowExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[PowExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[PowExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[PowExpression, _ParamData, operator.mul] = _get_expression_mul_param
_binary_map[PowExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[PowExpression, ScalarParam, operator.mul] = _get_expression_mul_param
_binary_map[PowExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[PowExpression, float, operator.add] = _get_expression_add_float
_binary_map[PowExpression, int, operator.add] = _get_expression_add_float
_binary_map[PowExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[PowExpression, _ParamData, operator.add] = _get_expression_add_param
_binary_map[PowExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[PowExpression, ScalarParam, operator.add] = _get_expression_add_param
_binary_map[PowExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[PowExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[PowExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[PowExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[PowExpression, _ParamData, operator.sub] = _get_expression_sub_param
_binary_map[PowExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[PowExpression, ScalarParam, operator.sub] = _get_expression_sub_param
_binary_map[PowExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[PowExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[PowExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[PowExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[PowExpression, _ParamData, operator.truediv] = _get_expression_div_param
_binary_map[PowExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[PowExpression, ScalarParam, operator.truediv] = _get_expression_div_param
_binary_map[PowExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[
    PowExpression, MonomialTermExpression, operator.truediv
] = _get_expression_div_expression
_binary_map[PowExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[PowExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[PowExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[PowExpression, _ParamData, operator.pow] = _get_expression_pow_param
_binary_map[PowExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[PowExpression, ScalarParam, operator.pow] = _get_expression_pow_param
_binary_map[PowExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[PowExpression, float, operator.le] = _get_expression_le_float
_binary_map[PowExpression, int, operator.le] = _get_expression_le_float
_binary_map[PowExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[PowExpression, _ParamData, operator.le] = _get_expression_le_param
_binary_map[PowExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[PowExpression, ScalarParam, operator.le] = _get_expression_le_param
_binary_map[PowExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[PowExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[PowExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[PowExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[PowExpression, _ParamData, operator.ge] = _get_expression_ge_param
_binary_map[PowExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[PowExpression, ScalarParam, operator.ge] = _get_expression_ge_param
_binary_map[PowExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[PowExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[PowExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[PowExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[PowExpression, _ParamData, operator.eq] = _get_expression_eq_param
_binary_map[PowExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[PowExpression, ScalarParam, operator.eq] = _get_expression_eq_param
_binary_map[PowExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[PowExpression, PowExpression, operator.eq] = _get_expression_eq_expression


def _get_operand_float(operand):
    return operand


def _get_operand_var(operand):
    return operand._pe


def _get_operand_expr(operand):
    return operand._pe


_operand_map = dict()
_operand_map[float] = _get_operand_float
_operand_map[int] = _get_operand_float
_operand_map[_GeneralVarData] = _get_operand_var
_operand_map[ScalarVar] = _get_operand_var
_operand_map[_ParamData] = _get_operand_var
_operand_map[ScalarParam] = _get_operand_var
_operand_map[ProductExpression] = _get_operand_expr
_operand_map[MonomialTermExpression] = _get_operand_expr
_operand_map[SumExpression] = _get_operand_expr
_operand_map[LinearExpression] = _get_operand_expr
_operand_map[NegationExpression] = _get_operand_expr
_operand_map[DivisionExpression] = _get_operand_expr
_operand_map[PowExpression] = _get_operand_expr


def _sum_get_operand_float(operand):
    return pk.expression_from_float(operand)


def _sum_get_operand_var(operand):
    return pk.expression_from_var(operand._pe)


_sum_operand_map = dict()
_sum_operand_map[float] = _sum_get_operand_float
_sum_operand_map[int] = _sum_get_operand_float
_sum_operand_map[_GeneralVarData] = _sum_get_operand_var
_sum_operand_map[ScalarVar] = _sum_get_operand_var
_sum_operand_map[_ParamData] = _sum_get_operand_var
_sum_operand_map[ScalarParam] = _sum_get_operand_var
_sum_operand_map[ProductExpression] = _get_operand_expr
_sum_operand_map[MonomialTermExpression] = _get_operand_expr
_sum_operand_map[SumExpression] = _get_operand_expr
_sum_operand_map[LinearExpression] = _get_operand_expr
_sum_operand_map[NegationExpression] = _get_operand_expr
_sum_operand_map[DivisionExpression] = _get_operand_expr
_sum_operand_map[PowExpression] = _get_operand_expr
