all: exprapi

clean:
	rm -f *.o *.so tmpexec _expr.c

libexprapi.so: expr_api.o model.o expr_types.o solvers.o
	g++ -O3 -shared $^ -o $@ -L$(GUROBI_HOME)/lib -lgurobi_c++ -lgurobi80

%.o: %.cpp
	#g++ -O3 -c -Wc++11-extensions -Wall -Werror -fpic -I/anaconda3/envs/py36/include/python3.6m $^
	g++ -O3 -c -Wall -Werror -fpic -I/anaconda3/envs/py36/include/python3.6m -I$(GUROBI_HOME)/include $^

tmp: tmp.o
	g++ -O3 tmp.o -o tmpexec libexprapi.so

exprapi: export LD_LIBRARY_PATH = $(shell pwd)
exprapi: libexprapi.so
	./build.py

