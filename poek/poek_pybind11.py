#
# Pull-in Pybind11 wrappers
#
from pycoek import _variable

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return _variable(**kwds)
        elif args[0].__class__ == ConcreteSet:
            return IndexedVariable(*args)
        return _variable(args[0], **kwds)

from .util import quicksum

from pycoek import *
