//Poker en C++ usando Qt
//Este programa implementa un juego de póker para 3 jugadores (1 humano y 2 bots).
//La estructura del programa está dividida en clases para representar cartas, manos y barajas, así como las distintas combinaciones.
//Cada clase tiene métodos específicos para gestionar cartas, mezclar la baraja y evaluar manos.

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

//1.Clases.
//1.1. Clase carta: tiene como objetivo definir el número palo de cada una de las cartas.
class Carta{

public:
    //Clases palo y número: se utiliza una clase de enumeración porque permite asignar un número a cada uno de los número o palos.
    //Es necesario definir Dos=2 porque por defecto empieza a contar en 1.
    enum class Palo{Corazones, Diamantes, Treboles, Picas};
    enum class Numero{Dos=2, Tres, Cuatro, Cinco, Seis, Siete, Ocho, Nueve, Diez, J, Q, K, A};

    //Constructor: permite crear una carta de manera sencilla.
    Carta(Palo p, Numero n) : p(p), n(n) {}

    //Métodos.
    //Obtener el palo.
    Palo getPalo(){
        return p;
    }

    //Obtener el número.
    Numero getNumero(){
        return n;
    }

private:
    //Propiedades: las dos propiedades son el palo de la carta y su número.
    Palo p;
    Numero n;
};

//1.2. Clase victorias: esta clase recoge las distintas formas de victoria y, al igual que antes, se ha utilizado una clase de enumeración
//porque permite asignar un número a cada carta y así simplificar las comparaciones.
enum class Victorias{CartaAlta, Pareja, DoblePareja, Trio, Escalera, Color, FullHouse, Poker, EscaleraColor, EscaleraReal};

//Puesto que no se puede imprimir en pantalla una clase de enumeración, ya que no es un string, es necesario encontrar una forma de asignar
//un string a cada tipo de victoria. La forma más inmediata de hacer esto es con map. Si utilizamos cartas en lugar de displays para los
//números y las letras, no es necesario ni el map ni la función de a continuación.
map<Victorias, string> victoriaToStringMap={
    {Victorias::CartaAlta, "Carta Alta"},
    {Victorias::Pareja, "Pareja"},
    {Victorias::DoblePareja, "Doble Pareja"},
    {Victorias::Trio, "Trio"},
    {Victorias::Escalera, "Escalera"},
    {Victorias::Color, "Color"},
    {Victorias::FullHouse, "Full House"},
    {Victorias::Poker, "Poker"},
    {Victorias::EscaleraColor, "Escalera de Color"},
    {Victorias::EscaleraReal, "Escalera Real"}
};

//Finalmente, creamos una función para asignar más rápidamente los valores string asociados, aunque realmente no sería necesario.
string victoriaToString(Victorias v) {
    return victoriaToStringMap[v];
}

//1.3. Clase mano: contiene las cartas que forman una mano y las acciones básicas que se van a realizar con ellas.
class Mano{

public:

    //Métodos.
    //Insertar una carta.
    void insertarCarta(Carta &c){
        cartas.push_back(c);
    }

    //Imprimir cartas.
    void imprimirMano(){
        int L=cartas.size();

        for (int i=0; i<L; i++){
            cout<<enum2str(cartas[i].getNumero())<<" "<<enum2str(cartas[i].getPalo())<<endl;
        }
    }

    //Combinar dos manos en una sola: este método lo utilizamos para juntar las manos de cada jugador con la mano
    //central sin necesidad de mezclar las dos. La razón por la cual se ha hecho esto es porque así tan solo hay
    //que añadir cartas a la mano central tras la inicialización y no ha cada una de las manos por separado.
    void combinarManos(Mano &otraMano){
        int L=otraMano.cartas.size();

        for (int i=0; i<L; i++) {
            cartas.push_back(otraMano.cartas[i]);
        }
    }

    //Decir puntuación: necesita ser revisado.
    Victorias obtenerVictoria() {
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
        else if (n[12] == 3 && n[12-1] >= 2) v = Victorias::FullHouse;
        else if (color) v = Victorias::Color;
        else if (escalera) v = Victorias::Escalera;
        else if (n[12] == 3) v = Victorias::Trio;
        else if (n[12] == 2 && n[12-1] >= 2) v = Victorias::DoblePareja;
        else if (n[12] == 2) v = Victorias::Pareja;
        return v;
    }

private:
    //Propiedades: las propiedades de la carta son las propias cartas.
    vector<Carta> cartas;

