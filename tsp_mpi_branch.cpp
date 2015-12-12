#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <set>
#include <stack>
#include <vector>
#include <utility>
#include <limits>
#include <mpi.h>
#include <new>

using namespace std;

vector<int> dfs(double** matrix, set<int> vertices, int starting) {
    double dist = numeric_limits<double>::max();
	stack< pair<vector<int>, double> > path;
    vector<int> node;
    set<int> initial = vertices;
    node.push_back(starting);
    path.push(make_pair(node, 0.0));
    while (!path.empty()) {
        vector<int> partial = path.top().first;
        double partial_dist = path.top().second;
        path.pop();

        for (int i = 0; i < partial.size(); i++) {
            vertices.erase(partial[i]);
        }
        if (vertices.empty()) {
            if (partial_dist < dist) {
                dist = partial_dist;
                node = partial;
            }
        }
        for (set<int>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
            int test = partial_dist + matrix[partial.back()][*it]; 
            if (test <= dist) {
                partial.push_back(*it);
                path.push(make_pair(partial, test));
                partial.pop_back();
            }
        }
        vertices = initial;
    }

    return node;
}


double ** allocate_matrix(int size) {
#if 0
    double *data = (double*)malloc(size * size * sizeof(double));
    if (!data) cout << "data failed" << endl;
    double **array = (double**)malloc(size * sizeof(double*));
    if (!array) cout << "array failed" << endl;
    for (int i = 0; i < size; i++) {
        array[i] = &(data[size * i]);
    }
    return array;
#endif
    double *data = new double[size * size];
    double **array = new double*[size];
    for (int i = 0; i < size; i++)
        array[i] = &(data[size * i]);
    return array;
}

void free_matrix(double** matrix, int size) {
    //free(&(matrix[0][0]));
    //free(matrix);
    delete[] matrix[0];
    delete[] matrix;
}

int main(int argc, char* argv[]) {
    int size, rank, graph_size;
    double** matrix;
    double shortest = numeric_limits<double>::max();
    struct timeval start, end;
    Graph graph;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        gettimeofday(&start, NULL);

        graph = Graph(argv[1]);
        graph_size = graph.getSize();
	    matrix = graph.getDistanceMatrix();
    }

    MPI_Bcast(&graph_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) matrix = allocate_matrix(graph_size);
    
    MPI_Bcast(&(matrix[0][0]), graph_size * graph_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    set<int> vertices;
    for (int i = 0; i < graph_size; i++) {
        vertices.insert(i);
    }
    
    //double distance;
    //int path_vec[graph_size];
    
    vector<int> path_vec = dfs(matrix, vertices, rank);
    if (rank != 0) {
        MPI_Send(&path_vec[0], graph_size, MPI_INT, 0, rank, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            int from_proc[graph_size];
            MPI_Status status;
            MPI_Recv(from_proc, graph_size, MPI_INT, i, i, MPI_COMM_WORLD, &status);
            double distance = graph.getPathDistance(from_proc);
            cout << distance << " calculated " << i << endl;
            if (distance < shortest) shortest = distance;
        }
    }
 
    //distance = graph.getPathDistance(&path_vec[0]);
    //MPI_Reduce(&distance, &shortest, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  
    if (rank != 0) free_matrix(matrix, graph_size);
    MPI_Finalize();

    if (rank == 0) {
        cout << shortest << endl;
        gettimeofday(&end, NULL);
        cout << (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec) << " microseconds" << endl;
    }
    return 0;
}
