from poek._expr import ffi, lib

__all__ = ['NAN', 'NULL', 'BUFFER']


NAN = float('nan')
NULL = lib.misc_getnull()
BUFFER = ffi.new("char []", 64)
