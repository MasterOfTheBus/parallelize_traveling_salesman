#include <map>
#include <string>

std::map<std::string, int> get_node_names(std::string filename);
int** load_locations(std::string filename, std::map<std::string, int> nodeMap);
void print_graph(int** graph, int nodes);
void free_graph(int** graph, int nodes);
int getTotalDistance(int** graph, int* tour, int nodes);