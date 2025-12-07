#include "gamecontroller.h"

namespace Chess {

GameController::GameController(QObject* parent)
    : QObject(parent),
      m_selected(-1, -1),
      m_currentPlayer(Color::White)
{
    m_board.setupInitialPosition();
}

void GameController::clearSelection()
{
    m_selected = QPoint(-1, -1);
    m_moves.clear();
}

void GameController::handleSquareClick(const QPoint& pos)
{
    Piece* clickedPiece = m_board.pieceAt(pos);

    // 1️⃣ Geen selectie → speler probeert een eigen stuk te selecteren
    if (m_selected.x() == -1) {

        if (!clickedPiece)
            return; // leeg vak → niets doen

        if (clickedPiece->color() != m_currentPlayer)
            return; // vijandelijk stuk → niet jouw beurt

        // Stuk selecteren
        m_selected = pos;
        m_moves = clickedPiece->validMoves(pos, m_board);

        emit selectionChanged();
        emit movesChanged();
        return;
    }

    // 2️⃣ Wel selectie → speler probeert een zet uit te voeren
    bool isValidMove = false;
    for (const QPoint &m : m_moves) {
        if (m == pos) {
            isValidMove = true;
            break;
        }
    }

    if (isValidMove) {
        // Zet uitvoeren
        m_board.movePiece(m_selected, pos);

        // beurt wisselen
        m_currentPlayer = (m_currentPlayer == Color::White
                           ? Color::Black
                           : Color::White);

        clearSelection();
        emit boardUpdated();
        emit selectionChanged();
        emit movesChanged();
        return;
    }

    // 3️⃣ Foutieve zet → nieuwe selectie?
    if (clickedPiece && clickedPiece->color() == m_currentPlayer) {
        // ander eigen stuk selecteren
        m_selected = pos;
        m_moves = clickedPiece->validMoves(pos, m_board);

        emit selectionChanged();
        emit movesChanged();
        return;
    }

    // 4️⃣ Klik op lege of vijandige square → selectie annuleren
    clearSelection();
    emit selectionChanged();
    emit movesChanged();
}

} // namespace Chess
