#include "pclviewer.h"
#include "../build/ui_pclviewer.h"

PCLViewer::PCLViewer (QWidget *parent) :
  QMainWindow (parent),
  ui (new Ui::PCLViewer)
{
  ui->setupUi (this);
  this->setWindowTitle ("PCL viewer");

  // Setup the cloud pointer
  cloud.reset (new PointCloudT);
  // The number of points in the cloud
//  cloud->points.resize (200);

  // The default color
  red   = 128;
  green = 128;
  blue  = 128;

  // Fill the cloud with some points
//  for (size_t i = 0; i < cloud->points.size (); ++i)
//  {
//    cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
//    cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
//    cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);

//    cloud->points[i].r = red;
//    cloud->points[i].g = green;
//    cloud->points[i].b = blue;
//  }

  // Set up the QVTK window
  viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
  ui->qvtkWidget->SetRenderWindow (viewer->getRenderWindow ());
  viewer->setupInteractor (ui->qvtkWidget->GetInteractor (), ui->qvtkWidget->GetRenderWindow ());
  ui->qvtkWidget->update ();

  // Connect "random" button and the function
  connect (ui->pushButton_random,  SIGNAL (clicked ()), this, SLOT (randomButtonPressed ()));

  // Connect R,G,B sliders and their functions
  connect (ui->horizontalSlider_R, SIGNAL (valueChanged (int)), this, SLOT (redSliderValueChanged (int)));
  connect (ui->horizontalSlider_G, SIGNAL (valueChanged (int)), this, SLOT (greenSliderValueChanged (int)));
  connect (ui->horizontalSlider_B, SIGNAL (valueChanged (int)), this, SLOT (blueSliderValueChanged (int)));
  connect (ui->horizontalSlider_R, SIGNAL (sliderReleased ()), this, SLOT (RGBsliderReleased ()));
  connect (ui->horizontalSlider_G, SIGNAL (sliderReleased ()), this, SLOT (RGBsliderReleased ()));
  connect (ui->horizontalSlider_B, SIGNAL (sliderReleased ()), this, SLOT (RGBsliderReleased ()));

  // Connect point size slider
  connect (ui->horizontalSlider_p, SIGNAL (valueChanged (int)), this, SLOT (pSliderValueChanged (int)));

  viewer->addPointCloud (cloud, "cloud");
  pSliderValueChanged (2);
  viewer->resetCamera ();
  ui->qvtkWidget->update ();
}

void
PCLViewer::randomButtonPressed ()
{
  printf ("Random button was pressed\n");

  // Set the new color
  for (size_t i = 0; i < cloud->size(); i++)
  {
    cloud->points[i].r = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
    cloud->points[i].g = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
    cloud->points[i].b = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
  }

  viewer->updatePointCloud (cloud, "cloud");
  ui->qvtkWidget->update ();
}

void
PCLViewer::RGBsliderReleased ()
{
  // Set the new color
  for (size_t i = 0; i < cloud->size (); i++)
  {
    cloud->points[i].r = red;
    cloud->points[i].g = green;
    cloud->points[i].b = blue;
  }
  viewer->updatePointCloud (cloud, "cloud");
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
    viewer->updatePointCloud (cloud, "cloud");
    ui->qvtkWidget->update ();
    viewer->resetCamera ();
}


void PCLViewer::loadOffFile(string filename)
{
//    cout << "\nWill load " << filename << " now!_______________" << endl;

//    ifstream _file;
//    _file.open(filename.c_str()/*, ios::in*/);

//    string _format;
//    int register _nNodes, _nTriangles, i, _nVertices;
//    _file >> _format >> _nNodes >> _nTriangles >> i;

//    if(_format.compare("OFF") != 0) {
//        cout << "Not an OFF file" << endl;
//        return;
//    }

//    // Setup the cloud pointer
//    cloud.reset (new PointCloudT);
//    // The number of points in the cloud
//    cloud->points.resize (_nNodes);
//    mesh.resize(_nNodes);

//    // Fill the cloud with the model's points
//    for(i = 0; i < _nNodes; i++) {
//        _file   >> cloud->points[i].x
//                >> cloud->points[i].y
//                >> cloud->points[i].z;

//        cloud->points[i].r = red;
//        cloud->points[i].g = green;
//        cloud->points[i].b = blue;
//    }

//    int _indx1, _indx2, _indx3;

//    for(i = 0; i < _nTriangles; i++) {
//        _file >> _nVertices     //Grabs the 'n' number of nodes in a face
//                >> _indx1
//                >> _indx2
//                >> _indx3;

//        if(_nVertices != 3)
//            break;

//        _indx1;
//        _indx2;
//        _indx3;   //-1 due the minimum index of a node is 1

//        mesh.add_edge(_indx1, _indx2);
//        mesh.add_edge(_indx1, _indx3);
//        mesh.add_edge(_indx2, _indx1);
//        mesh.add_edge(_indx2, _indx3);
//        mesh.add_edge(_indx3, _indx1);
//        mesh.add_edge(_indx3, _indx2);
//    }

    mesh.loadOffFile(filename);

    cloud->points.resize(mesh.m_nVertices);
    for (register size_t i = 0; i < cloud->points.size (); ++i)
    {
        cloud->points[i].x = mesh.m_vertices[i].m_pos.x;
        cloud->points[i].y = mesh.m_vertices[i].m_pos.y;
        cloud->points[i].z = mesh.m_vertices[i].m_pos.z;

        cloud->points[i].r = red;
        cloud->points[i].g = green;
        cloud->points[i].b = blue;
    }

//    _file.close();
//    cout << "\nFinished loading file!____________________" << endl;

    refreshView();
}

void PCLViewer::Harris3D()
{
//    cout << "Entro a fn Harris 3D" << endl;

//    if(cloud->size() == 0)
//        return;

    /////////////////////////////////////////////////////////////////
    HarrisDetector h3d;
    h3d.setMesh(&mesh);
    vector<Vertex> interestPoints;
    h3d.interestPoints(interestPoints, 7);

    vector<Vertex>::iterator    it = interestPoints.begin(),
                                it2 = interestPoints.end();


    cout << "Interest Points: --------------------------------->" << endl;
    for(; it != it2; it++) {
        cout << "Key " << it->m_pos << "\tIndex: " << it->index << "\tResponse: " << it->harrisResponse << endl;
        cloud->points[it->index].r = 255;
        cloud->points[it->index].g = 0;
        cloud->points[it->index].b = 10;
    }

    cout << "End of Interest Points----------------------------/" << endl;

    refreshView();

//    cout << "Saliendo de fn Harris 3D" << endl;
}

PCLViewer::~PCLViewer ()
{
  delete ui;
}
