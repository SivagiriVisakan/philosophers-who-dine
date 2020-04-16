#include "fork.h"
#include <QPropertyAnimation>

Fork::Fork(int id, int tableX, int tableY, QWidget *parent):QLabel (parent)
{
    coordinateOnTable = QPoint(tableX,tableY);
    this->id = id;
    move(coordinateOnTable);
}

void Fork::moveToPoint(QPoint p)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    QRect end = QRect(p.x(), p.y(), this->width(), this->height());

    animation->setStartValue(this->geometry());
    animation->setEndValue(end);
    animation->setDuration(500);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Fork::setTableCoordinate(int x, int y)
{
    coordinateOnTable = QPoint(x, y);
}
void Fork::moveToTable()
{
    moveToPoint(coordinateOnTable);
}

void Fork::moveToPoint(int x, int y)
{
    moveToPoint(QPoint(x, y));
}
