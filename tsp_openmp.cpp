#include "graph.hpp"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>

using namespace std;

int permute(int num);

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

    double distance = graph.getPathDistance(&path[0]);

    int thread_count = atoi(argv[2]);
#if 0 
    int chunksize = permute(graph.getSize()) / thread_count;
    vector<int> fastest;
    #pragma omp parallel num_threads(thread_count)
    {
	int tid = omp_get_thread_num();
	int start = chunksize * tid;
	int end = chunksize * (tid + 1);
	vector<int> working_arr;
	for (int i = start; i < end; i++) {
	    vector<int> temp_arr = path;
	    working_arr.clear();
	    int working_chunk = chunksize;
	    int n = i;
	  //  cout << "i: " << i << " working_chunk: " << working_chunk << " n: " << n << endl;
	    for (int j = 0; j < path.size(); j++) {
		int current = n / working_chunk;
		int rem = n % working_chunk;
	//	cout << "j: " << j << " current: " << current << " rem: " << rem << endl;
		working_arr.push_back(temp_arr[current]);
		temp_arr.erase(temp_arr.begin() + current);

		working_chunk /= (path.size() - j == 0) ? 1 : path.size() - j;
		if (working_chunk == 0) working_chunk = 1;
		n = rem;
	//	cout << "new working_chunk: " << working_chunk << " n: " << n << endl;
	    }
	    /*
	    #pragma omp critical (print)
	    {
		for (int j = 0; j < working_arr.size(); j++) {
		    cout << working_arr[j] << " ";
		}
		cout << endl;
	    }
	    */

	    int new_dist = graph.getPathDistance(&working_arr[0]);
	    #pragma omp critical (set)
	    {
		if (new_dist < distance) {
		    distance = new_dist;
		    fastest = working_arr;
		}
	    }
	}
	#pragma omp barrier
    }
    for (int i = 0; i < fastest.size(); i++) {
	cout << fastest[i] << " ";
    }
    cout << endl;
#else

    vector< vector<int> > path_perms;
    path_perms.push_back(path);
    while (next_permutation(path.begin(), path.end())) {
	path_perms.push_back(path);
    }

    int path_num = 0;
    #pragma omp parallel num_threads(thread_count)
    {
	#pragma omp for
	for (int i = 0; i < path_perms.size(); i++) {
	    double new_dist = graph.getPathDistance(&path_perms[i][0]);
	    #pragma omp critical
	    {
		if (new_dist < distance) {
		    distance = new_dist;
		    path_num = i;
		}
	    }
	}
    } 
    for (int i = 0; i < path_perms[path_num].size(); i++) {
        cout << path_perms[path_num][i] << " ";
    }
    cout << endl;

#endif
    cout << "Distance: " << distance << endl;
    return 0;
}

int permute(int num) {
    int ret = 1;
    for (int i = 1; i <= num; i++) 
	ret *= i;
    return ret;
}
