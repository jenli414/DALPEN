/*
 * jenli414 och sabse455
 */

#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"


class TileList {

public:
    /*
     * Constructs an empty list with a capaciry of 10 tiles.
     */
    TileList();


    /*
     * Deconstructor that frees all dynamically allocated memory used.
     */
    ~TileList();


    /*
     * Adds a tile to the back of the tile list.
     */
    void addTile(Tile tile);


    /*
     * Draws all tiles by calling each tile's draw-function.
     */
    void drawAll(QGraphicsScene* scene) const;


    /*
     * Puts the top tile that covers (x,y) at the bottom, if none, do nothing.
     */
    void lower(const int& x, const int& y);


    /*
     * Puts the top tile that covers (x,y) on top, if none, do nothing.
     */
    void raise(const int& x, const int& y);


    /*
     * Remove the top tile that covers (x,y), if none, do nothing.
     */
    void remove(const int& x, const int& y);


    /*
     * Removes all tiles that cover (x,y), if none, do nothing.
     */
    void removeAll(const int& x, const int& y);


private:
    int m_capacity = 10;                        // Tile list maximum capacity.
    int m_occupied = 0;                         // Number of tiles in tile list.
    Tile* m_tileList = new Tile[m_capacity];    // Array of tiles.


    /*
     * Checks if we need to expand the array and does so if needed.
     */
    void checkExpand();


    /*
     * Returns the index of the top tile that covers the coordinates (x,y),
     * if none, returns -1.
     */
    int indexOfTopTile(const int& x, const int& y) const;


    /*
     * Removes and returns the tile at given index in the tile list.
     */
    Tile removeTile(const int& index);


};

#endif // TILELIST_H
