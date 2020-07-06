
import itertools
import pycoek_cppyy

constraint = pycoek_cppyy.coek.Constraint
expression = pycoek_cppyy.coek.Expression
model = pycoek_cppyy.coek.Model
solver = pycoek_cppyy.coek.Solver
nlp_model = pycoek_cppyy.coek.NLPModel
nlp_solver = pycoek_cppyy.coek.NLPSolver

NAN = float('nan')
inf = model.inf

class parameter(object):
    """Class used to define a POEK parameter."""
    def __new__(cls, *args, **kwds):
        if len(args) == 1:
            return pycoek_cppyy.coek.Parameter(args[0], "")
        else:
            return pycoek_cppyy.coek.Parameter(*args)


class variable(object):
    """Class used to define a POEK variable."""

    def __new__(cls, *args, **kwds):
        name = kwds.get('name',"x")
        lb = kwds.get('lb', -inf)
        ub = kwds.get('ub', inf)
        init = kwds.get('value', NAN)
        binval = kwds.get('binary', 0)
        integer = kwds.get('integer', 0)
        fixed = kwds.get('fixed', 0)
        if len(args) == 0 or args[0] == 1:
            return pycoek_cppyy.coek.Variable(name, lb, ub, init, binval, integer)
        elif len(args) == 1:
            dimen = args[0]
            if type(dimen) is tuple or type(dimen) is list:
                return _variable_array(dimen, name, lb, ub, init, binval, integer, fixed)
            return pycoek_cppyy.coek.VariableArray(dimen, name, lb, ub, init, binval, integer, fixed)
        else:
            raise RuntimeError("variable() only accepts a single unnamed argument")


class _variable_array(object):
    """Wrapper for the pycoek_cppyy.VariableArray class when using multi-dimensional variable indices."""

    def __init__(self, dimen, name, lb, ub, init, binval, integer, fixed):
        nvars = 1
        for i in dimen:
            nvars *= i
        self.varray = pycoek_cppyy.coek.VariableArray(nvars, name, lb, ub, init, binval, integer, fixed)
        iters = [range(n) for n in dimen]
        self.index = {key : i for i,key in enumerate(itertools.product(*iters))}
        self.varray.order.resize(len(dimen))
        i=len(dimen)-1
        oval = 1;
        while i >= 0:
            self.varray.order[i] = oval;
            oval = oval*dimen[i]
            i = i-1
        if name == "":
            self._name = "x"
        else:
            self._name = name

    def __getitem__(self, i):
            return self.varray[ self.index[i] ]

    @property
    def name(self):
        return self._name

    def __iter__(self):
        for i in self.index:
            yield i


def model_use(self, vobj):
    if vobj.__class__ == pycoek_cppyy.coek.Variable or vobj.__class__ == pycoek_cppyy.coek.VariableArray:
        self.add_variable(vobj)
    elif vobj.__class__ == _variable_array:
        self.add_variable(vobj.varray)
    else:
        raise TypeError("Unrecognized variable object: "+str(type(vobj)))

#model.use = model_use


#
# Intrinsic functions
#
abs = pycoek_cppyy.coek.abs
ceil = pycoek_cppyy.coek.ceil
floor = pycoek_cppyy.coek.floor
exp = pycoek_cppyy.coek.exp
log = pycoek_cppyy.coek.log
log10 = pycoek_cppyy.coek.log10
sqrt = pycoek_cppyy.coek.sqrt

sin = pycoek_cppyy.coek.sin
cos = pycoek_cppyy.coek.cos
tan = pycoek_cppyy.coek.tan
asin = pycoek_cppyy.coek.asin
acos = pycoek_cppyy.coek.acos
atan = pycoek_cppyy.coek.atan
sinh = pycoek_cppyy.coek.sinh
cosh = pycoek_cppyy.coek.cosh
tanh = pycoek_cppyy.coek.tanh
asinh = pycoek_cppyy.coek.asinh
acosh = pycoek_cppyy.coek.acosh
atanh = pycoek_cppyy.coek.atanh

def affine_expression(arg1, arg2=None, arg3=None):
    if arg2 is None:
        return pycoek_cppyy.coek.affine_expression(arg1,0)
    elif arg3 is None:
        return pycoek_cppyy.coek.affine_expression(arg1,arg2)
    else:
        return pycoek_cppyy.coek.affine_expression(arg1,arg2,arg3)

from .func import quicksum, prod
