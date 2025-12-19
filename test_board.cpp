#include "test_board.h"
#include "piece.h"
#include "board.h"

using namespace Chess;

void TestBoard::testMovePiece()
{
    Board b;

    // startpositie pion wit
    Piece* p = b.pieceAt(QPoint(6,0));
    QVERIFY(p != nullptr);

    // valide zet
    QVERIFY(b.movePiece(QPoint(6,0), QPoint(4,0)) == true);
    QVERIFY(b.pieceAt(QPoint(4,0)) != nullptr);
    QVERIFY(b.pieceAt(QPoint(6,0)) == nullptr);

    // ongeldige zet buiten bord
    QVERIFY(b.movePiece(QPoint(4,0), QPoint(8,8)) == false);
}

void TestBoard::testIsInside()
{
    Board b;
    QVERIFY(b.isInside(QPoint(0,0)) == true);
    QVERIFY(b.isInside(QPoint(7,7)) == true);
    QVERIFY(b.isInside(QPoint(-1,0)) == false);
    QVERIFY(b.isInside(QPoint(0,8)) == false);
}

void TestBoard::testIsEmpty()
{
    Board b;
    QVERIFY(b.isEmpty(QPoint(3,3)) == true); // middenveld leeg
    QVERIFY(b.isEmpty(QPoint(6,0)) == false); // startpositie pion wit
}

void TestBoard::testPieceCapture()
{
    Board b;
    Piece* whitePawn = b.pieceAt(QPoint(6,0));
    QVERIFY(whitePawn != nullptr);

    Piece* blackPawn = b.pieceAt(QPoint(1,0));
    QVERIFY(blackPawn != nullptr);
    b.movePiece(QPoint(1,0), QPoint(3,0));

    QVERIFY(b.hasEnemyAt(QPoint(3,0), whitePawn) == true);

    // gebruik Chess::Color als namespace prefix
    QVERIFY(b.movePiece(QPoint(6,0), QPoint(3,0)) == true);
    QVERIFY(b.pieceAt(QPoint(3,0))->color() == Chess::Color::White);
}
