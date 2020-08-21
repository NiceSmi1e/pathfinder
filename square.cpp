#include "square.h"
#include "pathfinder.h"

extern pathfinder *Pathfinder;

Square::Square(QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    //making Square
    setRect(0,0,50,50);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-1);
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!Pathfinder->PointA && !this->checkIfWall()) //checking if we can choose square as point A
    {
        Pathfinder->PointA = this;
        Pathfinder->PointA->setColor(Qt::green); //setting point A green
        QString A_coords = Pathfinder->PointA->toolTip();
        QStringList A_coords_list = A_coords.split(" ");
        Pathfinder->xA = A_coords_list[0].toInt();
        Pathfinder->yA = A_coords_list[1].toInt();

    }
    else if (!Pathfinder->PointB && this != Pathfinder->PointA && !this->checkIfWall()) //checking if we can choose square as point B
    {
        Pathfinder->PointB = this;
        Pathfinder->PointB->setColor(Qt::green); //setting point B green

        QString B_coords = Pathfinder->PointB->toolTip();
        QStringList B_coords_list = B_coords.split(" ");
        Pathfinder->xB = B_coords_list[0].toInt();
        Pathfinder->yB = B_coords_list[1].toInt();

        Pathfinder->start_bfs();

        if (Pathfinder->path_found) { //if the path is found then generating it and drawing it
            Pathfinder->generatePath({Pathfinder->xA,Pathfinder->yA},
                                     {Pathfinder->xB,Pathfinder->yB});
            Pathfinder->drawPath();
        }
    }
}

void Square::setColor(QColor color)
{
    brush.setColor(color);
    setBrush(color);
}

void Square::setAsWall()
{
    this->isWall = true;
}

void Square::setAsNotWall()
{
    this->isWall = false;
}

bool Square::checkIfWall()
{
    return this->isWall;
}

Square::~Square()
{
    setAsNotWall();
}