    //Función para convertir Palo a string: permite hacer lo mismo que hacíamos antes con las victorias, pero
    //de una manera distinta. En este caso, se utiliza un switch-case que asigna un string a cada tipo de
    //victoria.
    string enum2str(Carta::Palo p){
        switch (p){
        case Carta::Palo::Corazones: return "Corazones";
        case Carta::Palo::Diamantes: return "Diamantes";
        case Carta::Palo::Treboles: return "Treboles";
        case Carta::Palo::Picas: return "Picas";
        }
        return "";
    }

    //Función para convertir Numero a string
    string enum2str(Carta::Numero n){
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
};

//1.4. Clase baraja: contiene las cartas que componene la baraja.
class Baraja{
public:
    //Constructor: en este caso es algo más complejo que el anterior y siempre tiene la misma forma inicial
    //Se utiliza un bucle para recorrer los cuatro palos y otro para recorrer las 14 cartas de cada palo.
    //En cada iteración del segundo bucle se crea una carta utilizando el static_cast para pasar el número
    //entero correspondiente a la iteración del bucle a la clase palo/número.
    Baraja(){
        for (int i=0; i<4; i++){
            for (int j=2; j<=14; j++){
                Carta::Palo palo=static_cast<Carta::Palo>(i);
                Carta::Numero numero=static_cast<Carta::Numero>(j);
                cartas.push_back(Carta(palo, numero));
            }
        }
    }

    //Métodos.
    //Barajar cartas: se encarga de mezclar las cartas de la baraja. Para poder hacerlo correctamente y de
    //manera prácticamente aleatoria, se tiene que utilizar la librería <chrono>, así podemos crear una
    //semilla que dependa de la hora del reloj y que sea lo más distinta a la anterior posible. Hay que
    //utilizar la función shuffle de los vectores que nos permite mezclar las posiciones de un vector en
    //función de un factor aleatorio.
    void barajar(){
        unsigned seed=chrono::system_clock::now().time_since_epoch().count();
        default_random_engine rng(seed);
        shuffle(cartas.begin(),cartas.end(),rng);
    }

