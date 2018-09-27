#!/usr/bin/env python

import cffi
import os

if __name__ == "__main__":
    ffi = cffi.FFI()

    ffi.cdef('extern "Python" void visitor_enter_callback(void*, void*, void*);')
    ffi.cdef('extern "Python" void visitor_exit_callback(void*, void*, void*);')

    with open(os.path.join(os.path.dirname(__file__), "expr_api.h")) as f:
        ffi.cdef(f.read())
    with open(os.path.join(os.path.dirname(__file__), "expr_intrinsic.h")) as f:
        ffi.cdef(f.read())

    ffi.set_source("_expr",
        '#include "expr_api.h"\n#include "expr_intrinsic.h"',
        libraries=["exprapi"],
        library_dirs=[os.path.dirname(__file__),],
    )

    ffi.compile()
