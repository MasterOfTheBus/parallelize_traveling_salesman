#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>    // std::next_permutation

using namespace std;

int main(int argc, char* argv[]) {
    int nodes;

    std::map<std::string, int> nameIndexMap = get_node_names(argv[1]);
#if 0
    for (std::map<std::string, int>::iterator it = nameIndexMap.begin(); it != nameIndexMap.end(); ++it) {
    std::cout << it->first << ", " << it->second << "\n";
    }
#endif
    int ** graph = load_locations(argv[1], nameIndexMap);

    printf("loaded graph\n");
    int tour[nameIndexMap.size()];
    for(int i = 0; i < nameIndexMap.size(); i++){
        tour[i] = i;
    }
    int score = getTotalDistance(graph, tour, nameIndexMap.size());;
    cout<< score <<endl;
    print_graph(graph, nameIndexMap.size());
    while(next_permutation(tour, tour + nameIndexMap.size())){
        int distance = getTotalDistance(graph, tour, nameIndexMap.size());
        if(distance < score)
        {
            score = distance;
            for(int i = 0; i < nameIndexMap.size(); i++){
                cout << tour[i] << ' ';
            }
            cout << endl << score << endl;
        }
    }
    free_graph(graph, nameIndexMap.size());

    return 0;
}
