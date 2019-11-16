# Main __init__.py file

#
# Need to create the following API
#
# create_model()
# create_variable()
# add_variable()
# add_constraint()
# add_objective()
# print_model()
# display_model()
# SolverFactory()
#
# Plus logic for creating simple expressions
#

__using_cppyy__ = False
__using_pybind11__ = False
__using_cffi__ = False
import pycoek

if hasattr(pycoek, 'coek'):
    #
    # Using cppyy package
    #
    __using_cppyy__ = True
    from poek.poek_cppyy import *
elif hasattr(pycoek, 'cffi'):
    #
    # Using cppyy package
    #
    __using_cffi__ = True
    from poek.poek_cffi import *
else:
    #
    # Using pybind11 package
    #
    __using_pybind11__ = True
    from poek.poek_pybind11 import *

