all: tsp_sequential tsp_openmp

tsp_openmp: tsp_openmp.o graph.o
	    g++ -std=c++11 -g -fopenmp -o tsp_openmp tsp_openmp.o graph.o

tsp_sequential: tsp_sequential.o graph.o
	    g++ -std=c++11 -g -fopenmp -o tsp_sequential tsp_sequential.o graph.o

tsp_openmp.o: tsp_openmp.cpp graph.hpp
	    g++ -std=c++11 -g -fopenmp -c tsp_openmp.cpp

tsp_sequential.o: tsp_sequential.cpp graph.hpp
	    g++ -std=c++11 -g -c tsp_sequential.cpp

graph.o: graph.cpp graph.hpp
	    g++ -std=c++11 -g -fopenmp -c graph.cpp

clean:
	    rm *.o tsp_sequential tsp_openmp
