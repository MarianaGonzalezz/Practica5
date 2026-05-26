#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <vector>
#include "obstaculo.h"
#include "vector2d.h"

class Jugador
{
public:
    Jugador(const QString& nombre, int id, double posLanzamientoX, double posLanzamientoY);

    // Getters
    QString getNombre() const;
    int getId() const;
    int getPuntaje() const;
    bool esMiTurno() const;
    vector2D getPosicionLanzamiento() const;
    std::vector<obstaculo>& getObstaculos();
    const std::vector<obstaculo>& getObstaculosConst() const;
    bool derrotado() const;

    // Setters y lógica
    void setTurno(bool turno);
    void sumarPuntaje(int puntos);
    void reiniciarPuntaje();
    void agregarObstaculo(const obstaculo& obs);
    void eliminarObstaculosDestruidos();
    void restarResistenciaObstaculo(int index, double danio);

private:
    QString nombre;
    int id;
    int puntaje;
    bool miTurno;
    vector2D posicionLanzamiento;
    std::vector<obstaculo> obstaculos;
};

#endif // JUGADOR_H
