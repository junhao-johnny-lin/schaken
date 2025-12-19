#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

namespace Chess {

class Pawn : public Piece
{
public:
    // Vraag 12: parameterized constructor
    Pawn(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'P' : 'p'); }
};

} // namespace Chess

#endif // PAWN_H
