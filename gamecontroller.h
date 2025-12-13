#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QPoint>
#include <vector>
#include "board.h"
#include "piece.h"

namespace Chess {

enum class MoveResult {
    Invalid,
    Normal,
    Check,
    Checkmate
};

class GameController
{
public:
    GameController();

    const Board& board() const { return m_board; }
    QPoint selectedSquare() const { return m_selected; }
    const std::vector<QPoint>& highlightedMoves() const { return m_moves; }
    Color currentPlayer() const { return m_currentPlayer; }

    MoveResult handleSquareClick(const QPoint& pos);

private:
    Board m_board;
    QPoint m_selected;
    std::vector<QPoint> m_moves;
    Color m_currentPlayer;

    void clearSelection();
    bool isKingInCheck(Color color) const;
    bool isCheckmate(Color color);
    void promotePawn(const QPoint &pos);
};

} // namespace Chess

#endif
