#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <math.h> 
#include "graph.hpp"

using namespace std;

Vertex::Vertex(double x, double y){
    this->x = x;
    this->y = y;
}

double Vertex::getDistanceTo(Vertex v){
    double distance = sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
    return distance; 
}

double ** allocateSquareMatrix(int size) {
    double ** matrix = (double**) malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (double*) malloc(size * sizeof(double));
    }
    return matrix;
}

Graph::Graph(string filename){
    int index = 0;
    string line;
    ifstream inputFile(filename.c_str());
    if (inputFile.is_open()) {
        getline(inputFile, line);
        char dest[1000];
        strcpy(dest, line.c_str());
        char* name = strtok(dest, " ,");
        while (name != NULL) {
            if (m_nodeMap.find(name) == m_nodeMap.end()) {
                m_nodeMap.insert(pair<string, int>(name, index));
                index++;
            }
            name = strtok(NULL, " ,");
        }
    }

    m_size = m_nodeMap.size();

    m_vertices = new Vertex[m_size];
    m_distanceMatrix = allocateSquareMatrix(m_size);
    if (inputFile.is_open()) {
        for (int i = 0; i < m_size; i++) {
            getline(inputFile, line);
            string::size_type sz;     // alias of size_t

            m_vertices[i].x = std::stod(line,&sz);
            m_vertices[i].y = std::stod(line.substr(sz));
        }
    }
    inputFile.close();

    for (int i = 0; i < m_size; ++i)
    {
        for (int j = 0; j < m_size; ++j)
        {
            if(i == j)
                m_distanceMatrix[i][j] = 0.0;
            else{
                m_distanceMatrix[i][j] = m_vertices[i].getDistanceTo(m_vertices[j]);
            }
        }
    }
}

Graph::~Graph(){
    delete[] m_vertices;
    for(int i = 0; i < m_size; ++i){
        delete[] m_distanceMatrix[i];
    }
    delete[] m_distanceMatrix;
}

void Graph::printDistanceMatrix() {
    for (int i = 0; i < m_size; ++i)
    {
        for (int j = 0; j < m_size; ++j)
        {
            if(i == j)
                cout << "0.00000 ";
            else
                cout << m_distanceMatrix[i][j] << ' ';
        }
        cout << endl;
    }
}

double Graph::getPathDistance(int *path){
    double result = m_distanceMatrix[path[0]][path[m_size - 1]];
    for(int i = 1; i < m_size; i++){
        result += m_distanceMatrix[path[i-1]][path[i]];
    }
    return result;
}
    // GRAPH_HPP
    // map<string, int> get_node_names(string filename) {
    //     map<string, int> nodeMap;
    //     int index = 0;
    //     string line;
    //     ifstream inputFile(filename.c_str());
    //     if (inputFile.is_open()) {
    //         getline(inputFile, line);
    //         char dest[1000];
    //         strcpy(dest, line.c_str());
    //         char* name = strtok(dest, " ,");
    //         while (name != NULL) {
    //             if (nodeMap.find(name) == nodeMap.end()) {
    //                 nodeMap.insert(pair<string, int>(name, index));
    //                 index++;
    //             }
    //             name = strtok(NULL, " ,");
    //         }
    //         inputFile.close();
    //     }
    //     return nodeMap;
    // }

    // int ** allocate_square_matrix(int locations) {
    //     int ** matrix = (int**) malloc(locations * sizeof(int*));
    //     for (int i = 0; i < locations; i++) {
    //         matrix[i] = (int*) malloc(locations * sizeof(int));
    //     }
    //     return matrix;
    // }

    // void parse_node(char* node, string* name, int* distance) {
    //     string node_s = node;
    //     size_t pos = node_s.find(",");
    //     *name = node_s.substr(0, pos);
    //     *distance = atoi(node_s.substr(pos+1).c_str());
    // }

    // int** parse_line(string line, int** graph, map<string, int> nodeMap) {
    //     char dest[1000];
    //     strcpy(dest, line.c_str());

    //     // get the node name
    //     char* name = strtok(dest, ";");

    //     // get the neighbors
    //     char* neighbors = strtok(NULL, ";");
    //     while (neighbors != NULL) {
    //         string n_name;
    //         int distance;

    //         parse_node(neighbors, &n_name, &distance);
    //         graph[nodeMap[name]][nodeMap[n_name]] = distance;

    //         neighbors = strtok(NULL, ";");
    //     }
    //     return graph;
    // }

    // int** load_locations(string filename, map<string, int> nodeMap) {
    //     int** graph = allocate_square_matrix(nodeMap.size());

    //     string line;
    //     ifstream inputFile;
    //     inputFile.open(filename.c_str());
    //     if (inputFile.is_open()) {
    //         getline(inputFile, line); // ignore the first line
            
    //         while(getline(inputFile, line)) {
    //             graph = parse_line(line, graph, nodeMap);
    //         }

    //         inputFile.close();
    //     }

    //     // make sure the diagonal is all 0
    //     for (int i = 0; i < nodeMap.size(); i++) {
    //         graph[i][i] = 0;
    //     }

    //     return graph;
    // }

    // void free_graph(int** graph, int nodes) {
    //     for (int i = 0; i < nodes; i++) free(graph[i]);
    //     free(graph);
    // }

    // void print_graph(int** graph, int nodes) {
    //     for (int i = 0; i < nodes; i++) {
    //         for (int j = 0; j < nodes; j++) {
    //             printf("%d ", graph[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }

    // int getTotalDistance(int** graph, int* tour, int nodes){
    //     double result = graph[tour[0]][tour[nodes - 1]];
    //     for(int i = 1; i < nodes; i++){
    //         result += graph[tour[i-1]][tour[i]];
    //     }
    //     return result;
    // }