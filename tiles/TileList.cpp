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
    checkExpand();
    m_tileList[m_occupied] = tile;
    m_occupied++;
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
    for (int i = 0; i < m_occupied; i++){
        m_tileList[i].draw(scene);
    }
}


int TileList::indexOfTopTile(int x, int y)
{
    for (int i = m_occupied-1; i >=0; i--) {
       if (m_tileList[i].contains(x,y)) {
           return i;
        }
    }
    return -1;
}


void TileList::raise(int x, int y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1) {
        addTile(removeTile(index));
    }
}


void TileList::lower(int x, int y)
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


void TileList::remove(int x, int y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1) {
       removeTile(index);
    }
}


Tile TileList::removeTile(const int& index) {
    Tile removedTile = m_tileList[index];
    for (int i = index; i < m_occupied; i++) {
        m_tileList[i] = m_tileList[i + 1];
    }
    m_occupied--;
    return removedTile;
}

void TileList::removeAll(int x, int y)
{
    int index = indexOfTopTile(x,y);
    while (index != -1) {
        remove(x,y);
        index = indexOfTopTile(x,y);
    }
}

bool TileList::isEmpty()
{
    return m_occupied == 0;
}
