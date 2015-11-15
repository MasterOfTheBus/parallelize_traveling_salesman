#include "graph.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

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

    print_graph(graph, nameIndexMap.size());

    free_graph(graph, nameIndexMap.size());

    return 0;
}
