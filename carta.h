#ifndef CARTA_H
#define CARTA_H
#include <string>
#include <vector>

using namespace std;
//1.Clases.
//1.1. Clase carta: tiene como objetivo definir el número palo de cada una de las cartas.
class Carta{

public:
    //Clases palo y número: se utiliza una clase de enumeración porque permite asignar un número a cada uno de los número o palos.
    //Es necesario definir Dos=2 porque por defecto empieza a contar en 1.
    enum class Palo{cora, diam, trebol, picas};
    enum class Numero{Dos=2, Tres, Cuatro, Cinco, Seis, Siete, Ocho, Nueve, Diez, J, Q, K, A};

    //Constructor: permite crear una carta de manera sencilla.
    Carta(Palo p, Numero n);

    //Métodos.
    //Obtener el palo.
    Palo getPalo();

    //Obtener el número.
    Numero getNumero();

private:
    //Propiedades: las dos propiedades son el palo de la carta y su número.
    Palo p;
    Numero n;
};

#endif // CARTA_H
