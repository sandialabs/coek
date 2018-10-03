from poek._expr import ffi, lib


NAN = float('nan')
NULL = lib.misc_getnull()
BUFFER = ffi.new("char []", 64)
