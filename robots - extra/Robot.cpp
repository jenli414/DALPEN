/*
 * jenli414 och sabse455
 */


/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "Robot.h"
#include "constants.h"

Robot::Robot() : Unit() {

}

Robot::Robot(const Unit& c) : Unit(c) {

}

bool Robot::at(const Unit& u) const {
    return (getX() == u.getX() && getY() == u.getY());
}

bool Robot::isJunk() const {
    return false;
}

bool Robot::attacks(const Unit& u) const {
    return (abs(getX() - u.getX()) <= 1 &&
            abs(getY() - u.getY()) <= 1);
}

void Robot::draw(QGraphicsScene *scene) const {
    Point corner = asPoint();
    scene->addEllipse(QRectF(corner.x * UNIT_WIDTH, corner.y * UNIT_HEIGHT,
                             JUNK_RADIUS, JUNK_RADIUS), QPen(), QBrush(ROBOT_COLOR));
}
