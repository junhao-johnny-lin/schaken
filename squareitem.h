#ifndef SQUAREITEM_H
#define SQUAREITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QPoint>

class SquareItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    SquareItem(int col, int row, qreal size, QGraphicsItem* parent = nullptr);

    int column() const { return m_col; }
    int row() const { return m_row; }

signals:
    void clicked(const QPoint& pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_col;
    int m_row;
};

#endif // SQUAREITEM_H
