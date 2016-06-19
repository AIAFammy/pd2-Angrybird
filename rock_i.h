#ifndef ROCK_I_H
#define ROCK_I_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define ROCK_DENSITY 4.0f
#define ROCK_FRICTION 0.5f
#define ROCK_RESTITUTION 0.3f

class Rock_i : public GameItem
{
public:
    Rock_i(float x, float y, float w, float h, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

#endif // ROCK_I_H
