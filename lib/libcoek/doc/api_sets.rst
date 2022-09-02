.. _api-sets:

Set Declarations and Operations
===============================

TODO

.. note::

    We will want to support indexed sets, such as:

    .. code::

        auto A = coek::RangeSet(1,10)
        auto B = coek::Set(A)

    This will be necessary to specify nested compact expressions:

    .. code::

        auto x = coek::variable( coek::Forall(i).In(A).
                                       Forall(j).In(B[i]) );

    We cannot just use a STL vector or something similar here, as we need the code ``B[i]`` to return a reference
    to a set object tha will be resolved later.

