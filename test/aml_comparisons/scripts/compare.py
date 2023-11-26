import os
import csv
import argparse
import pandas as pd
import math
import statistics
#import plotly.express as px
import plotly.graph_objects as go
import pprint
import datetime


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--artifact_dir', type=str, required=True)
    parser.add_argument('--branch_name', type=str, required=True)
    args = parser.parse_args()
    return args


def compare(source_dir, test_type):
    args = parse_args()
    branches = os.listdir(args.artifact_dir)  # ['dev-private', 'dev-public', 'mt', 'nl2']

    data = {}
    mindate = None
    maxdate = None
    maxbuild = -1
    maxvalue = -1.0
    for datadir in branches:
        branch_dir = os.path.join(args.artifact_dir, datadir)
        if not os.path.isdir(branch_dir):
            continue

        build_fname = os.path.join(branch_dir, 'build_number.txt')
        f = open(build_fname, 'r')
        build_number = int(f.read())
        f.close()

        tmp = {}
        for num in range(build_number):
            running_fname = os.path.join(branch_dir, str(num), f'{test_type}_summary.csv')
            created = os.path.getctime(running_fname)
            with open(running_fname, 'r') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    row.pop('build_number',0)
                    tmp[created] = statistics.mean(float(i) for i in row.values())
                    maxbuild = max(num, maxbuild)
                    maxvalue = max(tmp[created], maxvalue)
                    break
            if mindate is None or created < mindate:
                mindate = created
            if maxdate is None or created > maxdate:
                maxdate = created
            
        data[datadir] = tmp

    #print(mindate)
    #print(maxdate)
    #print(maxvalue)
    
    fig = go.Figure()

    for datadir in branches:
        #pprint.pprint(data[datadir])
        tmp = {'index': [datetime.datetime.fromtimestamp(key) for key in data[datadir]], 'value': [data[datadir][key] for key in data[datadir]]}
        df = pd.DataFrame.from_dict(tmp)
        fig.add_trace( go.Scatter(x=df['index'], y=df['value'], mode='lines', name=datadir ) )
                                #,     range_x=[mindate,maxdate], range_y=[0, 1.1*math.ceil(maxvalue)]) )

    fig.update_yaxes(range=[0, 1.1*math.ceil(maxvalue)])
    fig.update_layout(
        xaxis_title='Date',
        yaxis_title='coek time / gurobi time',
        title=test_type,
    )
    fig.write_html(f'{test_type}_trend.html')

    #if args.branch_name == 'dev-private':
    #    fwrite = open(running_fname, 'a')
    #    fread = open(fname, 'r')
    #    lines = fread.readlines()
    #    assert len(lines) == 2
    #    l = lines[1]
    #    fwrite.write(l)
    #    fwrite.close()
    #    fread.close()


if __name__ == '__main__':
    compare(os.path.join('results', 'solve0'), 'solve0')
    compare(os.path.join('results', 'writer'), 'writer')
