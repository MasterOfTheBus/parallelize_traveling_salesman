#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>
#include <string>

using namespace std;

class Vertex{
public:
    Vertex(){ }
    Vertex(double x, double y);
    double x;
    double y;
    double getDistanceTo(Vertex v);
};

class Graph{
private:
    int m_size;
    Vertex *m_vertices;
    double** m_distanceMatrix;
    map<string, int> m_nodeMap;
public:
    Graph(string filename);
    ~Graph();
    int getSize() { return m_size; }
    double** getDistanceMatrix() { return m_distanceMatrix; }
    map<string, int> getNodeMap() { return m_nodeMap; }
    double getPathDistance(int *path);
    void printDistanceMatrix();

};

#endif