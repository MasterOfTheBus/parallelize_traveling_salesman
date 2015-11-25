#include "graph.hpp"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>

using namespace std;

vector<int> nth_permutation(int n, vector<int> arr, int tid);

int main(int argc, char* argv[]) {

    int loop_counter = -1;
    int stop_cond = 0;
    int thread_counter, thread_stop;

    Graph graph = Graph(argv[1]);

    vector<int> path;
    path.reserve(graph.getSize());
    for (int i = 0; i < graph.getSize(); i++) {
	path.push_back(i);
    }

    vector< vector<int> > path_perms;
    path_perms.push_back(path);
    while (next_permutation(path.begin(), path.end())) {
	path_perms.push_back(path);
    }

    double distance = graph.getPathDistance(&path[0]);

    int thread_count = atoi(argv[2]);
    int path_num = 0;
    #pragma omp parallel num_threads(thread_count)
    {
	#pragma omp for
	for (int i = 0; i < path_perms.size(); i++) {
	    int new_dist = graph.getPathDistance(&path_perms[i][0]);
	    #pragma omp critical
	    {
		if (new_dist < distance) {
		    distance = new_dist;
		    path_num = i;
		}
	    }
	}
   } 

    cout << "Distance: " << distance << endl;
    for (int i = 0; i < path_perms[path_num].size(); i++) {
	cout << path_perms[path_num][i] << " ";
    }
    cout << endl;

    return 0;
}
