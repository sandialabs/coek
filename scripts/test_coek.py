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
    "lqcp_compact"
    ]

all_suffixes = ['lp', 'nl']



def test_writer(*, test_type, models, suffixes, executable):
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
                logfile = "results/"+ "_".join(cmd+[suffix]) + ".out"

                if os.path.exists(logfile):
                    print(". Skipping test {}-{}: logfile exists".format(model, suffix))
                    continue

                run = ["/usr/bin/time", "-p", "-o", logfile, executable, outfile]+cmd
                results = subprocess.run(run, capture_output=True)
                print(". Test complete {}: rc={} output='{}'".format(logfile, results.returncode, results.stdout))


if __name__ == "__main__":
    #test_type = "all"
    test_type = "short"
    #models = ["knapsack_scalar"]
    models = all_coek_models
    suffixes = all_suffixes
    test_writer(test_type=test_type, models=models, suffixes=suffixes, executable="coek/coek_writer")

