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

class PointCloud {
public:
    PointCloud();

    void push_back(Point3D pnt);

    void push_back(float _x, float _y, float _z);

    vector<Point3D> cloud;
};

#endif // POINTCLOUD_H
