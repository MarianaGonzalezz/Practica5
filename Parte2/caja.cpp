#include "caja.h"
#include <cmath>

caja::caja(double ancho, double alto) : ancho(ancho), alto(alto) {}

// Getters

double caja::getAncho() const { return ancho; }
double caja::getAlto() const { return alto; }

bool caja::colisionPared(const proyectil& proy) const
{
    vector2D pos = proy.getPosicion();
    double radio = proy.getRadio();

    // Colisión con bordes izquierdo o derecho
    if (pos.getX() - radio <= 0 || pos.getX() + radio >= ancho)
        return true;

    // Colisión con bordes superior o inferior
    if (pos.getY() - radio <= 0 || pos.getY() + radio >= alto)
        return true;

    return false;
}

void caja::manejarColisionPared(proyectil& proy) const
{
    vector2D pos = proy.getPosicion();
    vector2D vel = proy.getVelocidad();
    double radio = proy.getRadio();

    // Rebote en eje X (
    if (pos.getX() - radio <= 0) {
        vel.setX(-vel.getX());
        proy.setPosicion(vector2D(radio, pos.getY()));
    } else if (pos.getX() + radio >= ancho) {
        vel.setX(-vel.getX());
        proy.setPosicion(vector2D(ancho - radio, pos.getY()));
    }

    // Rebote en eje Y
    if (pos.getY() - radio <= 0) {
        vel.setY(-vel.getY());
        proy.setPosicion(vector2D(pos.getX(), radio));
    } else if (pos.getY() + radio >= alto) {
        vel.setY(-vel.getY());
        proy.setPosicion(vector2D(pos.getX(), alto - radio));
    }

    proy.setVelocidad(vel);

}
