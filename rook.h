#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

namespace Chess {

class Rook : public Piece
{
public:
    Rook(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'R' : 'r'); }
};

} // namespace Chess

#endif // ROOK_H
