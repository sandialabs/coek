all: exprapi

clean:
	rm -f *.o *.so

libexprapi.so: expr_api.o model.o expr_types.o
	g++ -O3 -shared $^ -o $@

%.o: %.cpp
	g++ -O3 -c -Wc++11-extensions -Wall -Werror -fpic -I/anaconda3/envs/py36/include/python3.6m $^

tmp: tmp.o
	g++ -O3 tmp.o -o tmpexec libexprapi.so

exprapi: export LD_LIBRARY_PATH = $(shell pwd)
exprapi: libexprapi.so
	./build.py

