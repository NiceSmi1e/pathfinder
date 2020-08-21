#include "pathfinder.h"

#include <QApplication>

pathfinder *Pathfinder;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pathfinder = new pathfinder;
    Pathfinder->show();
    return a.exec();
}
