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
from pyomo.core.base.set import Binary, Reals, Integers
from pyomo.common.collections import ComponentMap
from pyomo.core.expr.numvalue import nonpyomo_leaf_types, native_types
from pyomo.core.expr import logical_expr
from pyomo.common.deprecation import deprecation_warning


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

    __slots__ = ('_stale', '_pe')

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
    __slots__ = ('_pe')
    func = operator.neg


class ProductExpression(BinaryExpression, numeric_expr.ProductExpression):
    __slots__ = ('_pe',)
    func = operator.mul


class MonomialTermExpression(ProductExpression, numeric_expr.MonomialTermExpression):
    __slots__ = tuple()
    func = operator.mul


class DivisionExpression(BinaryExpression, numeric_expr.DivisionExpression):
    __slots__ = ('_pe',)
    func = operator.truediv


class PowExpression(BinaryExpression, numeric_expr.PowExpression):
    __slots__ = ('_pe',)
    func = operator.pow


class SumExpression(ExpressionBase, numeric_expr.SumExpression):
    __slots__ = ('_pe',)

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
    __slots__ = ('_pe',)

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
            if any(arg is not None for arg in
                   (constant, linear_coefs, linear_vars)):
                raise ValueError("Cannot specify both args and any of "
                                 "{constant, linear_coeffs, or linear_vars}")
            if len(args) > 1 and (args[1].__class__ in native_types
                                  or not args[1].is_potentially_variable()):
                deprecation_warning(
                    "LinearExpression has been updated to expect args= to "
                    "be a constant followed by MonomialTermExpressions.  "
                    "The older format (`[const, coefficient_1, ..., "
                    "variable_1, ...]`) is deprecated.", version='6.2')
                args = args[:1] + list(map(
                    MonomialTermExpression,
                    zip(args[1:1+len(args)//2], args[1+len(args)//2:])))
            self._args_ = args
        else:
            self.constant = constant if constant is not None else 0
            self.linear_coefs = linear_coefs if linear_coefs else []
            self.linear_vars = linear_vars if linear_vars else []
            self._args_cache_ = []

        self._pe = pk.construct_linear_expression(
            self.linear_coefs,
            [v._pe for v in self.linear_vars],
            self.constant
        )


class InequalityExpression(BinaryExpression, logical_expr.InequalityExpression):
    __slots__ = ('_pe',)
    func = operator.le

    def __init__(self, args, strict=False):
        super().__init__(args)
        self._strict = strict


class EqualityExpression(BinaryExpression, logical_expr.EqualityExpression):
    __slots__ = ('_pe',)
    func = operator.eq


def quicksum(args):
    return SumExpression(list(args))


def _get_float_mul_var(arg0, arg1):
    return pk.float_mul_var, arg0, arg1._pe


def _get_float_mul_expression(arg0, arg1):
    return pk.float_mul_expression, arg0, arg1._pe


def _get_float_add_var(arg0, arg1):
    return pk.float_add_var, arg0, arg1._pe


def _get_float_add_expression(arg0, arg1):
    return pk.float_add_expression, arg0, arg1._pe


def _get_float_sub_var(arg0, arg1):
    return pk.float_sub_var, arg0, arg1._pe


def _get_float_sub_expression(arg0, arg1):
    return pk.float_sub_expression, arg0, arg1._pe


def _get_float_div_var(arg0, arg1):
    return pk.float_div_var, arg0, arg1._pe


def _get_float_div_expression(arg0, arg1):
    return pk.float_div_expression, arg0, arg1._pe


def _get_float_pow_var(arg0, arg1):
    return pk.float_pow_var, arg0, arg1._pe


def _get_float_pow_expression(arg0, arg1):
    return pk.float_pow_expression, arg0, arg1._pe


def _get_float_le_var(arg0, arg1):
    return pk.float_le_var, arg0, arg1._pe


def _get_float_le_expression(arg0, arg1):
    return pk.float_le_expression, arg0, arg1._pe


def _get_float_ge_var(arg0, arg1):
    return pk.float_ge_var, arg0, arg1._pe


def _get_float_ge_expression(arg0, arg1):
    return pk.float_ge_expression, arg0, arg1._pe


def _get_float_eq_var(arg0, arg1):
    return pk.float_eq_var, arg0, arg1._pe


def _get_float_eq_expression(arg0, arg1):
    return pk.float_eq_expression, arg0, arg1._pe


def _get_var_mul_float(arg0, arg1):
    return pk.var_mul_float, arg0._pe, arg1


def _get_var_mul_var(arg0, arg1):
    return pk.var_mul_var, arg0._pe, arg1._pe


def _get_var_mul_expression(arg0, arg1):
    return pk.var_mul_expression, arg0._pe, arg1._pe


def _get_var_add_float(arg0, arg1):
    return pk.var_add_float, arg0._pe, arg1


def _get_var_add_var(arg0, arg1):
    return pk.var_add_var, arg0._pe, arg1._pe


def _get_var_add_expression(arg0, arg1):
    return pk.var_add_expression, arg0._pe, arg1._pe


def _get_var_sub_float(arg0, arg1):
    return pk.var_sub_float, arg0._pe, arg1


def _get_var_sub_var(arg0, arg1):
    return pk.var_sub_var, arg0._pe, arg1._pe


def _get_var_sub_expression(arg0, arg1):
    return pk.var_sub_expression, arg0._pe, arg1._pe


def _get_var_div_float(arg0, arg1):
    return pk.var_div_float, arg0._pe, arg1


def _get_var_div_var(arg0, arg1):
    return pk.var_div_var, arg0._pe, arg1._pe


def _get_var_div_expression(arg0, arg1):
    return pk.var_div_expression, arg0._pe, arg1._pe


def _get_var_pow_float(arg0, arg1):
    return pk.var_pow_float, arg0._pe, arg1


def _get_var_pow_var(arg0, arg1):
    return pk.var_pow_var, arg0._pe, arg1._pe


def _get_var_pow_expression(arg0, arg1):
    return pk.var_pow_expression, arg0._pe, arg1._pe


def _get_var_le_float(arg0, arg1):
    return pk.var_le_float, arg0._pe, arg1


def _get_var_le_var(arg0, arg1):
    return pk.var_le_var, arg0._pe, arg1._pe


def _get_var_le_expression(arg0, arg1):
    return pk.var_le_expression, arg0._pe, arg1._pe


def _get_var_ge_float(arg0, arg1):
    return pk.var_ge_float, arg0._pe, arg1


def _get_var_ge_var(arg0, arg1):
    return pk.var_ge_var, arg0._pe, arg1._pe


def _get_var_ge_expression(arg0, arg1):
    return pk.var_ge_expression, arg0._pe, arg1._pe


def _get_var_eq_float(arg0, arg1):
    return pk.var_eq_float, arg0._pe, arg1


def _get_var_eq_var(arg0, arg1):
    return pk.var_eq_var, arg0._pe, arg1._pe


def _get_var_eq_expression(arg0, arg1):
    return pk.var_eq_expression, arg0._pe, arg1._pe


def _get_expression_mul_float(arg0, arg1):
    return pk.expression_mul_float, arg0._pe, arg1


def _get_expression_mul_var(arg0, arg1):
    return pk.expression_mul_var, arg0._pe, arg1._pe


def _get_expression_mul_expression(arg0, arg1):
    return pk.expression_mul_expression, arg0._pe, arg1._pe


def _get_expression_add_float(arg0, arg1):
    return pk.expression_add_float, arg0._pe, arg1


def _get_expression_add_var(arg0, arg1):
    return pk.expression_add_var, arg0._pe, arg1._pe


def _get_expression_add_expression(arg0, arg1):
    return pk.expression_add_expression, arg0._pe, arg1._pe


def _get_expression_sub_float(arg0, arg1):
    return pk.expression_sub_float, arg0._pe, arg1


def _get_expression_sub_var(arg0, arg1):
    return pk.expression_sub_var, arg0._pe, arg1._pe


def _get_expression_sub_expression(arg0, arg1):
    return pk.expression_sub_expression, arg0._pe, arg1._pe


def _get_expression_div_float(arg0, arg1):
    return pk.expression_div_float, arg0._pe, arg1


def _get_expression_div_var(arg0, arg1):
    return pk.expression_div_var, arg0._pe, arg1._pe


def _get_expression_div_expression(arg0, arg1):
    return pk.expression_div_expression, arg0._pe, arg1._pe


def _get_expression_pow_float(arg0, arg1):
    return pk.expression_pow_float, arg0._pe, arg1


def _get_expression_pow_var(arg0, arg1):
    return pk.expression_pow_var, arg0._pe, arg1._pe


def _get_expression_pow_expression(arg0, arg1):
    return pk.expression_pow_expression, arg0._pe, arg1._pe


def _get_expression_le_float(arg0, arg1):
    return pk.expression_le_float, arg0._pe, arg1


def _get_expression_le_var(arg0, arg1):
    return pk.expression_le_var, arg0._pe, arg1._pe


def _get_expression_le_expression(arg0, arg1):
    return pk.expression_le_expression, arg0._pe, arg1._pe


def _get_expression_ge_float(arg0, arg1):
    return pk.expression_ge_float, arg0._pe, arg1


def _get_expression_ge_var(arg0, arg1):
    return pk.expression_ge_var, arg0._pe, arg1._pe


def _get_expression_ge_expression(arg0, arg1):
    return pk.expression_ge_expression, arg0._pe, arg1._pe


def _get_expression_eq_float(arg0, arg1):
    return pk.expression_eq_float, arg0._pe, arg1


def _get_expression_eq_var(arg0, arg1):
    return pk.expression_eq_var, arg0._pe, arg1._pe


def _get_expression_eq_expression(arg0, arg1):
    return pk.expression_eq_expression, arg0._pe, arg1._pe


_binary_map = dict()
_binary_map[float, _GeneralVarData, operator.mul] = _get_float_mul_var
_binary_map[int, _GeneralVarData, operator.mul] = _get_float_mul_var
_binary_map[float, ScalarVar, operator.mul] = _get_float_mul_var
_binary_map[int, ScalarVar, operator.mul] = _get_float_mul_var
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
_binary_map[float, ScalarVar, operator.add] = _get_float_add_var
_binary_map[int, ScalarVar, operator.add] = _get_float_add_var
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
_binary_map[float, ScalarVar, operator.sub] = _get_float_sub_var
_binary_map[int, ScalarVar, operator.sub] = _get_float_sub_var
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
_binary_map[float, ScalarVar, operator.truediv] = _get_float_div_var
_binary_map[int, ScalarVar, operator.truediv] = _get_float_div_var
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
_binary_map[float, ScalarVar, operator.pow] = _get_float_pow_var
_binary_map[int, ScalarVar, operator.pow] = _get_float_pow_var
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
_binary_map[float, ScalarVar, operator.le] = _get_float_le_var
_binary_map[int, ScalarVar, operator.le] = _get_float_le_var
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
_binary_map[float, ScalarVar, operator.ge] = _get_float_ge_var
_binary_map[int, ScalarVar, operator.ge] = _get_float_ge_var
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
_binary_map[float, ScalarVar, operator.eq] = _get_float_eq_var
_binary_map[int, ScalarVar, operator.eq] = _get_float_eq_var
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

_binary_map[_GeneralVarData, float, operator.mul] = _get_var_mul_float
_binary_map[_GeneralVarData, int, operator.mul] = _get_var_mul_float
_binary_map[ScalarVar, float, operator.mul] = _get_var_mul_float
_binary_map[ScalarVar, int, operator.mul] = _get_var_mul_float
_binary_map[_GeneralVarData, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[_GeneralVarData, ScalarVar, operator.mul] = _get_var_mul_var
_binary_map[ScalarVar, _GeneralVarData, operator.mul] = _get_var_mul_var
_binary_map[ScalarVar, ScalarVar, operator.mul] = _get_var_mul_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.add] = _get_var_add_var
_binary_map[ScalarVar, _GeneralVarData, operator.add] = _get_var_add_var
_binary_map[ScalarVar, ScalarVar, operator.add] = _get_var_add_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.sub] = _get_var_sub_var
_binary_map[ScalarVar, _GeneralVarData, operator.sub] = _get_var_sub_var
_binary_map[ScalarVar, ScalarVar, operator.sub] = _get_var_sub_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.truediv] = _get_var_div_var
_binary_map[ScalarVar, _GeneralVarData, operator.truediv] = _get_var_div_var
_binary_map[ScalarVar, ScalarVar, operator.truediv] = _get_var_div_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.pow] = _get_var_pow_var
_binary_map[ScalarVar, _GeneralVarData, operator.pow] = _get_var_pow_var
_binary_map[ScalarVar, ScalarVar, operator.pow] = _get_var_pow_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.le] = _get_var_le_var
_binary_map[ScalarVar, _GeneralVarData, operator.le] = _get_var_le_var
_binary_map[ScalarVar, ScalarVar, operator.le] = _get_var_le_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.ge] = _get_var_ge_var
_binary_map[ScalarVar, _GeneralVarData, operator.ge] = _get_var_ge_var
_binary_map[ScalarVar, ScalarVar, operator.ge] = _get_var_ge_var
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
_binary_map[_GeneralVarData, ScalarVar, operator.eq] = _get_var_eq_var
_binary_map[ScalarVar, _GeneralVarData, operator.eq] = _get_var_eq_var
_binary_map[ScalarVar, ScalarVar, operator.eq] = _get_var_eq_var
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
_binary_map[ProductExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[ProductExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[ProductExpression, float, operator.add] = _get_expression_add_float
_binary_map[ProductExpression, int, operator.add] = _get_expression_add_float
_binary_map[ProductExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[ProductExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[ProductExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[ProductExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[ProductExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[ProductExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[ProductExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[ProductExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[ProductExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[ProductExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[ProductExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[ProductExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[ProductExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[ProductExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[ProductExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[ProductExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[ProductExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[ProductExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[ProductExpression, float, operator.le] = _get_expression_le_float
_binary_map[ProductExpression, int, operator.le] = _get_expression_le_float
_binary_map[ProductExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[ProductExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[ProductExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[ProductExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_binary_map[MonomialTermExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[MonomialTermExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[MonomialTermExpression, float, operator.add] = _get_expression_add_float
_binary_map[MonomialTermExpression, int, operator.add] = _get_expression_add_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[MonomialTermExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[MonomialTermExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[MonomialTermExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[MonomialTermExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[MonomialTermExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[MonomialTermExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[MonomialTermExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[MonomialTermExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[MonomialTermExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[MonomialTermExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[MonomialTermExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[MonomialTermExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[MonomialTermExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[MonomialTermExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[MonomialTermExpression, float, operator.le] = _get_expression_le_float
_binary_map[MonomialTermExpression, int, operator.le] = _get_expression_le_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[MonomialTermExpression, ScalarVar, operator.le] = _get_expression_le_var
_binary_map[MonomialTermExpression, ProductExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, SumExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, PowExpression, operator.le] = _get_expression_le_expression
_binary_map[MonomialTermExpression, float, operator.ge] = _get_expression_ge_float
_binary_map[MonomialTermExpression, int, operator.ge] = _get_expression_ge_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.ge] = _get_expression_ge_var
_binary_map[MonomialTermExpression, ScalarVar, operator.ge] = _get_expression_ge_var
_binary_map[MonomialTermExpression, ProductExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, SumExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, PowExpression, operator.ge] = _get_expression_ge_expression
_binary_map[MonomialTermExpression, float, operator.eq] = _get_expression_eq_float
_binary_map[MonomialTermExpression, int, operator.eq] = _get_expression_eq_float
_binary_map[MonomialTermExpression, _GeneralVarData, operator.eq] = _get_expression_eq_var
_binary_map[MonomialTermExpression, ScalarVar, operator.eq] = _get_expression_eq_var
_binary_map[MonomialTermExpression, ProductExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, MonomialTermExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, SumExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, LinearExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, NegationExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, DivisionExpression, operator.eq] = _get_expression_eq_expression
_binary_map[MonomialTermExpression, PowExpression, operator.eq] = _get_expression_eq_expression

_binary_map[SumExpression, float, operator.mul] = _get_expression_mul_float
_binary_map[SumExpression, int, operator.mul] = _get_expression_mul_float
_binary_map[SumExpression, _GeneralVarData, operator.mul] = _get_expression_mul_var
_binary_map[SumExpression, ScalarVar, operator.mul] = _get_expression_mul_var
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
_binary_map[SumExpression, ScalarVar, operator.add] = _get_expression_add_var
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
_binary_map[SumExpression, ScalarVar, operator.sub] = _get_expression_sub_var
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
_binary_map[SumExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[SumExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[SumExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[SumExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[SumExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[SumExpression, ScalarVar, operator.pow] = _get_expression_pow_var
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
_binary_map[SumExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[SumExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[SumExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_binary_map[LinearExpression, ScalarVar, operator.mul] = _get_expression_mul_var
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
_binary_map[LinearExpression, ScalarVar, operator.add] = _get_expression_add_var
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
_binary_map[LinearExpression, ScalarVar, operator.sub] = _get_expression_sub_var
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
_binary_map[LinearExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[LinearExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[LinearExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[LinearExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[LinearExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[LinearExpression, ScalarVar, operator.pow] = _get_expression_pow_var
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
_binary_map[LinearExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[LinearExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[LinearExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_binary_map[NegationExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[NegationExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[NegationExpression, float, operator.add] = _get_expression_add_float
_binary_map[NegationExpression, int, operator.add] = _get_expression_add_float
_binary_map[NegationExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[NegationExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[NegationExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[NegationExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[NegationExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[NegationExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[NegationExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[NegationExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[NegationExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[NegationExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[NegationExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[NegationExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[NegationExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[NegationExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[NegationExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[NegationExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[NegationExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[NegationExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[NegationExpression, float, operator.le] = _get_expression_le_float
_binary_map[NegationExpression, int, operator.le] = _get_expression_le_float
_binary_map[NegationExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[NegationExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[NegationExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[NegationExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_binary_map[DivisionExpression, ScalarVar, operator.mul] = _get_expression_mul_var
_binary_map[DivisionExpression, ProductExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, SumExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, LinearExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, NegationExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, DivisionExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, PowExpression, operator.mul] = _get_expression_mul_expression
_binary_map[DivisionExpression, float, operator.add] = _get_expression_add_float
_binary_map[DivisionExpression, int, operator.add] = _get_expression_add_float
_binary_map[DivisionExpression, _GeneralVarData, operator.add] = _get_expression_add_var
_binary_map[DivisionExpression, ScalarVar, operator.add] = _get_expression_add_var
_binary_map[DivisionExpression, ProductExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, SumExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, LinearExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, NegationExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, DivisionExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, PowExpression, operator.add] = _get_expression_add_expression
_binary_map[DivisionExpression, float, operator.sub] = _get_expression_sub_float
_binary_map[DivisionExpression, int, operator.sub] = _get_expression_sub_float
_binary_map[DivisionExpression, _GeneralVarData, operator.sub] = _get_expression_sub_var
_binary_map[DivisionExpression, ScalarVar, operator.sub] = _get_expression_sub_var
_binary_map[DivisionExpression, ProductExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, SumExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, LinearExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, NegationExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, DivisionExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, PowExpression, operator.sub] = _get_expression_sub_expression
_binary_map[DivisionExpression, float, operator.truediv] = _get_expression_div_float
_binary_map[DivisionExpression, int, operator.truediv] = _get_expression_div_float
_binary_map[DivisionExpression, _GeneralVarData, operator.truediv] = _get_expression_div_var
_binary_map[DivisionExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[DivisionExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[DivisionExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[DivisionExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[DivisionExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[DivisionExpression, ScalarVar, operator.pow] = _get_expression_pow_var
_binary_map[DivisionExpression, ProductExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, MonomialTermExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, SumExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, LinearExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, NegationExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, DivisionExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, PowExpression, operator.pow] = _get_expression_pow_expression
_binary_map[DivisionExpression, float, operator.le] = _get_expression_le_float
_binary_map[DivisionExpression, int, operator.le] = _get_expression_le_float
_binary_map[DivisionExpression, _GeneralVarData, operator.le] = _get_expression_le_var
_binary_map[DivisionExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[DivisionExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[DivisionExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_binary_map[PowExpression, ScalarVar, operator.mul] = _get_expression_mul_var
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
_binary_map[PowExpression, ScalarVar, operator.add] = _get_expression_add_var
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
_binary_map[PowExpression, ScalarVar, operator.sub] = _get_expression_sub_var
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
_binary_map[PowExpression, ScalarVar, operator.truediv] = _get_expression_div_var
_binary_map[PowExpression, ProductExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, MonomialTermExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, SumExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, LinearExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, NegationExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, DivisionExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, PowExpression, operator.truediv] = _get_expression_div_expression
_binary_map[PowExpression, float, operator.pow] = _get_expression_pow_float
_binary_map[PowExpression, int, operator.pow] = _get_expression_pow_float
_binary_map[PowExpression, _GeneralVarData, operator.pow] = _get_expression_pow_var
_binary_map[PowExpression, ScalarVar, operator.pow] = _get_expression_pow_var
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
_binary_map[PowExpression, ScalarVar, operator.le] = _get_expression_le_var
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
_binary_map[PowExpression, ScalarVar, operator.ge] = _get_expression_ge_var
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
_binary_map[PowExpression, ScalarVar, operator.eq] = _get_expression_eq_var
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
_sum_operand_map[ProductExpression] = _get_operand_expr
_sum_operand_map[MonomialTermExpression] = _get_operand_expr
_sum_operand_map[SumExpression] = _get_operand_expr
_sum_operand_map[LinearExpression] = _get_operand_expr
_sum_operand_map[NegationExpression] = _get_operand_expr
_sum_operand_map[DivisionExpression] = _get_operand_expr
_sum_operand_map[PowExpression] = _get_operand_expr
