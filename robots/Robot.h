/*
 * jenli414 och sabse455
 */

/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"
#include <QGraphicsScene>

class Robot : public Unit {
public:

    /*
     * Contructor
     */
    Robot();

    /*
     * Constructor that takes a Unit.
     */
    Robot(const Unit& c);

    /*
     * Returns false because a robot is not junk.
     */
    virtual bool isJunk() const;

    /*
    * Draws this robot onto the given QGraphicsScene.
    */
    void draw(QGraphicsScene* scene) const override;

};

#endif // ROBOT_H
