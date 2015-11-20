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
