#include "utilidades.h"
#include <string>

string enum2str(Carta::Palo p){
    switch (p){
    case Carta::Palo::cora: return "cora";
    case Carta::Palo::diam: return "diam";
    case Carta::Palo::trebol: return "trebol";
    case Carta::Palo::picas: return "picas";
    }
    return "";
}
string enum2str(Carta::Numero n){
    switch (n) {
    case Carta::Numero::Dos: return "2";
    case Carta::Numero::Tres: return "3";
    case Carta::Numero::Cuatro: return "4";
    case Carta::Numero::Cinco: return "5";
    case Carta::Numero::Seis: return "6";
    case Carta::Numero::Siete: return "7";
    case Carta::Numero::Ocho: return "8";
    case Carta::Numero::Nueve: return "9";
    case Carta::Numero::Diez: return "10";
    case Carta::Numero::J: return "J";
    case Carta::Numero::Q: return "Q";
    case Carta::Numero::K: return "K";
    case Carta::Numero::A: return "A";
    }
    return "";
}
