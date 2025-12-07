#include "pawn.h"
#include "board.h"

namespace Chess {

Pawn::Pawn(Color color)
    : Piece(color)
{
}

std::vector<QPoint> Pawn::validMoves(const QPoint& pos,
                                     const Board& board) const
{
    std::vector<QPoint> moves;

    int dir = (m_color == Color::White ? -1 : 1);  // white moves up, black moves down
    int startRow = (m_color == Color::White ? 6 : 1);

    int x = pos.x();
    int y = pos.y();

    QPoint oneForward(x, y + dir);

    // 1 step forward
    if (board.isInside(oneForward) && board.isEmpty(oneForward))
        moves.push_back(oneForward);

    // 2 steps forward from start
    QPoint twoForward(x, y + 2 * dir);
    if (y == startRow &&
        board.isInside(oneForward) &&
        board.isEmpty(oneForward) &&
        board.isInside(twoForward) &&
        board.isEmpty(twoForward))
    {
        moves.push_back(twoForward);
    }

    // Capture left
    QPoint leftCapture(x - 1, y + dir);
    if (board.isInside(leftCapture)) {
        Piece* enemy = board.pieceAt(leftCapture);
        if (enemy && enemy->color() != this->color())
            moves.push_back(leftCapture);
    }

    // Capture right
    QPoint rightCapture(x + 1, y + dir);
    if (board.isInside(rightCapture)) {
        Piece* enemy = board.pieceAt(rightCapture);
        if (enemy && enemy->color() != this->color())
            moves.push_back(rightCapture);
    }

    return moves;
}

} // namespace Chess
