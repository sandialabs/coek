#!/usr/bin/env python

import cffi
import os

#
# Find COEK if it's installed next to the current directory
#
coekdir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))), 'src')
header = os.path.abspath(os.path.join( coekdir, "coek_capi.h"))
library_dirs=[ os.path.abspath(os.path.join( coekdir, "lib")) ]


ffi = cffi.FFI()

ffi.cdef('extern "Python" void visitor_enter_callback(void*, void*, void*);')
ffi.cdef('extern "Python" void visitor_exit_callback(void*, void*, void*);')

with open(header) as f:
  lines = []
  for line in f.readlines():
    if len(line) > 0 and line[0] != '#':
      lines.append(line)
  ffi.cdef("".join(lines))

ffi.set_source("poek.coek_cffi",
  '#include "%s"' % header,
  libraries=['coek', 'gurobi81', 'gurobi_g++5.2'],
  library_dirs=library_dirs,
  #extra_compile_args=['-g'],
  )


if __name__ == "__main__":
    ffi.compile(verbose=True)
