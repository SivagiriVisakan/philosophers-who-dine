#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "fork.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onButtonClick();
private:
    Ui::MainWindow *ui;
    Fork *forks[5];
    QLabel *foods[5];
    QLabel *philosophers[5];
    QPoint centerPoint; // The center of the screen.
    int philosopherDegrees[5]; //  The degree relative to center at which the philosophers are placed.
    const int philosophersRadius = 250; // The radius from the center of the table at which philosophers are placed.
    const int foodAndForksRadius = 120; // The radius from the center of the table at which the forks and food plates are placed.

    void setupDiningRoom();
    void moveForkToPhilosopher(int philosopherIndex, int forkIndex);
    void drawForksAndFood();
    void drawPhilosophers();
    QPoint calculatePoint(int radius, double degree, const QWidget *w=nullptr);
};

#endif // MAINWINDOW_H
