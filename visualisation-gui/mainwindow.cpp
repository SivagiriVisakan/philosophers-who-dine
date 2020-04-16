#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fork.h"

#include <QStyle>
#include <QDesktopWidget>
#include <QDebug>
#include <QPushButton>
#include <cmath>

// TODO: Remove this or make is meaningful
// Note: This is all for debug right now.
void MainWindow::onButtonClick()
{

    moveForkToPhilosopher(0, 0);
    moveForkToPhilosopher(0, 4);

//    moveForkToPhilosopher(1, 0);
    moveForkToPhilosopher(1, 1);

    forks[1]->moveToTable();

}

QPoint MainWindow::calculatePoint(int radius, double degree, const QWidget *w)
{
    int originX = centerPoint.x();
    int originY = centerPoint.y();

    int x = originX + radius*cos(degree*3.14/180);
    int y = originY + radius*sin(degree*3.14/180);

    if(w)
    {
        x -= w->width()/2;
        y -= w->height()/2;
    }
    return QPoint(x, y);
}

void MainWindow::moveForkToPhilosopher(int philosopherIndex, int forkIndex)
{
    Fork *fork = forks[forkIndex];

    int differenceRange = forkIndex % 2 == 0? 5:0; // The difference in degrees
    int r = 280;
    QPoint end = calculatePoint(r, philosopherDegrees[philosopherIndex] + differenceRange, fork);
    fork->move(end);
}

void MainWindow::drawPhilosophers()
{

    int r = philosophersRadius;
    int degree = 144;

    for(int i=0; i<5; i++)
    {
        philosopherDegrees[i] = degree;
        QPixmap bg = QPixmap();
        QString resourceName = QString();
        resourceName.sprintf(":/assets/assets/philosopher%d.png", i+1);
        bg.load(resourceName);

        philosophers[i] = new QLabel("test", ui->centralWidget);
        philosophers[i]->setPixmap(bg);
        philosophers[i]->setGeometry(0, 0, 70, 70);

        QPoint position = calculatePoint(r, degree, philosophers[i]);
        philosophers[i]->move(position);

        philosophers[i]->show();

        degree += 72;

    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        ));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onButtonClick()));

    this->centerPoint = ui->centerPoint->pos();
    setupDiningRoom();
}


void MainWindow::drawForksAndFood()
{
    int r = foodAndForksRadius;
    int degree = 180;

    for(int i=0; i<5; i++)
    {
        QPixmap bg = QPixmap();
        QString resourceName = QString();
        resourceName.sprintf(":/assets/assets/fork%d.png", i+1);
        bg.load(resourceName);

        forks[i] = new Fork(i, 0, 0, ui->centralWidget);
        forks[i]->setPixmap(bg);
        forks[i]->setGeometry(0, 0, 50, 50);


        QPoint end = calculatePoint(r, degree, forks[i]);
        forks[i]->setTableCoordinate(end.x(), end.y());
        forks[i]->move(end);

        forks[i]->show();

        degree += 72;
    }

    degree = 216;
    for(int i=0; i<5; i++)
    {
        QPixmap bg = QPixmap();
        QString resourceName = QString();
        resourceName.sprintf(":/assets/assets/food.png");
        bg.load(resourceName);

        foods[i] = new QLabel("food", ui->centralWidget);
        foods[i]->setPixmap(bg);
        foods[i]->setGeometry(0, 0, 70, 70);

        QPoint end = calculatePoint(r, degree, foods[i]);
        foods[i]->move(end);

        foods[i]->show();

        degree += 72;

    }

}

void MainWindow::setupDiningRoom()
{
    ui->table->move(width() / 2 - ui->table->width() / 2, height() / 2 - ui->table->height() / 2);
    ui->centerPoint->move(width() / 2 - ui->centerPoint->width() / 2, height() / 2 - ui->centerPoint->height() / 2);

    drawForksAndFood();
    drawPhilosophers();

}


MainWindow::~MainWindow()
{
    delete ui;
}


