#ifndef COMBINACIONES_H
#define COMBINACIONES_H

#include <string>
#include <map>

// Enumeración de las posibles jugadas
enum class Victorias {
    CartaAlta,
    Pareja,
    DoblePareja,
    Trio,
    Escalera,
    Color,
    FullHouse,
    Poker,
    EscaleraColor,
    EscaleraReal
};

// Mapa para traducir jugadas a texto (declaración extern)
extern std::map<Victorias, std::string> victoriaToStringMap;

//Función para convertir las victorias a String.
std::string victoriaToString(Victorias v);

#endif // COMBINACIONES_H
