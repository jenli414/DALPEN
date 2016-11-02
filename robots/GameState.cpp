/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */
using namespace std;

#include <iostream>
#include "GameState.h"
#include "utilities.h"
#include "constants.h"


GameState::GameState(){}


GameState::~GameState() {
    int numberOfRobots = robots.size();
    for (int i = 0; i < numberOfRobots; i++) {
        delete robots[i];
    }
    robots.clear();
    //Hero* heroPtr = &hero;
    //delete heroPtr;
}

GameState::GameState(const GameState& gameState) {
    robots = gameState.robots;
    hero = gameState.hero;
}


GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot* robotPtr = new Robot();
        while (!isEmpty(*robotPtr)) {
            delete robotPtr;
            robotPtr = new Robot();
        }
        robots.push_back(robotPtr);
    }
    teleportHero();
}

GameState& GameState::operator=(const GameState& gameState) {
    robots = gameState.robots;
    hero = gameState.hero;
    return *this;
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i) {
        robots[i]->draw(scene);
    }
    hero.draw(scene);
}


void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}


void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++) {
        robots[i]->moveTowards(hero);
    }
}


int GameState::countCollisions() {
    int numberDestroyed = 0;
    unsigned int i = 0;
    while (i < robots.size()) {
        if (!(robots[i]->isJunk()) && countRobotsAt(*robots[i]) > 1) {
            Robot* newJunk = new Junk(*robots[i]);
            delete robots[i];
            robots[i] = newJunk;
            numberDestroyed++;
        }
        i++;
    }
    return numberDestroyed;
}


bool GameState::anyRobotsLeft() const {
    int robotCount = 0;
    Robot* currRobot;
    for(unsigned int i = 0; i < robots.size(); i++) {
        currRobot = robots[i];
        if (!(currRobot->isJunk())) {
            robotCount++;
        }
    }
    return robotCount;
}


bool GameState::heroDead() const {
    return !isEmpty(hero);
}


bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++) {
        Robot* currRobot = robots[i];
        bool collisionWithJunk = currRobot->at(unit) && currRobot->isJunk();
        if (currRobot->attacks(unit) || collisionWithJunk) {
            return false;
        }
    }
    return true;
}


void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}


Hero GameState::getHero() const {return hero;}


/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return countRobotsAt(unit) == 0;
}


/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        Robot* currRobot = robots[i];
        if (currRobot->at(unit))
            count++;
    }
    return count;
}
