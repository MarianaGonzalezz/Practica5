#include "simulador.h"
#include <iostream>
#include <cmath>

// Constructor y Destructor

simulador::simulador(double dt, double anchoCaja, double altoCaja)
    : dt(dt), cajaSimulacion(new caja(anchoCaja, altoCaja)) {
    // La lista de inicialización ya asigna dt y crea la caja
}

simulador::~simulador() {
    // Liberar memoria de cada partícula
    for (auto p : particulas) {
        delete p;
    }

    // Liberar memoria de cada obstáculo
    for (auto o : obstaculos) {
        delete o;
    }

    // Liberar memoria de la caja
    delete cajaSimulacion;
}


// Agregar elementos

void simulador::agregarParticula(int id, double x, double y,
                                 double vx, double vy,
                                 double masa, double radio) {
    particulas.push_back(new particula(id,
                                       vector2D(x, y),
                                       vector2D(vx, vy),
                                       masa,
                                       radio));
}

void simulador::agregarObstaculo(double x, double y,
                                 double ancho, double alto,
                                 double epsilon) {
    obstaculos.push_back(new obstaculo(x, y, ancho, alto, epsilon));
}

// Bucle principal

void simulador::ejecutar(double tiempoTotal) {
    double tiempo = 0;
    int pasos = 0;

    std::cout << "=== INICIANDO SIMULACION ===" << std::endl;
    imprimirEstado(tiempo);

    while (tiempo < tiempoTotal) {
        // 1. Mover todas las partículas según su velocidad
        actualizarTodasPosiciones();

        // 2. Detectar y resolver todas las colisiones
        detectarYResolverColisiones();

        // 3. Eliminar partículas que se fusionaron
        limpiarParticulasInactivas();

        // 4. Avanzar el tiempo
        tiempo += dt;
        pasos++;

        if (pasos % 100 == 0) {
            imprimirEstado(tiempo);
        }
    }

    // Mostrar estado final
    std::cout << "=== SIMULACION FINALIZADA ===" << std::endl;
    std::cout << "Pasos totales: " << pasos << std::endl;
    imprimirEstado(tiempo);
}

// Actualizar posiciones

void simulador::actualizarTodasPosiciones() {
    for (auto p : particulas) {
        if (p->estaActiva()) {
            p->mover(dt);  // posición += velocidad * dt
        }
    }
}

// Deteccion de colisiones

void simulador::detectarYResolverColisiones() {

    for (int iter = 0; iter < 5; iter++) {
        colisionesParedParticula();      // Colisiones con paredes
        colisionesObstaculoParticula();  // Colisiones con obstáculos
        colisionesParticulaParticula();// Colisiones entre partículas
    }
}

void simulador::colisionesParedParticula() {
    for (auto p : particulas) {
        if (p->estaActiva() && cajaSimulacion->colisionPared(*p)) {
            cajaSimulacion->manejarColisionPared(*p);
        }
    }
}

void simulador::colisionesObstaculoParticula() {
    for (auto p : particulas) {
        if (!p->estaActiva()) continue;  // Saltar partículas fusionadas

        for (auto o : obstaculos) {
            o->colisionConParticula(*p);
        }
    }
}

void simulador::colisionesParticulaParticula() {
    // Recorrer todos los pares de partículas
    for (size_t i = 0; i < particulas.size(); i++) {
        if (!particulas[i]->estaActiva()) continue;

        for (size_t j = i + 1; j < particulas.size(); j++) {
            if (!particulas[j]->estaActiva()) continue;

            // Si colisionan (distancia < suma de radios), fusionar
            if (particulas[i]->colisiona(*particulas[j])) {
                fusionarParticulas(particulas[i], particulas[j]);
            }
        }
    }
}

// Fusionar particulas

void simulador::fusionarParticulas(particula* p1, particula* p2) {
    double m1 = p1->getMasa();
    double m2 = p2->getMasa();
    double masaTotal = m1 + m2;

    // 1. Calcular centro de masa: (m1*r1 + m2*r2) / (m1+m2)
    vector2D pos1 = p1->getPosicion();
    vector2D pos2 = p2->getPosicion();
    vector2D centroMasa = (pos1 * m1 + pos2 * m2) * (1.0 / masaTotal);

    // 2. Calcular nueva velocidad: (m1*v1 + m2*v2) / (m1+m2)

    vector2D vel1 = p1->getVelocidad();
    vector2D vel2 = p2->getVelocidad();
    vector2D nuevaVel = (vel1 * m1 + vel2 * m2) * (1.0 / masaTotal);

    // 3. Calcular nuevo radio conservando área total
    //    Área original: π*r1² + π*r2² = π*R²  →  R = √(r1² + r2²)
    double r1 = p1->getRadio();
    double r2 = p2->getRadio();
    double areaTotal = M_PI * r1 * r1 + M_PI * r2 * r2;
    double nuevoRadio = std::sqrt(areaTotal / M_PI);

    // 4. Actualizar p1 con los valores de la partícula fusionada
    p1->setPosicion(centroMasa);
    p1->setVelocidad(nuevaVel);
    p1->setMasa(masaTotal);
    p1->setRadio(nuevoRadio);

    // 5. Desactivar p2 (ya no existe como partícula independiente)
    p2->setActiva(false);


    std::cout << "Fusion: Particula " << p1->getId() << " + Particula " << p2->getId()
              << " -> Nueva masa: " << masaTotal << std::endl;
}

// Limpieza de particulas

void simulador::limpiarParticulasInactivas() {
    // Recorrer el vector y eliminar las partículas marcadas como inactivas
    for (auto it = particulas.begin(); it != particulas.end(); ) {
        if (!(*it)->estaActiva()) {
            delete *it;
            it = particulas.erase(it);
        } else {
            ++it;
        }
    }
}

// mostrar

void simulador::imprimirEstado(double tiempo) {
    std::cout << "t = " << tiempo << " | Particulas activas: " << particulas.size() << std::endl;

    for (auto p : particulas) {
        vector2D pos = p->getPosicion();
        vector2D vel = p->getVelocidad();

        std::cout << "  P" << p->getId()
                  << ": pos=(" << pos.getX() << ", " << pos.getY() << ")"
                  << " vel=(" << vel.getX() << ", " << vel.getY() << ")"
                  << " masa=" << p->getMasa()
                  << " radio=" << p->getRadio()
                  << std::endl;
    }
    std::cout << std::endl;
}

// Obtener particulas activas

std::vector<particula*> simulador::getParticulasActivas() const {
    std::vector<particula*> activas;

    for (auto p : particulas) {
        if (p->estaActiva()) {
            activas.push_back(p);
        }
    }
    return activas;
}




