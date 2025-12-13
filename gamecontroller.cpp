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

bool GameController::isKingInCheck(Color color) const
{
    QPoint kingPos(-1, -1);

    // Vind de koning
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

    // Vijandelijke zetten
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

bool GameController::isCheckmate(Color color)
{
    if (!isKingInCheck(color))
        return false;

    // Test alle mogelijke zetten
    for (int y = 0; y < Board::SIZE; y++) {
        for (int x = 0; x < Board::SIZE; x++) {
            Piece* p = m_board.pieceAt(QPoint(x,y));
            if (!p || p->color() != color)
                continue;

            auto moves = p->validMoves(QPoint(x,y), m_board);
            for (const QPoint& dest : moves) {

                Board copy = m_board;
                copy.movePiece(QPoint(x,y), dest);

                // Zoek koning opnieuw
                QPoint kp(-1, -1);
                for (int yy = 0; yy < Board::SIZE; yy++) {
                    for (int xx = 0; xx < Board::SIZE; xx++) {
                        Piece* pk = copy.pieceAt(QPoint(xx,yy));
                        if (pk && pk->color() == color &&
                            (pk->symbol() == 'K' || pk->symbol() == 'k')) {
                            kp = QPoint(xx,yy);
                        }
                    }
                }

                // Test of vijand nog steeds kan slaan
                bool stillCheck = false;
                for (int yy = 0; yy < Board::SIZE; yy++) {
                    for (int xx = 0; xx < Board::SIZE; xx++) {
                        Piece* enemy = copy.pieceAt(QPoint(xx,yy));
                        if (!enemy || enemy->color() == color)
                            continue;
                        auto mv = enemy->validMoves(QPoint(xx,yy), copy);
                        for (auto& m : mv)
                            if (m == kp)
                                stillCheck = true;
                    }
                }

                if (!stillCheck)
                    return false;
            }
        }
    }

    return true;
}

void GameController::promotePawn(const QPoint &pos)
{
    Piece* p = m_board.pieceAt(pos);
    if (!p) return;

    if ((p->symbol() == 'P' && pos.y() == 0) ||
        (p->symbol() == 'p' && pos.y() == 7)) {

        m_board.setPieceAt(pos, new Queen(p->color()));
       //delete p;
    }
}

MoveResult GameController::handleSquareClick(const QPoint& pos)
{
    Piece* clicked = m_board.pieceAt(pos);

    // Geen selectie?
    if (m_selected.x() == -1) {
        if (!clicked) return MoveResult::Invalid;
        if (clicked->color() != m_currentPlayer) return MoveResult::Invalid;

        m_selected = pos;
        m_moves = clicked->validMoves(pos, m_board);
        return MoveResult::Normal;
    }

    // Check geldige zet
    bool valid = false;
    for (const auto& m : m_moves)
        if (m == pos)
            valid = true;

    if (!valid) {
        // Nieuw stuk selecteren?
        if (clicked && clicked->color() == m_currentPlayer) {
            m_selected = pos;
            m_moves = clicked->validMoves(pos, m_board);
            return MoveResult::Normal;
        }
        // anders deselect
        clearSelection();
        return MoveResult::Invalid;
    }

    // Voer zet uit
    m_board.movePiece(m_selected, pos);
    promotePawn(pos);

    Color enemy = (m_currentPlayer == Color::White ? Color::Black : Color::White);

    bool check = isKingInCheck(enemy);
    bool mate  = isCheckmate(enemy);

    m_currentPlayer = enemy;
    clearSelection();

    if (mate)  return MoveResult::Checkmate;
    if (check) return MoveResult::Check;

    return MoveResult::Normal;
}

} // namespace Chess
