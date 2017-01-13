#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

// #include <gsl/gsl_linalg.h>
// #include <gsl/gsl_blas.h>
// #include <gsl/gsl_math.h>
// #include <gsl/gsl_eigen.h>

#include <cstring>
#include <algorithm>

#include "mesh.h"

using namespace std;

class HarrisDetector {
public:
    HarrisDetector(Mesh *_mesh = 0);  //Inicializar en NULL

    void interestPoints(vector<Vertex> &interestPoints, int k = 2);

    void setMesh(Mesh *_mesh);

private:
    Mesh* m_object;

};

#endif // HARRISDETECTOR_H
