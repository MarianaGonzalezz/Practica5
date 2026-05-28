#include "proyectil.h"
#include <cmath>

proyectil::proyectil(vector2D posicion, vector2D velocidad,
                     double masa, double radio, bool activo)
    : posicion(posicion), velocidad(velocidad), masa(masa), radio(radio), activo(activo) {}

vector2D proyectil::getPosicion() const{return posicion;}
vector2D proyectil::getVelocidad()const{return velocidad;}
double proyectil::getMasa()const{return masa;}
double proyectil::getRadio() const{ return radio;}
bool proyectil::estaActiva() const{return activo;}

void proyectil::setPosicion(vector2D nuevaPosicion) {posicion = nuevaPosicion;}
void proyectil::setVelocidad(vector2D nuevaVelocidad){velocidad = nuevaVelocidad;}
void proyectil::setMasa(double nuevaMasa){masa = nuevaMasa;}
void proyectil::setRadio(double nuevoRadio){radio = nuevoRadio;}
void proyectil::setActivo(bool estado){ activo = estado;}


void proyectil::mover(double dt){
    posicion = posicion + (velocidad*dt);
}

double proyectil::momentoLineal() const{
    return velocidad.modulo()*masa;
}

double proyectil::energiaCinetica() const {
    double rapidez = velocidad.modulo();
    return 0.5*masa*rapidez*rapidez;
}

double proyectil::calcularGolpe(double factor) const{
    return factor*masa*velocidad.modulo();
}