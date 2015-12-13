#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>    // std::next_permutation
#include <sys/time.h>
#include <math.h>

using namespace std;

void shuffle(int* path, int size){
    for (int i = 0; i < size; ++i)
    {
        int index = rand() % graph.getSize();
        int tmp;
        path[i] = tmp;
        path[i] = path[index];
        path[index] = tmp;
    }
}

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
    int thread_count = atoi(argv[2]);
    //graph.printDistanceMatrix();

    int path[graph.getSize()];
    for(int i = 0; i < graph.getSize(); i++){
        path[i] = i;
    }

    double distance = graph.getPathDistance(path);
    cout<< "Default distance: " << distance <<endl;

    double temperature = 0.1;
    double coolingRate = 0.000001;
    int bestPath[graph.getSize()];
    int currentPath[graph.getSize()];

    for(int i = 0; i < graph.getSize(); i++){
        bestPath[i] = i;
        currentPath[i] = i;
    }

    double currentDistance = distance;
    int iteration = 0;
    int success_count = 100;
    
    #pragma omp parallel num_threads(thread_count)
    {
        shuffle(path, graph.getSize());
        while(temperature > 0.001){
        iteration++;
        success_count = 0;
        for (int i = 0; i < 2000; ++i)
        {
            for(int i = 0; i < graph.getSize(); i++){
                path[i] = currentPath[i];
            }
            int index1 = rand() % graph.getSize();
            int index2 = rand() % graph.getSize();

            int tmp = path[index1];
            path[index1] = path[index2];
            path[index2] = tmp;
            
            double newDistance = graph.getPathDistance(path);
            //cout << acceptanceProbability(currentDistance, newDistance, temperature) << endl;

            if(acceptanceProbability(currentDistance, newDistance, temperature) > rand()/RAND_MAX){
                success_count++;
                currentDistance = newDistance;
                for(int i = 0; i < graph.getSize(); ++i){
                    currentPath[i] = path[i];
                }
            }
            if(distance > currentDistance){
                success_count+=10;
                distance = currentDistance;
                cout << "Path: ";
                for(int i = 0; i < graph.getSize(); ++i){
                    bestPath[i] = currentPath[i];
                    cout << bestPath[i] << ' ';
                }
                cout << endl << "Distance: " << distance << endl;
            }

        }
        cout << success_count << endl;
        temperature *= 1-coolingRate;
        //temperature = 10.0/(1+200*log10(1+iteration));
        cout << temperature << endl;
    }
    }

    cout << "Path: ";
    for(int i = 0; i < graph.getSize(); ++i){
        bestPath[i] = path[i];
        cout << bestPath[i] << ' ';
    }
    cout << endl << "Distance: " << distance << endl;

    gettimeofday(&end, NULL);
    cout << (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec) << " microseconds" << endl;

    return 0;
}