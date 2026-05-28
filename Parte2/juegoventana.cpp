#include "juegoventana.h"
#include <QGraphicsTextItem>
#include <cmath>

JuegoVentana::JuegoVentana(QWidget *parent)
    : QWidget(parent)
    , proyectilActivo(false)
    , turnoActual(1)
{
    // Configuración básica
    setWindowTitle("Juego de Artillería");
    setFixedSize(1200, 700);
    setStyleSheet("background-color: #2c3e50;");

    // ============================================================
    // MENÚ
    // ============================================================
    crearMenu();

    // ============================================================
    // ÁREA DE JUEGO
    // ============================================================
    vista = new QGraphicsView();
    escena = new QGraphicsScene();
    vista->setScene(escena);
    vista->setParent(this);
    vista->setGeometry(50, 50, 900, 550);
    vista->setStyleSheet("border: 3px solid #ecf0f1; background-color: #ecf0f1; border-radius: 5px;");

    // ============================================================
    // CONTROLES
    // ============================================================
    crearControles();

    // ============================================================
    // TEMPORIZADOR
    // ============================================================
    timer = new QTimer();
    timer->setParent(this);
    connect(timer, &QTimer::timeout, this, &JuegoVentana::actualizarSimulacion);
    timer->start(100);

    configurarEscenario();
    habilitarControles(true);
}

JuegoVentana::~JuegoVentana()
{
    delete timer;
    delete vista;
    delete escena;
}

void JuegoVentana::crearMenu()
{
    QMenuBar *menuBar = new QMenuBar();
    menuBar->setParent(this);

    QMenu *menuJuego = menuBar->addMenu("Juego");
    QMenu *menuAyuda = menuBar->addMenu("Ayuda");

    QAction *actionNuevaPartida = menuJuego->addAction("Nueva Partida");
    actionNuevaPartida->setShortcut(QKeySequence("Ctrl+N"));
    QAction *actionSalir = menuJuego->addAction("Salir");
    actionSalir->setShortcut(QKeySequence("Ctrl+Q"));
    QAction *actionInstrucciones = menuAyuda->addAction("Instrucciones");
    actionInstrucciones->setShortcut(QKeySequence("Ctrl+I"));
    QAction *actionAcercaDe = menuAyuda->addAction("Acerca de");

    connect(actionNuevaPartida, &QAction::triggered, this, &JuegoVentana::nuevaPartida);
    connect(actionSalir, &QAction::triggered, this, &JuegoVentana::salirJuego);
    connect(actionInstrucciones, &QAction::triggered, this, &JuegoVentana::mostrarInstrucciones);
    connect(actionAcercaDe, &QAction::triggered, this, &JuegoVentana::acercaDe);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMenuBar(menuBar);
    setLayout(layout);
}