    //Extraer cata: se utiliza una pila para eliminar el primer elemento de la baraja una vez extraído.
    Carta extraerCarta(){
        Carta cartaExtraida=cartas.back();
        cartas.pop_back();
        return cartaExtraida;
    }

private:
    //Propiedades: de nuevo, las propiedades son las propias cartas.
    vector<Carta> cartas;
};

//2. Main: contiene un menú de opciones y la lógica general de una partida fuera de lo que ya hacen las clases.
int main() {
    //2.1. Inicializar variables: en primer lugar, se crea la baraja y se mezcla, seguidamenete, se crean tres variables:
    //una para almacenar la opción del menú (compuesto por jugar/salir del programa), otra para contener los saldos
    //de los jugadores y una última para guardar la apuesta que pueden hacer los jugadores. Esta apuesta es siempre
    //constante para simplificar el programa.
    Baraja baraja;
    baraja.barajar();

    int opcion;
    vector<int> jugadores={100,100,100};

    srand(time(0));

    //2.2. Crear menú de opciones: se utiliza un bucle do-while para contener el normal funcionamiento del programa.
    //Dentro de este bucle se imprime el menú y se limpia la pantalla, para mayor claridad entre partidas.
    do {
        cout << "Menu:\n1. Jugar\n2. Salir\nOpcion: ";
        cin >> opcion;
        system("cls");

        //2.2.1. Opción 1: comienza una partida. Una partida dura de 4 rondas: en la primera se
        //reparten 2 cartas a cada jugador y se sacan 3 al centro, en la segunda y tercera ronda se tan solo se saca
        //1 carta al centro y en la última ronda se encuentra al ganador/ganadores.
        if (opcion==1){
            //2.2.1.1. Inicializar variables: estas varibles sirven para contener el bote y la apuesta correspondiente
            //a igualar y para recordar si se ha subido la apuesta, qué jugadores no se han retirado y cuáles ya han
            //subido la apuesta.
            int bote=0;

            vector<bool> activos={true, true, true, true};

            //2.2.1.2. Inicializar manos: antes de comenzar la primera ronda, hay que dar a cada jugador 2 cartas.
            Mano mano1, mano2, mano3, centro;
            for (int i = 0; i < 2; i++) {
                Carta c1=baraja.extraerCarta();
                Carta c2=baraja.extraerCarta();
                Carta c3=baraja.extraerCarta();

                mano1.insertarCarta(c1);
                mano2.insertarCarta(c2);
                mano3.insertarCarta(c3);
            }

            //2.2.1.2. Sistema de repartir cartas.
            //Sistema de rondas: con este bucle se recorren las 4 rondas rondas mencionadas anteriormente.
            for (int ronda=0; ronda<4; ronda++){
                cout<<endl<<"----RONDA "<<ronda<<"----"<<endl;
                //REPARTIR CARTAS.
                //Primera ronda: como dicho antes, tan solo se tienen que sacar tres cartas
                if (ronda==0){

                    for (int i=0; i<3; i++) {
                        Carta c=baraja.extraerCarta();
                        centro.insertarCarta(c);
                    }

                    //Segunda y tercera ronda: se saca una carta al centro.
                }else if (ronda<3){
                    Carta c=baraja.extraerCarta();
                    centro.insertarCarta(c);

                    //Última ronda: se muestran las cartas del centro y se obtienen quiénes son los ganadores.
                }else{
                    cout<<endl<<"----RONDA FINAL----"<<endl;
                    cout << "Cartas del centro:\n";

                    //Mostrar las cartas del centro.
                    centro.imprimirMano();

                    //Mostrar las cartas de los jugadores activos.
                    for (int j=0; j<3; j++) {
                        if (activos[j]) {
                            cout <<"\nCartas del Jugador "<<j+1<<":\n";
                            if (j==0) mano1.imprimirMano();
                            else if (j== 1) mano2.imprimirMano();
                            else mano3.imprimirMano();
                        }
                    }

                    //Mostrar las combinaciones de los jugadores activos.
                    vector<Victorias> puntuaciones(3, Victorias::CartaAlta);
                    Mano aux;

                    if (activos[0]){
                        aux=mano1;
                        aux.combinarManos(centro);
                        puntuaciones[0]=aux.obtenerVictoria();
                        cout<<"\nJugador 1: "<<victoriaToString(puntuaciones[0])<<endl;
                    }
                    if (activos[1]){
                        aux=mano2;
                        aux.combinarManos(centro);
                        puntuaciones[1]=aux.obtenerVictoria();
                        cout<<"Jugador 2: "<< victoriaToString(puntuaciones[1])<<endl;
                    }
                    if (activos[2]){
                        aux=mano3;
                        aux.combinarManos(centro);
                        puntuaciones[2]=aux.obtenerVictoria();
                        cout<<"Jugador 3: "<<victoriaToString(puntuaciones[2])<<endl;
                    }

                    //Indicar ganador.
                    int ganador=-1;
                    vector<int> empatados;

                    //En este bucle, se comprueba que jugadores si hay un claro ganador o si los jugadores tienen
                    //puntuaciones similares. Si se da el primer caso, se borra la lista de empatados; mientras que
                    //si se da el segundo, se amplía.
                    for (int i=0; i<3; i++){
                        if (activos[i]){
                            if ((ganador==-1)||(puntuaciones[i]>puntuaciones[ganador])) {
                                ganador=i;
                                empatados.clear();
                                empatados.push_back(i);
                            } else if (puntuaciones[i]==puntuaciones[ganador]) {
                                empatados.push_back(i);
                            }
                        }
                    }

                    //Si hay un ganador único
                    if (empatados.size()==1){
                        jugadores[ganador]+=bote;
                        cout<<"\nGANADOR: Jugador "<<ganador+1<<" (" << victoriaToString(puntuaciones[ganador])<< ")\n";
                    }
                    //Si hay empate, distribuimos el dinero entre los empatados
                    else{
                        int L=empatados.size();
                        int reparto=bote/L;
                        for (int i=0;i<L;i++){
                            jugadores[i]+=reparto;
                            cout<<"Jugador "<<empatados[i]+1<<" comparte la victoria con "<<victoriaToString(puntuaciones[empatados[i]])<< "\n";
                        }
                    }

                    //Se imprimen los saldos finales
                    cout<<"\nSaldos finales:"<<endl;
                    for (int i=0; i<3; i++) {
                        cout<< "Jugador "<<i+1<<": "<<jugadores[i]<<endl;
                    }
                }

                //SISTEMA DE APUSTAS.
                //En cada ronda, los jugadores pueden decidir pasar (seguir sin subir la apuesta)/igualar, subir la apuesta
                //o retirarse. Los jugadores solo pueden subir una vez por ronda la apuesta.
                if (ronda!=3){
                    //Estas variables sirven recordar si ha subido la apuesta, que jugadores la han subido y cuanto vale
                    //pasar/igual la apuesta.
                    bool apuestaSubida=false;
                    bool yaAposto[3]={false, false, false};
                    vector<bool> subioApuesta={false, false, false};
                    vector<bool> pasoTurno={false, false, false};
                    int apuesta=5;

                    for (int j=0; j<3; j++){
                        //Comprobar cuántos jugadores se han retirado.
                        int retirados=0;
                        int L=activos.size();

                        for (int k=0; k<L; k++){
                            if (activos[k]) retirados++;
                        }

                        //Solo apuestan aquellos jugadores que siguen activos y que no han apostado y si hay dos o más jugadores
                        //actuvos.
                        if ((yaAposto[j])||(!activos[j])||(retirados==2)) continue;

                        int eleccion;

                        //Tanto al jugador humano como los bots, se les da la opción de pasar, igual o retirarse si no han
                        //apostado o no se ha subido la apuesta. En caso de que se haya subido la apuesta y no hayan apostado
                        //todavía, la apuesta no se sube.

                        //Apuesta de jugador humano.
                        if (j==0){
                            cout<<endl<<"Tus cartas: "<<endl;
                            mano1.imprimirMano();
                            cout<<"\nCentro:"<<endl;
                            centro.imprimirMano();

                            if ((!subioApuesta[j])&&(!pasoTurno[j])){
                                cout<<"\nOpciones:\n1. Pasar/Igualar\n2. Subir apuesta\n3. Retirarse\nEleccion: ";
                            }else{
                                cout<<"\nOpciones:\n1. Pasar/Igualar \n3. Retirarse\nEleccion: ";
                            }
                            cin>>eleccion;
                        }

                        //Apuesta de jugadores bots
                        else{
                            unsigned seed=chrono::system_clock::now().time_since_epoch().count();
                            default_random_engine rng(seed);
                            uniform_real_distribution<double> dist(0.0, 1.0);
                            double n=dist(rng);

                            if ((!subioApuesta[j])&&(!pasoTurno[j])){
                                if (n<0.5) eleccion=1;
                                else if (n<0.95) eleccion=2;
                                else if (!yaAposto[j]) eleccion=3;
                            }else{
                                if (n<0.5) eleccion=1;
                                else if (!yaAposto[j]) eleccion=3;
                            }

                        }
                        pasoTurno[j]=true;

                        //Actualización del bote y jugadores: es bastante directo y simplemente se resta la cantidad
                        //de dinero del saldo correspondiente y se imprime la decisión que ha tomado cada jugador.
                        if (eleccion==1){
                            if ((subioApuesta[j])&&(apuestaSubida)){
                                jugadores[j]-=5;
                                bote+=5;
                            }else if (apuestaSubida){
                                jugadores[j]-=apuesta;
                                bote+=apuesta;
                            }

                            cout<<"Jugador "<<j+1<<" pasa/iguala. Saldo-----> "<<jugadores[j]<<endl;

                            yaAposto[j]=true;
                        }
                        else if (eleccion==2){
                            if (apuestaSubida){
                                apuesta+=5;
                            }

                            jugadores[j]-=apuesta;
                            bote+=apuesta;

                            subioApuesta[j]=true;
                            apuestaSubida=true;

                            cout<<"Jugador "<<j+1<<" sube la apuesta. Saldo-> "<<jugadores[j]<<endl;

                            //Reiniciar la ronda de apuestas para todos menos el que subió
                            fill(begin(yaAposto), end(yaAposto), false);

                            yaAposto[j]=true;//El que subió no debe volver a apostar
                            j=-1;// Reinicia el bucle
                        }
                        else{
                            activos[j]=false;
                            cout<<"Jugador "<<j+1<<" se retira. Saldo-------> "<<jugadores[j]<<endl;
                            yaAposto[j]=true; // Marcar que ya apostó
                        }

                        //Sirve para ajustar la apuesta después de una ronda en la que un jugador la haya
                        //subido y toque igualarla.
                        if ((activos[2])&&(j==2)&&(!subioApuesta[2])){
                            apuestaSubida=false;
                            apuesta=5;
                            fill(begin(subioApuesta), end(subioApuesta), false);
                            fill(begin(pasoTurno), end((pasoTurno)), false);
                        }else if ((!activos[2])&&(j==1)&&(!subioApuesta[1])){
                            apuestaSubida=false;
                            apuesta=5;
                            fill(begin(subioApuesta), end(subioApuesta), false);
                            fill(begin((pasoTurno)), end((pasoTurno)), false);

                        }
                    }
                }
            }
        }
    } while (opcion!=2);

    return 0;
}
