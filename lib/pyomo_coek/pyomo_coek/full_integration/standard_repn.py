import poek as pk
from pyomo.repn.standard_repn import StandardRepn, _repn_collectors
from .expression import (
    _var_term_to_var_map,
    SumExpression,
    ProductExpression,
    MonomialTermExpression,
    DivisionExpression,
    NegationExpression,
    PowExpression,
)


def _generate_standard_repn(expr, multiplier, idMap, compute_values, verbose, quadratic):
    coek_repn = pk.generate_standard_repn(expr._pe)
    res = StandardRepn()
    res.linear_coefs = tuple(coek_repn.linear_coefs)
    res.linear_vars = tuple(_var_term_to_var_map[v] for v in coek_repn.linear_vars)
    if coek_repn.is_quadratic():
        res.quadratic_coefs = tuple(coek_repn.quadratic_coefs)
        res.quadratic_vars = tuple(zip(coek_repn.quadratic_lvars, coek_repn.quadratic_rvars))
    return res


_repn_collectors[SumExpression] = _generate_standard_repn
_repn_collectors[ProductExpression] = _generate_standard_repn
_repn_collectors[MonomialTermExpression] = _generate_standard_repn
_repn_collectors[DivisionExpression] = _generate_standard_repn
_repn_collectors[NegationExpression] = _generate_standard_repn
_repn_collectors[PowExpression] = _generate_standard_repn
