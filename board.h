#ifndef BOARD_H
#define BOARD_H

#include <QPoint>

namespace Chess {
class Piece; // forward

class Board {
public:
    static constexpr int SIZE = 8;

    Board();
    ~Board();

    bool isInside(const QPoint &pos) const;
    bool isEmpty(const QPoint &pos) const;
    Piece* pieceAt(const QPoint &pos) const;
    void setPieceAt(const QPoint &pos, Piece* piece);
    bool movePiece(const QPoint &from, const QPoint &to); // returns true if move executed

    bool hasEnemyAt(const QPoint &pos, Chess::Piece* piece) const; // convenience
    bool hasEnemyAt(const QPoint &pos, int color) const; // overload using color enum integer

    void clear();
    void setupInitialPosition(); // place starting pieces

private:
    Piece* m_grid[SIZE][SIZE];
};

} // namespace Chess

#endif // BOARD_H
