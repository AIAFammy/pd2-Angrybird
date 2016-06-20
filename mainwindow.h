#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>
#include <QPushButton>
#include <QMediaPlayer>

#include <gameitem.h>
#include <land.h>
#include <bird.h>
#include <rock.h>
#include <rock_i.h>
#include <pig.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *,QEvent *event);
    void closeEvent(QCloseEvent *);
    void gameover();

signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    // For debug slot
    void QUITSLOT();
    void restartGame();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;

    int timecount;
    int timebin;
    bool Isprepare;
    bool Isclick;
    bool Isspeed;
    bool canspeed;
    bool Issplit;
    bool cansplit;
    bool Isover;
    bool Isop;

    QGraphicsPixmapItem* bullet;
    QGraphicsLineItem* line1;
    QGraphicsLineItem* line2;
    QGraphicsPixmapItem* score;
    QGraphicsPixmapItem* scoreboard;
    QGraphicsPixmapItem* star;
    QGraphicsPixmapItem* fscore;
    QPushButton* restartbutton;
    QGraphicsProxyWidget* restart;
    QPushButton* quitbutton;
    QGraphicsProxyWidget* quit;
    int number;

    Bird *birdie1,*birdie2,*birdie3,*birdie4,*birdie5,*birdie6;
    Pig *pigie;
    Rock *rock4,*rock5,*rock8;
    Rock_i *rock1,*rock2,*rock3,*rock6,*rock7;

    QMediaPlayer *bgm,*ladangone,*flying,*overm,*tnt;
};

#endif // MAINWINDOW_H
