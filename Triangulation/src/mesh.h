#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

struct Point_3D {
    Point_3D(double _x = 0.0, double _y = 0.0, double _z = 0.0) :
        x(_x), y(_y), z(_z)
    {}

    Point_3D& operator=(Point_3D &_other);

    friend ostream& operator<<(ostream& os, Point_3D& _point);

//Members
    double x;
    double y;
    double z;
};

struct Face;

struct Vertex {
    typedef size_t indx;

    Vertex() {  index = 0;  depth = 0;  isInterestPoint = false;    harrisResponse = 0.0;   }

    Vertex(Point_3D _pos, indx _index = 0, bool _isInterestPoint = false);

    friend ostream& operator<<(ostream& os, Vertex& _vertex);

    void addVertex(indx _ind_vertex){ m_adjacentVertices.insert(_ind_vertex);}

    void addFace(indx _ind_face){ m_faces.push_back(_ind_face); }

    void ring(int k, vector<Vertex*> &_neighborhood, Vertex* vertices);

    void defineInterest(Vertex* _vertices);

//Members
    Point_3D m_pos;

    indx index;

    indx depth;

    double harrisResponse;

    bool isMarked;

    bool isInterestPoint;

    vector<indx> m_faces;

    set<indx> m_adjacentVertices;
};

inline bool comp(Vertex i, Vertex j){	return i.harrisResponse < j.harrisResponse; }

struct Face {
    Face() {}

    void add(Vertex::indx _index);

    vector<Vertex::indx>& getVertices();

//Members
    vector<Vertex::indx> m_vertices;
};


struct Mesh {
    Mesh();

    void reset();

    void loadOffFile(string filename);

//Members
    Vertex* m_vertices;

    Face* m_faces;

    int m_nVertices;

    int m_nFaces;
};

#endif // MESH_H
