#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "fork.h"
#include "../philsophers_base.h"

#define TOTAL_PHILOSOPHERS 5

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
    void checkForStatusUpdates();

private:
    Ui::MainWindow *ui;
    Fork *forks[5];
    QLabel *foods[5];
    QLabel *philosophers[5];
    QPoint centerPoint; // The center of the screen.
    QTimer *timer; // To update the GUI periodically
    int philosopherDegrees[5]; //  The degree relative to center at which the philosophers are placed.

    bool isVisualisationEnabled; // Visualisation will update only if this is enabled

    struct philosopher_state *philosophers_state_info;
    struct fork_info *forks_state_info;

    const int philosophersRadius = 250; // The radius from the center of the table at which philosophers are placed.
    const int foodAndForksRadius = 120; // The radius from the center of the table at which the forks and food plates are placed.

    void setupDiningRoom();
    void moveForkToPhilosopher(int philosopherIndex, int forkIndex);
    void drawForksAndFood();
    void drawPhilosophers();
    QPoint calculatePoint(int radius, double degree, const QWidget *w=nullptr);
    void initSharedMemory();
};

#endif // MAINWINDOW_H
