import poek as pk
try:
    pyomo_available=True
    from pyomo.core.expr.visitor import _EvaluationVisitor
    from pyomo.core.expr.numvalue import nonpyomo_leaf_types, value
    from pyomo.environ import Constraint, Objective
    from pyomo.core.base import _ObjectiveData
except:
    _EvaluationVisitor=object
    pyomo_available=False


def quicksum(args, start=0):
    """
    A utility function to compute a sum of Poek expressions.

    The behavior of :func:`quicksum` is similar to the builtin :func:`sum`
    function, but this function generates a more compact Poek
    expression.

    Args:
        args: A generator for terms in the sum.

        start: A value that is initializes the sum.  If
            this value is not a numeric constant, then the +=
            operator is used to add terms to this object.
            Defaults to zero.

    Returns:
        The value of the sum, which may be a Poek expression object.
    """
    e = start
    for arg in args:
        e += arg
    return e


class ToCoekExpression(_EvaluationVisitor):

    def __init__(self, model):
        self._model = model
        self._var = {}

    def visit(self, node, values):
        """ Visit nodes that have been expanded """
        return node._apply_operation(values)

    def visiting_potential_leaf(self, node):
        """
        Visiting a potential leaf.

        Return True if the node is not expanded.
        """
        if node.__class__ in nonpyomo_leaf_types:
            return True, node

        if node.is_variable_type():
            tmp = id(node)
            if tmp in self._var:
                var = self._var[tmp]
            else:
                var = pk.variable(lb=node.lb, ub=node.ub, init=node.value, binary=node.is_binary(), integer=node.is_integer())
                self._model.use(var)
                self._var[tmp] = var
            return True, var

        if not node.is_expression_type():
            return True, value(node)

        return False, None


def pyomo_to_poek(pyomo_model, poek_model):
    if not pyomo_available:
        raise RuntimeError("Error running pyomo_to_poek - Pyomo is not installed.")
    visitor = ToCoekExpression(poek_model)
    for cdata in pyomo_model.component_data_objects(Objective, active=True):
        e = visitor.dfs_postorder_stack(cdata.expr)
        poek_model.add( e )
    for cdata in pyomo_model.component_data_objects(Constraint, active=True):
        e = visitor.dfs_postorder_stack(cdata.body)
        if cdata.equality:
            poek_model.add( e == value(cdata.lower) )
        else:
            if cdata.has_lb():
                poek_model.add( e - value(cdata.lower) >= 0)
            if cdata.has_ub():
                poek_model.add( e - value(cdata.upper) <= 0 )

