import os
import sys
import subprocess

config = {
    "knapsack": {
            "small": ["50"],
            "big":   ["1000"],
            "all":   ["50", "100", "500", "1000"]
            },
    "nqueens": {
            "small": ["400"],
            "big":   ["1000"],
            "all":   ["400", "600", "800", "1000"]
            },
    "pmedian": {
            "small": [["160","1"]],
            "big":   [["1280","1"]],
            "all":   [["160","1"], ["320","1"], ["640","1"], ["1280","1"]]
            },
    "lqcp": {
            "small": ["500"],
            "big":   ["2000"],
            "all":   ["500", "1000", "1500", "2000"]
            },
    "fac": {
            "small": ["25"],
            "big":   ["100"],
            "all":   ["25", "50", "75", "100"]
            },
    "srosenbr": {
            "small": ["10"],
            "big":   ["100000"]
            }
    }

all_coek_models = [
    "knapsack-array",
    "knapsack-scalar",
    "nqueens-array",
    "nqueens-scalar",
    "pmedian-array",
    "pmedian-scalar",
    "fac-array",
    "fac-array2",
    "fac-scalar",
    "lqcp-scalar",
    "lqcp-array",
    "lqcp-array2",
    "lqcp-map",
    #"lqcp-compact"         Problem constructing this model
    ]

all_gurobi_models = [
    "knapsack-scalar",
    "nqueens-scalar",
    "pmedian-scalar",
    "lqcp",
    "fac",
    ]

all_poek_models = [
    "fac",
    "knapsack",
    "knapsack-affine",
    "lqcp",
    "nqueens",
    "nqueens-affine",
    "pmedian",
    "pmedian-affine",
    ]

all_pyomo_models = [
    "fac",
    "knapsack",
    "lqcp",
    "nqueens",
    "pmedian",
    "pmedian-linear",
    ]



def test_writer(*, test_type, models, suffixes, executable, subdir, timeout=300, ntrials=1, errors=[]):
    if not os.path.exists("results"):
        os.mkdir("results")
    if not os.path.exists("results/writer"):
        os.mkdir("results/writer")
    if not os.path.exists("results/writer/"+subdir):
        os.mkdir("results/writer/"+subdir)

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type:  "+ test_type)
    print("  models:     "+ " ".join(models))
    print("  suffixes:   "+ " ".join(suffixes))
    print("  executable: "+ executable)
    print("  subdir:     "+ subdir)
    print("  timeout:    "+ str(timeout))
    print("  ntrials:    "+ str(ntrials))
    print("")

    for model in models:
        config_name = None
        for name in config:
            if model.startswith(name):
                config_name = name
        if config_name is None:
            print("Ignoring unknown model {}".format(model))
            continue

        print("\nTesting model {}".format(model))
        if test_type in config[config_name]:
            tests = config[config_name][test_type]
        else:
            print("Unknown test type {}".format(test_type))
            continue

        for test_args in tests:
            if type(test_args) is list:
                cmd = [model] + test_args
            else:
                cmd = [model] + [test_args]
            for suffix in suffixes:
                for trial in range(ntrials):
                    prefix = "results/writer/"+subdir+"/"+ "_".join([suffix]+cmd)+"."+str(trial)
                    outfile = prefix + "." + suffix
                    timefile = prefix + ".out"

                    if os.path.exists(timefile):
                        print(". SKIP {}: results exist".format(timefile))
                        continue

                    run = ["/usr/bin/time", "-p", "-o", timefile, executable, outfile]+cmd
                    try:
                        results = subprocess.run(run, capture_output=True, timeout=timeout)
                        if results.returncode == 0:
                            print(". OK {}: rc={} output='{}'".format(timefile, results.returncode, results.stdout))
                        else:
                            print(". ERROR {}: rc={} output='{}'".format(timefile, results.returncode, results.stdout))
                            if os.path.exists(timefile):
                                os.remove(timefile)
                            errors.append(timefile)
                    except subprocess.TimeoutExpired:
                        print(". TIMEOUT {}".format(timefile))
                        with open(timefile,'w') as OUTPUT:
                            OUTPUT.write("timeout {}".format(timeout))


