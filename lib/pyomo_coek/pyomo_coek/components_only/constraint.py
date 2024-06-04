from pyomo.core.base.component import ActiveComponentData
from pyomo.core.base.indexed_component import ActiveIndexedComponent
from pyomo.core.base.constraint import _ConstraintData
from pyomo.core.expr.numvalue import (
    NumericValue,
    value,
    as_numeric,
    is_fixed,
    native_numeric_types,
)
from pyomo.core.base.component import ActiveComponentData, ModelComponentFactory
from pyomo.core.base.indexed_component import (
    ActiveIndexedComponent,
    UnindexedComponent_set,
    rule_wrapper,
)
from typing import overload
from pyomo.core.base.initializer import (
    Initializer,
    IndexedCallInitializer,
    CountedCallInitializer,
)
from pyomo.common.timing import ConstructionTimer
from pyomo.common.log import is_debug_set
import logging
import sys
from pyomo.common.formatting import tabular_writer
from pyomo.core.base.global_set import UnindexedComponent_index
from pyomo.core.base.disable_methods import disable_methods


logger = logging.getLogger(__name__)


class _GeneralConstraintData(_ConstraintData):

    __slots__ = ("_pe",)

    def __init__(self, expr=None, component=None):
        super().__init__(component)
        self._pe = None
        if expr is not None:
            self.set_value(expr)

    @property
    def body(self):
        return self._pe.body()

    def _lb(self):
        if self._pe.has_lower():
            return self._pe.lower()
        else:
            return None

    def _ub(self):
        if self._pe.has_upper():
            return self._pe.upper()
        else:
            return None

    @property
    def lower(self):
        bound = self._lb()
        if bound is None:
            return None
        return as_numeric(bound)

    @property
    def upper(self):
        bound = self._ub()
        if bound is None:
            return None
        return as_numeric(bound)

    @property
    def lb(self):
        return self._pe.lb

    @property
    def ub(self):
        return self._pe.ub

    @property
    def expr(self):
        return self._pe

    def get_value(self):
        return self._pe

    def set_value(self, expr):
        expr_type = type(expr)

        if expr_type is tuple:
            if len(expr) == 2:
                self._pe = expr[0] == expr[1]
            elif len(expr) == 3:
                if expr[0] is None:
                    self._pe = expr[1] <= expr[2]
                elif expr[2] is None:
                    self._pe = expr[0] <= expr[1]
                else:
                    # This is a hack to detect if a pyomo_coek variable is being used
                    if hasattr(expr[1], "_pe"):
                        self._pe = pk.inequality(expr[0], expr[1]._pe, expr[2])
                    else:
                        self._pe = pk.inequality(expr[0], expr[1], expr[2])
                    # raise NotImplementedError("Ranged inequalities are not supported yet")
            else:
                raise ValueError(
                    f"expected 2 or 3 arguments for tuple expressions; got {len(expr)}"
                )
        else:
            self._pe = expr


@ModelComponentFactory.register("General constraint expressions.")
class Constraint(ActiveIndexedComponent):
    _ComponentDataClass = _GeneralConstraintData

    class Infeasible(object):
        pass

    Feasible = ActiveIndexedComponent.Skip
    NoConstraint = ActiveIndexedComponent.Skip
    Violated = Infeasible
    Satisfied = Feasible

    def __new__(cls, *args, **kwds):
        if cls != Constraint:
            return super(Constraint, cls).__new__(cls)
        if not args or (args[0] is UnindexedComponent_set and len(args) == 1):
            return super(Constraint, cls).__new__(AbstractScalarConstraint)
        else:
            return super(Constraint, cls).__new__(IndexedConstraint)

    @overload
    def __init__(self, *indexes, expr=None, rule=None, name=None, doc=None):
        ...

    def __init__(self, *args, **kwargs):
        _init = self._pop_from_kwargs("Constraint", kwargs, ("rule", "expr"), None)
        # Special case: we accept 2- and 3-tuples as constraints
        if type(_init) is tuple:
            self.rule = Initializer(_init, treat_sequences_as_mappings=False)
        else:
            self.rule = Initializer(_init)

        kwargs.setdefault("ctype", Constraint)
        ActiveIndexedComponent.__init__(self, *args, **kwargs)

    def construct(self, data=None):
        """
        Construct the expression(s) for this constraint.
        """
        if self._constructed:
            return
        self._constructed = True

        timer = ConstructionTimer(self)
        if is_debug_set(logger):
            logger.debug("Constructing constraint %s" % (self.name))

        rule = self.rule
        try:
            # We do not (currently) accept data for constructing Constraints
            index = None
            assert data is None

            if rule is None:
                # If there is no rule, then we are immediately done.
                return

            if rule.constant() and self.is_indexed():
                raise IndexError(
                    "Constraint '%s': Cannot initialize multiple indices "
                    "of a constraint with a single expression" % (self.name,)
                )

            block = self.parent_block()
            if rule.contains_indices():
                # The index is coming in externally; we need to validate it
                for index in rule.indices():
                    self[index] = rule(block, index)
            elif not self.index_set().isfinite():
                # If the index is not finite, then we cannot iterate
                # over it.  Since the rule doesn't provide explicit
                # indices, then there is nothing we can do (the
                # assumption is that the user will trigger specific
                # indices to be created at a later time).
                pass
            else:
                # Bypass the index validation and create the member directly
                for index in self.index_set():
                    self._setitem_when_not_present(index, rule(block, index))
        except Exception:
            err = sys.exc_info()[1]
            logger.error(
                "Rule failed when generating expression for "
                "Constraint %s with index %s:\n%s: %s"
                % (self.name, str(index), type(err).__name__, err)
            )
            raise
        finally:
            timer.report()

    def _getitem_when_not_present(self, idx):
        if self.rule is None:
            raise KeyError(idx)
        con = self._setitem_when_not_present(idx, self.rule(self.parent_block(), idx))
        if con is None:
            raise KeyError(idx)
        return con

    def _pprint(self):
        """
        Return data that will be printed for this component.
        """
        return (
            [
                ("Size", len(self)),
                ("Index", self._index_set if self.is_indexed() else None),
                ("Active", self.active),
            ],
            self.items(),
            ("Lower", "Body", "Upper", "Active"),
            lambda k, v: [
                "-Inf" if v.lower is None else v.lower,
                v.body,
                "+Inf" if v.upper is None else v.upper,
                v.active,
            ],
        )

    def display(self, prefix="", ostream=None):
        """
        Print component state information

        This duplicates logic in Component.pprint()
        """
        if not self.active:
            return
        if ostream is None:
            ostream = sys.stdout
        tab = "    "
        ostream.write(prefix + self.local_name + " : ")
        ostream.write("Size=" + str(len(self)))

        ostream.write("\n")
        tabular_writer(
            ostream,
            prefix + tab,
            ((k, v) for k, v in self._data.items() if v.active),
            ("Lower", "Body", "Upper"),
            lambda k, v: [
                value(v.lower, exception=False),
                value(v.body, exception=False),
                value(v.upper, exception=False),
            ],
        )


