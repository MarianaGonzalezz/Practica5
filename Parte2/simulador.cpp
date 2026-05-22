#include "simulador.h"
#include <cmath>

simulador::simulador(double anchoCaja, double altoCaja, double dt)
    : cajaJuego(anchoCaja, altoCaja),
    proyectilActual(nullptr), turnoActual(1),
    dt(dt), factorGolpe(10) {}

simulador::~simulador(){delete proyectilActual;}

void simulador::configurarJugadores(jugador j1, jugador j2){
    jugador1 = j1;
    jugador2 = j2;
}

void simulador::lanzarProyectil(double angulo, double velocidadInicial){

    double rad = angulo*(M_PI/180.0); //grados a radianes
    double vx = velocidadInicial*cos(rad);
    double vy = velocidadInicial*sin(rad);

    vector2D velocidad(vx, vy);

    vector2D origen;

    if (turnoActual==1){
        origen = vector2D(50, 300);
    } else{
        origen = vector2D(750,300);
    }

    delete proyectilActual;

    proyectilActual = new proyectil(origen, velocidad, 1.0, 5);
}

void simulador::actualizar(){
        if(proyectilActual == nullptr){
            return;
        }
        actualizarMovimiento();
        detectarColisiones();
}

void simulador::actualizarMovimiento(){
        proyectilActual -> mover(dt);
}

void simulador::detectarColisiones(){
    //caja
    if(cajaJuego.colisionPared(*proyectilActual)){
            cajaJuego.manejarColisionPared(*proyectilActual);
    }

    //obstaculos
    if(colisionConObstaculos()){
        proyectilActual->setActivo(false);
        cambiarTurno();
    }
}

bool simulador::colisionConObstaculos(){

    jugador& enemigo = (turnoActual == 1) ? jugador2:jugador1;

    for(auto& obstaculo : enemigo.getObstaculos()){
        if(obstaculo.colisionConProyectil(*proyectilActual)){
            double golpe = proyectilActual ->calcularGolpe(factorGolpe);

            obstaculo.recibirGolpe(golpe);
            return true;
        }
    }
    return false;
}

void simulador::cambiarTurno(){
    turnoActual = (turnoActual == 1) ? 2:1;

}

bool simulador::juegoTerminado() const{
    return jugador1.derrotado() || jugador2.derrotado();
}

int simulador::obtenerGanador() const{

    if(jugador1.derrotado()){return 2;}

    if(jugador2.derrotado()){return 1;}

    return 0;
}

int simulador::getTurno() const{
    return turnoActual;
}