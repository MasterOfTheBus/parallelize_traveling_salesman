#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>    // std::next_permutation
#include <sys/time.h>
#include <math.h>

using namespace std;

double acceptanceProbability(double currentDistance, double newDistance, double temperature){
    if(newDistance < currentDistance){
        return 1.0;
    }
    return exp((currentDistance-newDistance)/temperature);
}

int main(int argc, char* argv[]) {

    struct timeval start, end;
    gettimeofday(&start, NULL);
    srand (time(NULL));

    Graph graph = Graph(argv[1]);
    //graph.printDistanceMatrix();

    int path[graph.getSize()];
    for(int i = 0; i < graph.getSize(); i++){
        path[i] = i;
    }

    double distance = graph.getPathDistance(path);
    cout<< "Default distance: " << distance <<endl;
    while(next_permutation(path, path + graph.getSize())){
        
        double newDistance = graph.getPathDistance(path);
        if(newDistance < distance)
        {
            distance = newDistance;
            
            cout << "Path: ";
            for(int i = 0; i < graph.getSize(); i++){
                cout << path[i] << ' ';
            }
            cout << endl << "Distance: " << distance << endl;
        }
    }

    gettimeofday(&end, NULL);
    cout << (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec) << " microseconds" << endl;

    return 0;
}
