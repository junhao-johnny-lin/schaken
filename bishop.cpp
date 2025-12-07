#include "bishop.h"
#include "board.h"

namespace Chess {

Bishop::Bishop(Color color)
    : Piece(color)
{
}

std::vector<QPoint> Bishop::validMoves(const QPoint& pos,
                                       const Board& board) const
{
    std::vector<QPoint> moves;

    int x = pos.x();
    int y = pos.y();

    // Helper voor diagonaal scannen
    auto scanDir = [&](int dx, int dy) {
        int cx = x + dx;
        int cy = y + dy;

        while (board.isInside(QPoint(cx, cy))) {
            Piece* p = board.pieceAt(QPoint(cx, cy));

            if (!p) {
                // Lege vak → geldige zet
                moves.push_back(QPoint(cx, cy));
            } else {
                // Stuk gevonden → vijand = slaan, dan stoppen
                if (p->color() != this->color())
                    moves.push_back(QPoint(cx, cy));
                break;
            }

            cx += dx;
            cy += dy;
        }
    };

    // 4 diagonale richtingen
    scanDir( 1,  1); // rechts‑onder
    scanDir(-1,  1); // links‑onder
    scanDir( 1, -1); // rechts‑boven
    scanDir(-1, -1); // links‑boven

    return moves;
}

} // namespace Chess
