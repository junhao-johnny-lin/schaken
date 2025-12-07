#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

namespace Chess {

class Queen : public Piece
{
public:
    Queen(Color color);

    std::vector<QPoint> validMoves(const QPoint& pos,
                                   const Board& board) const override;

    char symbol() const override { return (m_color == Color::White ? 'Q' : 'q'); }
};

} // namespace Chess

#endif // QUEEN_H
