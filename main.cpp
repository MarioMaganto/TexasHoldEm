#include "pokerinformatica.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PokerInformatica w;
    w.show();
    return a.exec();
}
