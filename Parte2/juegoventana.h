#ifndef JUEGOVENTANA_H
#define JUEGOVENTANA_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QMenuBar>
#include <QVBoxLayout>

class JuegoVentana : public QWidget
{
    Q_OBJECT

public:
    JuegoVentana(QWidget *parent = nullptr);
    ~JuegoVentana();

private slots:
    void lanzarProyectil();
    void actualizarSimulacion();
    void nuevaPartida();
    void mostrarInstrucciones();
    void acercaDe();
    void salirJuego();

private:
    QGraphicsView *vista;
    QGraphicsScene *escena;
    QTimer *timer;

    QFrame *frameJ1;
    QLabel *labelRivalJ1;
    QLineEdit *entradaAnguloJ1;
    QLineEdit *entradaVelocidadJ1;
    QPushButton *botonLanzarJ1;

    QFrame *frameJ2;
    QLabel *labelRivalJ2;
    QLineEdit *entradaAnguloJ2;
    QLineEdit *entradaVelocidadJ2;
    QPushButton *botonLanzarJ2;

    QPushButton *botonNuevaPartida;

    bool proyectilActivo;
    int turnoActual;

    void configurarEscenario();
    void crearControles();
    void crearMenu();
    void habilitarControles(bool habilitado);
};

#endif
