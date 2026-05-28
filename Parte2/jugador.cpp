#include "jugador.h"
#include <algorithm>

Jugador::Jugador(const QString& nombre, int id, double posX, double posY)
    : nombre(nombre), id(id), puntaje(0), miTurno(false),
    posicionLanzamiento(posX, posY) {}

QString Jugador::getNombre() const { return nombre; }
int Jugador::getId() const { return id; }
int Jugador::getPuntaje() const { return puntaje; }
bool Jugador::esMiTurno() const { return miTurno; }
vector2D Jugador::getPosicionLanzamiento() const { return posicionLanzamiento; }

std::vector<obstaculo>& Jugador::getObstaculos() { return obstaculos; }

const std::vector<obstaculo>& Jugador::getObstaculosConst() const { return obstaculos; }

bool Jugador::derrotado() const
{
    for (const auto& obs : obstaculos) {
        if (!obs.destruido())
            return false;
    }
    return true;
}

void Jugador::setTurno(bool turno)
{
    miTurno = turno;
}

void Jugador::sumarPuntaje(int puntos)
{
    puntaje += puntos;
}

void Jugador::reiniciarPuntaje()
{
    puntaje = 0;
}

void Jugador::agregarObstaculo(const obstaculo& obs)
{
    obstaculos.push_back(obs);
}

void Jugador::eliminarObstaculosDestruidos()
{
    // Recorremos desde el final hacia el principio
    for (int i = (int)obstaculos.size() - 1; i >= 0; i--) {
        if (obstaculos[i].destruido()) {
            obstaculos.erase(obstaculos.begin() + i);
        }
    }
}

void Jugador::restarResistenciaObstaculo(int index, double danio)
{
    if (index >= 0 && index < (int)obstaculos.size()) {
        obstaculos[index].recibirGolpe(danio);
        if (obstaculos[index].destruido()) {
            sumarPuntaje(10);
        }
    }
}
