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
            "big":   ["50"],
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

all_hybrid_components_only_models = [
    "fac",
    "lqcp",
    "pmedian",
    "pmedian-linear",
    ]

all_hybrid_expression_wrappers_models = [
    "fac",
    "fac-linear",
    "lqcp",
    "pmedian",
    "pmedian-linear",
    ]



def test_writer(*, test_type, models, suffixes, executable, subdir, timeout=300):
    errors=[]
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
                outfile = "results/writer/"+subdir+"/"+ "_".join([suffix]+cmd) + "." + suffix
                timefile = "results/writer/"+subdir+"/"+ "_".join([suffix]+cmd) + ".out"

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

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


def test_solve0(*, test_type, models, solvers, executable, subdir, timeout=300):
    errors=[]
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
                logfile = "results/solve0/"+subdir+"/"+ "_".join([solver]+cmd) + ".log"
                timefile = "results/solve0/"+subdir+"/"+ "_".join([solver]+cmd) + ".out"

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

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


if __name__ == "__main__":
    import sys

    if sys.argv[1] == "dev":
        test_solve0(test_type="small", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")

    elif sys.argv[1] == "big_writer":
        test_writer(test_type="big", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek")
        test_writer(test_type="big", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi")
        test_writer(test_type="big", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek")
        test_writer(test_type="big", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo")

    elif sys.argv[1] == "smoke_writer":
        test_writer(test_type="small", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek")
        test_writer(test_type="small", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi")
        test_writer(test_type="small", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek")
        test_writer(test_type="small", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo")

    elif sys.argv[1] == "bench_writer":
        test_writer(test_type="all", models=all_coek_models, suffixes=['lp'], executable="coek/coek_writer", subdir="coek")
        test_writer(test_type="all", models=all_gurobi_models, suffixes=['lp'], executable="gurobi/gurobi_writer", subdir="gurobi")
        test_writer(test_type="all", models=all_poek_models, suffixes=['lp'], executable="../poek/poek_writer", subdir="poek")
        test_writer(test_type="all", models=all_pyomo_models, suffixes=['lp'], executable="../pyomo/pyomo_writer", subdir="pyomo")

    elif sys.argv[1] == "smoke_solve0":
        test_solve0(test_type="small", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="small", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="small", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="small", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="small", models=all_hybrid_components_only_models, solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")

    elif sys.argv[1] == "big_solve0_fac":
        test_solve0(test_type="big", models=['fac-scalar'], solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="big", models=['fac'], solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="big", models=['fac'], solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="big", models=['fac'], solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="big", models=['fac'], solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")

    elif sys.argv[1] == "small_solve0_lqcp":
        test_solve0(test_type="small", models=['lqcp-scalar'], solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="small", models=['lqcp'], solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="small", models=['lqcp'], solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="small", models=['lqcp'], solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="small", models=['lqcp'], solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")

    elif sys.argv[1] == "all_solve0_lqcp":
        test_solve0(test_type="all", models=['lqcp-scalar'], solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="all", models=['lqcp'], solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="all", models=['lqcp'], solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="all", models=['lqcp'], solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="all", models=['lqcp'], solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")
        test_solve0(test_type="all", models=['lqcp'], solvers=['gurobi'], executable="../hybrid_expression_wrappers/hybrid_expression_wrappers_solve0", subdir="hybrid_expression_wrappers")

    elif sys.argv[1] == "all_solve0_fac":
        test_solve0(test_type="all", models=['fac-scalar'], solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="all", models=['fac'], solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="all", models=['fac'], solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="all", models=['fac'], solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="all", models=['fac'], solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")
        test_solve0(test_type="all", models=['fac', 'fac-linear'], solvers=['gurobi'], executable="../hybrid_expression_wrappers/hybrid_expression_wrappers_solve0", subdir="hybrid_expression_wrappers")

    elif sys.argv[1] == "all_solve0_pmedian":
        test_solve0(test_type="all", models=['pmedian-scalar'], solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="all", models=['pmedian-scalar'], solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="all", models=['pmedian', 'pmedian-affine'], solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="all", models=['pmedian', 'pmedian-linear'], solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")
        test_solve0(test_type="all", models=['pmedian', 'pmedian-linear'], solvers=['appsi-gurobi'], executable="../pyomo/pyomo_solve0", subdir="appsi")
        test_solve0(test_type="all", models=['pmedian', 'pmedian-linear'], solvers=['gurobi'], executable="../hybrid_components_only/hybrid_components_only_solve0", subdir="hybrid_components_only")
        test_solve0(test_type="all", models=['pmedian', 'pmedian-linear'], solvers=['gurobi'], executable="../hybrid_expression_wrappers/hybrid_expression_wrappers_solve0", subdir="hybrid_expression_wrappers")

    elif sys.argv[1] == "bench_solve0":
        test_solve0(test_type="all", models=all_coek_models, solvers=['gurobi'], executable="coek/coek_solve0", subdir="coek")
        test_solve0(test_type="all", models=all_gurobi_models, solvers=['gurobi'], executable="gurobi/gurobi_solve0", subdir="gurobi")
        test_solve0(test_type="all", models=all_poek_models, solvers=['gurobi'], executable="../poek/poek_solve0", subdir="poek")
        test_solve0(test_type="all", models=all_pyomo_models, solvers=['gurobi'], executable="../pyomo/pyomo_solve0", subdir="pyomo")

    else:
        print("UNKNOWN TEST: "+sys.argv[1])
    

