import json

class StructuredDict_JSONEncoder(json.JSONEncoder):
    """
    Encode python dictionaries in an explicit format where the key-value pairs are stored in a list.

    The key and value types are assumed to be consistent, and they are stored to aid the ingest of dictionaries within
    C++.

    Top-level dictionaries are treated as named data that is not stored with explicit key and value types.
    """

    def _encode(self, obj, depth=0):

        def kv_value(val):
            if isinstance(val,list):
                return list(val)
            if isinstance(val,tuple):
                return list(val)
            if isinstance(val,set):
                return list(val)
            if type(val) in [float,int,str]:
                return val
            raise RuntimeError("Unexpected type: {}".format(type(val)))

        def get_key_type(val):
            if isinstance(val,str):
                return 's'
            elif isinstance(val,int):
                return 'i'
            raise RuntimeError("Unexpected key type: {}".format(type(val)))
                
        def key_type(obj):
            key = next(iter(obj))
            if isinstance(key,list):
                return tuple(get_key_type(v) for v in key)
            elif isinstance(key,tuple):
                return tuple(get_key_type(v) for v in key)
            return get_key_type(key)
            
        def get_value_type(val):
            if isinstance(val,tuple):
                return tuple(get_value_type(v) for v in val)
            if isinstance(val,str):
                return 's'
            elif isinstance(val,int):
                return 'i'
            elif isinstance(val,float):
                return 'd'
            raise RuntimeError("Unexpected parameter type: {}".format(type(val)))
                
        def param_value_type(obj):
            value = obj[next(iter(obj))]
            if isinstance(value,list):
                return tuple(get_value_type(v) for v in value)
            elif isinstance(value,tuple):
                return tuple(get_value_type(v) for v in value)
            return get_value_type(value)
            
        def set_value_type(obj):
            if len(obj) == 0:
                raise RuntimeError("Cannot serialize an empty set.  Failure to infer set type.")
            tmp = next(iter(obj))
            return get_value_type(tmp)

        if isinstance(obj, dict):
            if depth == 0:
                return { k:self._encode(v, depth+1) for k, v in obj.items() }
            else:
                v = obj[next(iter(obj))]
                if type(v) is list or type(v) is set:
                    tmp = []
                    for s in obj.values():
                        if len(s) > 0:
                            tmp = set_value_type(s)
                            break 
                    if len(tmp) == 0:
                        raise RuntimeError("Cannot serialize a dictionary with empty set values.  Failure to infer set type.")
                    return { "key_type": key_type(obj), "set_type": tmp, "data": [[kv_value(k), kv_value(v)] for k, v in obj.items()]}
                return { "key_type": key_type(obj), "param_type": param_value_type(obj), "data": [[kv_value(k), kv_value(v)] for k, v in obj.items()]}
        elif isinstance(obj, list) or isinstance(obj, set):
            assert depth > 0, "Unexpected top-level list"
            return { "set_type": set_value_type(obj), "data": [kv_value(v) for v in obj] }
        elif isinstance(obj, tuple):
            return [self._encode(v,depth+1) for v in obj]
        return obj

    def default(self, obj):
        return self._encode(obj)

    def iterencode(self, obj, _one_shot=False):
        return super(StructuredDict_JSONEncoder, self).iterencode(self._encode(obj), _one_shot)


class StructuredDict_JSONDecoder(json.JSONDecoder):
    """
    Decode python dictionaries in an explicit format where the key-value pairs are stored in a list.

    Note that the specification of key types are ignored.
    """

    def __init__(self):
        json.JSONDecoder.__init__(self, object_hook=StructuredDict_JSONDecoder.from_dict)

    @staticmethod
    def from_dict(obj):
        if "set_type" in obj:
            if "key_type" in obj:
                return {tuple(item[0]) if type(item[0]) is list else item[0] : set(item[1]) for item in obj['data']}
            else:
                return set(tuple(v) if type(v) is list else v for v in obj['data'])
        if "param_type" in obj:
            return {tuple(item[0]) if type(item[0]) is list else item[0] : tuple(item[1]) if type(item[1]) is list else item[1] for item in obj['data']}
        return obj


def dump_to_json_data_portal(*args, **kwargs):
    kwargs['cls'] = StructuredDict_JSONEncoder
    return json.dump(*args, **kwargs)

def load_from_json_data_portal(*args, **kwargs):
    kwargs['cls'] = StructuredDict_JSONDecoder
    return json.load(*args, **kwargs)


if __name__ == "__main__":
    def test(data):
        ans  = json.dumps(data, cls=StructuredDict_JSONEncoder)
        print( type(ans), ans)
        val = json.loads(ans, cls=StructuredDict_JSONDecoder)
        print(data)
        print( val, type(val))
        print("")

    # Parameter
    test({"foo": 1.1})                                      # double
    test({"foo": 1})                                        # int
    test({"foo": "1"})                                      # string
    test({"foo": ("bar",1.1)})                              # tuple

    # Set
    test({"foo": [1.1,2.1]})                                # set of doubles
    test({"foo": set([1.1,2.1])})                           # set of doubles
    test({"foo": [1,2]})                                    # set of ints
    test({"foo": set([1,2])})                               # set of ints
    test({"foo": ["bar","foo"]})                            # set of strings
    test({"foo": set(["bar","foo"])})                       # set of strings
    test({"foo": [(1,1), (2,2)]})                           # set of tuples
    test({"foo": set([(1,1), (2,2)])})                      # set of tuples

    # Indexed parameters
    test({"foo": {1:1.1}})                                  # map from ints to doubles
    test({"foo": {1:1}})                                    # map from ints to ints
    test({"foo": {1:"1"}})                                  # map from ints to strings
    test({"foo": {(1,"1"):(1,1.1,"1")}})                    # map from tuples to tuples
    test({"foo": {(1,"1"):"1"}})                            # map from tuples to string
    test({"foo": {1:(1,1.1,"1")}})                          # map from ints to tuples

    # Indexed sets
    test({"foo": {1:["1","2","3"]}})                        # map from ints to set of strings
    test({"foo": {1:set(["1","2","3"])}})                   # map from ints to set of strings
    test({"foo": {(1,"1"):["1","2","3"]}})                  # map from tuples to set of strings
    test({"foo": {(1,"1"):set(["1","2","3"])}})             # map from tuples to set of strings
    test({"foo": {1:["1","2","3"], 2:[]}})                  # map from ints to set of strings

