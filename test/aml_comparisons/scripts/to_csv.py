import json
import os
import csv
import argparse
from datetime import datetime


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dirname', type=str, required=True)
    parser.add_argument('--branch_name', type=str, required=False, default='not provided')
    parser.add_argument('--ci_commit_sha', type=str, required=False, default='not provided')
    parser.add_argument('--ci_commit_title', type=str, required=False, default='not provided')
    args = parser.parse_args()
    return args


def write_csv(source_dir, test_type, increase_build_number):
    args = parse_args()
    summary_fname = os.path.join(source_dir, 'summary.json')
    f = open(summary_fname, 'r')
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

    modified = os.path.getmtime(summary_fname)
    timestamp = datetime.fromtimestamp(modified).timestamp()

    row = {'build_number': build_number,
           'timestamp': timestamp,
           'branch_name': args.branch_name,
           'ci_commit_sha': args.ci_commit_sha,
           'ci_commit_title': args.ci_commit_title}
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

    dest_dir = os.path.join(args.dirname, str(build_number))
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    f = open(os.path.join(dest_dir, f'{test_type}_summary.csv'), 'w')
    writer = csv.DictWriter(f, fieldnames=list(row.keys()))
    writer.writeheader()
    writer.writerow(row)
    f.close()

    f = open(os.path.join(dest_dir, f'{test_type}_summary.json'), 'w')
    json.dump(res, f)
    f.close()


if __name__ == "__main__":
    directory = "results"
    
    write_csv(os.path.join(directory, 'solve0'), 'solve0', False)
    write_csv(os.path.join(directory, 'writer'), 'writer', True)
