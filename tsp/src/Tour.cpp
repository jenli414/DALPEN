// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour(Point a, Point b, Point c, Point d)
{
    Node* fourthNode = new Node(d);
    Node* thirdNode = new Node(c, fourthNode);
    Node* secondNode = new Node(b,thirdNode);
    Node* firstNode = new Node(a, secondNode);
    fourthNode->next = firstNode;

    m_firstNode = firstNode;
}

Tour::~Tour()
{
    // TODO: write this member
}

void Tour::show()
{
    if (m_firstNode != nullptr) {
        Node* currNode = m_firstNode->next;
        cout << m_firstNode->point.toString() << endl;
        while (currNode != m_firstNode) {
            cout << currNode->point.toString() << endl;
            currNode = currNode->next;
        }
    }
}

void Tour::draw(QGraphicsScene* scene)
{
    if (m_firstNode != nullptr) {
        Node* currNode = m_firstNode->next;
        m_firstNode->point.drawTo(currNode->point, scene);
        while (currNode != m_firstNode) {
            Node* nextNode = currNode->next;
            currNode->point.drawTo(nextNode->point, scene);
            currNode = currNode->next;
        }
    }
}

int Tour::size()
{
    if (m_firstNode == nullptr) {
        return 0;
    } else {
        int size = 1;
        Node* currNode = m_firstNode->next;
        while (currNode != m_firstNode) {
            size++;
            currNode = currNode->next;
        }
        return size;
    }
}

double Tour::distance()
{
    if (m_firstNode == nullptr) {
        return 0;
    } else {
        double distance = 0;
        Node* currNode = m_firstNode->next;
        distance += m_firstNode->point.distanceTo(currNode->point);
        while (currNode != m_firstNode) {
            Node* nextNode = currNode->next;
            distance += currNode->point.distanceTo(nextNode->point);
            currNode = currNode->next;
        }
        return distance;
    }
}

void Tour::insertNearest(Point p)
{
    // TODO: write this member
}

void Tour::insertSmallest(Point p)
{
    // TODO: write this member
}
