#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "particula.h"
#include "obstaculo.h"
#include "caja.h"

class simulador{
private:
    std::vector<particula*> particulas;   // Lista de todas las partículas
    std::vector<obstaculo*> obstaculos;   // Lista de todos los obstáculos
    caja* cajaSimulacion;
    double dt;

    // Metodos privados

    // Actualiza la posición de todas las partículas según su velocidad */
    void actualizarTodasPosiciones();


    void detectarYResolverColisiones();

    //Detecta y resuelve colisiones entre partículas y paredes */
    void colisionesParedParticula();

    /** Detecta y resuelve colisiones entre partículas y obstáculos */
    void colisionesObstaculoParticula();

    /** Detecta y resuelve colisiones entre partículas (fusión) */
    void colisionesParticulaParticula();


    void fusionarParticulas(particula* p1, particula* p2);

    /** Elimina del vector las partículas marcadas como inactivas (fusionadas) */
    void limpiarParticulasInactivas();


    void imprimirEstado(double tiempo);

public:

    simulador(double dt, double anchoCaja, double altoCaja);

    ~simulador();

    void agregarParticula(int id, double x, double y,
                          double vx, double vy,
                          double masa, double radio);


    void agregarObstaculo(double x, double y,
                          double ancho, double alto,
                          double epsilon);

    void ejecutar(double tiempoTotal);

    std::vector<particula*> getParticulasActivas() const;

};

#endif // SIMULADOR_H
