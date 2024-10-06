#
# Pull-in Pybind11 wrappers
#
from io import StringIO
from pycoek_pybind11 import variable_, parameter_, variable_array
from pycoek_pybind11 import *
from .func import quicksum, prod, seq

try:
    compact_model
except:

    class _DummyConcreteSet(object):
        pass

    ConcreteSet = _DummyConcreteSet


class data(object):
    def __new__(cls, *args, **kwds):
        #p = parameter_single(*args)
        #if value is not None:
        #    p.value = value
        #return p
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return data_(**kwds)
        if len(args) == 1:
            return data_(args[0], **kwds)
        else:
            raise RuntimeError("Data values only have one argument")


class parameter(object):
    def __new__(cls, *args, **kwds):
        #p = parameter_single(*args)
        #if value is not None:
        #    p.value = value
        #return p
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return parameter_(**kwds)
        if len(args) == 1:
            return parameter_(args[0], **kwds)
        else:
            raise RuntimeError("Parameters only have one argument")


class variable(object):
    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_(**kwds)
        if len(args) == 1:
            return variable_(args[0], **kwds)
        else:
            raise RuntimeError("Variables only have one argument")


def model_variable(self, *args, **kwds):
    if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
        if "index" in kwds:
            _index = list(kwds.pop("index"))
            v = variable_(len(_index), **kwds)
            self.add_variable_(v)
            ans = {}
            for i, key in enumerate(_index):
                ans[key] = v[i]
            return ans
        else:
            v = variable_(**kwds)
            self.add_variable_(v)
            return v
    elif args[0].__class__ == variable_array:
        self.add_variable_(args[0])
    else:
        v = variable_(args[0], **kwds)
        self.add_variable_(v)
        return v


setattr(model, "add_variable", model_variable)
setattr(compact_model, "add_variable", model_variable)

def print_equations(self, ostream=None):
    if ostream is None:
        self.print_equations_()
    else:
        ostream.write( self.print_equations_(0) );
    return ostream

setattr(model, "print_equations", print_equations)

def print_values(self, ostream=None):
    if ostream is None:
        self.print_values_()
    else:
        ostream.write( self.print_values_(0) );
    return ostream

setattr(model, "print_values", print_values)

def constraint_is_numeric_type(self, *args, **kwds):
    return True


setattr(constraint, "is_numeric_type", constraint_is_numeric_type)
