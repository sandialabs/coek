import pycoek_cppyy as pycoek

NAN = float('nan')

def pythonize_coek_all(klass, name):
    #print(("PYTHONIZE COEK", klass, name))

    def to_nested_list(b, e):
        tmp = []
        while (b != e):
            val = b.__deref__()
            b.__preinc__()
            if val == ']':
                return tmp
            elif val == '[':
                tmp.append( to_nested_list(b,e) )
            else:
                tmp.append( val )
        return tmp

    def to_list(self):
        "A pythonization of the COEK to_list() method"
        vals = self.__to_list()
        b = vals.begin()
        e = vals.end()
        ans = to_nested_list(b,e)
        if len(ans) == 1 and type(ans[0]) is list:
            return ans[0]
        return ans

    def get_name_str_or_None(self):
        name = self.get_name()
        if name:
            return name
        return "x"

    def Model_add_variable(self, *args, **kwds):
        if len(args) == 1:
            return self._add_variable_singlevar(args[0])
        else:
            name = kwds.get('name',"x")
            lb = kwds.get('lb', -self.inf)
            ub = kwds.get('ub', self.inf)
            init = kwds.get('value', NAN)
            binval = kwds.get('binary', 0)
            integer = kwds.get('integer', 0)
            return self._add_variable_newvar(name, lb, ub, init, binval, integer)

    def VariableArray_iter(self):
        for i in range(self.variables.size()):
            yield i

    def VariableArray_iter_BAD(self):
        if self.dimen.size() == 0:
            b = self.unindexed_begin()
            e = self.unindexed_end()
        else:
            b = self.indexed_begin()
            e = self.indexed_end()
        while b != e:
            yield b.__deref__()
            b.__preinc__()

    def bool_error(self):
        raise TypeError("bool() argument must be a boolean value.")

    def constraint_bool_error(self, other):
        raise TypeError("Constraint argument cannot be used in a boolean expression.")

    def varray_bool_error(self, other):
        raise TypeError("VariableArray argument cannot be used in a boolean expression.")

    def expression__init(self, *args, **kwargs):
        if type(args[0]) is pycoek.coek.VariableArray:
            raise TypeError("Variable array argument cannot be used in a boolean expression.")
        if type(args[0]) is pycoek.coek.Constraint:
            raise TypeError("Constraint argument cannot be used in a boolean expression.")
        return self.__init__bak(*args, **kwargs)

    if name == 'Variable':
        klass.__init__ = klass.__init__.__overload__("const string&,double,double,double,bool,bool")
        klass.value = property(klass.get_value, klass.set_value, doc="Value of this variable")
        klass.name = property(get_name_str_or_None, doc="Name of this variable")
        klass.__pos__ = pycoek.coek.Variable_operator_pos
        klass.__neg__ = pycoek.coek.Variable_operator_neg
        klass.__radd__ = pycoek.coek.Variable_operator_radd
        klass.__rsub__ = pycoek.coek.Variable_operator_rsub
        klass.__rmul__ = pycoek.coek.Variable_operator_rmul
        klass.__rtruediv__ = pycoek.coek.Variable_operator_rtruediv
        klass.__truediv__ = pycoek.coek.Variable_operator_truediv
        klass.__pow__ = pycoek.coek.Variable_pow
        klass.__rpow__ = pycoek.coek.Variable_rpow
        klass.__bool__ = bool_error
        #klass.__eq__ = klass.__cpp_eq__
        klass.__eq__ = pycoek.coek.Variable_operator_eq

    elif name == 'Objective':
        klass.__to_list = klass.to_list
        klass.to_list = to_list

    elif name == 'Expression':
        klass.__init__bak = klass.__init__
        klass.__init__ = expression__init
        klass.value = property(klass.get_value, doc="Value of this expression")
        klass.__to_list = klass.to_list
        klass.to_list = to_list
        klass.__pos__ = pycoek.coek.Expression_operator_pos
        klass.__neg__ = pycoek.coek.Expression_operator_neg
        klass.__radd__ = pycoek.coek.Expression_operator_radd
        klass.__rsub__ = pycoek.coek.Expression_operator_rsub
        klass.__rmul__ = pycoek.coek.Expression_operator_rmul
        klass.__rtruediv__ = pycoek.coek.Expression_operator_rtruediv
        klass.__pow__ = pycoek.coek.Expression_pow
        klass.__rpow__ = pycoek.coek.Expression_rpow
        klass.__bool__ = bool_error

    elif name == 'Parameter':
        klass.__init__ = klass.__init__.__overload__("double,const string&")
        klass.value = property(klass.get_value, klass.set_value, doc="Value of this parameter")
        klass.name = property(get_name_str_or_None, doc="Name of this parameter")
        klass.__pos__ = pycoek.coek.Parameter_operator_pos
        klass.__neg__ = pycoek.coek.Parameter_operator_neg
        klass.__radd__ = pycoek.coek.Parameter_operator_radd
        klass.__rsub__ = pycoek.coek.Parameter_operator_rsub
        klass.__rmul__ = pycoek.coek.Parameter_operator_rmul
        klass.__rtruediv__ = pycoek.coek.Parameter_operator_rtruediv
        klass.__pow__ = pycoek.coek.Parameter_pow
        klass.__rpow__ = pycoek.coek.Parameter_rpow
        klass.__bool__ = bool_error

    elif name == 'Constraint':
        klass.id = property(klass.id, doc="A unique integer id.")
        klass.feasible = property(klass.is_feasible, doc="This value is True if the constraint is feasible.")
        klass.lb = property(klass.get_lb, doc="The value of the constraint lower bound.")
        klass.ub = property(klass.get_ub, doc="The value of the constraint upper bound.")
        klass.value = property(lambda self: self.body().get_value(), doc="The value of the constraint body")
        klass.__to_list = klass.to_list
        klass.to_list = to_list
        klass.__bool__ = bool_error
        klass.__eq__ = constraint_bool_error
        klass.__lt__ = constraint_bool_error
        klass.__le__ = constraint_bool_error
        klass.__gt__ = constraint_bool_error
        klass.__ge__ = constraint_bool_error

    elif name == 'VariableArray':
        klass.__init__ = klass.__init__.__overload__("int,string,double,double,double,bool,bool,bool")
        klass.name = property(get_name_str_or_None, doc="Name of this variable")
        klass.__getitem__ = klass.get
        klass.__iter__ = VariableArray_iter
        klass.__eq__ = varray_bool_error
        klass.__lt__ = varray_bool_error
        klass.__le__ = varray_bool_error
        klass.__gt__ = varray_bool_error
        klass.__ge__ = varray_bool_error

    elif name == 'Model':
        klass._add_variable_newvar = klass.add_variable.__overload__('const string&,double,double,double,bool,bool')
        klass._add_variable_singlevar = klass.add_variable.__overload__('coek::Variable&')
        klass._add_variable_vararray = klass.add_variable.__overload__('coek::VariableArray&')
        klass.add_variable = Model_add_variable

