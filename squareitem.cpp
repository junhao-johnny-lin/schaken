#include "squareitem.h"
#include <QGraphicsSceneMouseEvent>

SquareItem::SquareItem(int col, int row, qreal size, QGraphicsItem* parent)
    : QObject(), QGraphicsRectItem(parent), m_col(col), m_row(row)
{
    setRect(0, 0, size, size);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    // by default no brush here; MainWindow will set colours
}

void SquareItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    // Vraag 41: useful Qt class
    emit clicked(QPoint(m_col, m_row));
    QGraphicsRectItem::mousePressEvent(event);
}
