#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QTransform>
#include "utilidades.h"
#include <QPixmap>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //Conectar los botones a las funciones.
    connect(ui->igualar, &QPushButton::clicked, this, &MainWindow::igualarApuestaHumano);
    connect(ui->subir, &QPushButton::clicked, this, &MainWindow::subirApuestaHumano);
    connect(ui->retirarse, &QPushButton::clicked, this, &MainWindow::retirarseApuestaHumano);
    connect(ui->comenzar, &QPushButton::clicked, this, &MainWindow::inicializarPartida);
    connect(ui->salir, &QPushButton::clicked, this, &MainWindow::salir);

    //Cambiar fondo.
    QPixmap pix("D:/Estudios/Grado/4º Curso/2º Cuatrimestre/Informática Avanzada/Trabajos/TexasHoldEm/Completo/Completo/Cartas/Fondo.webp");
    ui->fondo->setPixmap(pix.scaled(1300,1300,Qt::KeepAspectRatio));

    //Poner todo en invisible.
    //1. Poner todas las cartas en invisible.
    ui->ganado->setVisible(false);
    ui->perdido->setVisible(false);

    ui->centro1->setVisible(false);
    ui->centro2->setVisible(false);
    ui->centro3->setVisible(false);
    ui->centro4->setVisible(false);
    ui->centro5->setVisible(false);

    ui->j1_carta1->setVisible(false);
    ui->j1_carta2->setVisible(false);

    ui->j2_carta1->setVisible(false);
    ui->j2_carta2->setVisible(false);

    ui->j3_carta1->setVisible(false);
    ui->j3_carta2->setVisible(false);

    //2. Poner opciones de elección en invisible.
    ui->igualar->setVisible(false);
    ui->subir->setVisible(false);
    ui->retirarse->setVisible(false);

    //3. Poner marcador de ganador en invisible
    ui->ganador->setVisible(false);
    ui->ganador_2->setVisible(false);

    //4. Demás en invisible.
    ui->bote1->setVisible(false);
    ui->bote2->setVisible(false);
    ui->bote3->setVisible(false);
    ui->botetotal->setVisible(false);
    ui->bote_2->setVisible(false);

    ui->estado1->setVisible(false);
    ui->estado2->setVisible(false);
    ui->estado3->setVisible(false);

    ui->jugador1->setVisible(false);
    ui->jugador2->setVisible(false);
    ui->jugador3->setVisible(false);

    ui->ronda->setVisible(false);
    ui->ronda_2->setVisible(false);

    //A partir de esta función comienza el siguiente flujo: iniciar partida->Apuestas->1ª ronda (siguienteRonda)->Apuestas->2ª ronda (siguienteRonda)->Apuestas->
    //3ª ronda (terminarRondas)->Mostar cartas de los jugadores finales y determinar ganador.
    jugadores={100,100,100};
    eliminados={false,false,false};
}

// Función para rotar un QPixmap
QPixmap rotatePixmap(const QPixmap &pixmap, int angle)
{
    QTransform transform;
    transform.rotate(angle);  // Rotar la imagen en el ángulo deseado

    // Aplicar la transformación de rotación al QPixmap
    return pixmap.transformed(transform);
}

