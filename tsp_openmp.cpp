#include "graph.hpp"
#include "anneal.hpp"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <sys/time.h>
#include <string.h>

using namespace std;

int permute(int num);

int main(int argc, char* argv[]) {

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int loop_counter = -1;
    int stop_cond = 0;
    int thread_counter, thread_stop;

    int thread_count = atoi(argv[2]);
    Graph graph = Graph(argv[1], true, thread_count);

    vector<int> path;
    path.reserve(graph.getSize());
    for (int i = 0; i < graph.getSize(); i++) {
	path.push_back(i);
    }

    double distance = graph.getPathDistance(&path[0]);

    bool anneal = false;
    if (argc == 4)
	anneal = (strcmp(argv[3], "-a") == 0) ? true : false;

    if (!anneal) {
	vector< vector<int> > path_perms;
	path_perms.push_back(path);
	while (next_permutation(path.begin(), path.end())) {
	    path_perms.push_back(path);
        }

	int path_num = 0;
        #pragma omp parallel num_threads(thread_count)
	{
	    double local_dist;
	    int path_i = 0;
	    #pragma omp critical
	    local_dist = distance;

	    #pragma omp for
	    for (int i = 0; i < path_perms.size(); i++) {
		double new_dist = graph.getPathDistance(&path_perms[i][0]);
		if (new_dist < local_dist) {
		    local_dist = new_dist;
		    path_i = i;
		}
	    }

	    #pragma omp barrier
	    #pragma omp critical
	    {
		if (local_dist < distance) {
		    distance = local_dist;
		    path_num = path_i;
		}
	    }
	} 
	for (int i = 0; i < path_perms[path_num].size(); i++) {
	    cout << path_perms[path_num][i] << " ";
	}
	cout << endl;

    } else {

        vector<int> final_path = path;
	int permutations = permute(path.size());
	int total_iter = (permutations) / 2;

	srand(time(NULL));

	#pragma omp parallel num_threads(thread_count)
	{
	    Anneal annealer = Anneal(1000);
	
	    vector<int> temp_arr;
	    vector<int> local_path;
	    double local_dist;
	    #pragma omp critical (local_arr)
	    {
		temp_arr = path;
		local_dist = distance;
	    }

	    for (int i = 0; i < total_iter; i++) {
		int a = rand() % graph.getSize();
		int b;
		do {
		    b = rand() % graph.getSize();
		} while (b == a);

		int swap = temp_arr[a];
		temp_arr[a] = temp_arr[b];
		temp_arr[b] = swap;

		double new_dist = graph.getPathDistance(&temp_arr[0]);
		double difference = local_dist - new_dist;

		double thresh = annealer.get_accept_threshold(difference, i);
		double accept = (rand() % 100) / 100.0;
		if (accept < thresh) {
		    local_dist = new_dist;
		    local_path = temp_arr;
		}
	    }

	    #pragma omp barrier
	    #pragma omp critical (update)
	    {
		if (local_dist < distance) {
		    distance = local_dist;
		    final_path = local_path;
		}
	    }
	}

	for (int i = 0; i < final_path.size(); i++) {
	    cout << final_path[i];
	}
	cout << endl;
    }

    cout << "Distance: " << distance << endl;

    gettimeofday(&end, NULL);
    cout << (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec) << " microseconds" << endl;
    return 0;
}

int permute(int num) {
    int ret = 1;
    for (int i = 1; i <= num; i++) 
	ret *= i;
    return ret;
}
