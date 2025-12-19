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

    // Vraag 14: Polymorfisme #1 — validMoves is virtueel in Piece en overridden in alle stukken
    virtual ~Piece() = default;

    // Vraag 23: correct usage of inline function
    Color color() const { return m_color; }
    bool isWhite() const { return m_color == Color::White; }

    // Must be implemented by each piece
    // Vraag 17: virtual function — polymorfisme
    virtual std::vector<QPoint> validMoves(const QPoint& pos,
                                           const Board& board) const = 0;

    // Vraag 14: Polymorfisme #2 — symbol() is virtueel in Piece en overridden in alle stukken
    virtual char symbol() const = 0;

protected:
    // Vraag3: correct protections — protected zodat subclasses dit kunnen gebruiken
    Color m_color;
};

} // namespace Chess

#endif // PIECE_H
