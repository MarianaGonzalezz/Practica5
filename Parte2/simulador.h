#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "jugador.h"
#include "proyectil.h"
#include "caja.h"

class simulador
{
private:
    jugador jugador1;
    jugador jugador2;

    caja cajaJuego;

    proyectil* proyectilActual;

    int turnoActual;
    double dt;
    double factorGolpe;


    void actualizarMovimiento();
    void detectarColisiones();
    void cambiarTurno();
    bool colisionConObstaculos();

public:
    simulador(double anchoCaja, double altoCaja, double dt);
    ~simulador();

    void configurarJugadores (jugador j1, jugador j2);

    void lanzarProyectil(double angulo, double velocidadInicial);
    void actualizar();
    bool juegoTerminado() const;
    int obtenerGanador() const;
    int getTurno() const;
};

#endif // SIMULADOR_H
