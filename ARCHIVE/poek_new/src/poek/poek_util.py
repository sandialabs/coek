
from .poek import variable_single, variable_array


class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1 or type(args[0]) == str:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)

