#include "pclviewer.h"
#include "../build/ui_pclviewer.h"

PCLViewer::PCLViewer (QWidget *parent) :
  QMainWindow (parent),
  ui (new Ui::PCLViewer)
{
  ui->setupUi (this);
  this->setWindowTitle ("Mesher");

  // Setup the cloud pointer
  m_cloud.reset (new PointCloudT);
  // The number of points in the cloud
//  m_cloud->points.resize (200);

  // The default color
  red   = 150;
  green = 150;
  blue  = 172;

  // Set up the QVTK window
  viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
  ui->qvtkWidget->SetRenderWindow (viewer->getRenderWindow ());
  viewer->setupInteractor (ui->qvtkWidget->GetInteractor (), ui->qvtkWidget->GetRenderWindow ());
  ui->qvtkWidget->update ();

  // Connect point size slider
  connect (ui->horizontalSlider_p, SIGNAL (valueChanged (int)), this, SLOT (pSliderValueChanged (int)));

  viewer->addPointCloud (m_cloud, "cloud");
  pSliderValueChanged (2);
  viewer->resetCamera ();
  ui->qvtkWidget->update ();
}

void
PCLViewer::randomButtonPressed ()
{
  printf ("Random button was pressed\n");

  // Set the new color
  for (size_t i = 0; i < m_cloud->size(); i++)
  {
    m_cloud->points[i].r = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
    m_cloud->points[i].g = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
    m_cloud->points[i].b = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
  }

  viewer->updatePointCloud (m_cloud, "cloud");
  ui->qvtkWidget->update ();
}

void
PCLViewer::RGBsliderReleased ()
{
  // Set the new color
  for (size_t i = 0; i < m_cloud->size (); i++)
  {
    m_cloud->points[i].r = red;
    m_cloud->points[i].g = green;
    m_cloud->points[i].b = blue;
  }
  viewer->updatePointCloud (m_cloud, "cloud");
  ui->qvtkWidget->update ();
}

void
PCLViewer::pSliderValueChanged (int value)
{
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, value, "cloud");
  ui->qvtkWidget->update ();
}

void
PCLViewer::redSliderValueChanged (int value)
{
  red = value;
  printf ("redSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
}

void
PCLViewer::greenSliderValueChanged (int value)
{
  green = value;
  printf ("greenSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
}

void
PCLViewer::blueSliderValueChanged (int value)
{
  blue = value;
  printf("blueSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
}

void PCLViewer::refreshView()
{
    // Set up the QVTK window
//    viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
//    ui->qvtkWidget->SetRenderWindow (viewer->getRenderWindow ());
//    viewer->setupInteractor (ui->qvtkWidget->GetInteractor (), ui->qvtkWidget->GetRenderWindow ());
//    ui->qvtkWidget->update ();

    //Final Drawing
    viewer->updatePointCloud (m_cloud, "cloud");
    ui->qvtkWidget->update ();
    viewer->resetCamera ();
}


void PCLViewer::loadOffFile(string filename)
{
    mesh.loadOffFile(filename);

    m_cloud->points.resize(mesh.m_nVertices);
    for (register size_t i = 0; i < m_cloud->points.size (); ++i)
    {
        m_cloud->points[i].x = mesh.m_vertices[i].m_pos.x;
        m_cloud->points[i].y = mesh.m_vertices[i].m_pos.y;
        m_cloud->points[i].z = mesh.m_vertices[i].m_pos.z;

        m_cloud->points[i].r = red;
        m_cloud->points[i].g = green;
        m_cloud->points[i].b = blue;
    }

    refreshView();
}

void PCLViewer::fastTriangulation()
{
    // Load input file into a PointCloud<T> with an appropriate type
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

    cloud->points.resize(m_cloud->size());
    for (size_t i = 0; i < m_cloud->points.size(); i++) {
        cloud->points[i].x = m_cloud->points[i].x;
        cloud->points[i].y = m_cloud->points[i].y;
        cloud->points[i].z = m_cloud->points[i].z;
    }

    // Normal estimation*
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (cloud);
    n.setInputCloud (cloud);
    n.setSearchMethod (tree);
    n.setKSearch (20);
    n.compute (*normals);
    //* normals should not contain the point normals + surface curvatures

    // Concatenate the XYZ and normal fields*
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields (*cloud, *normals, *cloud_with_normals);
    //* cloud_with_normals = cloud + normals

    // Create search tree*
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud (cloud_with_normals);

    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    pcl::PolygonMesh triangles;

    // Set the maximum distance between connected points (maximum edge length)
    // gp3.setSearchRadius (0.025);
    gp3.setSearchRadius (50.5);

    // // Set typical values for the parameters
    // gp3.setMu (4.5);
    // gp3.setMaximumNearestNeighbors (100);
    // gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
    // gp3.setMinimumAngle(M_PI/18); // 10 degrees
    // gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
    // gp3.setNormalConsistency(false);

    // Set typical values for the parameters
    gp3.setMu (7.5);
    gp3.setMaximumNearestNeighbors (500);
    gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
    gp3.setMinimumAngle(M_PI/18); // 10 degrees
    gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
    gp3.setNormalConsistency(false);

    // Get result
    gp3.setInputCloud (cloud_with_normals);
    gp3.setSearchMethod (tree2);
    gp3.reconstruct (triangles);

    // Additional vertex information
    std::vector<int> parts = gp3.getPartIDs();
    std::vector<int> states = gp3.getPointStates();

    pcl::io::saveVTKFile ("mesh.vtk", triangles);

    pcl::io::loadPolygonFileVTK("mesh.vtk", proc_mesh);
    viewer->addPolygonMesh (proc_mesh, "mesh");

    refreshView();
}

PCLViewer::~PCLViewer ()
{
  delete ui;
}
