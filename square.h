#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QBrush>


class Square:public QGraphicsRectItem
{
public:
    //Constructor
    Square(QGraphicsItem *parent=0);
    ~Square();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void setColor(QColor color);
    void setAsWall();
    void setAsNotWall();
    bool checkIfWall();

private:
    QBrush brush;
    bool isWall;
};

#endif // SQUARE_H
