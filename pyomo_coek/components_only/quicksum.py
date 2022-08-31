import poek as pk
from .variable import _GeneralVarData, ScalarVar


def _get_arg(arg):
    return arg


def _get_arg_float(arg):
    return pk.expression_from_float(arg)


def _get_arg_var(arg):
    return pk.expression_from_var(arg._pe)


_arg_map = dict()
_arg_map[float] = _get_arg_float
_arg_map[int] = _get_arg_float
_arg_map[_GeneralVarData] = _get_arg_var
_arg_map[ScalarVar] = _get_arg_var
_arg_map[pk.expression] = _get_arg


def quicksum(args):
    return pk.sum([_arg_map[type(arg)](arg) for arg in args])


def _expr_repn_float(arg, cexpr):
    cexpr.set_repn_from_double(arg)


_expr_repn_map = dict()
_expr_repn_map[float] = _expr_repn_float
_expr_repn_map[int] = _expr_repn_float


def construct_linear_expression(linear_coefs, linear_vars, constant=0):
    linear_vars = [v._pe for v in linear_vars]
    return pk.construct_linear_expression(linear_coefs, linear_vars, constant)
