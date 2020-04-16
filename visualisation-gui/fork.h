#ifndef FORK_H
#define FORK_H

#include <QLabel>

class Fork : public QLabel
{
public:
    Fork(int id, int tableX=0, int tableY=0, QWidget *parent=nullptr);
    void moveToPoint(QPoint p);
    void moveToTable();
    void moveToPoint(int x, int y);
    void setTableCoordinate(int x, int y);
private:
    QPoint coordinateOnTable;
    int id;
};

#endif // FORK_H
