import json
import os
import csv


def write_csv(dirname, test_type):
    f = open(os.path.join(dirname, 'summary.json'), 'r')
    res = json.load(f)
    f.close()

    gurobi_times = dict()
    for k, d in res['raw']['gurobi'].items():
        if k.startswith('_'):
            continue
        m = d['model'].split('-')[0]
        s = d['size']
        t = d['data']['real']['mean']
        assert m not in gurobi_times
        gurobi_times[m] = (s, t)

    row = dict()
    for k, d in res['raw']['coek'].items():
        if k.startswith('_'):
            continue
        m = d['model'].split('-')[0]
        s = d['size']
        t = d['data']['real']['mean']
        gs, gt = gurobi_times[m]
        assert s == gs
        ratio = t / gt
        row[k] = ratio

    f = open(os.path.join(dirname, f'{test_type}_summary.csv'), 'w')
    writer = csv.DictWriter(f, fieldnames=list(row.keys()))
    writer.writeheader()
    writer.writerow(row)
    f.close()


if __name__ == "__main__":
    directory = "results"
    
    write_csv(os.path.join(directory, 'solve0'), 'solve0')
    write_csv(os.path.join(directory, 'writer'), 'writer')
