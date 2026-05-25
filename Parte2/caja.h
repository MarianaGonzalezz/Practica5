#ifndef CAJA_H
#define CAJA_H

#include "vector2d.h"
#include "proyectil.h"

class caja{
private:
    double ancho;
    double alto;

public:
    caja(double ancho, double alto);

    double getAncho() const;
    double getAlto() const;

    bool colisionPared(const proyectil& proy) const;
    void manejarColisionPared(proyectil& proy) const;
};

#endif // CAJA_H