//FUNCIONES CORRESPONDIENTES A LAS RONDAS.
//Función para inicializar la partida.
void MainWindow::inicializarPartida(){
    //1. Actualizar contador de rondas: iniciar partida -> ronda=0
    ronda=0;
    retirados=0;
    ui->ronda->setText(QString::number(ronda));
    ui->botetotal->setText(QString::number(bote));

    mano1=Mano();
    mano2=Mano();
    mano3=Mano();
    centro=Mano();

    //1. Barajar cartas.
    baraja.reiniciar();
    baraja.barajar();

    //2. Poner todo lo necesario en invisible/visible.
    ui->centro1->setVisible(false);
    ui->centro2->setVisible(false);
    ui->centro3->setVisible(false);
    ui->centro4->setVisible(false);
    ui->centro5->setVisible(false);

    ui->j1_carta1->setVisible(false);
    ui->j1_carta2->setVisible(false);

    ui->j2_carta1->setVisible(false);
    ui->j2_carta2->setVisible(false);

    ui->j3_carta1->setVisible(false);
    ui->j3_carta2->setVisible(false);

    //2. Poner opciones de elección en invisible.
    ui->igualar->setVisible(true);
    ui->subir->setVisible(true);
    ui->retirarse->setVisible(true);

    //3. Poner marcador de ganador en invisible
    ui->ganador->setVisible(false);
    ui->ganador_2->setVisible(false);

    ui->salir->setVisible(false);
    ui->bote1->setVisible(true);
    ui->bote2->setVisible(true);
    ui->bote3->setVisible(true);
    ui->botetotal->setVisible(true);
    ui->bote_2->setVisible(true);

    ui->estado1->setVisible(true);
    ui->estado2->setVisible(true);
    ui->estado3->setVisible(true);

    ui->jugador1->setVisible(true);
    ui->jugador2->setVisible(true);
    ui->jugador3->setVisible(true);

    ui->ronda->setVisible(true);
    ui->ronda_2->setVisible(true);

    ui->comenzar->setVisible(false);

    //Eliminar al jugador que ha perdido.
    if ((jugadores[1]==0)||(jugadores[2]==0)){
        if (eliminados[1]==true){
            ui->jugador2->setVisible(false);
            ui->estado2->setVisible(false);
            ui->bote2->setVisible(false);
        }else{
            ui->jugador3->setVisible(false);
            ui->estado3->setVisible(false);
            ui->bote3->setVisible(false);
        }
    }

    //5. Inicializar variables.
    srand(time(0));
    bote=0;
    activos={true, true, true};

    //6. Repartir cartas.
    //6.1. Repartir cartas a los jugadores.
    for (int i=0; i<3; i++){
        //Sacar nuevas cartas de la baraja.
        Carta c1=baraja.extraerCarta();
        Carta c2=baraja.extraerCarta();

        switch (i){
            //Primero, repartir al jugador 1.
            case 0: {
                //Asignar las cartas a una mano.
                mano1.insertarCarta(c1);
                mano1.insertarCarta(c2);

                //Mostrar cartas.
                ui->j1_carta1->setVisible(true);
                QPixmap pix1(obtenerDireccionCarta(c1));
                ui->j1_carta1->setPixmap(pix1.scaled(100,100,Qt::KeepAspectRatio));

                ui->j1_carta2->setVisible(true);
                QPixmap pix2(obtenerDireccionCarta(c2));
                ui->j1_carta2->setPixmap(pix2.scaled(100,100,Qt::KeepAspectRatio));
                break;

                //Luego, repartir al jugador 2.
            }case 1:{
                //Asignar las cartas a una mano.
                mano2.insertarCarta(c1);
                mano2.insertarCarta(c2);

                //Mostrar cartas.
                QPixmap pix1(obtenerDireccionCarta(c1));
                QPixmap pix2(obtenerDireccionCarta(c2));

                //Rotar cartas.
                QPixmap rotatePixmap1 = rotatePixmap(pix1, alfa1);  // Rotar la primera carta
                QPixmap rotatePixmap2 = rotatePixmap(pix2, alfa1);  // Rotar la segunda carta

                //Mostrar las cartas rotadas (no es necesario escalarlas después de rotarlas si ya se ve bien).
                ui->j2_carta1->setPixmap(rotatePixmap1);
                ui->j2_carta2->setPixmap(rotatePixmap2);

                ui->j2_carta1->setPixmap(rotatePixmap1.scaled(100, 100, Qt::KeepAspectRatio));
                ui->j2_carta2->setPixmap(rotatePixmap2.scaled(100, 100, Qt::KeepAspectRatio));
                break;

                //Finalmente, repartir al jugador 3.
            }case 2:{
                //Asignar las cartas a una mano.
                mano3.insertarCarta(c1);
                mano3.insertarCarta(c2);

                //Mostrar cartas.
                QPixmap pix1(obtenerDireccionCarta(c1));
                QPixmap pix2(obtenerDireccionCarta(c2));

                //Rotar cartas.
                QPixmap rotatePixmap1 = rotatePixmap(pix1, alfa2);  // Rotar la primera carta
                QPixmap rotatePixmap2 = rotatePixmap(pix2, alfa2);  // Rotar la segunda carta

                //Mostrar las cartas rotadas (no es necesario escalarlas después de rotarlas si ya se ve bien).
                ui->j3_carta1->setPixmap(rotatePixmap1);
                ui->j3_carta2->setPixmap(rotatePixmap2);

                ui->j3_carta1->setPixmap(rotatePixmap1.scaled(100, 100, Qt::KeepAspectRatio));
                ui->j3_carta2->setPixmap(rotatePixmap2.scaled(100, 100, Qt::KeepAspectRatio));
                break;
            }
        }
    }
    //6.2. Repartir cartas del centro.
    for (int i=0; i<3; i++) {
        //Extraer carta.
        Carta c=baraja.extraerCarta();

        //Asignar carta a la mano del centro.
        centro.insertarCarta(c);

        //Mostrar carta.
        switch (i){
            case 0:{
                QPixmap pix(obtenerDireccionCarta(c));
                ui->centro1->setVisible(true);
                ui->centro1->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
                break;
            }case 1:{
                QPixmap pix(obtenerDireccionCarta(c));
                ui->centro2->setVisible(true);
                ui->centro2->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
                break;
            }case 2:{
                QPixmap pix(obtenerDireccionCarta(c));
                ui->centro3->setVisible(true);
                ui->centro3->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
                break;
            }
        }
    }

    //6. Realizar apuestas.
    inicializarApuestas();
}

