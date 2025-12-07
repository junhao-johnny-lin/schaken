#ifndef KING_H
#define KING_H

#include "piece.h"

namespace Chess {

class King : public Piece
{
public:
    King(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'K' : 'k'); }
};

} // namespace Chess

#endif // KING_H
