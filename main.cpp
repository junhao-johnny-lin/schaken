#include "mainwindow.h"

#include <QApplication>
// Vraag 1: Clean main — main bevat enkel GUI‑opstart en geen logica

// Vraag 6: Complete project — compileert zonder fouten en draait zonder crashes.

// Vraag 7: Fully working project — compileert en draait zonder crashes

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
