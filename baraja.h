#ifndef BARAJA_H
#define BARAJA_H

#include "carta.h"
#include <vector>

using namespace std;

//1.4. Clase baraja: contiene las cartas que componene la baraja.
class Baraja{
public:
    //Constructor: en este caso es algo más complejo que el anterior y siempre tiene la misma forma inicial
    //Se utiliza un bucle para recorrer los cuatro palos y otro para recorrer las 14 cartas de cada palo.
    //En cada iteración del segundo bucle se crea una carta utilizando el static_cast para pasar el número
    //entero correspondiente a la iteración del bucle a la clase palo/número.
    Baraja();

    //Métodos.
    //Barajar cartas: se encarga de mezclar las cartas de la baraja. Para poder hacerlo correctamente y de
    //manera prácticamente aleatoria, se tiene que utilizar la librería <chrono>, así podemos crear una
    //semilla que dependa de la hora del reloj y que sea lo más distinta a la anterior posible. Hay que
    //utilizar la función shuffle de los vectores que nos permite mezclar las posiciones de un vector en
    //función de un factor aleatorio.
    void barajar();

    //Extraer cata: se utiliza una pila para eliminar el primer elemento de la baraja una vez extraído.
    Carta extraerCarta();

    void reiniciar();

private:
    //Propiedades: de nuevo, las propiedades son las propias cartas.
    vector<Carta> cartas;
};

#endif // BARAJA_H
