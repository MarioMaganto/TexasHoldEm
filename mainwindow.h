#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "carta.h"
#include "mano.h"
#include "baraja.h"
#include "utilidades.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void igualarApuesta(int jugador);   // Para los bots
    void subirApuesta(int jugador);     // Para los bots
    void retirarseApuesta(int jugador); // Para los bots

    void igualarApuestaHumano();
    void subirApuestaHumano();
    void retirarseApuestaHumano();

private:
    Ui::MainWindow *ui;

    // Estados del juego
    int ronda=0;
    float apuesta=0;
    float bote=0;
    int retirados=0;
    int apuestaSubida=0;

    // Jugadores y manos
    std::vector<float> jugadores;             // Saldo de cada jugador
    std::vector<bool> activos;              // Jugadores activos en la ronda
    std::vector<bool> yaAposto;            // Jugadores que ya han apostado
    std::vector<bool> subioApuesta;        // Jugadores que subieron la apuesta
    std::vector<bool> pasoTurno;           // Jugadores que han pasado

    Mano mano1, mano2, mano3, centro;
    Baraja baraja;

    // Funciones principales del juego
    void inicializarPartida();
    void siguienteRonda();
    void terminarRondas();
    void salir();

    // Apuestas
    void inicializarApuestas();
    void humanoApuesta();
    void bot1Apuesta();
    void bot2Apuesta();

    // Utilidades
    void realizarApuestas();               // Declarada si se usa en inicializarPartida()
    void actualizarBotesyEstados(int j);        // Declarada si se usa en terminarRondas()
    QString obtenerDireccionCarta(Carta &c);
    const int alfa1=90;
    const int alfa2=-90;
};

#endif // MAINWINDOW_H
