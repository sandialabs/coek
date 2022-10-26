import os
import csv
import argparse
import plotly.express as px
import math


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--artifact_dir', type=str, required=True)
    parser.add_argument('--branch_name', type=str, required=True)
    args = parser.parse_args()
    return args


def compare(source_dir, test_type):
    args = parse_args()
    branch_dir = os.path.join(args.artifact_dir, args.branch_name)
    baseline_dir = os.path.join(args.artifact_dir, 'dev-private')

    build_fname = os.path.join(branch_dir, 'build_number.txt')
    f = open(build_fname, 'r')
    build_number = int(f.read()) - 1
    f.close()

    build_list = list()
    ratio_list = list()
    running_fname = os.path.join(baseline_dir, f'{test_type}_running.csv')
    f = open(running_fname, 'r')
    reader = csv.DictReader(f)
    fieldnames = reader.fieldnames
    for row in reader:
        build_list.append(int(row.pop('build_number')))
        rlist = [float(i) for i in row.values()]
        r = sum(rlist) / len(rlist)
        ratio_list.append(r)
    f.close()

    build_list.append(build_list[-1] + 1)
    fname = os.path.join(branch_dir, str(build_number), f'{test_type}_summary.csv')
    f = open(fname)
    reader = csv.DictReader(f)
    assert reader.fieldnames == fieldnames
    row = next(reader)
    row.pop('build_number')
    rlist = [float(i) for i in row.values()]
    r = sum(rlist) / len(rlist)
    ratio_list.append(r)
    f.close()

    print(build_list)
    print(ratio_list)
    fig = px.scatter(
        x=build_list,
        y=ratio_list,
        size=[10]*len(build_list),
        range_y=[0, math.ceil(max(ratio_list))],
    )
    fig.update_layout(
        xaxis_title='build number',
        yaxis_title='coek time / gurobi time',
        title=test_type,
    )
    fig.write_html(f'{test_type}_trend.html')

    if args.branch_name == 'dev-private':
        fwrite = open(running_fname, 'a')
        fread = open(fname, 'r')
        lines = fread.readlines()
        assert len(lines) == 2
        l = lines[1]
        fwrite.write(l)
        fwrite.close()
        fread.close()


if __name__ == '__main__':
    compare(os.path.join('results', 'solve0'), 'solve0')
    compare(os.path.join('results', 'writer'), 'writer')
