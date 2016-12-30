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
