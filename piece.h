#ifndef PIECE_H
#define PIECE_H

#include <cmath>
#include <QPoint>
#include <vector>


namespace Chess {

enum class Color { White, Black };

class Board; // forward declaration

class Piece
{
public:
    Piece(Color color);
    // OOP: Polymorfisme — validMoves is virtual en overridden

    virtual ~Piece() = default;
    // OOP: Inline function — symbol() is inline in header

    Color color() const { return m_color; }
    bool isWhite() const { return m_color == Color::White; }

    // Must be implemented by each piece
    virtual std::vector<QPoint> validMoves(const QPoint& pos,
                                           const Board& board) const = 0;

    virtual char symbol() const = 0;

protected:
    Color m_color;
};

} // namespace Chess

#endif // PIECE_H
