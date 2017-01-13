#include "harrisdetector.h"

HarrisDetector::HarrisDetector(Mesh *_mesh) :
    m_object(_mesh)
{}

void HarrisDetector::setMesh(Mesh *_mesh)
{
    m_object = _mesh;
}

void HarrisDetector::interestPoints(vector<Vertex>& interestPoints, int k)
{
    cout << "\nfn: Harris 3D with k: " << k << "." << endl;

    if(!m_object) {
        cout << "m_object sin contenido" << endl;
        return;
    }

    double max = 0.0;

    for(int i = 0; i < m_object->m_nVertices; i++) {
        vector<Vertex*> _neighborhood;
        m_object->m_vertices[i].ring(k, _neighborhood, m_object->m_vertices);

        //Process "_neighborhood", first, calculate the centroid
        double xc = 0, yc = 0, zc = 0;
        vector<Vertex*>::iterator it = _neighborhood.begin(),
                                    it2 = _neighborhood.end();
        for(; it != it2; it++) {
            xc += (*it)->m_pos.x;
            yc += (*it)->m_pos.y;
            zc += (*it)->m_pos.z;
        }

        xc /= _neighborhood.size();
        yc /= _neighborhood.size();
        zc /= _neighborhood.size();

        //Translate the vertex, in order the centroid is in [0 0 0]
        for(register int j = 0; j< _neighborhood.size();j++) {
            _neighborhood[j]->m_pos.x -= xc;
            _neighborhood[j]->m_pos.y -= yc;
            _neighborhood[j]->m_pos.z -= zc;
        }

        //Aplicar PCA para encontrar una pose de la nube de puntos de manera que la mayor dispersión de los puntos esté en el plano XY
        //La media de las 3 coordenadas ya es (0, 0, 0), así que en realidad no es necesario calcularla, directamente calculamos la
        //matriz de covarianza
        double A[9];
        memset(A, 0, sizeof(double)*9);

        for(register int j = 0; j < _neighborhood.size(); j++){
            double x = _neighborhood[j]->m_pos.x;
            double y = _neighborhood[j]->m_pos.y;
            double z = _neighborhood[j]->m_pos.z;

            A[0] += x*x;	A[1] += x*y;	A[2] += x*z;
                            A[4] += y*y;	A[5] += y*z;
                                            A[8] += z*z;
        }
        A[3] = A[1];	A[6] = A[2];	A[7] = A[5];
        for(int j = 0; j < 9; j++)
            A[j] /= (_neighborhood.size()-1);

        //Con la matriz de covarianza, calculamos PCA
        gsl_matrix_view m = gsl_matrix_view_array(A, 3, 3);
        gsl_vector* eval = gsl_vector_alloc(3);
        gsl_matrix* evec = gsl_matrix_alloc(3, 3);

        gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(3);
        gsl_eigen_symmv(&m.matrix, eval, evec, w);
        gsl_eigen_symmv_free(w);

        //Ordenar autovectores decrecientemente por autovalor
        gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);

        //Sacamos las componentes del nuevo sistema de coordenadas
        double x_1 = gsl_matrix_get(evec, 0, 0);	double x_2 = gsl_matrix_get(evec, 1, 0); double x_3 = gsl_matrix_get(evec, 2, 0);
        double y_1 = gsl_matrix_get(evec, 0, 1);	double y_2 = gsl_matrix_get(evec, 1, 1); double y_3 = gsl_matrix_get(evec, 2, 1);
        double z_1 = gsl_matrix_get(evec, 0, 2);	double z_2 = gsl_matrix_get(evec, 1, 2); double z_3 = gsl_matrix_get(evec, 2, 2);

        double x2 = _neighborhood[0]->m_pos.x - xc;
        double y2 = _neighborhood[0]->m_pos.y - yc;
        double z2 = _neighborhood[0]->m_pos.z - zc;

        if((z_1*x2 + z_2*y2 + z_3*z2) < 0){
            z_1 = -z_1;
            z_2 = -z_2;
            z_3 = -z_3;

            double aux_x1 = x_1;
            double aux_x2 = x_2;
            double aux_x3 = x_3;

            x_1 = y_1;
            x_2 = y_2;
            x_3 = y_3;
            y_1 = aux_x1;
            y_2 = aux_x2;
            y_3 = aux_x3;
        }

        //Realizamos la rotacion, con el nuevo sistema de coordenadas
        for(register int j = 0; j < _neighborhood.size(); j++){
            double x = _neighborhood[j]->m_pos.x;
            double y = _neighborhood[j]->m_pos.y;
            double z = _neighborhood[j]->m_pos.z;

            _neighborhood[j]->m_pos.x = x_1*x + x_2*y + x_3*z;
            _neighborhood[j]->m_pos.y = y_1*x + y_2*y + y_3*z;
            _neighborhood[j]->m_pos.z = z_1*x + z_2*y + z_3*z;
        }

        //Movemos todos los puntos para que el punto de analisis se encuentre en el origen del plano XY. Solo movemos en las coordenadas X e Y
        double x = _neighborhood[0]->m_pos.x;
        double y = _neighborhood[0]->m_pos.y;

        for(register int j = 0; j < _neighborhood.size(); j++){
            _neighborhood[j]->m_pos.x = _neighborhood[j]->m_pos.x - x;
            _neighborhood[j]->m_pos.y = _neighborhood[j]->m_pos.y - y;
        }

        //Fit a quadratic surface
        double C[36];
        double D[6];

        memset(C, 0, sizeof(double)*36);
        memset(D, 0, sizeof(double)*6);

        for(register int j = 0; j < _neighborhood.size(); j++){
            double x = _neighborhood[j]->m_pos.x;
            double y = _neighborhood[j]->m_pos.y;
            double z = _neighborhood[j]->m_pos.z;

            double x2 = x*x;
            double y2 = y*y;
            double x3 = x2*x;
            double y3 = y2*y;

            C[0] += x*x3;	C[1] += x3*y;	C[2] += x2*y2;	C[3] += x3;		C[4] += x2*y;	C[5] += x2;
                            C[7] += x2*y2;	C[8] += x*y3;	C[9] += x2*y;	C[10] += x*y2;	C[11] += x*y;
                                            C[14] += y*y3;	C[15] += x*y2;	C[16] += y3;	C[17] += y2;
                                                            C[21] += x2;	C[22] += x*y;	C[23] += x;
                                                                            C[28] += y2;	C[29] += y;
            D[0] += z*x2;	D[1] += z*x*y;	D[2] += z*y2;	D[3] += z*x;	D[4] += z*y;	D[5] += z;
        }

        C[6] = C[1];
        C[12] = C[2];	C[13] = C[8];
        C[18] = C[3];	C[19] = C[9];	C[20] = C[15];
        C[24] = C[4];	C[25] = C[10];	C[26] = C[16];	C[27] = C[22];
        C[30] = C[5];	C[31] = C[11];	C[32] = C[17];	C[33] = C[23];	C[34] = C[29];

        C[35] = (double)_neighborhood.size();

        //Using GSL for solve linear system
        gsl_matrix_view m1 = gsl_matrix_view_array(C, 6, 6);
        gsl_vector_view b1 = gsl_vector_view_array(D, 6);
        gsl_vector *x1 = gsl_vector_alloc(6);

        int s1;

        gsl_permutation *p11 = gsl_permutation_alloc(6);
        gsl_linalg_LU_decomp(&m1.matrix, p11, &s1);
        gsl_linalg_LU_solve(&m1.matrix, p11, &b1.vector, x1);

        //Extract solution of quadratic surface
        double c20_2 = gsl_vector_get(x1, 0);
        double c11 = gsl_vector_get(x1, 1);
        double c02_2 = gsl_vector_get(x1, 2);
        double c10 = gsl_vector_get(x1, 3);
        double c01 = gsl_vector_get(x1, 4);
        double c0 = gsl_vector_get(x1, 5);

        double c20 = c20_2*2;
        double c02 = c02_2*2;

        double fx2 = c10*c10 + 2*c20*c20 + 2*c11*c11; //A
        double fy2 = c10*c10 + 2*c11*c11 + 2*c02*c02; //B
        double fxfy = c10*c01 + 2*c20*c11 + 2*c11*c02; //C

        //double k = 0.04;
        double resp = fx2*fy2 - fxfy*fxfy - k*(fx2 + fy2)*(fx2 + fy2);

        m_object->m_vertices[i].harrisResponse = resp;

        if(resp > max)
            max = resp;

        gsl_vector_free(x1);
        gsl_vector_free(eval);
        gsl_matrix_free(evec);
        gsl_permutation_free(p11);

        for(register int j = 0; j < _neighborhood.size(); j++)
            delete _neighborhood[j];
        _neighborhood.clear();
    }

    vector<Vertex> candidatePoints;

    //Search for local maximum
    for(register int i = 0; i< m_object->m_nVertices; i++){
        m_object->m_vertices[i].defineInterest(m_object->m_vertices);
        if(m_object->m_vertices[i].isInterestPoint){
            candidatePoints.push_back(m_object->m_vertices[i]);
        }
    }

    cout << "Candidates calculated ..." << endl;

    sort(candidatePoints.begin(), candidatePoints.end(), comp); ///SE PUEDE UTILIZAR UNA FUNCIÓN OBJETO

    //Seleccionar los puntos de mayor respuesta
    int numPoints = // paramSelection * m_object->m_nNodes;
                    0.2 * m_object->m_nVertices;
    //int numPoints = candidatePoints.size();
    for(register int i = 0; i < numPoints; i++)
        interestPoints.push_back(candidatePoints[i]);

//    //Aplicar proceso de Clustering
//    for(int i = 0; i < candidatePoints.size(); i++){
//        bool found = false;
//        int j = 0;
//        while(j < interestPoints.size() && !found){
//            double distX = interestPoints[j].m_pos.x - candidatePoints[i].m_pos.x;
//            double distY = interestPoints[j].m_pos.y - candidatePoints[i].m_pos.y;
//            double distZ = interestPoints[j].m_pos.z - candidatePoints[i].m_pos.z;

//            if(sqrt(distX*distX + distY*distY + distZ*distZ) < 5)
//                found = true;
//            j++;
//        }
//        if(!found)
//            interestPoints.push_back(candidatePoints[i]);
//    }

    ///////////////////////FIN DEL CODIGO//////////////////////////

    cout << "End of Harris Interest 3D Points :)" << endl;
}
