#include "board.h"
#include "piece.h"

// If you have concrete pieces, include them here for setupInitialPosition:
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#include <cstring> // for std::memset

namespace Chess {

Board::Board()
{
    // initialize all squares to nullptr
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            m_grid[r][c] = nullptr;
}

Board::~Board()
{
    clear();
}

bool Board::isInside(const QPoint &pos) const
{
    int x = pos.x();
    int y = pos.y();
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool Board::isEmpty(const QPoint &pos) const
{
    if (!isInside(pos)) return false;
    return m_grid[pos.y()][pos.x()] == nullptr;
}

Piece* Board::pieceAt(const QPoint &pos) const
{
    if (!isInside(pos)) return nullptr;
    return m_grid[pos.y()][pos.x()];
}

void Board::setPieceAt(const QPoint &pos, Piece* piece)
{
    if (!isInside(pos)) return;
    // delete existing piece? we don't delete here to avoid accidental deletes.
    m_grid[pos.y()][pos.x()] = piece;
}

bool Board::movePiece(const QPoint &from, const QPoint &to)
{
    if (!isInside(from) || !isInside(to)) return false;
    Piece* p = m_grid[from.y()][from.x()];
    if (!p) return false;

    // basic move: capture or empty dest
    Piece* dest = m_grid[to.y()][to.x()];
    if (dest) {
        // delete captured piece (simple ownership model)
        delete dest;
    }
    m_grid[to.y()][to.x()] = p;
    m_grid[from.y()][from.x()] = nullptr;
    return true;
}

bool Board::hasEnemyAt(const QPoint &pos, Piece* piece) const
{
    if (!isInside(pos) || !piece) return false;
    Piece* other = pieceAt(pos);
    return other && (other->color() != piece->color());
}

bool Board::hasEnemyAt(const QPoint &pos, int color) const
{
    if (!isInside(pos)) return false;
    Piece* other = pieceAt(pos);
    if (!other) return false;
    // convert color int to enum compare if needed; here we assume matching underlying values
    return static_cast<int>(other->color()) != color;
}

void Board::clear()
{
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (m_grid[r][c]) {
                delete m_grid[r][c];
                m_grid[r][c] = nullptr;
            }
        }
    }
}

void Board::setupInitialPosition()
{
    clear();

    // Pawns
    for (int c = 0; c < SIZE; ++c) {
        m_grid[6][c] = new Pawn(Color::White); // white pawns on row 6 (rank 2 visually)
        m_grid[1][c] = new Pawn(Color::Black); // black pawns on row 1 (rank 7 visually)
    }

    // Rooks
    m_grid[7][0] = new Rook(Color::White);
    m_grid[7][7] = new Rook(Color::White);
    m_grid[0][0] = new Rook(Color::Black);
    m_grid[0][7] = new Rook(Color::Black);

    // Knights
    m_grid[7][1] = new Knight(Color::White);
    m_grid[7][6] = new Knight(Color::White);
    m_grid[0][1] = new Knight(Color::Black);
    m_grid[0][6] = new Knight(Color::Black);

    // Bishops
    m_grid[7][2] = new Bishop(Color::White);
    m_grid[7][5] = new Bishop(Color::White);
    m_grid[0][2] = new Bishop(Color::Black);
    m_grid[0][5] = new Bishop(Color::Black);

    // Queens
    m_grid[7][3] = new Queen(Color::White);
    m_grid[0][3] = new Queen(Color::Black);

    // Kings
    m_grid[7][4] = new King(Color::White);
    m_grid[0][4] = new King(Color::Black);

    // Note: row = y, column = x. We use 0..7 coordinates.
}
} // namespace Chess
