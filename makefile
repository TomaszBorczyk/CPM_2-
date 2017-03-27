CXX=g++
CXXFLAGS=-std=c++11

CPM: Graph.o CPM2.o
	$(CXX) $(CXXFLAGS) Graph.o CPM2.o -o CPM

CPM2.o: Graph.o
	$(CXX) $(CXXFLAGS) -c CPM2.cpp -o CPM2.o

Graph.o: Graph.hpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp -o Graph.o

clean:
	rm *.o CPM
