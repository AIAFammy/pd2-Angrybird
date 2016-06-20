#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Isprepare = true;
    Isclick = false;
    Isspeed = false;
    canspeed = true;
    Issplit = false;
    cansplit = true;
    Isop = false;
    Isover = false;
    number = 0;
    timecount = 0;

    ladangone = new QMediaPlayer();
    ladangone->setMedia(QUrl("qrc:/music/slingshot.wav"));
    ladangone->setVolume(150);
    bgm = new QMediaPlayer();
    bgm->setMedia(QUrl("qrc:/music/title_theme.mp3"));
    bgm->setVolume(70);
    flying = new QMediaPlayer();
    flying->setMedia(QUrl("qrc:/music/flying.wav"));
    flying->setVolume(100);
    overm = new QMediaPlayer();
    overm->setMedia(QUrl("qrc:/music/game_complete.mp3"));
    overm->setVolume(80);
    tnt = new QMediaPlayer();
    tnt->setMedia(QUrl("qrc:/music/tnt.wav"));
    tnt->setVolume(150);
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);  
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));
    bgm->play();
    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());
    // Create ground (You can edit here)
    QGraphicsPixmapItem* background = scene->addPixmap(QPixmap(":/image/back.png").scaled(1000,600));
    background->setPos(0, 0);
    itemList.push_back(new Land(16,1.5,32,3,QPixmap(":/ground.png").scaled(width(),height()/6.0),world,scene));
    // Create rock item
    rock1 = new Rock_i(18.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock2 = new Rock_i(23.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock3 = new Rock_i(28.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock4 = new Rock(19.8f,12.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    rock5 = new Rock(25.9f,12.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    rock6 = new Rock_i(20.5f,15.5f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock7 = new Rock_i(25.5f,15.5f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock8 = new Rock(22.9f,20.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    // Create bird (You can edit here)
    pigie = new Pig(22.5f,15.0f,0.27f,&timer,QPixmap(":/image/pig.png").scaled(height()/9.0,height()/9.0),world,scene);
    //pigie = new Pig(10.5f,5.0f,0.27f,&timer,QPixmap(":/image/pig.png").scaled(height()/9.0,height()/9.0),world,scene);
    // Setting the Velocity
    //birdie->setLinearVelocity(b2Vec2(20,0));
    //itemList.push_back(birdie);

    // Create dangone part
    QGraphicsPixmapItem* dangone = scene->addPixmap(QPixmap(":/image/dangone.png").scaled(50,300));
    dangone->setPos(148, 151);
    line1 = scene->addLine(-10,-10,-10,-10,QPen(Qt::blue,20));
    bullet = scene->addPixmap(QPixmap(":/bird.png").scaled(height()/9.0,height()/9.0));
    bullet->setPos(150,170);
    line2 = scene->addLine(-10,-10,-10,-10,QPen(Qt::black,20));
    QGraphicsPixmapItem* dangone_front = scene->addPixmap(QPixmap(":/image/dangone_front.png").scaled(50,300));
    dangone_front->setPos(148, 151);

    // End part
    score = scene->addPixmap(QPixmap(":/image/zeroscore.png").scaled(200,40));
    score->setPos(250,20);

    scoreboard = scene->addPixmap(QPixmap(":/image/scoreboard_1.png").scaled(1000,600));
    scoreboard->setPos(0,0);
    scoreboard->hide();

    star = scene->addPixmap(QPixmap(":/image/0star.png").scaled(100,30));
    star->setPos(475,325);
    star->hide();

    fscore = scene->addPixmap(QPixmap(":/image/score0.png").scaled(100,30));
    fscore->setPos(320,325);
    fscore->hide();

    quitbutton = new QPushButton();
    quitbutton->setGeometry(QRect(482,103,70,50));
    quitbutton->setText("Quit");
    quit = scene->addWidget(quitbutton);
    connect(quitbutton,SIGNAL(clicked()),this,SLOT(QUITSLOT()));
    quitbutton->hide();

    restartbutton = new QPushButton();
    restartbutton->setGeometry(QRect(350,103,70,50));
    restartbutton->setText("Restart");
    restart = scene->addWidget(restartbutton);
    connect(restartbutton,SIGNAL(clicked()),this,SLOT(restartGame()));
    restartbutton->hide();

    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    // Record mouse position
    QMouseEvent *mEvent = (QMouseEvent*)event;
    float x = mEvent->x();
    float y = mEvent->y();
    // Hint: Notice the Number of every event!
    /******************************************************************************************/
    if(event->type() == QEvent::MouseButtonPress)
    {
        /* TODO : add your code here */
        //std::cout << "Press !" << std::endl ;
        if(x>160&&x<195&&y>180&&y<225&& Isprepare && number<4) //鼠標在彈弓上
        {
            ladangone->play();
            Isclick = true;
            Isprepare = false;
            line1->setLine(-10,-10,-10,-10);
            line2->setLine(-10,-10,-10,-10);
            line1->show();
            line2->show();
        }
        if(number == 2 && !Isspeed && canspeed)
        {
            birdie2->setLinearVelocity(b2Vec2(birdie2->vel().x*5,birdie2->vel().y*5));
            Isspeed = true;
        }
        if(number == 3 && !Issplit && cansplit)
        {
            birdie5 = new Bird(birdie3->pos().x,birdie3->pos().y,0.27f,&timer,QPixmap(":/image/splitbird.png").scaled(height()/9.0,height()/9.0),world,scene);
            birdie6 = new Bird(birdie3->pos().x,birdie3->pos().y,0.27f,&timer,QPixmap(":/image/splitbird.png").scaled(height()/9.0,height()/9.0),world,scene);
            birdie5->setLinearVelocity(b2Vec2(birdie3->vel().x*2,birdie3->vel().y));
            birdie6->setLinearVelocity(b2Vec2(birdie3->vel().x*2,birdie3->vel().y-10));
            birdie3->setLinearVelocity(b2Vec2(birdie3->vel().x*2,birdie3->vel().y+10));
            Issplit = true;
        }
        if(number == 4 && !Isop)
        {
            birdie4->setLinearVelocity(b2Vec2((x/33-birdie4->pos().x)*5,-30));
            Isop = true;
        }
    }
    /******************************************************************************************/
    if(event->type() == QEvent::MouseMove)
    {
        /* TODO : add your code here */
        //std::cout << "Move !" << std::endl ;
        if(Isclick && number<4)
        {
            line2->setLine(x,y,195,203);
            bullet->setPos(x-30,y-30);
            line1->setLine(x,y,165,203);
        }
        //std::cout << "(" << (180-x)/10 << ","<< -(203-y)/10 << ")" <<std::endl ;
        //std::cout << x << "," << y << std::endl ;

    }
    /******************************************************************************************/
    if(event->type() == QEvent::MouseButtonRelease)
    {
        /* TODO : add your code here */
        //std::cout << "Release !" << std::endl ;
        if(Isclick)
        {
            timebin = timecount;
            number++;
            switch(number)
            {
                case 1:
                {
                        birdie1 = new Bird(5.0f,12.0f,0.27f,&timer,QPixmap(":/bird.png").scaled(height()/9.0,height()/9.0),world,scene);
                        birdie1->setLinearVelocity(b2Vec2((180-x)/10,-(203-y)/10));
                        itemList.push_back(birdie1);
                        break;
                }
                case 2:
                {
                        birdie2 = new Bird(5.0f,12.0f,0.27f,&timer,QPixmap(":/image/speedbird.png").scaled(height()/9.0,height()/9.0),world,scene);
                        birdie2->setLinearVelocity(b2Vec2((180-x)/10,-(203-y)/10));
                        itemList.push_back(birdie2);
                        break;
                }
                case 3:
                {
                        birdie3 = new Bird(5.0f,12.0f,0.27f,&timer,QPixmap(":/image/splitbird.png").scaled(height()/8.0,height()/8.0),world,scene);
                        birdie3->setLinearVelocity(b2Vec2((180-x)/10,-(203-y)/10));
                        itemList.push_back(birdie3);
                        break;
                }
                case 4:
                {
                        birdie4 = new Bird(5.0f,12.0f,0.27f,&timer,QPixmap(":/image/opbird.png").scaled(height()/9.0,height()/9.0),world,scene);
                        birdie4->setLinearVelocity(b2Vec2((180-x)/10,-(203-y)/10));
                        itemList.push_back(birdie4);
                        break;
                }
            }
            switch(number)
            {
                case 1: bullet->setPixmap(QPixmap(":/image/speedbird.png").scaled(height()/9.0,height()/9.0));break;
                case 2: bullet->setPixmap(QPixmap(":/image/splitbird.png").scaled(height()/9.0,height()/9.0));break;
                case 3: bullet->setPixmap(QPixmap(":/image/opbird.png").scaled(height()/9.0,height()/9.0));break;
                case 4: bullet->hide();
            }
            bullet->hide();
            bullet->setPos(150,170);
            line1->hide();
            line2->hide();
            flying->play();
        }
        Isclick = false;
    }
    /******************************************************************************************/
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    timecount++;
    //碰撞後pig消失
    if((timecount - timebin > 10))
    switch(number)
    {
        case 4:
        {
            if(fabs(birdie4->pos().x-pigie->pos().x)<0.8 && fabs(birdie4->pos().y-pigie->pos().y)<0.8)
            {
                pigie->deleteLater();
                Isover = true;
                score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                tnt->play();
            }
        }
        case 3:
        {
            if(Issplit)
            {
                if(fabs(birdie3->pos().x-pigie->pos().x)<0.8 && fabs(birdie3->pos().y-pigie->pos().y)<0.8)
                {
                    pigie->deleteLater();
                    Isover = true;
                    score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                    tnt->play();
                }
                if(fabs(birdie5->pos().x-pigie->pos().x)<0.8 && fabs(birdie5->pos().y-pigie->pos().y)<0.8)
                {
                    pigie->deleteLater();
                    Isover = true;
                    score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                    tnt->play();
                }
                if(fabs(birdie6->pos().x-pigie->pos().x)<0.8 && fabs(birdie6->pos().y-pigie->pos().y)<0.8)
                {
                    pigie->deleteLater();
                    Isover = true;
                    score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                    tnt->play();
                }
            }
            else
            {
                if(fabs(birdie3->pos().x-pigie->pos().x)<0.8 && fabs(birdie3->pos().y-pigie->pos().y)<0.8)
                {
                    pigie->deleteLater();
                    Isover = true;
                    score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                    tnt->play();
                }
            }
        }
        case 2:
        {
            if(fabs(birdie2->pos().x-pigie->pos().x)<0.8 && fabs(birdie2->pos().y-pigie->pos().y)<0.8)
            {
                pigie->deleteLater();
                Isover = true;
                score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                tnt->play();
            }
        }
        case 1:
        {
            if(fabs(birdie1->pos().x-pigie->pos().x)<0.8 && fabs(birdie1->pos().y-pigie->pos().y)<0.8)
            {
                pigie->deleteLater();
                Isover = true;
                score->setPixmap(QPixmap(":/image/fullscore.png").scaled(200,40));
                tnt->play();
            }
            break;
        }
    }
    if(Isover) gameover();
    //射出bird停下後或出界，下一隻才可發射
    if((timecount - timebin > 50) && !Isprepare)
    {
        switch(number)
        {
            case 1:
            {
                if(fabs(birdie1->vel().x)<0.2 && fabs(birdie1->vel().y)<0.2)
                {
                    Isprepare = true;
                    timebin = 99999999;
                }
                else if(birdie1->pos().x<-1 || birdie1->pos().x>35)
                {
                    Isprepare = true;
                    timebin = 99999999;
                }
                break;
            }
            case 2:
            {
                if(fabs(birdie2->vel().x)<0.05 && fabs(birdie2->vel().y)<0.05)
                {
                    Isprepare = true;
                    canspeed = false;
                    timebin = 99999999;
                }
                else if(birdie2->pos().x<-1 || birdie2->pos().x>35)
                {
                    Isprepare = true;
                    canspeed = false;
                    timebin = 99999999;
                }
                break;
            }
            case 3:
            {
                if(Issplit)
                {
                    if(((fabs(birdie3->vel().x)<0.05&&fabs(birdie3->vel().y)<0.05)||(birdie3->pos().x<-1||birdie3->pos().x>35)) &&
                       ((fabs(birdie5->vel().x)<0.05&&fabs(birdie5->vel().y)<0.05)||(birdie5->pos().x<-1||birdie5->pos().x>35)) &&
                       ((fabs(birdie6->vel().x)<0.05&&fabs(birdie6->vel().y)<0.05)||(birdie6->pos().x<-1||birdie6->pos().x>35)))
                    {
                        Isprepare = true;
                        timebin = 99999999;
                    }
                }
                else
                {
                    if(fabs(birdie3->vel().x)<0.05 && fabs(birdie3->vel().y)<0.05)
                    {
                        Isprepare = true;
                        cansplit = false;
                        timebin = 99999999;
                    }
                    else if(birdie3->pos().x<-1 || birdie3->pos().x>35)
                    {
                        Isprepare = true;
                        cansplit = false;
                        timebin = 99999999;
                    }
                }
                break;
            }
            case 4:
            {
                if(fabs(birdie4->vel().x)==0 && fabs(birdie4->vel().y)==0)
                {
                    Isprepare = true;
                    timebin = 99999999;
                }
                else if(birdie4->pos().x<0 || birdie4->pos().x>30)
                {
                    Isprepare = true;
                    timebin = 99999999;
                }
                break;
            }
        }
    }
    if(Isprepare && number<4) bullet->show();
    if(Isprepare && Isover) gameover();
    else if(Isprepare && number==4)
    {
        number++;
        gameover();
    }
    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}

void MainWindow::gameover()
{
    bgm->stop();
    overm->play();
    switch(number)
    {
        case 5:
        {
            timer.stop();
            scoreboard->show();
            star->show();
            fscore->show();
            quitbutton->show();
            restartbutton->show();
            break;
        }
        case 4:
        {
            timer.stop();
            scoreboard->show();
            star->setPixmap(QPixmap(":/image/1star.png").scaled(100,30));
            star->show();
            fscore->setPixmap(QPixmap(":/image/score10000.png").scaled(100,30));
            fscore->show();
            quitbutton->show();
            restartbutton->show();
            break;
        }
        case 3:
        {
            timer.stop();
            scoreboard->show();
            star->setPixmap(QPixmap(":/image/2star.png").scaled(100,30));
            star->show();
            fscore->setPixmap(QPixmap(":/image/score10000.png").scaled(100,30));
            fscore->show();
            quitbutton->show();
            restartbutton->show();
            break;
        }
        case 2:
        {
            timer.stop();
            scoreboard->show();
            star->setPixmap(QPixmap(":/image/3star.png").scaled(100,30));
            star->show();
            fscore->setPixmap(QPixmap(":/image/score10000.png").scaled(100,30));
            fscore->show();
            quitbutton->show();
            restartbutton->show();
            break;
        }
    }
}

void MainWindow::restartGame()
{
    switch(number)
    {
        case 2:
        {
            birdie1->deleteLater();
            birdie2->deleteLater();
            break;
        }
        case 3:
        {
            birdie1->deleteLater();
            birdie2->deleteLater();
            birdie3->deleteLater();
            if(Issplit)
            {
                birdie5->deleteLater();
                birdie6->deleteLater();
            }
            break;
        }
        case 4: case 5:
        {
            birdie1->deleteLater();
            birdie2->deleteLater();
            birdie3->deleteLater();
            if(Issplit)
            {
                birdie5->deleteLater();
                birdie6->deleteLater();
            }
            birdie4->deleteLater();
            break;
        }
    }
    scoreboard->hide();
    star->hide();
    fscore->hide();
    quitbutton->hide();
    restartbutton->hide();
    rock1->deleteLater();
    rock2->deleteLater();
    rock3->deleteLater();
    rock4->deleteLater();
    rock5->deleteLater();
    rock6->deleteLater();
    rock7->deleteLater();
    rock8->deleteLater();
    rock1 = new Rock_i(18.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock2 = new Rock_i(23.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock3 = new Rock_i(28.0f,6.0f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock4 = new Rock(19.8f,12.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    rock5 = new Rock(25.9f,12.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    rock6 = new Rock_i(20.5f,15.5f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock7 = new Rock_i(25.5f,15.5f,0.5f,3.0f,&timer,QPixmap(":/image/rock.png").scaled(width()/25,height()/2.84),world,scene);
    rock8 = new Rock(22.9f,20.0f,3.0f,0.5f,&timer,QPixmap(":/image/rock.png").scaled(195,42.87),world,scene);
    if(number==5) pigie->deleteLater();
    pigie = new Pig(22.5f,15.0f,0.27f,&timer,QPixmap(":/image/pig.png").scaled(height()/9.0,height()/9.0),world,scene);
    Isprepare = true;
    Isclick = false;
    Isspeed = false;
    canspeed = true;
    Issplit = false;
    cansplit = true;
    Isop = false;
    Isover = false;
    number = 0;
    timecount = 0;
    score->setPixmap(QPixmap(":/image/zeroscore.png").scaled(200,40));
    bullet->setPixmap(QPixmap(":/bird.png").scaled(height()/9.0,height()/9.0));
    star->setPixmap(QPixmap(":/image/0star.png").scaled(100,30));
    fscore->setPixmap(QPixmap(":/image/score0.png").scaled(100,30));
    overm->stop();
    bgm->play();
    timer.start(100/6);
}
