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
requires = []
ext_modules = []
readme = os.path.join(base_dir, "README.md")
with open(readme) as f:
    long_description = f.read()
about = {}
with open(os.path.join(base_dir, "poek", "__about__.py")) as f:
    exec(f.read(), about)
packages = find_packages(exclude=["test*", "*test*", "example*"])


setup(
    name=about["__title__"],
    version=about["__version__"],
    description=about["__summary__"],
    long_description=long_description,
    author=about["__author__"],
    author_email=about["__email__"],
    url=about["__url__"],
    license=about["__license__"],
    classifiers=[
        #'Development Status :: 5 - Production/Stable',
        "Intended Audience :: End Users/Desktop",
        "License :: OSI Approved :: BSD License",
        "Natural Language :: English",
        "Operating System :: MacOS",
        #'Operating System :: Microsoft :: Windows',
        "Operating System :: Unix",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: Implementation :: CPython",
        "Topic :: Scientific/Engineering :: Mathematics",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: Utilities",
    ],
    keywords=["utility"],
    zip_safe=False,
    packages=packages,
    ext_modules=ext_modules,
    install_requires=requires,
    setup_requires=requires,
)
