#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include "carta.h"

using namespace std;

// Declaración de la función para convertir un Palo en un string
string enum2str(Carta::Palo p);
string enum2str(Carta::Numero n);

#endif // UTILIDADES_H
