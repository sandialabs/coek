#
# Pull-in Pybind11 wrappers
#
from pycoek import _variable

class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return _variable(**kwds)
        return _variable(args[0], **kwds)

from .util import quicksum

from pycoek import *
