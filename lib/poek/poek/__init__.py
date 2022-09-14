# poek.__init__.py

import os
import sys
import os.path
from . import config

#
# This defines the order of resolution of poek interfaces
# to pycoek.
#
# NOTE: CFFI is currently not supported
#
imports = ["pycoek_pybind11", "pycoek_cppyy"]

__using_cppyy__ = False
__using_pybind11__ = False
__using_cffi__ = False

for import_ in imports:
    if import_ == "pycoek_cppyy":
        try:
            import pycoek_cppyy

            __using_cppyy__ = True
        except ImportError:
            if config.coek_home is not None:
                sys.path.insert(0, os.path.join(config.coek_home, "lib"))
                sys.path.insert(0, os.path.join(config.coek_home, "lib64"))
            try:
                import pycoek_cppyy

                __using_cppyy__ = True
            except ImportError:
                pass
        if __using_cppyy__:
            break

    elif import_ == "pycoek_pybind11":
        try:
            import pycoek_pybind11

            __using_pybind11__ = True
        except ImportError:
            if config.coek_home is not None:
                sys.path.insert(0, os.path.join(config.coek_home, "lib"))
                sys.path.insert(0, os.path.join(config.coek_home, "lib64"))
            try:
                import pycoek_pybind11

                __using_pybind11__ = True
            except ImportError:
                pass
            pass
        if __using_pybind11__:
            break

from . import __about__

#
# Import pycoek symbols
#
if __using_cppyy__:
    __doc__ = "cppyy"
    print("<Poek %s using pycoek built with cppyy>" % __about__.__version__)
    from poek.poek_cppyy import *

elif __using_pybind11__:
    __doc__ = "pybind11"
    print("<Poek %s using pycoek built with pybind11>" % __about__.__version__)
    from poek.poek_pybind11 import *

else:
    raise ImportError("No pycoek interface installed!")

from . import util
