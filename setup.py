#  _________________________________________________________________________
#
#  POEK: A Python Optimization Expression Kernel
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  _________________________________________________________________________

"""
Setup for poek package
"""

import sys
import os
from setuptools import setup


def _find_packages(path):
    """
    Generate a list of nested packages
    """
    pkg_list=[]
    if not os.path.exists(path):
        return []
    if not os.path.exists(path+os.sep+"__init__.py"):
        return []
    else:
        pkg_list.append(path)
    for root, dirs, files in os.walk(path, topdown=True):
        if root in pkg_list and "__init__.py" in files:
            for name in dirs:
                if os.path.exists(root+os.sep+name+os.sep+"__init__.py"):
                    pkg_list.append(root+os.sep+name)
    return [pkg for pkg in map(lambda x:x.replace(os.sep,"."), pkg_list)]

def read(*rnames):
    return open(os.path.join(os.path.dirname(__file__), *rnames)).read()

packages = _find_packages('poek')

requires=[ ]

if 'develop' in sys.argv:
    using_cython = False
else:
    using_cython = True
if '--with-cython' in sys.argv:
    using_cython = True
    sys.argv.remove('--with-cython')

ext_modules = []
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

setup(name="poek",
    version='1.0',
    maintainer='William E. Hart',
    maintainer_email='wehart@sandia.gov',
    #url = 'https://github.com/PyUtilib/pyutilib',
    license = 'BSD',
    platforms = ["any"],
    description = 'POEK: Python Optimization Expression Kernel',
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
      packages=packages,
      ext_modules = ext_modules,
      keywords=['utility'],
      install_requires=requires,
      )

