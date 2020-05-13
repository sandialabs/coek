#
# Pull-in Pybind11 wrappers
#
from pycoek import _variable
from pycoek import *
from .util import quicksum

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return _variable(**kwds)
        elif args[0].__class__ == ConcreteSet:
            return IndexedVariable(*args)
        return _variable(args[0], **kwds)


def model_variable(self, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            if 'index' in kwds:
                _index = kwds.pop('index')
                v = _variable(len(_index), **kwds)
                self.use(v)
                ans = {}
                for i, key in enumerate(_index):
                    ans[key] = v[i]
                return ans
            else:
                v =_variable(**kwds)
                self.use(v)
                return v
        elif args[0].__class__ == ConcreteSet:
            v = IndexedVariable(*args)
            self.use(v)
            return v
        else:
            v = _variable(args[0], **kwds)
            self.use(v)
            return v

setattr(model, 'variable', model_variable)
