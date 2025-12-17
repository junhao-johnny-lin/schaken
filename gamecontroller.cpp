#include "gamecontroller.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"

namespace Chess {

GameController::GameController()
    : m_selected(-1, -1),
      m_currentPlayer(Color::White)
{
    m_board.setupInitialPosition();
}

void GameController::clearSelection()
{
    m_selected = QPoint(-1, -1);
    m_moves.clear();
}

/* ==============================
   SCHAAK CONTROLE
   ============================== */

bool GameController::isKingInCheck(Color color) const
{
    QPoint kingPos(-1, -1);

    // Zoek koning
    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* p = m_board.pieceAt(QPoint(x,y));
            if (p && p->color() == color &&
                (p->symbol() == 'K' || p->symbol() == 'k')) {
                kingPos = QPoint(x,y);
            }
        }
    }

    if (kingPos.x() == -1)
        return false;

    // Vijandelijke dreiging
    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* enemy = m_board.pieceAt(QPoint(x,y));
            if (!enemy || enemy->color() == color)
                continue;

            auto moves = enemy->validMoves(QPoint(x,y), m_board);
            for (const auto& m : moves)
                if (m == kingPos)
                    return true;
        }
    }

    return false;
}

/* ==============================
   LEGALITEIT ZET
   ============================== */

bool GameController::isLegalMove(Color color,
                                 const QPoint& from,
                                 const QPoint& to) const
{
    Board copy = m_board;
    copy.movePiece(from, to);

    QPoint kingPos(-1, -1);

    // Zoek koning na zet
    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* p = copy.pieceAt(QPoint(x,y));
            if (p && p->color() == color &&
                (p->symbol() == 'K' || p->symbol() == 'k')) {
                kingPos = QPoint(x,y);
            }
        }
    }

    if (kingPos.x() == -1)
        return false;

    // Kan vijand koning slaan?
    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* enemy = copy.pieceAt(QPoint(x,y));
            if (!enemy || enemy->color() == color)
                continue;

            auto moves = enemy->validMoves(QPoint(x,y), copy);
            for (const auto& m : moves)
                if (m == kingPos)
                    return false;
        }
    }

    return true;
}

/* ==============================
   SCHAAKMAT
   ============================== */

bool GameController::isCheckmate(Color color)
{
    if (!isKingInCheck(color))
        return false;

    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* p = m_board.pieceAt(QPoint(x,y));
            if (!p || p->color() != color)
                continue;

            auto moves = p->validMoves(QPoint(x,y), m_board);
            for (const auto& dest : moves) {
                if (isLegalMove(color, QPoint(x,y), dest))
                    return false;
            }
        }
    }

    return true;
}

/* ==============================
   PROMOTIE
   ============================== */

void GameController::promotePawn(const QPoint &pos)
{
    Piece* p = m_board.pieceAt(pos);
    if (!p) return;

    if ((p->symbol() == 'P' && pos.y() == 0) ||
        (p->symbol() == 'p' && pos.y() == 7)) {

        Color c = p->color();
        delete p;
        m_board.setPieceAt(pos, new Queen(c));
    }
}

/* ==============================
   INPUT AFHANDELING
   ============================== */

MoveResult GameController::handleSquareClick(const QPoint& pos)
{
    Piece* clicked = m_board.pieceAt(pos);

    // Geen selectie
    if (m_selected.x() == -1) {
        if (!clicked) return MoveResult::Invalid;
        if (clicked->color() != m_currentPlayer) return MoveResult::Invalid;

        m_selected = pos;
        m_moves.clear();

        auto rawMoves = clicked->validMoves(pos, m_board);
        for (const auto& m : rawMoves) {
            if (isLegalMove(m_currentPlayer, pos, m))
                m_moves.push_back(m);
        }

        return MoveResult::Normal;
    }

    // Is dit een geldige zet?
    bool valid = false;
    for (const auto& m : m_moves)
        if (m == pos)
            valid = true;

    if (!valid) {
        if (clicked && clicked->color() == m_currentPlayer) {
            m_selected = pos;
            m_moves.clear();

            auto rawMoves = clicked->validMoves(pos, m_board);
            for (const auto& m : rawMoves) {
                if (isLegalMove(m_currentPlayer, pos, m))
                    m_moves.push_back(m);
            }

            return MoveResult::Normal;
        }

        clearSelection();
        return MoveResult::Invalid;
    }

    // Extra veiligheid
    if (!isLegalMove(m_currentPlayer, m_selected, pos)) {
        clearSelection();
        return MoveResult::Invalid;
    }

    // Voer zet uit
    m_board.movePiece(m_selected, pos);
    promotePawn(pos);

    Color enemy = (m_currentPlayer == Color::White
                   ? Color::Black
                   : Color::White);

    bool check = isKingInCheck(enemy);
    bool mate  = isCheckmate(enemy);

    m_currentPlayer = enemy;
    clearSelection();

    if (mate)  return MoveResult::Checkmate;
    if (check) return MoveResult::Check;

    return MoveResult::Normal;
}

} // namespace Chess