class ScalarConstraint(_GeneralConstraintData, Constraint):
    """
    ScalarConstraint is the implementation representing a single,
    non-indexed constraint.
    """

    def __init__(self, *args, **kwds):
        _GeneralConstraintData.__init__(self, component=self, expr=None)
        Constraint.__init__(self, *args, **kwds)
        self._index = UnindexedComponent_index

    #
    # Since this class derives from Component and
    # Component.__getstate__ just packs up the entire __dict__ into
    # the state dict, we do not need to define the __getstate__ or
    # __setstate__ methods.  We just defer to the super() get/set
    # state.  Since all of our get/set state methods rely on super()
    # to traverse the MRO, this will automatically pick up both the
    # Component and Data base classes.
    #

    #
    # Singleton constraints are strange in that we want them to be
    # both be constructed but have len() == 0 when not initialized with
    # anything (at least according to the unit tests that are
    # currently in place). So during initialization only, we will
    # treat them as "indexed" objects where things like
    # Constraint.Skip are managed. But after that they will behave
    # like _ConstraintData objects where set_value does not handle
    # Constraint.Skip but expects a valid expression or None.
    #
    @property
    def body(self):
        """Access the body of a constraint expression."""
        if not self._data:
            raise ValueError(
                "Accessing the body of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.body.fget(self)

    @property
    def lower(self):
        """Access the lower bound of a constraint expression."""
        if not self._data:
            raise ValueError(
                "Accessing the lower bound of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.lower.fget(self)

    @property
    def upper(self):
        """Access the upper bound of a constraint expression."""
        if not self._data:
            raise ValueError(
                "Accessing the upper bound of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.upper.fget(self)

    @property
    def equality(self):
        """A boolean indicating whether this is an equality constraint."""
        if not self._data:
            raise ValueError(
                "Accessing the equality flag of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.equality.fget(self)

    @property
    def strict_lower(self):
        """A boolean indicating whether this constraint has a strict lower bound."""
        if not self._data:
            raise ValueError(
                "Accessing the strict_lower flag of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.strict_lower.fget(self)

    @property
    def strict_upper(self):
        """A boolean indicating whether this constraint has a strict upper bound."""
        if not self._data:
            raise ValueError(
                "Accessing the strict_upper flag of ScalarConstraint "
                "'%s' before the Constraint has been assigned "
                "an expression. There is currently "
                "nothing to access." % (self.name)
            )
        return _GeneralConstraintData.strict_upper.fget(self)

    def clear(self):
        self._data = {}

    def set_value(self, expr):
        """Set the expression on this constraint."""
        if not self._data:
            self._data[None] = self
        return super(ScalarConstraint, self).set_value(expr)

    #
    # Leaving this method for backward compatibility reasons.
    # (probably should be removed)
    #
    def add(self, index, expr):
        """Add a constraint with a given index."""
        if index is not None:
            raise ValueError(
                "ScalarConstraint object '%s' does not accept "
                "index values other than None. Invalid value: %s" % (self.name, index)
            )
        self.set_value(expr)
        return self


@disable_methods(
    {"add", "set_value", "body", "lower", "upper", "equality", "strict_lower", "strict_upper"}
)
class AbstractScalarConstraint(ScalarConstraint):
    pass


class IndexedConstraint(Constraint):

    #
    # Leaving this method for backward compatibility reasons
    #
    # Note: Beginning after Pyomo 5.2 this method will now validate that
    # the index is in the underlying index set (through 5.2 the index
    # was not checked).
    #
    def add(self, index, expr):
        """Add a constraint with a given index."""
        return self.__setitem__(index, expr)