//Función para preparar la siguiente ronda.
void MainWindow::siguienteRonda(){
    //1. Actualizar contador de rondas: primera ronda -> ronda=0
    ronda++;
    ui->ronda->setText(QString::number(ronda));

    //2. Sacar carta al centro.
    //Extraer carta.
    Carta c=baraja.extraerCarta();

    //Asignar carta a la mano del centro.
    centro.insertarCarta(c);

    //Mostrar carta: si la ronda es la primera ocupa el hueco de centro4, si es la segunda ocupa centro5.
    if (ronda==1){
        QPixmap pix(obtenerDireccionCarta(c));
        ui->centro4->setVisible(true);
        ui->centro4->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
    }else{
        QPixmap pix(obtenerDireccionCarta(c));
        ui->centro5->setVisible(true);
        ui->centro5->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
    }

    //2. Realizar apuestas.
    inicializarApuestas();
}

//Función para preparar la siguiente ronda.
void MainWindow::terminarRondas(){
    //1. Actualizar contador de rondas:
    ronda++;
    ui->ronda->setText(QString::number(ronda));

    //2. Mostrar las cartas de los jugadores activos.
    for (int j=1; j<3; j++){
        if ((activos[j])&&(j==1)&&(eliminados[j]==false)){
            ui->j2_carta1->setVisible(true);
            ui->j2_carta2->setVisible(true);

        }else if ((activos[j])&&(j==2)&&(eliminados[j]==false)){
            ui->j3_carta1->setVisible(true);
            ui->j3_carta2->setVisible(true);
        }
    }

    //3. Obtener puntuaciones de los jugadores activos.
    vector<Victorias> puntuaciones(3, Victorias::CartaAlta);
    Mano aux;

    //Guardar puntuaciones.
    if ((activos[0])&&(eliminados[0]==false)){
        aux=mano1;
        aux.combinarManos(centro);
        puntuaciones[0]=aux.obtenerVictoria();
    }
    if ((activos[1])&&(eliminados[1]==false)){
        aux=mano2;
        aux.combinarManos(centro);
        puntuaciones[1]=aux.obtenerVictoria();
    }
    if ((activos[2])&&(eliminados[2]==false)){
        aux=mano3;
        aux.combinarManos(centro);
        puntuaciones[2]=aux.obtenerVictoria();
    }

    //Indicar ganador.
    int ganador=-1;
    vector<int> empatados;

    //En este bucle, se comprueba que jugadores si hay un claro ganador o si los jugadores tienen
    //puntuaciones similares. Si se da el primer caso, se borra la lista de empatados; mientras que
    //si se da el segundo, se amplía.
    for (int i=0; i<3; i++){
        if ((activos[i])&&(eliminados[i]==false)){
            if ((ganador==-1)||(puntuaciones[i]>puntuaciones[ganador])) {
                ganador=i;
                empatados.clear();
                empatados.push_back(i);
            } else if (puntuaciones[i]==puntuaciones[ganador]){
                empatados.push_back(i);
            }
        }
    }

    //Si hay ganador único, mostrarlo y dar el bote.
    if (empatados.size()==1){
        jugadores[ganador]+=bote;

        ui->ganador->setText("GANADOR: Jugador " + QString::number(ganador+1) + ": " + QString::fromStdString(victoriaToString(puntuaciones[empatados[0]])));
    }

    //Si hay empate, mostrar a los ganadores y distribuir el dinero entre los empatados
    else{
        int L=empatados.size();
        float reparto=bote/L;

        //Mostrar ganadores.
        if (L==2){
            ui->ganador->setText("GANADORES: Jugadores " + QString::number(empatados[0] + 1) + " y " + QString::number(empatados[1] + 1) + ": " + QString::fromStdString(victoriaToString(puntuaciones[empatados[0]])));
        }else{
            ui->ganador->setText("EMPATAN TODOS: " + QString::fromStdString(victoriaToString(puntuaciones[empatados[0]])));
        }

        //Repartir bote.
        for (int i=0;i<L;i++){
            jugadores[empatados[i]]+=reparto;
        }
    }

    //4. Actualizar botes y estados.
    bote=0;
    ui->botetotal->setText("0");

    actualizarBotesyEstados(0);
    actualizarBotesyEstados(1);
    actualizarBotesyEstados(2);

    //5. Poner marcador de ganador visibles.
    ui->ganador->setVisible(true);
    ui->ganador_2->setVisible(true);

    //6. Dar la opción de seguir jugando.
    if (jugadores[0]==0){
        ui->perdido->setVisible(true);
    } else if ((jugadores[1]==0)&&(jugadores[2]==0)){
        ui->ganado->setVisible(true);
    } else{
        ui->comenzar->setVisible(true);
    }

    if (jugadores[1]==0){
        eliminados[1]=true;
    }else if (jugadores[2]==0){
        eliminados[2]=true;
    }

    ui->salir->setVisible(true);
}

