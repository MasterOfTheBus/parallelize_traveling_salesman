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
#include <omp.h>

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

int main(int argc, char* argv[]) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    Graph graph = Graph(argv[1]);

    bool parallel = false;
    int threads = 1;
    if (argc > 2) {
        parallel = true;
        threads = atoi(argv[2]);
    }

	double** matrix = graph.getDistanceMatrix();
    double shortest = numeric_limits<double>::max();

    if (parallel) {
        #pragma omp parallel num_threads(threads)
        {
            set<int> vertices;
            for (int i = 0; i < graph.getSize(); i++) {
                vertices.insert(i);
            }

            double distance;
            #pragma omp for
	        for (int i = 0; i < vertices.size(); i++) {
                vector<int> path_vec = dfs(matrix, vertices, i);
                distance = graph.getPathDistance(&path_vec[0]);
            }
            #pragma omp critical
            {
                if (distance < shortest) shortest = distance;
            }
        }
    } else {
        set<int> vertices;
        for (int i = 0; i < graph.getSize(); i++) {
            vertices.insert(i);
        }

        double distance;
	    for (int i = 0; i < vertices.size(); i++) {
            vector<int> path_vec = dfs(matrix, vertices, i);
            distance = graph.getPathDistance(&path_vec[0]);
            if (distance < shortest) shortest = distance;
        }
    }

    cout << shortest << endl;
    gettimeofday(&end, NULL);
    cout << (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec) << " microseconds" << endl;

    return 0;
}
