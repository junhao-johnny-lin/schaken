#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsPixmapItem>
#include <QFile>

// zorg dat pad/naam van resources overeenkomt met jouw qrc of map
//static const QString PIECE_PATH = ":/pieces/"; // als je resources.qrc gebruikt
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
  //  connect(&m_controller, &GameController::selectionChanged, this, &MainWindow::handleSelectionChanged);
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
QPixmap MainWindow::piecePixmapForChar(char symbol)
{
    // map symbols to filenames in your resources (:/pieces/). Use names like pawn_w.png, pawn_b.png etc.
    switch (symbol) {
    case 'P': return QPixmap(PIECE_PATH + "WhitePawn.png");
    case 'p': return QPixmap(PIECE_PATH + "BlackPawn.png");

    case 'R': return QPixmap(PIECE_PATH + "WhiteRook.png");
    case 'r': return QPixmap(PIECE_PATH + "BlackRook.png");

    case 'N': return QPixmap(PIECE_PATH + "WhiteKnight.png");
    case 'n': return QPixmap(PIECE_PATH + "BlackKnight.png");

    case 'B': return QPixmap(PIECE_PATH + "WhiteBishop.png");
    case 'b': return QPixmap(PIECE_PATH + "BlackBishop.png");

    case 'Q': return QPixmap(PIECE_PATH + "WhiteQueen.png");
    case 'q': return QPixmap(PIECE_PATH + "BlackQueen.png");

    case 'K': return QPixmap(PIECE_PATH + "WhiteKing.png");
    case 'k': return QPixmap(PIECE_PATH + "BlackKing.png");

    default: return QPixmap();
    }
}
