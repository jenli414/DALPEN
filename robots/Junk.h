/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

/*
 * jenli414 och sabse455
 */

#ifndef JUNK_H
#define JUNK_H

#include "Robot.h"
#include <QGraphicsScene>

class Junk : public Robot {
public:

    /*
     * Constructor
     */
    Junk();


    /*
     * Constructor with Unit parameter
     */
    Junk(const Unit& c);


    /*
     * Overrides Unit's moveTowards. Does nothing
     */
    void moveTowards(const Unit& u) override;


    /*
     * Overrides Unit's attacks. Does nothing
     */
    bool attacks(const Unit& u) const override;


    /*
     * Returns true because Junk is junk #deep
     */
    bool isJunk() const override;

    /*
     * Draws this junk onto the given QGraphicsScene.
     */
    void draw(QGraphicsScene* scene) const override;
};

#endif // JUNK_H
