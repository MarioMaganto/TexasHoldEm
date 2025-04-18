#include "baraja.h"
#include <algorithm>
#include <random>
#include <chrono>

Baraja::Baraja(){
    for (int i=0; i<4; i++){
        for (int j=2; j<=14; j++){
            Carta::Palo palo=static_cast<Carta::Palo>(i);
            Carta::Numero numero=static_cast<Carta::Numero>(j);
            cartas.push_back(Carta(palo, numero));
        }
    }
}

void Baraja::barajar(){
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    shuffle(cartas.begin(),cartas.end(),rng);
}

Carta Baraja::extraerCarta(){
    Carta cartaExtraida=cartas.back();
    cartas.pop_back();
    return cartaExtraida;
}

void Baraja::reiniciar(){
    cartas.clear();

    for (int i = 0; i < 4; i++) {
        for (int j = 2; j <= 14; j++) {
            Carta::Palo palo = static_cast<Carta::Palo>(i);
            Carta::Numero numero = static_cast<Carta::Numero>(j);
            cartas.push_back(Carta(palo, numero));
        }
    }
}
