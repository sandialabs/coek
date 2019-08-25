#!/bin/sh

tests/resolve 2000 10 baseline | ts -s "%.s" > resolve_baseline.out 2>&1
tests/resolve 2000 10 obj | ts -s "%.s" > resolve_obj.out 2>&1
tests/resolve 2000 10 row | ts -s "%.s" > resolve_row.out 2>&1
tests/resolve 2000 10 col | ts -s "%.s" > resolve_col.out 2>&1
tests/resolve 2000 10 rhs | ts -s "%.s" > resolve_rhs.out 2>&1
