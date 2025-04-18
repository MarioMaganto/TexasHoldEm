#include "carta.h"

Carta::Carta(Palo p, Numero n) : p(p), n(n){}

Carta::Palo Carta::getPalo(){
    return p;
}
Carta::Numero Carta::getNumero(){
    return n;
}
