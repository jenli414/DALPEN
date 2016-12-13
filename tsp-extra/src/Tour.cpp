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
        double newDistance = m_firstNode->point.distanceTo(p) +
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
        bool checkAgain = true;
        while(checkAgain) {
            checkAgain = removeIntersectionsHelper();
        }
    }
}

// Checks for intersections.
// If found, removes one and returns true, else returns false.
bool Tour::removeIntersectionsHelper() {
    Node* n1 = m_firstNode;
    Node* n2;
    Node* tempNode;
    unsigned int tourSize = size();
    for (unsigned int i = 0; i < tourSize; i++) {
        n2 = (n1->next)->next;
        for (unsigned int j = 0; j < tourSize - 2; j++) {
            if (intersects(n1, n2)) {
                cout << "intersects" << endl;
                tempNode = n2->next;
                reverseNodes(n1->next, n2);
                (n1->next)->next = tempNode;
                n1->next = n2;
                return true;
            }
            n2 = n2->next;
        }
        n1 = n1->next;
    }
    return false;
}

// Returns true if line between n1->point and n1->next->point intersects with
// line between n2->point and n1->next->point
bool Tour::intersects(Node* n1, Node* n2) {
    cout << "checks intersect" << endl;
    Point p1 = n1->point;
    Point p2 = (n1->next)->point;
    Point p3 = n2->point;
    Point p4 = (n2->next)->point;
    bool connectsAtEndPoints1 = p1.x == p3.x && p1.y == p3.y;
    bool connectsAtEndPoints2 = p1.x == p4.x && p1.y == p4.y;
    bool connectsAtEndPoints3 = p2.x == p3.x && p2.y == p3.y;
    bool connectsAtEndPoints4 = p2.x == p4.x && p2.y == p4.y;
    if (connectsAtEndPoints1 || connectsAtEndPoints2 || connectsAtEndPoints3 || connectsAtEndPoints4) {
        return false;
    }
    double gradient1 = (p1.y-p2.y)/(p1.x-p2.x);
    double gradient2 = (p3.y-p4.y)/(p3.x-p4.x);
    if (gradient1 != gradient2) {
        double m1 = p1.y - (gradient1*p1.x);
        double m2 = p3.y - (gradient2*p3.x);
        double xIntersection = (m2-m1)/(gradient1-gradient2);
        double yIntersection = (m1*gradient2-m2*gradient1)/(gradient2-gradient1);
        bool xIntersectionInRange1 = false;
        if (p1.x > p2.x) {
            xIntersectionInRange1 = (xIntersection < p1.x) && (xIntersection > p2.x);
        } else if (p2.x > p1.x) {
            xIntersectionInRange1 = (xIntersection < p2.x) && (xIntersection > p1.x);
        }
        bool xIntersectionInRange2 = false;
        if (p3.x > p4.x) {
            xIntersectionInRange2 = (xIntersection < p3.x) && (xIntersection > p4.x);
        } else if (p4.x > p3.x) {
            xIntersectionInRange2 = (xIntersection < p4.x) && (xIntersection > p3.x);
        }
        bool yIntersectionInRange1 = false;
        if (p1.y > p2.y) {
            yIntersectionInRange1 = (yIntersection < p1.y) && (yIntersection > p2.y);
        } else if (p2.y > p1.y) {
            yIntersectionInRange1 = (yIntersection < p2.y) && (yIntersection > p1.y);
        }
        bool yIntersectionInRange2 = false;
        if (p3.y > p4.y) {
            yIntersectionInRange2 = (yIntersection < p3.y) && (yIntersection > p4.y);
        } else if (p4.y > p3.y) {
            yIntersectionInRange2 = (yIntersection < p4.y) && (yIntersection > p3.y);
        }
        return xIntersectionInRange1 && xIntersectionInRange2 && yIntersectionInRange1 && yIntersectionInRange2;
    } else {
        return false;
    }
}

//Reverses direction of nodes from previousNode to lastNode.
void Tour::reverseNodes(Node* previousNode, Node* lastNode) {
    Node* currNode = previousNode->next;
    if (currNode != lastNode) {
        reverseNodes(currNode, lastNode);
    }
    currNode->next = previousNode;
}
































