/*
 * jenli414 och sabse455
 */

#include "TileList.h"

TileList::TileList() {}

TileList::~TileList()
{
    delete[] m_tileList;
}


void TileList::addTile(Tile tile)
{
    checkExpand();
    m_tileList[m_occupied] = tile;
    m_occupied++;
}


void TileList::drawAll(QGraphicsScene* scene) const
{
    for (int i = 0; i < m_occupied; i++){
        m_tileList[i].draw(scene);
    }
}


void TileList::raise(const int& x, const int& y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1) {
        addTile(removeTile(index));
    }
}


void TileList::lower(const int& x, const int& y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1) {
        Tile tileToBeLowered = m_tileList[index];
        for (int i = index; i > 0; i--) {
            m_tileList[i] = m_tileList[i - 1];
        }
        m_tileList[0] = tileToBeLowered;
    }
}


void TileList::remove(const int& x, const int& y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1) {
       removeTile(index);
    }
}


void TileList::removeAll(const int& x, const int& y)
{
    int index = indexOfTopTile(x,y);
    while (index != -1) {
        removeTile(index);
        index = indexOfTopTile(x,y);
    }
}


void TileList::checkExpand()
{
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


int TileList::indexOfTopTile(const int& x, const int& y) const
{
    for (int i = m_occupied-1; i >= 0; i--) {
       if (m_tileList[i].contains(x,y)) {
           return i;
        }
    }
    return -1;
}


Tile TileList::removeTile(const int& index) {
    Tile removedTile = m_tileList[index];
    for (int i = index; i < m_occupied; i++) {
        m_tileList[i] = m_tileList[i + 1];
    }
    m_occupied--;
    return removedTile;
}
