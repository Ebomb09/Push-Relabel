.PHONY: all clean test
FLAGS = -std=c++14

all: bin/pr bin/ff
	
clean: 
	rm bin/ff bin/pr

bin/pr: src/graph.hpp src/push-relabel.cpp
	g++ -o "bin/pr" src/push-relabel.cpp ${FLAGS}

bin/ff: src/graph.hpp src/ford-main.cpp
	g++ -o "bin/ff" src/ford-main.cpp ${FLAGS}

test: all
	python3 tests/gen.py