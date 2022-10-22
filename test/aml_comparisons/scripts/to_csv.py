import json
import os
import csv
import argparse


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dirname', type=str, required=True)
    args = parser.parse_args()
    return args


def write_csv(source_dir, test_type, increase_build_number):
    args = parse_args()
    f = open(os.path.join(source_dir, 'summary.json'), 'r')
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

    bn_fname = os.path.join(args.dirname, 'build_number.txt')
    if not os.path.exists(args.dirname):
        os.makedirs(args.dirname)
        f = open(bn_fname, 'w')
        f.write('0')
        f.close()
    f = open(bn_fname, 'r')
    build_number = int(f.read())
    f.close()
    if increase_build_number:
        f = open(bn_fname, 'w')
        f.write(str(build_number + 1))
        f.close()
    dest_dir = os.path.join(args.dirname, str(build_number))
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    f = open(os.path.join(dest_dir, f'{test_type}_summary.csv'), 'w')
    writer = csv.DictWriter(f, fieldnames=list(row.keys()))
    writer.writeheader()
    writer.writerow(row)
    f.close()


if __name__ == "__main__":
    directory = "results"
    
    write_csv(os.path.join(directory, 'solve0'), 'solve0', False)
    write_csv(os.path.join(directory, 'writer'), 'writer', True)
