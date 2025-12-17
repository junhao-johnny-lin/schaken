#include "rook.h"
#include "board.h"

namespace Chess {

Rook::Rook(Color color)
    : Piece(color)
{
}

std::vector<QPoint> Rook::validMoves(const QPoint& pos,
                                     const Board& board) const
{
    std::vector<QPoint> moves;

    int x = pos.x();
    int y = pos.y();

    // helper lambda for scanning a direction
    auto scanDir = [&](int dx, int dy) {
        int cx = x + dx;
        int cy = y + dy;

        while (board.isInside(QPoint(cx, cy))) {
            Piece* p = board.pieceAt(QPoint(cx, cy));

            if (!p) {
                // empty square -> valid move
                moves.push_back(QPoint(cx, cy));
            } else {
                // square occupied -> capture only if enemy
                if (p->color() != this->color())
                    moves.push_back(QPoint(cx, cy));
                break; // cannot jump over pieces
            }

            cx += dx;
            cy += dy;
        }
    };

    // four rook directions
    // C++: Lambda functie gebruikt voor scan logic
    scanDir(1, 0);   // right
    scanDir(-1, 0);  // left
    scanDir(0, 1);   // down
    scanDir(0, -1);  // up

    return moves;
}

} // namespace Chess
