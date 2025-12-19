#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QPoint>
#include <vector>
#include "board.h"
#include "piece.h"
#include <thread>

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

    GameController(const GameController& other);

    // Vraag 22: useful getter
    const Board& board() const { return m_board; }
    QPoint selectedSquare() const { return m_selected; }

    //vraag 28: const references for variables
    //vraag 29:const references for functions
    const std::vector<QPoint>& highlightedMoves() const { return m_moves; }
    Color currentPlayer() const { return m_currentPlayer; }

    //vraag 28: const references for variables
    //vraag 29:const references for functions
    MoveResult handleSquareClick(const QPoint& pos);



private:
    // Vraag 3: Correct protections — interne spelstatus is private
    Board m_board;
    QPoint m_selected;

    //vraag 35: useful container class
    std::vector<QPoint> m_moves;

       // Vraag 21: useful member variable —
    Color m_currentPlayer;

    //vraag 28: const references for variables
    //vraag 29:const references for functions
    // Vraag 30:useful bool
    bool isLegalMove(Color color,
                     const QPoint& from,
                     const QPoint& to) const;


    void clearSelection();

    // Vraag 30:useful bool
    bool isKingInCheck(Color color) const;

    // Vraag 30:useful bool
    bool isCheckmate(Color color);
    void promotePawn(const QPoint &pos);
    void calculateAllMovesAsync(Color player, std::vector<QPoint>& outMoves);

};

} // namespace Chess

#endif
