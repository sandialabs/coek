__all__ = ['load_data']

import os
from collections import defaultdict
try:
    import ujson as json
except:
    import json


class ModelDataFromJson(object):

    def __init__(self, jsonfile):
        self._tables_ = {}
        self._params_ = {}
        self._sets_ = {}
        with open(jsonfile, 'r') as INPUT:
            data = json.load(INPUT)
        #
        tables = data.get('table',{})
        for t, table in tables.items():
            ans = {}
            tmp = {}
            i = 0
            for name in table[0]:
                ans[name] = []
                tmp[name] = i
                i += 1
            for row in table[1:]:
                for name in tmp:
                    ans[name].append( row[tmp[name]] )
            setattr(self, t, ans)
            self._tables_[t] = ans
        #
        params = data.get('param',{})
        for p, table in params.items():
            if type(table) is list:
                ans = {}
                for row in table:
                    index = row[:-1]
                    if len(index) > 1:
                        ans[tuple(index)] = row[-1]
                    else:
                        ans[index[0]] = row[-1]
                setattr(self, p, ans)
                self._params_[p] = ans
            else:
                self._params_[p] = table
                setattr(self, p, table)
        #
        sets = data.get('set',{})
        for s, table in sets.items():
            indexed = None
            for row in table:
                if indexed is None:
                    indexed = type(row[-1]) is list
                break
            if indexed:
                ans = {}
                for row in table:
                    index = row[:-1]
                    if len(index) > 1:
                        ans[tuple(index)] = row[-1]
                    else:
                        ans[index[0]] = row[-1]
            else:
                ans = []
                for row in table:
                    ans.append( row )
            setattr(self, s, ans)
            self._sets_[s] = ans

    def unpack(self, *args, table=None, index=None, default=None):
        ans = tuple(val for val in self._unpack(*args, table=table, index=index, default=default))
        if len(ans) == 1:
            return ans[0]
        else:
            return ans

    def _unpack(self, *args, table=None, index=None, default=None):
        for key in args:
            if key in self._params_:
                if default is None:
                    yield self._params_[key]
                else:
                    yield defaultdict(lambda:default, self._params_[key])
            elif key in self._sets_:
                yield self._sets_[key]
            else:
                ans = None
                if table is None:
                    tlist = list(self._tables_.keys())
                else:
                    tlist = [table]
                for t in tlist:
                    if key in self._tables_[t]:
                        table_ = self._tables_[t]
                        if index is None:
                            ans = table_[key]
                        else:
                            ans = {}
                            for i in range(len(table_[key])):
                                ndx = tuple(list(table_[index_][i] for index_ in index))
                                if len(ndx) == 1:
                                    ans[ndx[0]] = table_[key][i]
                                else:
                                    ans[ndx] = table_[key][i]
                        break
                yield ans

def load_data(filename):
    if not os.path.exists(filename):
        raise RuntimeError("Missing file '%s'" % filename)
    return ModelDataFromJson(filename)
