#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <array>

#include "gamecontroller.h"
#include "squareitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace Chess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSquareClicked(const QPoint& pos);
    void handleBoardUpdated();
    void handleSelectionChanged();
    void handleMovesChanged();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;
    GameController m_controller;

    // Vraag 2: No globals — static constexpr gebruikt voor configuratie,
    // geen globale variabelen buiten klassen
    static constexpr int TILE_SIZE = 80; // pas aan naar smaak
    static constexpr int BOARD_PIXELS = TILE_SIZE * Board::SIZE;

    // store pointers to square items and piece pixmaps
    // Vraag 24: useful template class
    std::array<std::array<SquareItem*, 8>, 8> m_squares;
    std::array<std::array<QGraphicsPixmapItem*, 8>, 8> m_pieceItems;

    void setupBoardGraphics();
    void refreshBoardGraphics();
    void highlightSelectionAndMoves();
    QPixmap piecePixmapForChar(char symbol);
};

#endif // MAINWINDOW_H
