#ifndef TEST_BOARD_H
#define TEST_BOARD_H

#include <QtTest/QtTest>
#include "board.h"

class TestBoard : public QObject
{
    Q_OBJECT

private slots:
    void testMovePiece();
    void testIsInside();
    void testIsEmpty();
    void testPieceCapture();
};

#endif // TEST_BOARD_H
