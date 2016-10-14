// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"


class TileList {

public:
    TileList();                             // Constructs an empty list with a capacity of 10 tiles.
    ~TileList();                            // Destructor, frees all dynamically allocated memory used.
    void addTile(Tile tile);                // Adds a tile to itself, on top.
    void checkExpand();                     // Checks if we need to expand the array
    void drawAll(QGraphicsScene* scene);    // Draws its own tiles, bottom to top, by calling each tile's draw-function.
    int indexOfTopTile(int x, int y);       // Index of the top tile, if none, return -1.
    void lower(int x, int y);               // Puts the tile in index x,y at the bottom, if none, do nothing.
    void raise(int x, int y);               // Puts the tile in index x,y at the top, if none, do nothing.
    void remove(int x, int y);              // Remove the (top) tile in index x,y. Re-orders tiles if needed.
    void removeAll(int x, int y);           // Removes all tiles in index x,y. Re-orders tiles if needed.
    bool isEmpty();

private:
    int m_capacity = 10;
    int m_occupied = 0;
    Tile *m_tileList = new Tile[m_capacity];
};

#endif // TILELIST_H
