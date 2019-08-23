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

setup(
    name = about['__title__'],
    version = about['__version__'],
    description = about['__summary__'],
    long_description = long_description,
    author = about['__author__'],
    author_email = about['__email__'],
    url = about['__url__'],
    license = about['__license__'],

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
      cffi_modules=[ 'pycoek/build.py:ffi' ],
      )

