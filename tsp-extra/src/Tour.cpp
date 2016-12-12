/*
 * jenli414 och sabse455
 */

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour() {}


Tour::~Tour()
{
    Node* currNode;
    Node* nextNode = m_firstNode->next;
    delete m_firstNode;
    while (nextNode != m_firstNode) {
        currNode = nextNode;
        nextNode = currNode->next;
        delete currNode;
    }
}


void Tour::show() const
{
    if (m_firstNode != nullptr) {
        Node* currNode = m_firstNode->next;
        cout << m_firstNode->point.toString() << endl;
        while (currNode != m_firstNode) {
            cout << currNode->point.toString() << endl;
            currNode = currNode->next;
        }
        cout << endl;
    }
}


void Tour::draw(QGraphicsScene* scene) const
{
    if (m_firstNode != nullptr) {
        m_firstNode->point.draw(scene);
        Node* currNode = m_firstNode->next;
        m_firstNode->point.drawTo(currNode->point, scene);
        while (currNode != m_firstNode) {
            currNode->point.draw(scene);
            Node* nextNode = currNode->next;
            currNode->point.drawTo(nextNode->point, scene);
            currNode = currNode->next;
        }
    }
}


int Tour::size() const
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


double Tour::distance() const
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


void Tour::insertNearest(const Point& p)
{
    Node* newNode = new Node(p);
    if (m_firstNode == nullptr) {
        m_firstNode = newNode;
        newNode->next = newNode;
    }
    else {
        Node* nearestNode = m_firstNode;
        double shortestDistance = m_firstNode->point.distanceTo(p);
        Node* currNode = m_firstNode->next;
        while (currNode != m_firstNode) {
            double currDistance = currNode->point.distanceTo(p);
            if (currDistance < shortestDistance) {
                   shortestDistance = currDistance;
                   nearestNode = currNode;
            }
            currNode = currNode->next;
        }
        newNode->next = nearestNode->next;
        nearestNode->next = newNode;
    }
}


void Tour::insertSmallest(const Point& p)
{
    Node* newNode = new Node(p);
    if (m_firstNode == nullptr) {
        m_firstNode = newNode;
        newNode->next = newNode;
    }
    else {
        Node* bestNode = m_firstNode;
        Node* nextNode = m_firstNode->next;
        double originalDistance = m_firstNode->point.distanceTo(nextNode->point);
        double newDistance = m_firstNode->ponextNode = currNode->next;
        originalDistance = currNode->point.distanceTo(nextNode->point);
        newDistance = currNoint.distanceTo(p) +
                p.distanceTo(nextNode->point);
        double leastAddedDistance = newDistance - originalDistance;
        Node* currNode = nextNode;
        while (currNode != m_firstNode && leastAddedDistance != 0) {
            nextNode = currNode->next;
            originalDistance = currNode->point.distanceTo(nextNode->point);
            newDistance = currNode->point.distanceTo(p) +
                            p.distanceTo(nextNode->point);
            double addedDistance = newDistance - originalDistance;
            if (addedDistance < leastAddedDistance) {
                leastAddedDistance = addedDistance;
                bestNode = currNode;
            }
            currNode = currNode->next;
        }
        newNode->next = bestNode->next;
        bestNode->next = newNode;
    }
}

// Removes intersections until no intersections remain.
void Tour::removeIntersections() {
    if (size() > 3) {
        Node* n2 = m_firstNode->next->next;
        // first case
        while (n2->next != m_firstNode) {
            if (removeIntersectionsHelper(m_firstNode, n2)) {
                return removedIntersections();
            } else {
                n2 = n2->next;
            }
        }
        Node* n1 = m_firstNode->next;
        n2 = n1->next->next;
        // go around
        while(n1 != m_firstNode) {
            while (n2->next != n1) {
                if (removeIntersectionsHelper(n1, n2)) {
                    return removeIntersections();
                } else {
                    n2 = n2->next;
                }
            }
            n1 = n1->next;
            n2 = n1->next->next;
        }
    }
}

// Removes intersection between line between n1->point and n1->next->point and
// line between n2->point and n1->next->point if needed.
bool Tour::removeIntersectionsHelper(Node* n1, Node* n2) {
    if (intersects(n1,n2)) {
        n1->next->next = n2->next;
        reverseNodes(n1->next,n2);
        n1->next = n2;
        return true;
    } else {
        return false;
    }
}

// Returns true if line between n1->point and n1->next->point intersects with
// line between n2->point and n1->next->point
bool Tour::intersects(Node* n1, Node* n2) {
    Point p1 = n1->point;
    Point p2 = n1->next->point;
    Point p3 = n2->point;
    Point p4 = n2->next->point;
}

//Reverses direction of nodes from previousNode to lastNode.
void reverseNodes(Node* previousNode, Node* lastNode) {
    Node* currNode = previousNode->next;
    if (currNode != lastNode) {
        reverseNodes(currNode, lastNode);
    }
    currNode->next = previousNode;
}

/*Node* reverseNodesHelper(Node* previousNode, Node* lastNode) {
    Node* currNode = previousNode->next;
    if (currNode != lastNode) {
        reverseNodesHelper(currNode, lastNode);
    }
    currNode->next = previousNode;
}*/

































