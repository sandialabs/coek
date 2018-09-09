#!/usr/bin/env python

import cffi
import os

if __name__ == "__main__":
    ffi = cffi.FFI()

    with open(os.path.join(os.path.dirname(__file__), "expr_api.h")) as f:
        ffi.cdef(f.read())

    ffi.set_source("_expr",
        '#include "expr_api.h"',
        libraries=["exprapi"],
        library_dirs=[os.path.dirname(__file__),],
    )

    ffi.compile()
