#include "oglwidget.h"


OGLWidget::OGLWidget(QWidget *parent)
{
//    cout << "Numero de vertices primer poligono (azul): ";
//    cin >> n1;
//    if(n1 < 2)
//        n1 = 3;

//    cout << "Numero de vertices segundo poligono (plomo): ";
//    cin >> n2;
//    if(n2 < 2)
//        n2 = 3;

//    cout << "Coordenadas punto (x1, y1, x2, y2): ";
//    cin >> x1 >> y1 >> x2 >> y2;
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::drawCircle()
{
    glColor3f(0.0, 0.5, 0.0);
    glBegin(GL_POINTS);
        for(int i=0;i<1000;++i) {
            glVertex3f(cos(2*3.14159*i/1000.0),sin(2*3.14159*i/1000.0),0);
        }
    glEnd();
}

void OGLWidget::drawPolygon(int n, float r)
{
    glColor3f(0.0, 0.0, 1.0);
    float   step = 2 * 3.14159 / n,
            it_ang = 0.0,
            p0_x = 0.0,//r * cos(it_ang),
            p0_y = r,//r * sin(it_ang),
            pN_x = 0.0,
            pN_y = 0.0;

    glBegin(GL_LINES);
        for(int i=0; i<n; ++i) {
            it_ang += step;
            pN_x = r * sin(it_ang);
            pN_y = r * cos(it_ang);
            glVertex2f(p0_x, p0_y);
            glVertex2f(pN_x, pN_y);
            p0_x = pN_x;
            p0_y = pN_y;
        }
    glEnd();
}

void OGLWidget::drawLine(float p1_x, float p1_y, float p2_x, float p2_y)
{
//    int a = p2_y - p1_y,
//        b = p1_x - p2_x,
//        c = p1_y * -1 * b;
//    float b_2 = b / 2.0,
//          F = 0.0;
//    GLfloat x = p1_x,
//            y = p1_y,
//            inc = 0.00001;

    GLfloat x = p1_x,
            y = p1_y,
            dx = p2_x - p1_x,
            dy = p2_y - p1_x,
            inc_E = 2 * dy,
            inc_NE = 2 * (dy - dx),
            d = 2*dy - dx,
            inc_x = 0.00001,
            inc_y = dy * inc_x / dx;

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POINTS);
        while(x < p2_x) {
            if (d <= 0) {
                d = d + inc_E;
                x = x + inc_x;
            }
            else {
                d = d + inc_NE;
                x = x + inc_x;
                y = y + inc_y;
            }

            glVertex2f(x, y);
//            F = (a*x + b*y + c) + a + b_2;
//            y += F > 0 ? inc : 0.0;
//            x += inc;
        }

    glEnd();
    glFlush();

//    //glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(1,1,1);
////     glLoadIdentity();
//    GLfloat m=(p2_y-p1_y)/(p2_x-p1_x);
//    cout<<"esto es m"<<m<<p1_x<<p1_y<<p2_x<<p2_y<<endl;
//    glBegin(GL_POINTS);
//    GLfloat x,y;
//    for (x=p1_x; x<p2_x; x+=0.000001)
//    {
//       y=(p1_y+m*(x-p1_x));
//       glVertex2f(x,y);
//    }
//    glEnd();
    //    glFlush();
}


void OGLWidget::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/////TRIANGLE
//    glBegin(GL_TRIANGLES);
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex3f(-0.5, -0.5, 0);
//        glColor3f(0.0, 1.0, 0.0);
//        glVertex3f( 0.5, -0.5, 0);
//        glColor3f(0.0, 0.0, 1.0);
//        glVertex3f( 0.0,  0.5, 0);
//    glEnd();

///SIMPLE LINE
//    glLineWidth(2.5);
//    glColor3f(1.0, 0.0, 0.0);
//    glBegin(GL_LINES);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(15, 0, 0);
//    glEnd();


///HOMEWORK01
//    drawCircle();
//    drawPolygon(n1, 1);
//    drawLine(x1, y1, x2, y2);
//    fillPolygon(n2, 1);

///LAB01
/// 3.4.1
//    glBegin(GL_TRIANGLES);
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(2.0f, 0.0f, 0.0f);
//        glVertex3f(1.0f, 1.0f, 0.0f);

//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(-1.0f, 0.0f, 0.0f);
//        glVertex3f(-3.0f, 2.0f, 0.0f);
//        glVertex3f(-2.0f, 0.0f, 0.0f);
//    glEnd();

/// 3.4.2
    glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // activamos el color rojo
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); // activamos el color verde
        glVertex3f(1.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f); // activamos el color azul
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

//    Edge A(0, 1, 0.8, 0.2);
//    GLfloat x;
//    A.intersectsAt(0.5, x);
//    cout << "x: " << x << endl;
}
