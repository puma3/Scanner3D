#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <iostream>
#include <vector>

using namespace std;

struct Point3D {
    Point3D()   {}
    Point3D(float _x, float _y, float _z);

    float   x,
            y,
            z;
};

class Polygon {
public:
    Polygon(Point3D *, Point3D *, Point3D *, Point3D *);

    Point3D *edge_1,
            *edge_2,
            *edge_3,
            *edge_4;
};

class PointCloud {
public:
    PointCloud();

    void push_back(Point3D pnt);

    void push_back(float _x, float _y, float _z);

    void setHeight(int _height) {   height = _height;    }

    void setFrames(int _frames) {   frames = _frames;    }

    void clean();

    void meshify(int step = 5);

    int height,
        frames;

    vector<Point3D> cloud;

    vector<Polygon> mesh;
};

#endif // POINTCLOUD_H