void JuegoVentana::crearControles()
{
    // ==================== CAÑÓN IZQUIERDO ====================
    frameJ1 = new QFrame();
    frameJ1->setParent(this);
    frameJ1->setGeometry(50, 610, 250, 80);
    frameJ1->setStyleSheet("background-color: #34495e; border-radius: 10px;");

    labelRivalJ1 = new QLabel();
    labelRivalJ1->setParent(frameJ1);
    labelRivalJ1->setGeometry(80, 5, 90, 25);
    labelRivalJ1->setText("RIVAL");
    labelRivalJ1->setStyleSheet("color: #e74c3c; font-size: 14px; font-weight: bold;");
    labelRivalJ1->setAlignment(Qt::AlignCenter);

    QLabel *lblAngulo1 = new QLabel();
    lblAngulo1->setParent(frameJ1);
    lblAngulo1->setGeometry(15, 30, 60, 20);
    lblAngulo1->setText("ÁNGULO");
    lblAngulo1->setStyleSheet("color: white; font-size: 10px;");

    entradaAnguloJ1 = new QLineEdit();
    entradaAnguloJ1->setParent(frameJ1);
    entradaAnguloJ1->setGeometry(15, 48, 80, 25);
    entradaAnguloJ1->setText("100");
    entradaAnguloJ1->setStyleSheet("font-size: 12px; border: 2px solid #e74c3c; border-radius: 5px; background-color: white;");
    entradaAnguloJ1->setAlignment(Qt::AlignCenter);

    QLabel *lblVel1 = new QLabel();
    lblVel1->setParent(frameJ1);
    lblVel1->setGeometry(105, 30, 70, 20);
    lblVel1->setText("VELOCIDAD");
    lblVel1->setStyleSheet("color: white; font-size: 10px;");

    entradaVelocidadJ1 = new QLineEdit();
    entradaVelocidadJ1->setParent(frameJ1);
    entradaVelocidadJ1->setGeometry(105, 48, 80, 25);
    entradaVelocidadJ1->setText("200");
    entradaVelocidadJ1->setStyleSheet("font-size: 12px; border: 2px solid #e74c3c; border-radius: 5px; background-color: white;");
    entradaVelocidadJ1->setAlignment(Qt::AlignCenter);

    botonLanzarJ1 = new QPushButton();
    botonLanzarJ1->setParent(frameJ1);
    botonLanzarJ1->setGeometry(195, 48, 50, 25);
    botonLanzarJ1->setText("LANZAR");
    botonLanzarJ1->setStyleSheet("background-color: #e74c3c; color: white; font-weight: bold; border-radius: 3px;");
    connect(botonLanzarJ1, &QPushButton::clicked, [this]() {
        if (turnoActual == 1 && !proyectilActivo) lanzarProyectil();
    });

    // ==================== CAÑÓN DERECHO ====================
    frameJ2 = new QFrame();
    frameJ2->setParent(this);
    frameJ2->setGeometry(900, 610, 250, 80);
    frameJ2->setStyleSheet("background-color: #34495e; border-radius: 10px;");

    labelRivalJ2 = new QLabel();
    labelRivalJ2->setParent(frameJ2);
    labelRivalJ2->setGeometry(80, 5, 90, 25);
    labelRivalJ2->setText("RIVAL");
    labelRivalJ2->setStyleSheet("color: #3498db; font-size: 14px; font-weight: bold;");
    labelRivalJ2->setAlignment(Qt::AlignCenter);

    QLabel *lblAngulo2 = new QLabel();
    lblAngulo2->setParent(frameJ2);
    lblAngulo2->setGeometry(165, 30, 60, 20);
    lblAngulo2->setText("ÁNGULO");
    lblAngulo2->setStyleSheet("color: white; font-size: 10px;");

    entradaAnguloJ2 = new QLineEdit();
    entradaAnguloJ2->setParent(frameJ2);
    entradaAnguloJ2->setGeometry(165, 48, 80, 25);
    entradaAnguloJ2->setText("100");
    entradaAnguloJ2->setStyleSheet("font-size: 12px; border: 2px solid #3498db; border-radius: 5px; background-color: white;");
    entradaAnguloJ2->setAlignment(Qt::AlignCenter);

    QLabel *lblVel2 = new QLabel();
    lblVel2->setParent(frameJ2);
    lblVel2->setGeometry(15, 30, 70, 20);
    lblVel2->setText("VELOCIDAD");
    lblVel2->setStyleSheet("color: white; font-size: 10px;");

    entradaVelocidadJ2 = new QLineEdit();
    entradaVelocidadJ2->setParent(frameJ2);
    entradaVelocidadJ2->setGeometry(15, 48, 80, 25);
    entradaVelocidadJ2->setText("200");
    entradaVelocidadJ2->setStyleSheet("font-size: 12px; border: 2px solid #3498db; border-radius: 5px; background-color: white;");
    entradaVelocidadJ2->setAlignment(Qt::AlignCenter);

    botonLanzarJ2 = new QPushButton();
    botonLanzarJ2->setParent(frameJ2);
    botonLanzarJ2->setGeometry(5, 48, 50, 25);
    botonLanzarJ2->setText("LANZAR");
    botonLanzarJ2->setStyleSheet("background-color: #3498db; color: white; font-weight: bold; border-radius: 3px;");
    connect(botonLanzarJ2, &QPushButton::clicked, [this]() {
        if (turnoActual == 2 && !proyectilActivo) lanzarProyectil();
    });

    // Botón Nueva Partida
    botonNuevaPartida = new QPushButton();
    botonNuevaPartida->setParent(this);
    botonNuevaPartida->setGeometry(500, 640, 200, 35);
    botonNuevaPartida->setText("NUEVA PARTIDA");
    botonNuevaPartida->setStyleSheet("background-color: #f39c12; color: white; font-weight: bold; font-size: 14px; border-radius: 5px");
    connect(botonNuevaPartida, &QPushButton::clicked, this, &JuegoVentana::nuevaPartida);
}

