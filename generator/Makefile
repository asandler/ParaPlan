CXX=g++
CXXFLAGS=-std=c++0x -Wall -pedantic

all: generator task_generator splitter

generator:
	$(CXX) $(CXXFLAGS) VPoint.h VParabola.cpp Voronoi.cpp main.cpp -o generator

task_generator:
	$(CXX) $(CXXFLAGS) task_generator.cpp -o task_generator

splitter:
	$(CXX) $(CXXFLAGS) splitter.cpp -o splitter
