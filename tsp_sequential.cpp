#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>    // std::next_permutation

using namespace std;

int main(int argc, char* argv[]) {
    
#if 0
    for (std::map<std::string, int>::iterator it = nameIndexMap.begin(); it != nameIndexMap.end(); ++it) {
    std::cout << it->first << ", " << it->second << "\n";
    }
#endif

    Graph graph = Graph(argv[1]);
    //graph.printDistanceMatrix();

    int path[graph.getSize()];
    for(int i = 0; i < graph.getSize(); i++){
        path[i] = i;
    }

    double distance = graph.getPathDistance(path);
    cout<< "Default distance: " << distance <<endl;
    while(next_permutation(path, path + graph.getSize())){
        
        double new_distance = graph.getPathDistance(path);
        if(new_distance < distance)
        {
            distance = new_distance;
            
            cout << "Path: ";
            for(int i = 0; i < graph.getSize(); i++){
                cout << path[i] << ' ';
            }
            cout << endl << "Distance: " << distance << endl;
        }
    }
    return 0;
}
