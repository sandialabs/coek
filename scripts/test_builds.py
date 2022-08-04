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
    "knapsack_scalar",
    "nqueens_scalar",
    "pmedian_scalar",
    "lqcp_scalar",
    "lqcp_array",
    "lqcp_map",
    #"lqcp_compact"         Problem constructing this model
    ]

all_suffixes = ['lp', 'nl']



def test_writer(*, test_type, models, suffixes, executable):
    errors=[]
    if not os.path.exists("results"):
        os.mkdir("results")

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type: "+ test_type)
    print("  models:    "+ " ".join(models))
    print("  suffixes:  "+ " ".join(suffixes))
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
                outfile = "results/"+ "_".join(cmd+[suffix]) + "." + suffix
                timefile = "results/"+ "_".join(cmd+[suffix]) + ".out"

                if os.path.exists(timefile):
                    print(". Skipping test {}: results exist".format(timefile))
                    continue

                run = ["/usr/bin/time", "-p", "-o", timefile, executable, outfile]+cmd
                results = subprocess.run(run, capture_output=True)
                print(". Test complete {}: rc={} output='{}'".format(timefile, results.returncode, results.stdout))
                if results.returncode:
                    if os.path.exists(timefile):
                        os.remove(timefile)
                    errors.append(timefile)

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


def test_solve0(*, test_type, models, solvers, executable):
    errors=[]
    if not os.path.exists("results"):
        os.mkdir("results")

    print("")
    print("TESTING ARGUMENTS")
    print("  test_type: "+ test_type)
    print("  models:    "+ " ".join(models))
    print("  suffixes:  "+ " ".join(suffixes))
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
                logfile = "results/"+ "_".join(cmd+[solver]) + ".log"
                timefile = "results/"+ "_".join(cmd+[solver]) + ".out"

                if os.path.exists(timefile):
                    print(". Skipping test {}: timefile exists".format(timefile))
                    continue

                run = ["/usr/bin/time", "-p", "-o", timefile, executable, solver]+cmd
                results = subprocess.run(run, capture_output=True)
                with open(logfile,'wb') as OUTPUT:
                    OUTPUT.write(results.stdout)
                print(". Test complete {}: rc={}".format(timefile, results.returncode))
                if results.returncode:
                    if os.path.exists(timefile):
                        os.remove(timefile)
                    errors.append(timefile)

    if len(errors) > 0:
        print("\n\nERRORS")
        for f in errors:
            print("  ",f)


if __name__ == "__main__":
    #test_type = "all"
    test_type = "short"
    #models = ["knapsack_scalar"]
    models = all_coek_models
    suffixes = all_suffixes
    solvers = ['ipopt']
    #test_writer(test_type=test_type, models=models, suffixes=suffixes, executable="coek/coek_writer")
    test_solve0(test_type=test_type, models=models, solvers=solvers, executable="coek/coek_solve0")

