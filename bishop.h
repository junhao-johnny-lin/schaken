#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

namespace Chess {

class Bishop : public Piece
{
public:
    Bishop(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'B' : 'b'); }
};

} // namespace Chess

#endif // BISHOP_H
