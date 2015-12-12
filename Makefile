all: tsp_sequential tsp_openmp tsp_branch tsp_mpi_branch

tsp_mpi_branch: tsp_mpi_branch.o graph.o
		mpiCC -std=c++11 -g -fopenmp -o tsp_mpi_branch tsp_mpi_branch.o graph.o

tsp_openmp: tsp_openmp.o graph.o anneal.o
	    g++ -std=c++11 -g -fopenmp -o tsp_openmp tsp_openmp.o graph.o anneal.o

tsp_branch: tsp_branch.o graph.o
		g++ -std=c++11 -g -fopenmp -o tsp_branch tsp_branch.o graph.o

tsp_sequential: tsp_sequential.o graph.o
	    g++ -std=c++11 -g -fopenmp -o tsp_sequential tsp_sequential.o graph.o

tsp_mpi_branch.o: tsp_mpi_branch.cpp graph.hpp
		mpiCC -std=c++11 -g -fopenmp -c tsp_mpi_branch.cpp

tsp_openmp.o: tsp_openmp.cpp graph.hpp anneal.hpp
	    g++ -std=c++11 -g -fopenmp -c tsp_openmp.cpp

tsp_branch.o: tsp_branch.cpp graph.hpp
		g++ -std=c++11 -g -fopenmp -c tsp_branch.cpp

tsp_sequential.o: tsp_sequential.cpp graph.hpp
	    g++ -std=c++11 -g -c tsp_sequential.cpp

graph.o: graph.cpp graph.hpp
	    g++ -std=c++11 -g -fopenmp -c graph.cpp

anneal.o: anneal.cpp anneal.hpp
	    g++ -std=c++11 -g -c anneal.cpp

clean:
	    rm *.o tsp_sequential tsp_openmp tsp_branch tsp_mpi_branch
