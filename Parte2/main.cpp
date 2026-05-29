#include "juegoventana.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JuegoVentana ventana;
    ventana.show();
    return a.exec();
}
