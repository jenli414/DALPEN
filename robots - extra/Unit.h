/*
 * jenli414 och sabse455
 */


/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef UNIT_H
#define UNIT_H

#include "utilities.h"
#include <QGraphicsScene>

/* Root class for all pieces on the board.
 * Subclasses are Robot, Hero and Junk.
 */
class Unit {

protected:

    /*
     * Constructor.
     */
    explicit Unit();


    /*
     * Constructor.
     */
    explicit Unit(const Unit& u);


    /*
     * Constructor.
     */
    explicit Unit(const Point& p);

    /*
     * Destructor.
     */
    virtual ~Unit();

public:

    /*
     * Return Point representation of Unit.
     */
    Point asPoint() const;

    /*
     * Teleport. Does not check for collision.
     */
    void teleport();

    /*
     * Draws this unit onto the given QGraphicsScene.
     */
    virtual void draw(QGraphicsScene *scene) const = 0;

    /*
     * Take one step closer to u.
     */
    virtual void moveTowards(const Unit& u);

    /*
     * Returns x coordinate of this unit.
     */
    int getX() const {
        return x;
    }

    /*
     * Returns y coordinate of this unit.
     */
    int getY() const {
        return y;
    }

private:
    int x;  // x position of this unit
    int y;  // y position of this unit

    // private helpers
    void checkBounds();
};

#endif // UNIT_H
