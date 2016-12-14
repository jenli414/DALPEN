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
     * Contructor.
     */
    explicit Robot();

    /*
     * Constructor that takes a Unit.
     */
    explicit Robot(const Unit& c);

    /*
    * Am I in the same square as u?
    */
    bool at(const Unit& u) const;

    /*
     * Returns false because a robot is not junk.
     */
    virtual bool isJunk() const;

    /*
    * Can I catch u in one move?
    */
    virtual bool attacks(const Unit& u) const;

    /*
    * Draws this robot onto the given QGraphicsScene.
    */
    virtual void draw(QGraphicsScene* scene) const override;

};

#endif // ROBOT_H
