import sys
import poek as pk
try:
    from pyutilib.misc import Options
    from pyomo.core.expr.visitor import _EvaluationVisitor
    from pyomo.core.expr.numvalue import nonpyomo_leaf_types, value
    from pyomo.core.base import _ObjectiveData, Constraint, Objective
    import pyomo.core.expr.current as EXPR
    pyomo_available=True
except:
    _EvaluationVisitor=object
    pyomo_available=False


class ToCoekExpression(_EvaluationVisitor):

    def __init__(self, model, default_variable_value=False):
        self._model = model
        self.default_variable_value = default_variable_value
        self._var = {}
        self._param = {}
        self._functionMap = {
            'exp': pk.exp,
            'log': pk.log,
            'log10': pk.log10, 
            'sin': pk.sin,
            'asin': pk.asin,
            'sinh': pk.sinh,
            'asinh': pk.asinh,
            'cos': pk.cos,
            'acos': pk.acos,
            'cosh': pk.cosh,
            'acosh': pk.acosh,
            'tan': pk.tan,
            'atan': pk.atan,
            'tanh': pk.tanh,
            'atanh': pk.atanh,
            #'ceil': pk.ceiling,
            #'floor': pk.floor,
            'sqrt': pk.sqrt,
            }

    def visit(self, node, values):
        """ Visit nodes that have been expanded """
        if node.__class__ is EXPR.UnaryFunctionExpression:
            return self._functionMap[node._name](values[0])
        return node._apply_operation(values)

    def visiting_potential_leaf(self, node):
        """
        Visiting a potential leaf.

        Return True if the node is not expanded.
        """
        if node.__class__ in nonpyomo_leaf_types:
            return True, node

        if node.is_parameter_type():
            tmp = id(node)
            #print("PARAM",str(node),tmp)
            if tmp in self._param:
                param = self._param[tmp]
            else:
                param = pk.parameter(value(node))
                self._param[tmp] = param
            return True, param

        if node.is_variable_type():
            tmp = id(node)
            #print("VAR",str(node),tmp)
            if tmp in self._var:
                var = self._var[tmp]
            else:
                if self.default_variable_value is None and node.value is None:
                    print("WARNING: Variable %s is not initialized.  Setting initial value to zero." % str(node))
                    val = 0
                elif self.default_variable_value is not None and node.value is None:
                    val = self.default_variable_value
                else:
                    val = node.value
                var = pk.variable(lb=node.lb, ub=node.ub, value=val, binary=node.is_binary(), integer=node.is_integer(), fixed=node.fixed, name=str(node))
                self._model.add_variable(var)
                self._var[tmp] = var
            return True, var

        if not node.is_expression_type():
            return True, value(node)

        return False, None


def pyomo_to_poek(pyomo_model, default_variable_value=None):
    if not pyomo_available:
        raise RuntimeError("Error running pyomo_to_poek - Pyomo is not installed.")

    poek_model = pk.model()

    visitor = ToCoekExpression(poek_model, default_variable_value=default_variable_value)
    for cdata in pyomo_model.component_data_objects(Objective, active=True):
        e = visitor.dfs_postorder_stack(cdata.expr)
        poek_model.add_objective( e )
    for cdata in pyomo_model.component_data_objects(Constraint, active=True):
        e = visitor.dfs_postorder_stack(cdata.body)
        if cdata.equality:
            poek_model.add_constraint( e == value(cdata.lower) )
        else:
            if cdata.has_lb() and cdata.has_ub():
                poek_model.add_constraint( inequality(value(cdata.lower), e, value(cdata.upper)) )
            elif cdata.has_lb():
                poek_model.add_constraint( e >= value(cdata.lower) )
            elif cdata.has_ub():
                poek_model.add_constraint( e <= value(cdata.upper) )

    data = Options()
    data.poek_model = poek_model
    data.pyo2pk_var = visitor._var
    data.pyo2pk_param = visitor._param
    sys.stdout.flush()
    return data

