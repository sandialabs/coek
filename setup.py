#  _________________________________________________________________________
#
#  POEK: A Python Optimization Expression Kernel
#  Copyright (c) 2019 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  _________________________________________________________________________

import sys
import os
from setuptools import setup, find_packages

base_dir = os.path.dirname(__file__)



#
# Manage setup() options
#
requires=['cffi>=1.0.0']
ext_modules = []
readme = os.path.join(base_dir, "README.rst")
with open(readme) as f:
    long_description = f.read()
about = {}
with open(os.path.join(base_dir, "poek", "__about__.py")) as f:
    exec(f.read(), about)
packages = find_packages(exclude=["test*", "*test*", "example*"])

#
# MANAGE CYTHON
#
##
## NOTE: For now, I'm disabling cython by default
##
##if 'develop' in sys.argv:
##    using_cython = False
##else:
##    using_cython = True
if '--with-cython' in sys.argv:
    using_cython = True
    sys.argv.remove('--with-cython')
using_cython=False

if using_cython:
    try:
        import platform
        if not platform.python_implementation() == "CPython":
            raise RuntimeError()
        from Cython.Build import cythonize
        #
        # Note: The Cython developers recommend that you destribute C source
        # files to users.  But this is fine for evaluating the utility of Cython
        #
        import shutil
        files = ["poek/expr.pyx"]
        for f in files:
            shutil.copyfile(f[:-1], f)
        ext_modules = cythonize(files)
    except:
        ext_modules = []
        using_cython = False

setup(
    name = about['__title__'],
    version = about['__version__'],
    description = about['__summary__'],
    long_description = long_description,
    author = about['__author__'],
    author_email = about['__email__'],
    url = about['__url__'],
    license = about['__license__'],

    #name="poek",
    #version='1.0',
    #maintainer='William E. Hart',
    #maintainer_email='wehart@sandia.gov',
    ##url = 'https://github.com/PyUtilib/pyutilib',
    #license = 'BSD',
    #platforms = ["any"],
    #description = 'POEK: Python Optimization Expression Kernel',
    #long_description = read('README.md'),

    classifiers = [
        #'Development Status :: 5 - Production/Stable',
        'Intended Audience :: End Users/Desktop',
        'License :: OSI Approved :: BSD License',
        'Natural Language :: English',
        'Operating System :: MacOS',
        #'Operating System :: Microsoft :: Windows',
        'Operating System :: Unix',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Scientific/Engineering :: Mathematics',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Utilities'],

      keywords=['utility'],
      zip_safe = False,
      packages = packages,
      ext_modules = ext_modules,
      install_requires=requires,
      setup_requires=requires,
      cffi_modules=[ 'poek/coek_build.py:ffi' ],
      )

