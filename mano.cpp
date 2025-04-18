#include "mano.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Mano::insertarCarta(Carta &c){
    cartas.push_back(c);
}

void Mano::combinarManos(Mano &otraMano){
    int L=otraMano.cartas.size();

    for (int i=0; i<L; i++) {
        cartas.push_back(otraMano.cartas[i]);
    }
}

vector<Carta> Mano::obtenerCartas(){
    return cartas;
}

Victorias Mano::obtenerVictoria() {
    Victorias v = Victorias::CartaAlta;
    vector<int> n(13, 0);
    vector<int> p(4, 0);
    int L = cartas.size();

    for (int i = 0; i < L; i++) {
        int num = static_cast<int>(cartas[i].getNumero()) - 2;
        int pal = static_cast<int>(cartas[i].getPalo());
        n[num]++;
        p[pal]++;
    }

    bool color = any_of(p.begin(), p.end(), [](int count) {
        return count==5;
    });

    bool escalera=false;
    for (int i = 0; i <= 8; i++) {
        if (n[i] && n[i+1] && n[i+2] && n[i+3] && n[i+4]) {
            escalera = true;
            break;
        }
    }

    sort(n.begin(), n.end());

    if (color && escalera && n[12]) v = Victorias::EscaleraReal;
    else if (color && escalera) v = Victorias::EscaleraColor;
    else if (n[12] == 4) v = Victorias::Poker;
    else if (n[12] == 3 && n[11] == 2) v = Victorias::FullHouse;
    else if (color) v = Victorias::Color;
    else if (escalera) v = Victorias::Escalera;
    else if (n[12] == 3) v = Victorias::Trio;
    else if (n[12] == 2 && n[11] == 2) v = Victorias::DoblePareja;
    else if (n[12] == 2) v = Victorias::Pareja;
    return v;
}

string Mano::enum2str(Carta::Palo p){
    switch (p){
    case Carta::Palo::cora: return "Corazones";
    case Carta::Palo::diam: return "Diamantes";
    case Carta::Palo::trebol: return "Treboles";
    case Carta::Palo::picas: return "Picas";
    }
    return "";
}

string Mano::enum2str(Carta::Numero n){
    switch (n) {
    case Carta::Numero::Dos: return "Dos";
    case Carta::Numero::Tres: return "Tres";
    case Carta::Numero::Cuatro: return "Cuatro";
    case Carta::Numero::Cinco: return "Cinco";
    case Carta::Numero::Seis: return "Seis";
    case Carta::Numero::Siete: return "Siete";
    case Carta::Numero::Ocho: return "Ocho";
    case Carta::Numero::Nueve: return "Nueve";
    case Carta::Numero::Diez: return "Diez";
    case Carta::Numero::J: return "J";
    case Carta::Numero::Q: return "Q";
    case Carta::Numero::K: return "K";
    case Carta::Numero::A: return "A";
    }
    return "";
}
