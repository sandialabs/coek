
import itertools
import pycoek

model = pycoek.coek.Model
solver = pycoek.coek.Solver
nlp_model = pycoek.coek.NLPModel
nlp_solver = pycoek.coek.NLPSolver

NAN = float('nan')


class parameter(object):
    """Class used to define a POEK parameter."""
    def __new__(cls, *args, **kwds):
        if len(args) == 1:
            return pycoek.coek.Parameter(args[0], "")
        else:
            return pycoek.coek.Parameter(*args)


class variable(object):
    """Class used to define a POEK variable."""

    def __new__(cls, *args, **kwds):
        name = kwds.get('name',"")
        lb = kwds.get('lb', NAN)
        ub = kwds.get('ub', NAN)
        init = kwds.get('initial', NAN)
        binval = kwds.get('binary', 0)
        integer = kwds.get('integer', 0)
        fixed = kwds.get('fixed', 0)
        if len(args) == 0 or args[0] == 1:
            return pycoek.coek.Variable(lb, ub, init, binval, integer, name)
        elif len(args) == 1:
            dimen = args[0]
            if type(dimen) is tuple or type(dimen) is list:
                return _variable_array(dimen, lb, ub, init, binval, integer, fixed, name)
            return pycoek.coek.VariableArray(dimen, lb, ub, init, binval, integer, fixed, name)
        else:
            raise RuntimeError("variable() only accepts a single unnamed argument")


class _variable_array(object):
    """Wrapper for the pycoek.VariableArray class when using multi-dimensional variable indices."""

    def __init__(self, dimen, lb, ub, init, binval, integer, fixed, name):
        nvars = 1
        for i in dimen:
            nvars *= i
        self.varray = pycoek.coek.VariableArray(nvars, lb, ub, init, binval, integer, fixed, name)
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
    if vobj.__class__ == pycoek.coek.Variable or vobj.__class__ == pycoek.coek.VariableArray:
        self.addVariable(vobj)
    elif vobj.__class__ == _variable_array:
        self.addVariable(vobj.varray)
    else:
        raise TypeError("Unrecognized variable object: "+str(type(vobj)))

model.use = model_use


#
# Intrinsic functions
#
abs = pycoek.coek.abs
ceil = pycoek.coek.ceil
floor = pycoek.coek.floor
exp = pycoek.coek.exp
log = pycoek.coek.log
log10 = pycoek.coek.log10
sqrt = pycoek.coek.sqrt

sin = pycoek.coek.sin
cos = pycoek.coek.cos
tan = pycoek.coek.tan
asin = pycoek.coek.asin
acos = pycoek.coek.acos
atan = pycoek.coek.atan
sinh = pycoek.coek.sinh
cosh = pycoek.coek.cosh
tanh = pycoek.coek.tanh
asinh = pycoek.coek.asinh
acosh = pycoek.coek.acosh
atanh = pycoek.coek.atanh

def affine_expression(arg1, arg2=None, arg3=None):
    if arg2 is None:
        return pycoek.coek.affine_expression(arg1,0)
    elif arg3 is None:
        return pycoek.coek.affine_expression(arg1,arg2)
    else:
        return pycoek.coek.affine_expression(arg1,arg2,arg3)

from .util import quicksum
