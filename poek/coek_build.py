#!/usr/bin/env python

import cffi
import os

#
# Find COEK if it's installed next to the current directory
#
coekdir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))), 'coek', '_install')
if os.path.exists( coekdir ):
    header = os.path.abspath(os.path.join( coekdir, "include", "coek_capi.h"))
    library_dirs=[ os.path.abspath(os.path.join( coekdir, "lib")) ]
else:
    header = os.path.abspath(os.path.join( os.path.dirname(__file__), "coek_capi.h"))
    library_dirs=[ ]
print(header)
print(library_dirs)


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
  libraries=['coek'],
  library_dirs=library_dirs
  )


if __name__ == "__main__":
    ffi.compile(verbose=True)
