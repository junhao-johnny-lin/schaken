#ifndef BOARD_H
#define BOARD_H

#include <QPoint>
#include <array>

namespace Chess {

class Piece;

class Board {
public:
    static constexpr int SIZE = 8;

    Board();
    Board(const Board& other);                  // deep copy constructor
    Board& operator=(const Board& other);       // deep copy assignment
    ~Board();

    bool isInside(const QPoint &pos) const;
    bool isEmpty(const QPoint &pos) const;
    Piece* pieceAt(const QPoint &pos) const;
    void setPieceAt(const QPoint &pos, Piece* piece);

    bool movePiece(const QPoint &from, const QPoint &to);

    bool hasEnemyAt(const QPoint &pos, Piece* piece) const;
    bool hasEnemyAt(const QPoint &pos, int color) const;

    void clear();
    void setupInitialPosition();

private:
    Piece* m_grid[SIZE][SIZE];

    Piece* clonePiece(const Piece* p) const;   // helper for deep copy
};

} // namespace Chess

#endif // BOARD_H
