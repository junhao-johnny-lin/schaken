#include "board.h"
#include "piece.h"

#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

namespace Chess {

Board::Board() {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            m_grid[y][x] = nullptr;
}

Board::~Board() {
    clear();
}

// --------------------------------
//     Deep Copy Helper
// --------------------------------
Piece* Board::clonePiece(const Piece* p) const {
    if (!p) return nullptr;

    switch (p->symbol()) {
    case 'P': return new Pawn(Color::White);
    case 'p': return new Pawn(Color::Black);

    case 'R': return new Rook(Color::White);
    case 'r': return new Rook(Color::Black);

    case 'N': return new Knight(Color::White);
    case 'n': return new Knight(Color::Black);

    case 'B': return new Bishop(Color::White);
    case 'b': return new Bishop(Color::Black);

    case 'Q': return new Queen(Color::White);
    case 'q': return new Queen(Color::Black);

    case 'K': return new King(Color::White);
    case 'k': return new King(Color::Black);
    }

    return nullptr;
}

// --------------------------------
//     Deep Copy Constructor
// --------------------------------
Board::Board(const Board& other) {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            m_grid[y][x] = clonePiece(other.m_grid[y][x]);
        }
    }
}

// --------------------------------
//     Deep Copy Assignment
// --------------------------------
Board& Board::operator=(const Board& other) {
    if (this == &other) return *this;

    clear();

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            m_grid[y][x] = clonePiece(other.m_grid[y][x]);
        }
    }

    return *this;
}

bool Board::isInside(const QPoint& pos) const {
    int x = pos.x(), y = pos.y();
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool Board::isEmpty(const QPoint& pos) const {
    return isInside(pos) && (m_grid[pos.y()][pos.x()] == nullptr);
}

Piece* Board::pieceAt(const QPoint& pos) const {
    if (!isInside(pos)) return nullptr;
    return m_grid[pos.y()][pos.x()];
}

void Board::setPieceAt(const QPoint& pos, Piece* piece) {
    if (!isInside(pos)) return;
    m_grid[pos.y()][pos.x()] = piece;
}

bool Board::movePiece(const QPoint& from, const QPoint& to) {
    if (!isInside(from) || !isInside(to)) return false;

    Piece* p = pieceAt(from);
    if (!p) return false;

    // capture
    Piece* dest = pieceAt(to);
    if (dest) delete dest;

    m_grid[to.y()][to.x()] = p;
    m_grid[from.y()][from.x()] = nullptr;
    return true;
}

bool Board::hasEnemyAt(const QPoint& pos, Piece* piece) const {
    if (!piece) return false;
    Piece* o = pieceAt(pos);
    return o && o->color() != piece->color();
}

bool Board::hasEnemyAt(const QPoint& pos, int color) const {
    Piece* o = pieceAt(pos);
    return o && (static_cast<int>(o->color()) != color);
}

void Board::clear() {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x) {
            delete m_grid[y][x];
            m_grid[y][x] = nullptr;
        }
}

void Board::setupInitialPosition() {
    clear();

    for (int x = 0; x < SIZE; ++x) {
        m_grid[6][x] = new Pawn(Color::White);
        m_grid[1][x] = new Pawn(Color::Black);
    }

    m_grid[7][0] = new Rook(Color::White);
    m_grid[7][7] = new Rook(Color::White);
    m_grid[0][0] = new Rook(Color::Black);
    m_grid[0][7] = new Rook(Color::Black);

    m_grid[7][1] = new Knight(Color::White);
    m_grid[7][6] = new Knight(Color::White);
    m_grid[0][1] = new Knight(Color::Black);
    m_grid[0][6] = new Knight(Color::Black);

    m_grid[7][2] = new Bishop(Color::White);
    m_grid[7][5] = new Bishop(Color::White);
    m_grid[0][2] = new Bishop(Color::Black);
    m_grid[0][5] = new Bishop(Color::Black);

    m_grid[7][3] = new Queen(Color::White);
    m_grid[0][3] = new Queen(Color::Black);

    m_grid[7][4] = new King(Color::White);
    m_grid[0][4] = new King(Color::Black);
}

} // namespace Chess
