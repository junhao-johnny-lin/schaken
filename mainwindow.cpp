#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsPixmapItem>
#include <QFile>

// zorg dat pad/naam van resources overeenkomt met jouw qrc of map
//static const QString PIECE_PATH = ":/pieces/"; // als je resources.qrc gebruikt
// Vraag 34: useful string class usage
// Vraag 37: useful usage of (modern) file-I/O
static const QString PIECE_PATH = ":/pieces/image_resources/";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene(this)),
    m_controller(/*this*/) // parent for QObject
{
    ui->setupUi(this);

    // set scene to view
    ui->boardView->setScene(m_scene);
    ui->boardView->setFixedSize(BOARD_PIXELS + 10, BOARD_PIXELS + 10);
    m_scene->setSceneRect(0, 0, BOARD_PIXELS, BOARD_PIXELS);

    // setup visuals
    setupBoardGraphics();

    // connect controller signals
   // connect(&m_controller, &GameController::boardUpdated, this, &MainWindow::handleBoardUpdated);
   //connect(&m_controller, &GameController::selectionChanged, this, &MainWindow::handleSelectionChanged);
    //connect(&m_controller, &GameController::movesChanged, this, &MainWindow::handleMovesChanged);

    // initial paint from controller (constructed with setupInitialPosition)
    refreshBoardGraphics();
}


MainWindow::~MainWindow()
{
   delete ui;
}

// create 8x8 SquareItems and background colours
void MainWindow::setupBoardGraphics()
{
    const QColor light(222, 184, 135);
    const QColor dark(139, 69, 19);

    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            // create SquareItem
            SquareItem* s = new SquareItem(c, r, TILE_SIZE);
            s->setPos(c * TILE_SIZE, r * TILE_SIZE);

            // set base color
            bool lightSquare = ((r + c) % 2 == 0);
            s->setBrush(QBrush(lightSquare ? light : dark));
            s->setPen(Qt::NoPen);

            m_scene->addItem(s);
            m_squares[r][c] = s;

            // connect clicks
            // Vraag 18: useful usage of "this"
            connect(s, &SquareItem::clicked, this, &MainWindow::onSquareClicked);

            // create empty pixmap item for pieces (stacked above squares)
            QGraphicsPixmapItem* pi = new QGraphicsPixmapItem();
            pi->setOffset(0, 0);
            pi->setPos(c * TILE_SIZE, r * TILE_SIZE);
            pi->setZValue(1); // above squares
            m_scene->addItem(pi);
            m_pieceItems[r][c] = pi;
        }
    }
}

// handle click from any square
void MainWindow::onSquareClicked(const QPoint& pos)
{
    // pass to controller
    m_controller.handleSquareClick(pos);
     refreshBoardGraphics();
}

// refresh all piece images according to controller.board()
void MainWindow::refreshBoardGraphics()
{
    const Board& b = m_controller.board();

    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            QPoint p(c, r);
            Piece* piece = b.pieceAt(p);

            QGraphicsPixmapItem* pi = m_pieceItems[r][c];

            if (!piece) {
                pi->setPixmap(QPixmap()); // clear
            } else {
                char sym = piece->symbol();
                QPixmap px = piecePixmapForChar(sym);
                if (!px.isNull()) {
                    // scale to tile size (keep aspect)
                    QPixmap scaled = px.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    // center the pixmap in the tile
                    qreal dx = (TILE_SIZE - scaled.width())/2.0;
                    qreal dy = (TILE_SIZE - scaled.height())/2.0;
                    pi->setOffset(dx, dy);
                    pi->setPixmap(scaled);
                } else {
                    pi->setPixmap(QPixmap()); // if missing image
                }
            }
        }
    }

    highlightSelectionAndMoves();
}

// highlight selection and moves (simple overlay by changing pen or brush)
void MainWindow::highlightSelectionAndMoves()
{
    // reset pens/brushes (base colors already set but recolour to remove previous highlights)
    const QColor light(222, 184, 135);
    const QColor dark(139, 69, 19);
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            bool lightSquare = ((r + c) % 2 == 0);
            m_squares[r][c]->setBrush(QBrush(lightSquare ? light : dark));
        }
    }

    // selection
    QPoint sel = m_controller.selectedSquare();
    if (sel.x() != -1) {
        if (m_scene && m_squares[sel.y()][sel.x()]) {
            m_squares[sel.y()][sel.x()]->setBrush(QBrush(QColor(255, 215, 0))); // gold
        }
    }

    // moves
    const auto &moves = m_controller.highlightedMoves();
    for (const QPoint &m : moves) {
        m_squares[m.y()][m.x()]->setBrush(QBrush(QColor(50, 205, 50))); // lime green
    }
}

// signal handlers
void MainWindow::handleBoardUpdated() { refreshBoardGraphics(); }
void MainWindow::handleSelectionChanged() { highlightSelectionAndMoves(); }
void MainWindow::handleMovesChanged() { highlightSelectionAndMoves(); }

// helper to choose image file based on piece symbol
// Vraag 38: useful exception handling
QPixmap MainWindow::piecePixmapForChar(char symbol)
{
     QPixmap px;
    // map symbols to filenames in your resources (:/pieces/). Use names like pawn_w.png, pawn_b.png etc.
    switch (symbol) {
     case 'P':  px =  QPixmap(PIECE_PATH + "WhitePawn.png");break;
     case 'p':  px =  QPixmap(PIECE_PATH + "BlackPawn.png");break;

     case 'R':  px =  QPixmap(PIECE_PATH + "WhiteRook.png");break;
     case 'r':  px =  QPixmap(PIECE_PATH + "BlackRook.png");break;

     case 'N':  px =  QPixmap(PIECE_PATH + "WhiteKnight.png");break;
     case 'n':  px =   QPixmap(PIECE_PATH + "BlackKnight.png");break;

     case 'B':  px =  QPixmap(PIECE_PATH + "WhiteBishop.png");break;
     case 'b':  px =  QPixmap(PIECE_PATH + "BlackBishop.png");break;

     case 'Q':  px =  QPixmap(PIECE_PATH + "WhiteQueen.png");break;
     case 'q':  px =  QPixmap(PIECE_PATH + "BlackQueen.png");break;

     case 'K':  px =  QPixmap(PIECE_PATH + "WhiteKing.png");break;
     case 'k':  px =  QPixmap(PIECE_PATH + "BlackKing.png");break;

    default: return QPixmap();
        break;
    }
    if (px.isNull()) {
        // exception handling: afbeelding niet gevonden
        throw std::runtime_error("Failed to load piece image for symbol: " + std::string(1, symbol));
    }

    return px;
}
