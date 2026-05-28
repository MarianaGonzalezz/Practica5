#include "obstaculo.h"

obstaculo::obstaculo(vector2D posicion, double ancho, double alto, double resistencia)
    : posicion(posicion), ancho(ancho), alto(alto), resistencia(resistencia) {}

vector2D obstaculo::getPosicion() const{ return posicion;}
double obstaculo::getAncho() const{return ancho;}
double obstaculo::getAlto() const{return alto;}
double obstaculo::getResistencia() const {return resistencia;}

void obstaculo::setPosicion(vector2D nuevaPosicion){posicion = nuevaPosicion;}
void obstaculo::setResistencia(double nuevaResistencia) {resistencia = nuevaResistencia;}

void obstaculo::recibirGolpe(double golpe){
    resistencia -= golpe;

    if (resistencia < 0){
        resistencia =0;
    }
}

bool obstaculo::destruido() const{
    return resistencia <= 0;
}