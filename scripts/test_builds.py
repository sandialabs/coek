import os
import sys
import subprocess

config = {
    "knapsack": {
            "short": ["50"],
            "long":  ["100", "500", "1000"]
            },
    "nqueens": {
            "short": ["400"],
            "long":  ["600", "800", "1000"]
            },
    "pmedian": {
            "short": [["160","1"]],
            "long":  [["320","1"], ["640","1"], ["1280","1"]]
            },
    "lqcp": {
            "short": ["500"],
            "long":  ["1000", "1500", "2000"]
            },
    "fac": {
            "short": ["25"],
            "long":  ["50", "75", "100"]
            },
    "srosenbr": {
            "short": ["10"],
            "long":  ["100000"]
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
    "knapsack",
    "knapsack-affine",
    "nqueens",
    "nqueens-affine",
    "pmedian",
    "pmedian-affine",
    ]

all_pyomo_models = [
    "knapsack",
    "nqueens",
    "pmedian",
    "pmedian-linear",
    ]



def test_writer(*, test_type, models, suffixes, executable, subdir, timeout=300):
    errors=[]
    if not os.path.exists("results"):
        os.mkdir("results")
    if not os.path.exists("results/"+subdir):
        os.mkdir("results/"+subdir)

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type:  "+ test_type)
    print("  models:     "+ " ".join(models))
    print("  suffixes:   "+ " ".join(suffixes))
    print("  executable: "+ executable)
    print("  subdir:     "+ subdir)
    print("  timeout:    "+ str(timeout))
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
        if test_type == "all":
            tests = []
            for v in config[config_name].values():
                tests += v
        elif test_type in config[config_name]:
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
                outfile = "results/"+subdir+"/writer_"+ "_".join([suffix]+cmd) + "." + suffix
                timefile = "results/"+subdir+"/writer_"+ "_".join([suffix]+cmd) + ".out"

                if os.path.exists(timefile):
                    print(". SKIP {}: results exist".format(timefile))
                    continue

                run = ["/usr/bin/time", "-p", "-o", timefile, executable, outfile]+cmd
                results = subprocess.run(run, capture_output=True, timeout=timeout)
                if results.returncode == 0:
                    print(". OK {}: rc={} output='{}'".format(timefile, results.returncode, results.stdout))
                else:
                    print(". ERROR {}: rc={} output='{}'".format(timefile, results.returncode, results.stdout))
                    if os.path.exists(timefile):
                        os.remove(timefile)
                    errors.append(timefile)

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


def test_solve0(*, test_type, models, solvers, executable, subdir, timeout=300):
    errors=[]
    if not os.path.exists("results"):
        os.mkdir("results")
    if not os.path.exists("results/"+subdir):
        os.mkdir("results/"+subdir)

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type:  "+ test_type)
    print("  models:     "+ " ".join(models))
    print("  solvers:    "+ " ".join(solvers))
    print("  executable: "+ executable)
    print("  subdir:     "+ subdir)
    print("  timeout:    "+ str(timeout))
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
        if test_type == "all":
            tests = []
            for v in config[config_name].values():
                tests += v
        elif test_type in config[config_name]:
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
                logfile = "results/"+subdir+"/solve0_"+ "_".join([solver]+cmd) + ".log"
                timefile = "results/"+subdir+"/solve0_"+ "_".join([solver]+cmd) + ".out"

                if os.path.exists(timefile):
                    print(". SKIP {}: timefile exists".format(timefile))
                    continue

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

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


if __name__ == "__main__":
    import sys

    if sys.argv[1] == "dev":
        test_solve0(test_type="short", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")

    elif sys.argv[1] == "smoke_writer":
        test_writer(test_type="short", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek")
        test_writer(test_type="short", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi")
        test_writer(test_type="short", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek")
        test_writer(test_type="short", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo")

    elif sys.argv[1] == "bench_writer":
        test_writer(test_type="long", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek")
        test_writer(test_type="long", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi")
        test_writer(test_type="long", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek")
        test_writer(test_type="long", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo")

    elif sys.argv[1] == "smoke_solve0":
        test_solve0(test_type="short", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="short", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="short", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="short", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")

    elif sys.argv[1] == "bench_solve0":
        test_solve0(test_type="long", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="long", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="long", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="long", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")

    else:
        print("UNKNOWN TEST: "+sys.argv[1])
    

