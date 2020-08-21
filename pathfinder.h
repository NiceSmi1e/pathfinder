#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "square.h"
#include <QQueue>
#include <QPair>
#include <QMap>
#include <QRandomGenerator>
#include <QSet>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QList>
#include <QMouseEvent>
#include <QSettings>
#include <QScrollBar>
#include <QtConcurrent/QtConcurrent>

QT_BEGIN_NAMESPACE
namespace Ui { class pathfinder; }
QT_END_NAMESPACE

class pathfinder : public QMainWindow
{
    Q_OBJECT

public:
    pathfinder(QWidget *parent = nullptr);
    ~pathfinder();

    Square *PointA;
    Square *PointB;

    typedef QPair<int,int> Location;
    typedef QMap<Location, Location> came_from_type;
    came_from_type came_from_final;
    QSet<Location> walls;
    void start_bfs();
    void breadth_first_search(Location start, Location goal);
    bool can_be_visited(int x, int y);
    bool is_wall(int x, int y);
    void generatePath(Location start, Location goal);
    QList<Location> path;
    void drawPath();
    void readSettings();

    int WEIGHT;
    int HEIGHT;
    int xA,yA;
    int xB,yB;
    bool path_found;

private slots:
    void on_pushButton_clicked();
    void wheelEvent(QWheelEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::pathfinder *ui;
    QGraphicsScene *gameScene;
};
#endif // PATHFINDER_H