//FUNCIONES CORRESPONDIENTES A LAS APUESTAS.
//Función para inicializar apuestas.
void MainWindow::inicializarApuestas(){
    //1. Declarar variables de apuestas: sirven recordar si ha subido la apuesta, que jugadores la han subido y cuanto vale
    //pasar/igual la apuesta.
    apuestaSubida=0;
    yaAposto={false, false, false};
    subioApuesta={false, false, false};
    pasoTurno={false, false, false};
    apuesta=5;

    //2. Comenzar apuestas por el jugador humano.
    humanoApuesta();
}

//Apuesta el jugador humano.
void MainWindow::humanoApuesta(){

    //1. Dejar apostar al jugador si el jugador está activo, todavía no ha apostado y no es el único que sigue activo.
    if ((!yaAposto[0])&&(activos[0])&&(retirados!=2)&&(jugadores[0]>0)){

        //Darle todas las opciones si todavía no ha subido la apuesta.
        if ((!subioApuesta[0])&&(!pasoTurno[0])&&(jugadores[0]>0)){
            ui->igualar->setVisible(true);
            ui->subir->setVisible(true);
            ui->retirarse->setVisible(true);
            //Darle todas las opciones menos subir la apuesta, si ya la ha subido.
        }else{
            ui->igualar->setVisible(true);
            ui->retirarse->setVisible(true);
        }
        //2. Si el jugador no está activo, ya ha apostado o es el único que queda, pasar el turno de apuesta al siguiente jugador.
    }else{
        bot1Apuesta();
    }
}

//Apuestan jugadores bots.
void MainWindow::bot1Apuesta(){
    //1. Declarar variables para la elección aleatoria de apuesta.
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    double n=dist(rng);

    //2. Dejar apostar al jugador si el jugador está activo, todavía no ha apostado y no es el único que sigue activo.
    if ((!yaAposto[1])&&(activos[1])&&(retirados!=2)&&(jugadores[1]>0)){

        //Darle todas las opciones si todavía no ha subido la apuesta.
        if ((!subioApuesta[1])&&(!pasoTurno[1])&&(jugadores[1]>0)){
            if (n<0.5) igualarApuesta(1);
            else if (n<0.9) subirApuesta(1);
            else if (!yaAposto[1]) retirarseApuesta(1);
            //Darle todas las opciones menos subir la apuesta, si ya la ha subido.
        }else{
            if (n<0.7) igualarApuesta(1);
            else if (!yaAposto[1]) retirarseApuesta(1);
        }
        //3. Si el jugador no está activo, ya ha apostado o es el único que queda, pasar el turno de apuesta al siguiente jugador.
    }else{
        bot2Apuesta();
    }
}

