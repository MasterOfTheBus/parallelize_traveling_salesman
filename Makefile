all: tsp_sequential

tsp_sequential: tsp_sequential.o graph.o
	    g++ -std=c++11 -g -o tsp_sequential tsp_sequential.o graph.o

tsp_sequential.o: tsp_sequential.cpp graph.hpp
	    g++ -std=c++11 -g -c tsp_sequential.cpp

graph.o: graph.cpp graph.hpp
	    g++ -std=c++11 -g -c graph.cpp

clean:
	    rm *.o tsp_sequential
