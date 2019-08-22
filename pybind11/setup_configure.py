"""
    Implements a new custom Distutils command for handling library
    configuration.

    The "configure" command here doesn't directly affect things like
    config.pxi; rather, it exists to provide a set of attributes that
    are used by to configure cmake.  Options from the command line and
    environment variables are stored between invocations in a pickle file.
"""

from distutils.cmd import Command
import os
#import os.path as op
#import sys
import pickle

def loadpickle():
    """ Load settings dict from the pickle file
    """
    try:
        with open('coekconfig.pkl', 'rb') as f:
            cfg = pickle.load(f)
        if not isinstance(cfg, dict):
            raise TypeError
    except Exception:
        return {}
    return cfg


def savepickle(dct):
    """ Save settings dict to the pickle file
    """
    with open('coekconfig.pkl', 'wb') as f:
        pickle.dump(dct, f, protocol=0)


def validate_version(s):
    """ Ensure that s contains an X.Y.Z format version string, or ValueError.
    """
    try:
        tpl = tuple(int(x) for x in s.split('.'))
        if len(tpl) != 3: raise ValueError
    except Exception:
        raise ValueError("HDF5 version string must be in X.Y.Z format")


class EnvironmentOptions(object):
    """ Convenience class representing the current environment variables.
    """

    def __init__(self):
        if "IPOPT_HOME" in os.environ:
            self.ipopt_home = os.environ["IPOPT_HOME"]
        if "GUROBI_HOME" in os.environ:
            self.gurobi_home = os.environ["GUROBI_HOME"]


class configure(Command):
    """ Configure build options for pycoek.  Options come from the
        following sources, in order of priority:

        1. Current command-line options
        2. Old command-line options
        3. Environment variables
    """

    description = "Configure pycoek build options"

    user_options = [('ipopt', None, 'Build with IPOPT'),
                    ('ipopt-home=', None, 'Override IPOPT home'),
                    ('gurobi', None, 'Build with GUROBI'),
                    ('gurobi-home=', None, 'Override GUROBI home'),
                    ('cppad', None, 'Build with CPPAD'),
                    ('debug', 'd', 'Build with debugging'),
                    ('gprof', 'p', 'Build with gprof flags'),
                    ('reset', 'r', 'Reset options')]

    def initialize_options(self):
        self.ipopt = None
        self.ipopt_home = None
        self.gurobi = None
        self.gurobi_home = None
        self.cppad = None
        self.debug = False
        self.gprof = False
        self.reset = False

    def finalize_options(self):
        pass

    def run(self):
        """ Distutils calls this when the command is run
        """

        # Step 1: determine if settings have changed and update cache

        if self.reset:
            dct = {}
        else:
            oldsettings = loadpickle()
            dct = oldsettings.copy()

        # Only update settings which have actually been specified this
        # round; ignore the others (which have value None).
        if self.ipopt is not None:
            dct['ipopt'] = self.ipopt
        if self.ipopt_home is not None:
            dct['ipopt-home'] = self.ipopt_home
        if self.gurobi is not None:
            dct['gurobi'] = self.gurobi
        if self.gurobi_home is not None:
            dct['gurobi-home'] = self.gurobi_home
        if self.cppad is not None:
            dct['cppad'] = self.cppad
        if self.debug is not None:
            dct['debug'] = self.debug
        if self.gprof is not None:
            dct['gprof'] = self.gprof

        # Step 2: update public config attributes according to priority rules

        env = EnvironmentOptions()

        if self.ipopt_home is None:
            dct['ipopt-home'] = env.ipopt_home
        if self.gurobi_home is None:
            dct['gurobi-home'] = env.gurobi_home

        savepickle(dct)

        # Step 3: print the resulting configuration to stdout

        print('*' * 80)
        print(' ' * 23 + "Summary of the pycoek configuration")
        print('')
        print("    IPOPT Enabled: " + repr(bool(dct.get('ipopt',None))))
        print("    Path to IPOPT: " + repr(dct.get('ipopt-home',None)))
        print("   GUROBI Enabled: " + repr(bool(dct.get('gurobi',None))))
        print("   Path to GUROBI: " + repr(dct.get('gurobi-home',None)))
        print("    CPPAD Enabled: " + repr(bool(dct.get('cppad',None))))
        print("Debugging Enabled: " + repr(bool(dct.get('debug',None))))
        print("    Gprof Enabled: " + repr(bool(dct.get('gprof',None))))
        print('')
        print('*' * 80)
