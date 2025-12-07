#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QPoint>
#include <vector>

#include "board.h"
#include "piece.h"

namespace Chess {

class GameController : public QObject
{
    Q_OBJECT

public:
    explicit GameController(QObject* parent = nullptr);

    const Board& board() const { return m_board; }

    // Called by GUI when player clicks a square
    void handleSquareClick(const QPoint& pos);

    // Data for GUI
    QPoint selectedSquare() const { return m_selected; }
    const std::vector<QPoint>& highlightedMoves() const { return m_moves; }
    Color currentPlayer() const { return m_currentPlayer; }

signals:
    void boardUpdated();          // GUI must redraw pieces
    void selectionChanged();      // GUI must rehighlight selection
    void movesChanged();          // GUI must highlight moves

private:
    Board m_board;
    QPoint m_selected;            // -1,-1 means nothing selected
    std::vector<QPoint> m_moves;  // valid moves for selected piece
    Color m_currentPlayer;        // whose turn

    void clearSelection();
};

} // namespace Chess

#endif // GAMECONTROLLER_H
