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
    if (pos.x - radio <= 0 || pos.x + radio >= ancho)
        return true;

    // Colisión con bordes superior o inferior
    if (pos.y - radio <= 0 || pos.y + radio >= alto)
        return true;

    return false;
}

void caja::manejarColisionPared(proyectil& proy) const
{
    vector2D pos = proy.getPosicion();
    vector2D vel = proy.getVelocidad();
    double radio = proy.getRadio();

    // Rebote en eje X (
    if (pos.x - radio <= 0) {
        vel.x = -vel.x;
        proy.setPosicion(vector2D(radio, pos.y));
    } else if (pos.x + radio >= ancho) {
        vel.x = -vel.x;
        proy.setPosicion(vector2D(ancho - radio, pos.y));
    }

    // Rebote en eje Y
    if (pos.y - radio <= 0) {
        vel.y = -vel.y;
        proy.setPosicion(vector2D(pos.x, radio));
    } else if (pos.y + radio >= alto) {
        vel.y = -vel.y;
        proy.setPosicion(vector2D(pos.x, alto - radio));
    }

    proy.setVelocidad(vel);

}
