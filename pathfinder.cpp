#include "pathfinder.h"
#include "ui_pathfinder.h"
#include "square.h"

QSettings settings("pathfinder.ini", QSettings::IniFormat); //choosing settings file

pathfinder::pathfinder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::pathfinder)
{
    ui->setupUi(this);

    readSettings();
    //Making the Scene
    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0,0,100,100);

    ui->graphicsView->setScene(gameScene);
}

pathfinder::~pathfinder()
{
    delete ui;
}

//When generate button is clicked
void pathfinder::on_pushButton_clicked()
{
    QRandomGenerator generator = QRandomGenerator::securelySeeded();
    gameScene->clear(); //clearing scene
    came_from_final.clear(); //clearing map
    walls.clear(); //clearing walls
    int SHIFT = 50;
    int w = ui->le_width->text().toInt();
    int h = ui->le_height->text().toInt();
    if (w < 1 || w > 500 || h < 1 || h > 500) {
        QMessageBox msgBox;
        msgBox.setText("Ширина и высота должны быть в пределах от 1 до 500");
        msgBox.setWindowTitle("Ошибка");
        msgBox.exec();
        return;
    }
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Square *sq = new Square();
            sq->setPos(i*SHIFT,j*SHIFT);
            sq->setToolTip(QString("%1 %2").arg(i).arg(j));
            gameScene->addItem(sq);

            int randint = generator.generateDouble() * 7; //generating number from 0 to 7
            if (randint == 6) { //adding random "walls" (1/8 probability)
                sq->setColor(Qt::black);
                walls.insert({i,j}); //adding wall to the list
                sq->setAsWall();
            }
            else {
                sq->setAsNotWall();
            }
        }
        qApp->processEvents();
    }
    gameScene->setSceneRect(0,0,w*SHIFT,h*SHIFT);
    WEIGHT = w;
    HEIGHT = h;
    PointA = nullptr;
    PointB = nullptr;
}

//Starting breadth_first_search in a new multi-threaded pool
void pathfinder::start_bfs()
{
    QThreadPool pool;
    Location lA = {xA,yA};
    Location lB = {xB,yB};
    QtConcurrent::run(&pool,this,&pathfinder::breadth_first_search,lA,lB);
    while (pool.waitForDone(250)==false) //проверка завершения функций в многопоточности
    {
        qApp->processEvents();
    }
    if (!path_found) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось найти путь от A до B");
        msgBox.setWindowTitle("Неудача");
        msgBox.exec();
    }
}

//Breadth First Search (which stops as soon as we are reaching our B point)
void pathfinder::breadth_first_search(Location start, Location goal) {
    path_found = false;
    QQueue<Location> frontier;
    frontier.enqueue(start); //putting start location in the queue

    QMap<Location, Location> came_from;
    came_from[start] = start;

    while (!frontier.empty()) { //when we still have locations in queue
        Location current = frontier.dequeue();

        if (current == goal) { //stopping if we just found the goal
            path_found = true;
            break;
        }

        QVector<Location> moves {
            {1, 0}, {0, -1}, {-1, 0}, {0, 1},
        };

        int x = current.first;
        int y = current.second;
        for (auto move: moves) {
            int dx = move.first; int dy = move.second;
            if (!came_from.contains({x + dx, y + dy}) && can_be_visited(x + dx,y + dy)) {
                frontier.append({x + dx, y + dy});
                came_from[{x + dx, y + dy}] = current;
            }
        }
        qApp->processEvents();
    }
    if (path_found) {
        came_from_final = came_from;
    }
}

//Generating path from A to B
void pathfinder::generatePath(Location start, Location goal) {
    Location current = goal;
    path.clear();
    path.append(current);
    while (current != start) {
        current = came_from_final[current];
        path.append(current);
    }
    //path.append(start);
}

//Drawing path from A to B
void pathfinder::drawPath() {
    QPen red_pen;
    red_pen.setColor(Qt::red);
    red_pen.setWidth(15);
    for (int i = 0; i < path.size()-1; i++) {
        int x1 = path[i].first;
        int y1 = path[i].second;
        int x2 = path[i+1].first;
        int y2 = path[i+1].second;
        gameScene->addLine(x1*50+25,y1*50+25,x2*50+25,y2*50+25, red_pen);
    }
}

//Checking if square can be visited
bool pathfinder::can_be_visited(int x, int y) {
    return x >= 0 && x < WEIGHT
        && y >= 0 && y < HEIGHT
        && !is_wall(x,y);
}

//Checking if square is a wall
bool pathfinder::is_wall(int x, int y) {
    return walls.contains({x,y});
}

//Zooming on mouse wheel
void pathfinder::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
        ui->graphicsView->scale(1.25, 1.25);
    else
        ui->graphicsView->scale(0.8, 0.8);
}

//Saving window geometry when closing app
void pathfinder::closeEvent(QCloseEvent *event)
{
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

//Restoring window geometry
void pathfinder::readSettings()
{
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}
