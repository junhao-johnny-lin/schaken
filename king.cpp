#include "king.h"
#include "board.h"

namespace Chess {

King::King(Color color)
    : Piece(color)
{
}

std::vector<QPoint> King::validMoves(const QPoint& pos,
                                     const Board& board) const
{
    std::vector<QPoint> moves;

    const int dirs[8][2] = {
        {1,0}, {-1,0}, {0,1}, {0,-1},     // recht
        {1,1}, {-1,1}, {1,-1}, {-1,-1}    // diagonaal
    };

    for (auto &d : dirs) {
        QPoint np(pos.x() + d[0], pos.y() + d[1]);

        if (!board.isInside(np)) continue;

        Piece* p = board.pieceAt(np);

        // Leeg → ok
        if (!p) {
            moves.push_back(np);
        }
        // Vijand → ok
        else if (p->color() != this->color()) {
            moves.push_back(np);
        }
    }

    return moves;
}

} // namespace Chess
