#ifndef PCLVIEWER_H
#define PCLVIEWER_H

#include <iostream>

// Qt
#include <QMainWindow>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/ascii_io.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include <pcl/io/vtk_io.h>

#include "mesh.h"
// #include "harrisdetector.h"

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

using namespace std;

namespace Ui
{
  class PCLViewer;
}

class PCLViewer : public QMainWindow
{
  Q_OBJECT

public:
  explicit PCLViewer (QWidget *parent = 0);
  ~PCLViewer ();

public slots:
  void
  randomButtonPressed ();

  void
  RGBsliderReleased ();

  void
  pSliderValueChanged (int value);

  void
  redSliderValueChanged (int value);

  void
  greenSliderValueChanged (int value);

  void
  blueSliderValueChanged (int value);

  void
  refreshView();

  void
  loadOffFile(string filename);

  void
  fastTriangulation();

protected:
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  PointCloudT::Ptr m_cloud;
  
  Mesh mesh;

  pcl::PolygonMesh proc_mesh;

  unsigned int red;
  unsigned int green;
  unsigned int blue;

private:
  Ui::PCLViewer *ui;

};

#endif // PCLVIEWER_H
