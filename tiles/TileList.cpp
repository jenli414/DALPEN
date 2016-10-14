// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

TileList::TileList() {}

TileList::~TileList()
{
    delete[] m_tileList;
}

void TileList::addTile(Tile tile)
{
    // TODO: write this member
}

void TileList::checkExpand() {
    if (m_occupied == m_capacity) {
        Tile* expandedTileList = new Tile[m_capacity * 2];
        for (int i = 0; i < m_capacity; i++) {
            expandedTileList[i] = m_tileList[i];
        }
        m_capacity *= 2;
        delete[] m_tileList;
        m_tileList = expandedTileList;
    }
}

void TileList::drawAll(QGraphicsScene* scene)
{
    // TODO: write this member
}

int TileList::indexOfTopTile(int x, int y)
{
    // TODO: write this member
}

void TileList::raise(int x, int y)
{
    // TODO: write this member
}

void TileList::lower(int x, int y)
{
    // TODO: write this member
}

void TileList::remove(int x, int y)
{
    // TODO: write this member
}

void TileList::removeAll(int x, int y)
{
    // TODO: write this member
}

bool TileList::isEmpty()
{
    return m_occupied == 0;
}

/*
 * TileList();                             // Constructor
 * ~TileList();                            // Destructor
 * void addTile(Tile tile);                // Adds a tile to itself (on top)
 * void drawAll(QGraphicsScene* scene);    // Draws its own tiles
 * int indexOfTopTile(int x, int y);       // Index of the top tile
 * void lower(int x, int y);               // Puts the tile in index x,y at the bottom
 * void raise(int x, int y);               // Puts the tile in index x,y at the top
 * void remove(int x, int y);              // Remove the tile in index x,y
 * void removeAll(int x, int y);           // Removes all of its tiles
 */
