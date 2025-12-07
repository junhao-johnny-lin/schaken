#include "queen.h"
#include "board.h"

namespace Chess {

Queen::Queen(Color color)
    : Piece(color)
{
}

std::vector<QPoint> Queen::validMoves(const QPoint& pos,
                                      const Board& board) const
{
    std::vector<QPoint> moves;

    int x = pos.x();
    int y = pos.y();

    auto scanDir = [&](int dx, int dy) {
        int cx = x + dx;
        int cy = y + dy;

        while (board.isInside(QPoint(cx, cy))) {
            Piece* p = board.pieceAt(QPoint(cx, cy));

            if (!p) {
                moves.push_back(QPoint(cx, cy));
            } else {
                if (p->color() != this->color())
                    moves.push_back(QPoint(cx, cy));
                break;
            }

            cx += dx;
            cy += dy;
        }
    };

    // rook directions
    scanDir(1, 0); scanDir(-1, 0);
    scanDir(0, 1); scanDir(0, -1);

    // bishop directions
    scanDir(1, 1); scanDir(-1, 1);
    scanDir(1, -1); scanDir(-1, -1);

    return moves;
}

} // namespace Chess
