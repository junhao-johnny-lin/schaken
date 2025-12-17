#include "mainwindow.h"

#include <QApplication>
// Vraag 1: Clean main — main bevat enkel GUI‑opstart en geen logica


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
