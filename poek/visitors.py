from poek.coek_cffi import ffi, lib
from poek.globals import NULL, BUFFER

__all__ = ['Visitor', 'ValueVisitor']


class Visitor(object):

    def walk(self, expr):
        visitor = ffi.new_handle(self)
        self._visitor = visitor       # keep this object alive
        lib.visitor_walk(expr.ptr, lib.visitor_enter_callback, lib.visitor_exit_callback, visitor)
        return self._return()

    def visit_enter(self, ptr, parent):
        """
        This method needs to be defined by a subclass.  It is
        called when a node in an expression tree is visited.
        """
        pass
        
    def visit_exit(self, ptr, parent):
        """
        This method needs to be defined by a subclass.  It is
        called after all children of a node have been visited.
        """
        pass
        
@ffi.def_extern()
def visitor_enter_callback(ptr, parent, visitor):
    self = ffi.from_handle(visitor)
    self.visit_enter(ptr, parent)

@ffi.def_extern()
def visitor_exit_callback(ptr, parent, visitor):
    self = ffi.from_handle(visitor)
    self.visit_exit(ptr, parent)



#
# This class generates a nested list of lists, which 
# represents the tree structure for an expression.
#
class ValueVisitor(Visitor):

    def walk(self, expr, show=False):
        self.show = show
        return Visitor.walk(self, expr)

    def _return(self):
        return self.values[0]

    def visit_enter(self, ptr, parent):
        if parent == NULL:
            self.values = [ ]
            self.curr = [self.values]
        lib.get_numval_str(ptr, BUFFER, 64)
        tmp = [ffi.string(BUFFER).decode('utf-8') ]
        self.curr[-1].append( tmp )
        self.curr.append( tmp )

    def visit_exit(self, ptr, parent):
        tmp = self.curr.pop()
        if type(tmp) is list and len(tmp) == 1:
            self.curr[-1].pop()
            if self.show:
                self.curr[-1].append(tmp[0])
            else:
                self.curr[-1].append( tmp[0].split('{')[0] )
        #
        # This is a hack for inequality and equalities.  The RHS is implicit in these classe,
        # but we make it explicit here.
        #
        if self.curr[-1][0] == '<=':
            self.curr[1].append( '0' )
        elif self.curr[-1][0] == '<':
            self.curr[1].append( '0' )
        elif self.curr[-1][0] == '==':
            self.curr[1].append( '0' )


