#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "vector2d.h"

class obstaculo
{
private:
    vector2D posicion;
    double ancho;
    double alto;
    double resistencia;

public:
    obstaculo(vector2D posicion, double ancho, double alto, double resistencia);

    vector2D getPosicion() const;
    double getAncho() const;
    double getAlto() const;
    double getResistencia() const;

    void setPosicion(vector2D nuevaPosicion);
    void setResistencia(double nuevaResistencia);

    void recibirGolpe(double golpe);
    bool destruido() const;
};

#endif // OBSTACULO_H