def test_solve0(*, test_type, models, solvers, executable, subdir, timeout=300, ntrials=1, errors=[]):
    if not os.path.exists("results"):
        os.mkdir("results")
    if not os.path.exists("results/solve0"):
        os.mkdir("results/solve0")
    if not os.path.exists("results/solve0/"+subdir):
        os.mkdir("results/solve0/"+subdir)

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type:  "+ test_type)
    print("  models:     "+ " ".join(models))
    print("  solvers:    "+ " ".join(solvers))
    print("  executable: "+ executable)
    print("  subdir:     "+ subdir)
    print("  timeout:    "+ str(timeout))
    print("  ntrials:    "+ str(ntrials))
    print("")

    for model in models:
        config_name = None
        for name in config:
            if model.startswith(name):
                config_name = name
        if config_name is None:
            print("Ignoring unknown model {}".format(model))
            continue

        print("\nTesting model {}".format(model))
        if test_type in config[config_name]:
            tests = config[config_name][test_type]
        else:
            print("Unknown test type {}".format(test_type))
            continue

        for test_args in tests:
            if type(test_args) is list:
                cmd = [model] + test_args
            else:
                cmd = [model] + [test_args]
            for solver in solvers:
                for trial in range(ntrials):
                    prefix = "results/solve0/"+subdir+"/"+ "_".join([solver]+cmd)+"."+str(trial)
                    logfile = prefix + ".log"
                    timefile = prefix + ".out"

                    if os.path.exists(timefile):
                        print(". SKIP {}: timefile exists".format(timefile))
                        continue

                    try:
                        run = ["/usr/bin/time", "-p", "-o", timefile, executable, solver]+cmd
                        results = subprocess.run(run, capture_output=True, timeout=timeout)
                        with open(logfile,'wb') as OUTPUT:
                            OUTPUT.write(results.stdout)
                        if results.returncode == 0:
                            print(". OK {}: rc={}".format(timefile, results.returncode))
                        else:
                            print(". ERROR {}: rc={}".format(timefile, results.returncode))
                            if os.path.exists(timefile):
                                os.remove(timefile)
                            errors.append(timefile)
                    except subprocess.TimeoutExpired:
                        print(". TIMEOUT {}".format(timefile))
                        with open(timefile,'w') as OUTPUT:
                            OUTPUT.write("timeout {}".format(timeout))

if __name__ == "__main__":
    import sys

    if len(sys.argv) == 3:
        ntrials = int(sys.argv[2])
    else:
        ntrials = 1

    errors = []

    if sys.argv[1] == "dev":
        test_solve0(test_type="small", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo", ntrials=ntrials, errors=errors)

    elif sys.argv[1] == "big_writer":
        test_writer(test_type="big", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek", ntrials=ntrials, errors=errors)
        test_writer(test_type="big", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi", ntrials=ntrials, errors=errors)
        test_writer(test_type="big", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek", ntrials=ntrials, errors=errors)
        test_writer(test_type="big", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo", ntrials=ntrials, errors=errors)

    elif sys.argv[1] == "smoke_writer":
        test_writer(test_type="small", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek", ntrials=ntrials, errors=errors)
        test_writer(test_type="small", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi", ntrials=ntrials, errors=errors)
        test_writer(test_type="small", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek", ntrials=ntrials, errors=errors)
        test_writer(test_type="small", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo", ntrials=ntrials, errors=errors)

    elif sys.argv[1] == "bench_writer":
        test_writer(test_type="all", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek", ntrials=ntrials, errors=errors)
        test_writer(test_type="all", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi", ntrials=ntrials, errors=errors)
        test_writer(test_type="all", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek", ntrials=ntrials, errors=errors)
        test_writer(test_type="all", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo", ntrials=ntrials, errors=errors)

    elif sys.argv[1] == "smoke_solve0":
        test_solve0(test_type="small", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek", ntrials=ntrials, errors=errors)
        test_solve0(test_type="small", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi", ntrials=ntrials, errors=errors)
        test_solve0(test_type="small", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek", ntrials=ntrials, errors=errors)
        test_solve0(test_type="small", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo", ntrials=ntrials, errors=errors)

    elif sys.argv[1] == "bench_solve0":
        test_solve0(test_type="all", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek", ntrials=ntrials, errors=errors)
        test_solve0(test_type="all", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi", ntrials=ntrials, errors=errors)
        test_solve0(test_type="all", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek", ntrials=ntrials, errors=errors)
        test_solve0(test_type="all", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo", ntrials=ntrials, errors=errors)

    else:
        print("UNKNOWN TEST: "+sys.argv[1])

    print("")
    print("")
    print("COLLECTED ERRORS")
    for err in errors:
        print(" ",err) 
    print("")

