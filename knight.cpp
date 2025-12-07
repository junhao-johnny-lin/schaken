#include "knight.h"
#include "board.h"

namespace Chess {

Knight::Knight(Color color)
    : Piece(color)
{
}

std::vector<QPoint> Knight::validMoves(const QPoint& pos,
                                       const Board& board) const
{
    std::vector<QPoint> moves;

    // Alle mogelijke paard-sprongen
    const int offsets[8][2] = {
        { 2, 1}, { 2, -1},
        {-2, 1}, {-2, -1},
        { 1, 2}, { 1, -2},
        {-1, 2}, {-1, -2}
    };

    for (auto& o : offsets) {
        QPoint np(pos.x() + o[0], pos.y() + o[1]);

        if (!board.isInside(np)) continue;

        Piece* p = board.pieceAt(np);

        // Lege spot → ok
        if (!p) {
            moves.push_back(np);
        }
        // Vijand → ok (capture)
        else if (p->color() != this->color()) {
            moves.push_back(np);
        }
        // Eigen stuk → overslaan
    }

    return moves;
}

} // namespace Chess
