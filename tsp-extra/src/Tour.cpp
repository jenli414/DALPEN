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
        Node* nextNode;
        m_firstNode->point.drawTo(currNode->point, scene);
        while (currNode != m_firstNode) {
            currNode->point.draw(scene);
            nextNode = currNode->next;
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

void Tour::removeIntersections() {
    if (size() > 3) {
        bool checkAgain = true;
        while(checkAgain) {
            checkAgain = removeIntersectionsHelper();
        }
    }
}

bool Tour::removeIntersectionsHelper() {
    Node* n1 = m_firstNode;
    Node* n2;
    Node* tempNode;
    unsigned int tourSize = size();
    for (unsigned int i = 0; i < tourSize; i++) {
        n2 = (n1->next)->next;
        for (unsigned int j = 0; j < tourSize - 3; j++) {
            if (intersects(n1, n2)) {
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

bool Tour::isVertical(const Node* node) {
    return node->point.x == (node->next)->point.x;
}

bool Tour::isInRange(const Node* node, const double& xIntersection, const double& yIntersection) {
    Point a1 = node->point;
    Point a2 = (node->next)->point;
    bool xIntersectionInRange;
    bool yIntersectionInRange;
    if (a1.x == a2.x) {
        xIntersectionInRange = xIntersection == a1.x;
    } else if (a1.x > a2.x) {
        xIntersectionInRange = (xIntersection < a1.x) && (xIntersection > a2.x);
    } else {
        xIntersectionInRange = (xIntersection < a2.x) && (xIntersection > a1.x);
    }
    if (a1.y == a2.y) {
        yIntersectionInRange = a1.y == yIntersection;
    } else if (a1.y > a2.y) {
        yIntersectionInRange = (yIntersection < a1.y) && (yIntersection > a2.y);
    } else {
        yIntersectionInRange = (yIntersection < a2.y) && (yIntersection > a1.y);
    }
    return xIntersectionInRange && yIntersectionInRange;
}

bool Tour::intersects(const Node* n1, const Node* n2) {
    Point a1 = n1->point;
    Point a2 = (n1->next)->point;
    Point b1 = n2->point;
    Point b2 = (n2->next)->point;
    double k1 = (a1.y-a2.y)/(a1.x-a2.x);
    double k2 = (b1.y-b2.y)/(b1.x-b2.x);
    double m1 = a1.y - (k1*a1.x);
    double m2 = b1.y - (k2*b1.x);
    double xIntersection;
    double yIntersection;
    if (k1 == k2) {
        return false;
    } else if (isVertical(n1)) {
        xIntersection = a1.x;
        yIntersection = k2*xIntersection + m2;
    } else if (isVertical(n2)) {
        xIntersection = b1.x;
        yIntersection = k1*xIntersection + m1;
    } else {
        xIntersection = (m2-m1)/(k1-k2);
        yIntersection = k1*xIntersection + m1;
    }
    return isInRange(n1, xIntersection, yIntersection) &&
            isInRange(n2, xIntersection, yIntersection);
}

void Tour::reverseNodes(Node*& previousNode, Node*& lastNode) {
    Node* currNode = previousNode->next;
    if (currNode != lastNode) {
        reverseNodes(currNode, lastNode);
    }
    currNode->next = previousNode;
}
































