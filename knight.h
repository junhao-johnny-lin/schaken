#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

namespace Chess {

class Knight : public Piece
{
public:
    Knight(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'N' : 'n'); }
};

} // namespace Chess

#endif // KNIGHT_H
