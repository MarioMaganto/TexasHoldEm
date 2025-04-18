#ifndef MANO_H
#define MANO_H

#include "carta.h"
#include "combinaciones.h"
#include <vector>
#include <string>

using namespace std;

//1.3. Clase mano: contiene las cartas que forman una mano y las acciones básicas que se van a realizar con ellas.
class Mano{

public:

    //Métodos.
    //Insertar una carta.
    void insertarCarta(Carta &c);

    //Imprimir cartas.
    vector<Carta> obtenerCartas();

    //Combinar dos manos en una sola: este método lo utilizamos para juntar las manos de cada jugador con la mano
    //central sin necesidad de mezclar las dos. La razón por la cual se ha hecho esto es porque así tan solo hay
    //que añadir cartas a la mano central tras la inicialización y no ha cada una de las manos por separado.
    void combinarManos(Mano &otraMano);

    //Decir puntuación: necesita ser revisado.
    Victorias obtenerVictoria();

private:
    //Propiedades: las propiedades de la carta son las propias cartas.
    vector<Carta> cartas;

    //Función para convertir Palo a string: permite hacer lo mismo que hacíamos antes con las victorias, pero
    //de una manera distinta. En este caso, se utiliza un switch-case que asigna un string a cada tipo de
    //victoria.
    string enum2str(Carta::Palo p);

    //Función para convertir Numero a string
    string enum2str(Carta::Numero n);
};

#endif // MANO_H
