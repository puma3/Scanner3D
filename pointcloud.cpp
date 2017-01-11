#include "pointcloud.h"

Point3D::Point3D(float _x, float _y, float _z) :
    x(_x),
    y(_y),
    z(_z)
{}

PointCloud::PointCloud()
{}

void PointCloud::push_back(Point3D pnt)
{
    cloud.push_back(pnt);
}

void PointCloud::push_back(float _x, float _y, float _z)
{
    cloud.push_back(Point3D(_x, _y, _z));
}

void PointCloud::clean()
{
    cloud.clear();
    mesh.clear();
}

void PointCloud::meshify(int step)
{
    for(auto i = 0; i < height*(frames); i+=height) {
        for (int j = i; j < (i + height-step-2); j+=step) {
            if(i != height*(frames)) {
                mesh.push_back(Polygon(&cloud[j], &cloud[j+height], &cloud[j+height+step], &cloud[j+step]));
            }
            else
                mesh.push_back(Polygon(&cloud[j], &cloud[j%height], &cloud[j%height+step], &cloud[j+step]));
        }
    }
}

Polygon::Polygon(Point3D *e1, Point3D *e2, Point3D *e3, Point3D *e4) :
    edge_1(e1),
    edge_2(e2),
    edge_3(e3),
    edge_4(e4)
{}
