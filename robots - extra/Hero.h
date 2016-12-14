/*
 * jenli414 och sabse455
 */

/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef HERO_H
#define HERO_H

#include "Unit.h"
#include <QGraphicsScene>

class Hero : public Unit {
public:

    /*
     * Constructor.
     */
    explicit Hero();

    /*
     * Constructor that takes a point.
     */
    explicit Hero(const Point& p);

    /*
    * Draws this hero onto the given QGraphicsScene.
    */
    void draw(QGraphicsScene *scene) const override;

};

#endif // HERO_H
