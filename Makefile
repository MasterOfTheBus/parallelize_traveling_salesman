all: tsp_sequential

tsp_sequential: tsp_sequential.o graph.o
	    g++ -g -o tsp_sequential tsp_sequential.o graph.o

tsp_sequential.o: tsp_sequential.cpp graph.hpp
	    g++ -g -c tsp_sequential.cpp

graph.o: graph.cpp graph.hpp
	    g++ -g -c graph.cpp

clean:
	    rm *.o tsp_sequential