//Apuestan jugadores bots.
void MainWindow::bot2Apuesta(){
    //1. Declarar variables para la elección aleatoria de apuesta.
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    double n=dist(rng);

    //2. Dejar apostar al jugador si el jugador está activo, todavía no ha apostado y no es el único que sigue activo.
    if ((!yaAposto[2])&&(activos[2])&&(retirados!=2)&&(jugadores[2]>0)){
        //Darle todas las opciones si todavía no ha subido la apuesta.
        if ((!subioApuesta[2])&&(!pasoTurno[2])&&(jugadores[2]>0)){
            if (n<0.6) igualarApuesta(2);
            else if (n<0.95) subirApuesta(2);
            else if (!yaAposto[2]) retirarseApuesta(2);
            //Darle todas las opciones menos subir la apuesta, si ya la ha subido.
        }else{
            if (n<0.7) igualarApuesta(2);
            else if (!yaAposto[2]) retirarseApuesta(2);
        }
        //3. Si el jugador no está activo, ya ha apostado o es el único que queda pasar a la siguiente ronda o terminar la partida dependiendo de en qué ronda se esté.
    }else{
        if (ronda<2){
            siguienteRonda();
        }else{
            terminarRondas();
        }
    }
}

//Funciones para el jugador humano.
void MainWindow::igualarApuestaHumano(){
    igualarApuesta(0);
}

void MainWindow::subirApuestaHumano(){
    subirApuesta(0);
}

void MainWindow::retirarseApuestaHumano(){
    retirarseApuesta(0);
}

//Igualar apuesta.
void MainWindow::igualarApuesta(int j){
    //1. Realizar operaciones.
    //Si el jugador ya subió la apuesta y la apuesta ha vuelto a ser subida, el jugador que iguale solo iguala 5.
    if ((subioApuesta[j])&&(apuestaSubida>0)){
        (jugadores[j]-5*(apuestaSubida-1)<0) ? (bote+=jugadores[j]) : (bote+=5*(apuestaSubida-1));
        (jugadores[j]-5*(apuestaSubida-1)<0) ? (jugadores[j]=0) : (jugadores[j]-=5*(apuestaSubida-1));

    //Si el jugador no subió la apuesta y la apuesta ha subido, el jugador iguala todo lo que haya.
    }else if (apuestaSubida>0){
        (jugadores[j]-5*(apuestaSubida)<0) ? (bote+=jugadores[j]) : (bote+=5*(apuestaSubida));
        (jugadores[j]-5*(apuestaSubida)<0) ? (jugadores[j]=0) : (jugadores[j]-=5*(apuestaSubida));
    }

    //2. Actualizar estado del jugador y bote.
    yaAposto[j]=true;
    actualizarBotesyEstados(j);

    //3. Desactivar opciones de apuesta.
    ui->igualar->setVisible(false);
    ui->subir->setVisible(false);
    ui->retirarse->setVisible(false);

    //4. Pasar al siguiente estado.
    switch (j){
        //Si el jugador humano acaba de apostar, pasar al primer bot.
        case 0:{
            ui->estado1->setText("Igualo/Paso");
            QTimer::singleShot(1000, this, [=](){
                bot1Apuesta();
            });
            break;
            //Si el primer bot acaba de apostar, pasar al segundo bot.
        }case 1:{
            ui->estado2->setText("Igualo/Paso");
            QTimer::singleShot(2000, this, [=]() {
                bot2Apuesta();
            });
            break;
            //Si el segundo bot acaba de apostar, pasar a la siguiente ronda o terminar la partida dependiendo de en qué ronda se esté.
        }case 2:{
            ui->estado3->setText("Igualo/Paso");
            if (ronda<2){
                QTimer::singleShot(2000, this, [=](){
                    siguienteRonda();
                });
                break;
            }else{
                QTimer::singleShot(2000, this, [=]() {
                    terminarRondas();
                });
                break;
            }
        }
    }
}

