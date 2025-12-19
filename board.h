#ifndef BOARD_H
#define BOARD_H


#include <QPoint>
#include <array>

// Vraag 26: alles zit in een zelfgemaakte namespace (Chess)
namespace Chess {

class Piece;

class Board {
     // Vraag 25: useful friend class
  friend class GameController;
public:
    // Vraag 2: no globals — SIZE is static binnen de klasse i.p.v. globale variabele
    // Vraag 27: memory-efficient type
    static constexpr unsigned char SIZE = 8;

    // Vraag 11: default constructor
    Board();
    Board(const Board& other);                  // deep copy constructor
    Board& operator=(const Board& other);       // deep copy assignment
    ~Board();

    // Vraag 21: default values in function definition
    //vraag 28: const references for variables
//vraag 29:const references for functions
    bool isInside(const QPoint &pos, int size = SIZE) const;

    // Vraag 30:useful bool
    bool isEmpty(const QPoint &pos) const;
    Piece* pieceAt(const QPoint &pos) const;
    void setPieceAt(const QPoint &pos, Piece* piece);

    bool movePiece(const QPoint &from, const QPoint &to);

    bool hasEnemyAt(const QPoint &pos, Piece* piece) const;
    bool hasEnemyAt(const QPoint &pos, int color) const;

    void clear();
    void setupInitialPosition();

private:
    Piece* m_grid[SIZE][SIZE];

    Piece* clonePiece(const Piece* p) const;   // helper for deep copy
};

} // namespace Chess

#endif // BOARD_H