void JuegoVentana::configurarEscenario()
{
    if (!escena) return;
    escena->clear();

    escena->setBackgroundBrush(QBrush(QColor(236, 240, 241)));
    escena->addRect(0, 530, 900, 20, QPen(Qt::NoPen), QBrush(QColor(139, 69, 19)));
    escena->addRect(0, 0, 900, 550, QPen(Qt::black, 3), QBrush(Qt::NoBrush));
    escena->addLine(450, 0, 450, 550, QPen(Qt::gray, 2, Qt::DashLine));

    // Cañón izquierdo
    escena->addRect(20, 510, 50, 20, QPen(Qt::black, 2), QBrush(QColor(100, 100, 100)));
    escena->addEllipse(25, 525, 12, 12, QPen(Qt::black, 2), QBrush(QColor(80, 80, 80)));
    escena->addEllipse(50, 525, 12, 12, QPen(Qt::black, 2), QBrush(QColor(80, 80, 80)));

    double angulo = entradaAnguloJ1->text().toDouble();
    double rad = angulo * M_PI / 180.0;
    int tuboX = 45 + 35 * cos(rad);
    int tuboY = 505 - 35 * sin(rad);
    escena->addLine(45, 505, tuboX, tuboY, QPen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap));

    // Cañón derecho
    escena->addRect(830, 510, 50, 20, QPen(Qt::black, 2), QBrush(QColor(100, 100, 100)));
    escena->addEllipse(835, 525, 12, 12, QPen(Qt::black, 2), QBrush(QColor(80, 80, 80)));
    escena->addEllipse(860, 525, 12, 12, QPen(Qt::black, 2), QBrush(QColor(80, 80, 80)));

    angulo = entradaAnguloJ2->text().toDouble();
    rad = angulo * M_PI / 180.0;
    tuboX = 855 - 35 * cos(rad);
    tuboY = 505 - 35 * sin(rad);
    escena->addLine(855, 505, tuboX, tuboY, QPen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap));

    // Indicador de turno
    if (turnoActual == 1 && !proyectilActivo) {
        escena->addEllipse(38, 480, 15, 15, QPen(Qt::red, 2), QBrush(QColor(255, 100, 100)));
        QGraphicsTextItem *turnoText = escena->addText("▲");
        turnoText->setPos(42, 478);
        turnoText->setDefaultTextColor(Qt::red);
    } else if (turnoActual == 2 && !proyectilActivo) {
        escena->addEllipse(848, 480, 15, 15, QPen(Qt::blue, 2), QBrush(QColor(100, 100, 255)));
        QGraphicsTextItem *turnoText = escena->addText("▲");
        turnoText->setPos(852, 478);
        turnoText->setDefaultTextColor(Qt::blue);
    }

    // Obstáculos visuales (simples)
    escena->addRect(150, 450, 40, 40, QPen(Qt::red, 2), QBrush(QColor(231, 76, 60, 200)));
    escena->addRect(220, 350, 40, 40, QPen(Qt::red, 2), QBrush(QColor(231, 76, 60, 200)));
    escena->addRect(180, 250, 40, 40, QPen(Qt::red, 2), QBrush(QColor(231, 76, 60, 200)));
    escena->addRect(120, 150, 40, 40, QPen(Qt::red, 2), QBrush(QColor(231, 76, 60, 200)));

    escena->addRect(710, 450, 40, 40, QPen(Qt::blue, 2), QBrush(QColor(52, 152, 219, 200)));
    escena->addRect(640, 350, 40, 40, QPen(Qt::blue, 2), QBrush(QColor(52, 152, 219, 200)));
    escena->addRect(680, 250, 40, 40, QPen(Qt::blue, 2), QBrush(QColor(52, 152, 219, 200)));
    escena->addRect(740, 150, 40, 40, QPen(Qt::blue, 2), QBrush(QColor(52, 152, 219, 200)));
}

