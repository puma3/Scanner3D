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

void PointCloud::meshify()
{
    for(auto i = 0; i < height*(frames-1); i+=height) {
//        for (int j = i; j < i + height-1; j+=4) {
        for (int j = i; j < i + height-1; j++) {
//            mesh.push_back(Polygon(&cloud[j], &cloud[j+height], &cloud[j+4+height], &cloud[j+4]));
            mesh.push_back(Polygon(&cloud[j], &cloud[j+height], &cloud[j+1+height], &cloud[j+1]));
        }
    }
}

Polygon::Polygon(Point3D *e1, Point3D *e2, Point3D *e3, Point3D *e4) :
    edge_1(e1),
    edge_2(e2),
    edge_3(e3),
    edge_4(e4)
{}
