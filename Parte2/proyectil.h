#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "vector2d.h"

class proyectil
{
private:

    vector2D posicion;
    vector2D velocidad;

    double masa;
    double radio;
    bool activo;

public:

    proyectil(vector2D posicion,
              vector2D velocidad,
              double masa,
              double radio,
              bool activo);

    //Getters

    vector2D getPosicion() const;
    vector2D getVelocidad() const;
    double getMasa() const;
    double getRadio() const;
    bool estaActiva() const;

    //setters
    void setPosicion(vector2D nuevaPosicion);
    void setVelocidad(vector2D nuevaVelocidad);
    void setMasa(double nuevaMasa);
    void setRadio(double nuevoRadio);
    void setActivo(bool estado);


    void mover(double dt);
    double momentoLineal() const;
    double energiaCinetica () const;
    double calcularGolpe(double factor) const;

};

#endif // PROYECTIL_H

