// pokerinformatica.h
#ifndef POKERINFORMATICA_H
#define POKERINFORMATICA_H

#include <QMainWindow>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class PokerInformatica; }
QT_END_NAMESPACE

class PokerInformatica : public QMainWindow
{
    Q_OBJECT

public:
    PokerInformatica(QWidget *parent = nullptr);
    ~PokerInformatica();

private slots:
    void on_EmpezarButton_clicked();
    void on_PasarButton_clicked();
    void on_SubirButton_clicked();
    void on_RetirarseButton_clicked();

private:
    Ui::PokerInformatica *ui;

    QString obtenerRutaCarta(Carta::Numero numero, Carta::Palo palo);
    void mostrarImagenCarta(QLabel* label, Carta::Numero numero, Carta::Palo palo);

    // Clases del juego
    class Carta {
    public:
        enum class Palo{Corazones, Diamantes, Treboles, Picas};
        enum class Numero{Dos=2, Tres, Cuatro, Cinco, Seis, Siete, Ocho, Nueve, Diez, J, Q, K, A};

        Carta(Palo p, Numero n) : p(p), n(n) {}

        Palo getPalo() { return p; }
        Numero getNumero() { return n; }

    private:
        Palo p;
        Numero n;
    };

    enum class Victorias{CartaAlta, Pareja, DoblePareja, Trio, Escalera, Color, FullHouse, Poker, EscaleraColor, EscaleraReal};

    class Mano {
    public:
        void insertarCarta(Carta &c) { cartas.push_back(c); }
        std::vector<Carta> getCartas() { return cartas; }
        void combinarManos(Mano &otraMano);
        Victorias obtenerVictoria();

    private:
        std::vector<Carta> cartas;
        std::string enum2str(Carta::Palo p);
        std::string enum2str(Carta::Numero n);
    };

    class Baraja {
    public:
        Baraja();
        void barajar();
        Carta extraerCarta();

    private:
        std::vector<Carta> cartas;
    };

    // Variables del juego
    Baraja baraja;
    std::vector<Mano> manosJugadores;
    Mano manoCentro;
    std::vector<int> saldos;
    int rondaActual;
    int bote;
    std::vector<bool> jugadoresActivos;
    bool apuestaSubida;
    int apuestaActual;

    // Métodos auxiliares
    void inicializarPartida();
    void repartirCartasIniciales();
    void mostrarCartasEnUI();
    void procesarRonda();
    void determinarGanador();
    void actualizarSaldosUI();
    void accionBot(int jugadorBot);
};
#endif // POKERINFORMATICA_H
