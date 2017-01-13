#include "pclviewer.h"
#include <QApplication>
#include <QMainWindow>

int main (int argc, char *argv[])
{
    if(argc < 1) {
        cout << "Ingresar nombre de archivo" << endl;
        return 0;
    }
    QApplication a (argc, argv);
    PCLViewer w;
    w.loadOffFile(argv[1]);
    w.show();
    w.fastTriangulation();

    return a.exec ();
}
