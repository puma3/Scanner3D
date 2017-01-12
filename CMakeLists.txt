cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project      (pcl-visualizer)
find_package (Qt4 REQUIRED)
find_package (VTK REQUIRED)
find_package (PCL 1.7.1 REQUIRED)

include_directories (${PCL_INCLUDE_DIRS})
link_directories    (${PCL_LIBRARY_DIRS})
add_definitions     (${PCL_DEFINITIONS})

set  (project_SOURCES main.cpp mainwindow.cpp oglwidget.cpp overlaywidget.cpp pointcloud.cpp renderer.cpp scan.cpp pclviewer.cpp)
set  (project_HEADERS mainwindow.h oglwidget.h overlaywidget.h pointcloud.h renderer.h scan.h pclviewer.h)
set  (project_FORMS   mainwindow.ui renderer.ui)
set  (VTK_LIBRARIES   vtkRendering vtkGraphics vtkHybrid QVTK)

QT4_WRAP_CPP (project_HEADERS_MOC   ${project_HEADERS})
QT4_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

INCLUDE         (${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

ADD_EXECUTABLE  (scanner3D  ${project_SOURCES}
                            ${project_FORMS_HEADERS}
                            ${project_HEADERS_MOC})

TARGET_LINK_LIBRARIES (scanner3D ${QT_LIBRARIES} ${PCL_LIBRARIES} ${VTK_LIBRARIES} gsl gslcblas)