//Subir apuesta.
void MainWindow::subirApuesta(int j){
    apuestaSubida++;

    //1. Realizar operaciones.
    (jugadores[j]-5*(apuestaSubida)<0) ? (bote+=jugadores[j]) : (bote+=5*(apuestaSubida));
    (jugadores[j]-5*(apuestaSubida)<0) ? (jugadores[j]=0) : (jugadores[j]-=5*(apuestaSubida));

    //3. Indicar que se ha subido la apuesta, quién la ha subido y que el resto de los jugadores aún no han apostado.
    subioApuesta[j]=true;
    fill(begin(yaAposto), end(yaAposto), false);
    yaAposto[j]=true;//El que subió no debe volver a apostar

    //4. Actualizar bote y estado.
    actualizarBotesyEstados(j);

    //5. Desactivar opciones.
    ui->igualar->setVisible(false);
    ui->subir->setVisible(false);
    ui->retirarse->setVisible(false);

    //6. Pasar al siguiente estado.
    switch (j){
        //Si el jugador humano acaba de apostar, pasar al primer bot.
        case 0:{
            ui->estado1->setText("Subio");
            QTimer::singleShot(2000, this, [=](){
                bot1Apuesta();
            });
            break;
            //Si el primer bot acaba de apostar, pasar al segundo bot.
        }case 1:{
            ui->estado2->setText("Subio");
            QTimer::singleShot(2000, this, [=](){
                humanoApuesta();
            });
            break;
            //Si el segundo bot acaba de apostar, pasar a la siguiente ronda o terminar la partida dependiendo de en qué ronda se esté.
        }case 2:{
            ui->estado3->setText("Subio");
            QTimer::singleShot(2000, this, [=](){
                humanoApuesta();
            });
            break;
        }
    }
}

//Retirarse.
void MainWindow::retirarseApuesta(int j){
    //1. Realizar operaciones.
    activos[j]=false;
    yaAposto[j]=true;
    retirados++;

    //2. Desactivar opciones.
    ui->igualar->setVisible(false);
    ui->subir->setVisible(false);
    ui->retirarse->setVisible(false);

    //3. Actualizar bote y estado.
    actualizarBotesyEstados(j);

    //4. Pasar al siguiente estado.
    switch (j){
        case 0:{
            //Si el jugador humano acaba de apostar, pasar al primer bot.
            ui->estado1->setText("Se retiro");
            QTimer::singleShot(2000, this, [=](){
                bot1Apuesta();
            });
            break;
            //Si el primer bot acaba de apostar, pasar al segundo bot.
        }case 1:{
            ui->estado2->setText("Se retiro");
            QTimer::singleShot(2000, this, [=](){
                bot2Apuesta();
            });
            break;
            //Si el segundo bot acaba de apostar, pasar a la siguiente ronda o terminar la partida dependiendo de en qué ronda se esté.
        }case 2:{
            ui->estado3->setText("Se retiro");
            if (ronda<2){
                QTimer::singleShot(2000, this, [=](){
                    siguienteRonda();
                });
                break;
            }else{
                QTimer::singleShot(2000, this, [=](){
                    terminarRondas();
                });
                break;
            }
        }
    }
}

//FUNCIONES PARA OTRAS COSAS.
//Actualizar bote y estados.
void MainWindow::actualizarBotesyEstados(int j){
    //1. Actualizar estados individuales.
    ui->botetotal->setText(QString::number(bote));

    if (!activos[j]){
        switch (j){
            case 0 :{
                ui->estado1->setText("Retirado");
                break;
            }case 1 :{
                ui->estado2->setText("Retirado");
                break;
            }case 2 :{
                ui->estado3->setText("Retirado");
                break;
            }
        }
    //2. Actualizar bote.
    }else{
        switch (j){
            case 0 :{
                ui->bote1->setText(QString::number(jugadores[j]));
                break;
            }case 1 :{
                ui->bote2->setText(QString::number(jugadores[j]));
                break;
            }case 2 :{
                ui->bote3->setText(QString::number(jugadores[j]));
                break;
            }
        }
    }
}

//Función para encontrar dirección de las cartas
QString MainWindow::obtenerDireccionCarta(Carta &c){
    //1. Declarar dirección en la qué está la carpeta con las cartas.
    QString basePath="D:/Estudios/Grado/4º Curso/2º Cuatrimestre/Informática Avanzada/Trabajos/TexasHoldEm/Completo/Completo/Cartas/";

    //2. Convertimos el número y el palo a string
    QString numero=QString::fromStdString(enum2str(c.getNumero()));
    QString palo=QString::fromStdString(enum2str(c.getPalo()));

    //3. Creamos el nombre de la imagen
    QString nombreArchivo=numero+"_"+palo+".png";

    //4. Ruta completa
    QString ruta=basePath+nombreArchivo;
    return ruta;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::salir()
{
    close();
}

