all: exprapi

clean:
	rm -f *.o *.so

libexprapi.so: expr_api.o
	g++ -g -shared $^ -o $@

%.o: %.cpp
	g++ -g -c -Wc++11-extensions -Wall -Werror -fpic $^

exprapi: export LD_LIBRARY_PATH = $(shell pwd)
exprapi: libexprapi.so
	./build.py

