#
# NOTE: Since the to_list() method is only used for testing API
# compatibility, we include some simplifications of the Pyomo
# expression tree to simplify test comparisons
#
# IDEA: Pull these out and apply them consistently in the testing
# using a 'simplify()' function.
#
import pyomo.core.expr.expr_common as common
from pyomo.core.expr.visitor import ExpressionValueVisitor
from pyomo.common.numeric_types import (
    nonpyomo_leaf_types,
    native_types,
    native_numeric_types,
    value,
)

namemap = {'sum':'+', 'mon':'*', 'prod':'*'}

class _ToListVisitor(ExpressionValueVisitor):
    _expression_handlers = None

    def __init__(self, verbose, smap):
        super(_ToListVisitor, self).__init__()
        self.verbose = verbose
        self.smap = smap

    def visit(self, node, values):
        """Visit nodes that have been expanded"""
        for i, val in enumerate(values):
            arg = node._args_[i]

            if arg is None:
                values[i] = 'Undefined'
            elif arg.__class__ in native_numeric_types:
                pass
            elif arg.__class__ in nonpyomo_leaf_types:
                values[i] = f"{val}"
            else:
                values[i] = val

        if self._expression_handlers and node.__class__ in self._expression_handlers:
            return [ self._expression_handlers[node.__class__](self, node, values) ]

        #print("HERE", values, self.verbose, self.smap)
        #print("HERE", node._to_string(values, self.verbose, self.smap))
        name = node.getname()
        if name == 'mon' and values[0] == "1.000000":
            return values[1]
        elif name == 'prod' and values[0] == "1.000000":
            return values[1]
        elif name == '*' and values[0] == "1.000000":
            return values[1]
        name = namemap.get(name,name)
        return [ name ] + values

    def visiting_potential_leaf(self, node):
        """
        Visiting a potential leaf.

        Return True if the node is not expanded.
        """
        if node is None:
            return True, None

        if node.__class__ in nonpyomo_leaf_types:
            return True, f"{node:.6f}"

        if node.is_expression_type():
            return False, None

        if hasattr(node, 'to_string'):
            return True, node.to_string(verbose=self.verbose, smap=self.smap)
        else:
            return True, f"{node:.6f}"


def to_list(
    expr, verbose=None, labeler=None, smap=None, compute_values=False
):
    """Return a string representation of an expression.

    Parameters
    ----------
    expr: ExpressionBase
        The root node of an expression tree.

    verbose: bool
        If :const:`True`, then the output is a nested functional form.
        Otherwise, the output is an algebraic expression.  Default is
        retrieved from :py:attr:`common.TO_STRING_VERBOSE`

    labeler: Callable
        If specified, this labeler is used to generate the string
        representation for leaves (Var / Param objects) in the
        expression.

    smap:  SymbolMap
        If specified, this :class:`SymbolMap
        <pyomo.core.expr.symbol_map.SymbolMap>` is used to cache labels.

    compute_values: bool
        If :const:`True`, then parameters and fixed variables are
        evaluated before the expression string is generated.  Default is
        :const:`False`.

    Returns:
        A string representation for the expression.

    """
    verbose = common.TO_STRING_VERBOSE if verbose is None else verbose
    #
    # Setup the symbol map
    #
    if labeler is not None:
        if smap is None:
            smap = SymbolMap()
        smap.default_labeler = labeler
    #
    # TODO: should we deprecate the compute_values option?
    #
    if compute_values:
        expr = evaluate_fixed_subexpressions(expr)
    #
    # Create and execute the visitor pattern
    #
    visitor = _ToListVisitor(verbose, smap)
    val = visitor.dfs_postorder_stack(expr)
    #
    # NOTE: Does this make sense?  Maybe to_list() should return a non-list for numbers and variables.
    #
    #return val
    if type(val) is not list:
        return [val]
    return val