void JuegoVentana::lanzarProyectil()
{
    if (proyectilActivo) return;

    double angulo, velocidad;

    if (turnoActual == 1) {
        angulo = entradaAnguloJ1->text().toDouble();
        velocidad = entradaVelocidadJ1->text().toDouble();
    } else {
        angulo = entradaAnguloJ2->text().toDouble();
        velocidad = entradaVelocidadJ2->text().toDouble();
    }

    if (angulo < 0 || angulo > 90) {
        QMessageBox::warning(this, "Error", "Ángulo entre 0° y 90°");
        return;
    }

    if (velocidad <= 0 || velocidad > 500) {
        QMessageBox::warning(this, "Error", "Velocidad entre 1 y 500 m/s");
        return;
    }

    proyectilActivo = true;
    habilitarControles(false);
    QMessageBox::information(this, "Disparo", QString("Disparo realizado!\nÁngulo: %1°\nVelocidad: %2 m/s").arg(angulo).arg(velocidad));
}

void JuegoVentana::actualizarSimulacion()
{
    if (!proyectilActivo) return;

    static int contador = 0;
    contador++;
    if (contador > 30) {
        contador = 0;
        proyectilActivo = false;

        if (turnoActual == 1) {
            turnoActual = 2;
        } else {
            turnoActual = 1;
        }

        habilitarControles(true);
        configurarEscenario();

        // Actualizar etiquetas RIVAL/TÚ
        if (turnoActual == 1) {
            labelRivalJ1->setText("RIVAL");
            labelRivalJ2->setText("TÚ");
        } else {
            labelRivalJ1->setText("TÚ");
            labelRivalJ2->setText("RIVAL");
        }
    }
}

void JuegoVentana::habilitarControles(bool habilitado)
{
    if (turnoActual == 1) {
        entradaAnguloJ1->setEnabled(habilitado && !proyectilActivo);
        entradaVelocidadJ1->setEnabled(habilitado && !proyectilActivo);
        botonLanzarJ1->setEnabled(habilitado && !proyectilActivo);
        entradaAnguloJ2->setEnabled(false);
        entradaVelocidadJ2->setEnabled(false);
        botonLanzarJ2->setEnabled(false);
    } else {
        entradaAnguloJ1->setEnabled(false);
        entradaVelocidadJ1->setEnabled(false);
        botonLanzarJ1->setEnabled(false);
        entradaAnguloJ2->setEnabled(habilitado && !proyectilActivo);
        entradaVelocidadJ2->setEnabled(habilitado && !proyectilActivo);
        botonLanzarJ2->setEnabled(habilitado && !proyectilActivo);
    }
}

void JuegoVentana::mostrarInstrucciones()
{
    QMessageBox::information(this, "Instrucciones",
                             " JUEGO DE ARTILLERÍA \n\n"
                             "ÁNGULO: 0° a 90°\n"
                             "VELOCIDAD: 1 a 500 m/s\n"
                             "Destruye los 4 obstáculos del RIVAL\n\n"
                             " JUGADOR 1: Cañón izquierdo\n"
                             " JUGADOR 2: Cañón derecho");
}

void JuegoVentana::acercaDe()
{
    QMessageBox::about(this, "Acerca de",
                       "Juego de Artillería\nVersión 1.0\nPráctica 5 - POO");
}

void JuegoVentana::nuevaPartida()
{
    turnoActual = 1;
    proyectilActivo = false;
    habilitarControles(true);
    configurarEscenario();
    labelRivalJ1->setText("RIVAL");
    labelRivalJ2->setText("TÚ");
    QMessageBox::information(this, "Nueva Partida", "¡Juego reiniciado!");
}

void JuegoVentana::salirJuego()
{
    close();
}
