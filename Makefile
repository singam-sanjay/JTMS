CXXFLAGS=-std=c++11 -Wall

prgm: all_func.h all_class.h main.cpp
	g++ main.cpp $(CXXFLAGS) -o prgm
prgm-g: all_func.h all_class.h main.cpp
	g++ main.cpp $(CXXFLAGS) -o prgm-g -g
