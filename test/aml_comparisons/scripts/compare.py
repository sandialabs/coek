import os
import csv
import argparse
import pandas as pd
import math
import statistics

# import plotly.express as px
import plotly.graph_objects as go
import pprint
from datetime import datetime


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--artifact_dir", type=str, required=True)
    parser.add_argument("--branch_name", type=str, required=True)
    args = parser.parse_args()
    return args


def compare(source_dir, test_type):
    args = parse_args()
    branches = [
        i
        for i in os.listdir(args.artifact_dir)
        if os.path.isdir(os.path.join(args.artifact_dir, i))
    ]  # ['dev-private', 'dev-public', 'mt', 'nl2']

    fig_list = list()
    for datadir in branches:
        branch_dir = os.path.join(args.artifact_dir, datadir)

        build_fname = os.path.join(branch_dir, "build_number.txt")
        f = open(build_fname, "r")
        build_number = int(f.read())
        f.close()

        xlist = list()
        ylist = list()
        text_list = list()
        for num in range(build_number):
            running_fname = os.path.join(branch_dir, str(num), f"{test_type}_summary.csv")
            if not os.path.exists(running_fname):
                continue
            created = os.path.getctime(running_fname)
            with open(running_fname, "r") as f:
                print(running_fname)
                print(f.read())
                f.seek(0)
                reader = csv.DictReader(f)
                for row in reader:
                    row.pop("build_number", 0)
                    created = float(row.pop("timestamp", created))
                    branch_name = row.pop("branch_name", datadir)
                    if branch_name == "not provided":
                        branch_name = datadir
                    ci_commit_sha = row.pop("ci_commit_sha", "not provided")
                    ci_commit_title = row.pop("ci_commit_title", "not provided")
                    created_dt = datetime.fromtimestamp(created)
                    xlist.append(created_dt)
                    y = statistics.mean(float(i) for i in row.values())
                    ylist.append(y)
                    text_list.append(
                        f"test type: {test_type}<br>branch: {branch_name}<br>commit: {ci_commit_title}<br>commit SHA: {ci_commit_sha}<br>date: {created_dt}"
                    )
                    break

        fig_list.append(
            go.Scatter(
                x=xlist,
                y=ylist,
                mode="lines+markers",
                marker=dict(size=10),
                name=f"{datadir}_{test_type}",
                hovertext=text_list,
            )
        )

    return fig_list

    fig.update_yaxes(range=[0, 1.1 * math.ceil(maxvalue)])
    fig.update_layout(
        xaxis_title="Date",
        yaxis_title="coek time / gurobi time",
        title=test_type,
    )
    fig.write_html(f"{test_type}_trend.html")

    # if args.branch_name == 'dev-private':
    #    fwrite = open(running_fname, 'a')
    #    fread = open(fname, 'r')
    #    lines = fread.readlines()
    #    assert len(lines) == 2
    #    l = lines[1]
    #    fwrite.write(l)
    #    fwrite.close()
    #    fread.close()


def main():
    fig_list = compare(os.path.join("results", "solve0"), "solve0")
    fig_list.extend(compare(os.path.join("results", "writer"), "writer"))
    fig = go.Figure(data=fig_list)
    fig.update_layout(
        xaxis_title="Date",
        yaxis_title="coek time / gurobi time",
    )
    fig.write_html("perf_trend.html")


if __name__ == "__main__":
    main()
