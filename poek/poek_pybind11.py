#
# Pull-in Pybind11 wrappers
#
from pycoek_pybind11 import variable_, parameter_single, variable_array
from pycoek_pybind11 import *
from .func import quicksum, prod

try:
    compact_model
except:
    class _DummyConcreteSet(object): pass
    ConcreteSet = _DummyConcreteSet


class parameter(object):

    def __new__(cls, *args, value=None):
        p = parameter_single(*args)
        if value is not None:
            p.value = value
        return p

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_(**kwds)
        elif args[0].__class__ == ConcreteSet:
            return IndexedVariable(*args)
        return variable_(args[0], **kwds)


def model_variable(self, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            if 'index' in kwds:
                _index = list(kwds.pop('index'))
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
        elif args[0].__class__ == ConcreteSet:
            v = IndexedVariable(*args)
            self.add_variable_(v)
            return v
        elif args[0].__class__ == variable_array:
            self.add_variable_(args[0])
        else:
            v = variable_(args[0], **kwds)
            self.add_variable_(v)
            return v

setattr(model, 'add_variable', model_variable)
