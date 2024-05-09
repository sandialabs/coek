Installing Coek
===============

There are several options for installing Coek:

* Install the latest release or development branch using Spack
* Install from source using CMake

Developers will find the top-level ``build_dev.sh`` script useful for setting up a
build environment after cloning the Coek repository.

Installing with Spack
---------------------

[Spack]_ is a software package manager that simplifies the installation of
software with complex dependencies. With Spack, you can build a package
with multiple versions, configurations, platforms, and compilers, and
all of these builds can coexist on the same machine.

Downloading Spack
~~~~~~~~~~~~~~~~~

Spack is configured using a github repository that defines package specifications, and 
Coek can be installed using the OR-Fusion clone of Spack:

.. code-block:: bash

    git clone git@github.com:or-fusion/spack.git

Spack includes scripts that configure the user's shell environment to use Spack commands.  For example, you can configure a BASH environment after cloning Spack:

.. code-block:: bash

    . spack/share/spack/setup-env.sh

Creating a Spack Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A Spack environment is used to group together a set of package
installations for the purpose of building, rebuilding and deploying in
a coherent fashion. Environments are not strictly necessary when using
Spack, but they allow different package specifications (a.k.a. specs)
to be installed simultaneously.

You create and activate the *coekenv* environment as follows:

.. code-block:: bash

    spack env create coekenv
    spacktivate coekenv

Spack configures the installation of this environment using a ``spack.yaml`` file, which can
be further customized by the user either directly or via Spack commands.

.. code-block:: bash

    spack/var/spack/environments/coekenv/spack.yaml

When done using a Spack environment, you can deactivate the environment with:

.. code-block:: bash

    spack env deactivate


Installing Coek
~~~~~~~~~~~~~~~

A Simple Installation
*********************

You can install the latest version of Coek in the main development branch as follows:

.. code-block:: bash

    spack install coek@dev
   
Similarly, you can see the packages installed with Spack with the *find* command: 

.. code-block:: bash

    spack find coek@dev

.. note::

    The this installation uses the Coek main development branch. After an upcoming Coek release,
    the Spack will be configured to install the latest Coek release with:

    .. code-block:: bash

        spack install coek

Coek Configuration Options
**************************

Coek includes a variety of configuration options that can be used to customize a Coek installation.  The following are options that end-users are likely to consider:

.. list-table::
    :header-rows: 1

    * - Option 
      - Description
    * - python
      - Build pycoek and install coek python libraries
    * - gurobi
      - Build with Gurobi optimization library
    * - cppad
      - Build with the CppAD library
    * - asl
      - Build with the ASL library

.. warning::

    Installing with the ``python`` or ``gurobi`` options will trigger a Spack build of python or
    Gurobi, since those are dependencies that Spack can build.  The first time Spack builds these
    dependencies, the build may take quite a while.

    You can treat python, Gurobi and other tools as external dependencies using the Spack ``external``
    subcommand:

    .. code-block:: bash

        spack external find python
        spack external find gurobi

    The configuration for these packages is cached here:

    .. code-block:: bash

        $HOME/.spack/packages.yaml

The following install command will build Coek with support for Poek as well as Gurobi and Ipopt, using either CppAD or ASL for automatic differentiation:

.. code-block:: bash

    spack install coek@dev +python+gurobi+asl+cppad

.. note::

    The this installation uses the Coek main development branch. After an upcoming Coek release,
    the Spack will be configured to install the latest Coek release with:

    .. code-block:: bash

        spack install coek

Installing with CMake
---------------------

Without Dependencies
~~~~~~~~~~~~~~~~~~~~

The default configuration of Coek requires no external dependencies.  Thus, you can build Coek
with the standard CMake build steps:

.. code-block:: bash

    git clone git@github.com:sandialabs/coek.git
    mkdir build
    cd build
    cmake ..
    make

.. note::

    The Coek build can be significantly accelerated using the ``make -j`` option. For example, the
    following command uses up to 20 parallel build threads:

    .. code-block:: bash

        make -j 20

With Dependencies
~~~~~~~~~~~~~~~~~

Coek's CMake configuration uses standard ``find_package()`` functions to
find pre-installed software dependencies.  These conditional dependencies
are configured with the following CMake options:

.. list-table::
    :header-rows: 1

    * - Option 
      - Description
    * - with_python
      - Build pycoek and install coek python libraries
    * - with_gurobi
      - Build with Gurobi optimization library
    * - with_cppad
      - Build with the CppAD library
    * - with_asl
      - Build with the ASL library
    * - with_fmtlib
      - Build with the fmt library
    * - with_rapidjson
      - Build with the rapidjson library

The default value for these options is OFF.

Coek's CMake configuration includes a deprecated ``install_tpls`` target to build commonly used dependencies.  However, it is easy to (1) install dependent libraries using Spack, and (2) configure Coek to automatically find these libraries using the CMAKE_PREFIX_PATH option. For example, here is a complete installation script for Coek using Spack:

.. code-block:: bash

    git clone git@github.com:or-fusion/spack.git
    cd spack
    . spack/share/spack/setup-env.sh
    spack env create coekenv
    spack env activate coekenv
    spack add cppad fmt rapidjson catch2
    spack install
    spack env deactivate
    cd ..

    git clone git@github.com:sandialabs/coek.git
    cd coek
    mkdir build
    cd build
    cmake -DCMAKE_PREFIX_PATH=`pwd`/../spack/var/spack/environments/coekenv/.spack-env/view -Dwith_python=ON -Dwith_gurobi=ON -Dwith_cppad=ON -Dwith_fmtlib=ON -Dwith_rapidjson=ON -Dwith_tests=ON ..
    make -j20
    
Coek includes a ``build_dev.sh`` script that executes a variant of this script.

.. note::

    Support for ASL in developer mode is a bit more complicated to setup because
    Spack creates libraries for ASL that may conflict with system libraries.

