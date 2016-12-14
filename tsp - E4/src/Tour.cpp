/*
 * jenli414 och sabse455
 */

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"
#include "set"
#include <algorithm>
#include <vector>


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


double Tour::distance() const{
    if (m_firstNode == nullptr) {
        return 0;
    } else {
        double distance = 0;
        Node* currNode = m_firstNode->next;
        Node* nextNode;
        distance += m_firstNode->point.distanceTo(currNode->point);
        while (currNode != m_firstNode) {
            nextNode = currNode->next;
            distance += currNode->point.distanceTo(nextNode->point);
            currNode = currNode->next;
        }
        return distance;
    }
}


void Tour::insertNearest(const Point& p){
    Node* newNode = new Node(p);
    if (m_firstNode == nullptr) {
        m_firstNode = newNode;
        newNode->next = newNode;
    } else {
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


void Tour::insertSmallest(const Point& p) {
    Node* newNode = new Node(p);
    if (m_firstNode == nullptr) {
        m_firstNode = newNode;
        newNode->next = newNode;
    } else {
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



void Tour::insertFarthestNodes(vector<Node*>& remainingNodes) {
    int nodeAmount = remainingNodes.size();
    double farthestDistance = -1;
    double currDistance;
    Node* currRefNode;
    for (int i = 0; i < nodeAmount; i++) {
        currRefNode = remainingNodes.at(i);
        for (int j = i+1; j < nodeAmount; j++) {
            currDistance = currRefNode->point.distanceTo(remainingNodes.at(j)->point);
            if (abs(farthestDistance) < abs(currDistance)) {
                farthestDistance = currDistance;
                m_firstNode = remainingNodes.at(i);
                m_firstNode->next = remainingNodes.at(j);
            }
        }
   }
   m_firstNode->next->next = m_firstNode;
   remainingNodes.erase(find(remainingNodes.begin(), remainingNodes.end(), m_firstNode));
   remainingNodes.erase(find(remainingNodes.begin(), remainingNodes.end(), m_firstNode->next));
}


void Tour::farthestInsertion(vector<Node*>& remainingNodes){
    insertFarthestNodes(remainingNodes);
    Node* nodeToInsert;
    while (!remainingNodes.empty()) {
        nodeToInsert = getFarthestNode(remainingNodes);
        insertSmallest(nodeToInsert->point);
    }
}


Node* Tour::getFarthestNode(vector<Node*>& remainingNodes) {
    int tourSize = size();
    int remaining = remainingNodes.size();
    double farthestDistance = -1;
    double shortestRefDistance;
    double currDistance;
    Node* farthestNode;
    Node* currRefNode;
    Node* currTourNode = m_firstNode;
    for (int i = 0; i < remaining; i++) {
        currRefNode = remainingNodes.at(i);
        shortestRefDistance = numeric_limits<double>::infinity();
        for (int j = 0; j < tourSize; j++) {
            currDistance = currRefNode->point.distanceTo(currTourNode->point);
            if (currDistance < shortestRefDistance) {
                shortestRefDistance = currDistance;
            }
            currTourNode = currTourNode->next;
        }
        if (shortestRefDistance > farthestDistance) {
            farthestDistance = shortestRefDistance;
            farthestNode = currRefNode;
        }
    }
    remainingNodes.erase(find(remainingNodes.begin(), remainingNodes.end(), farthestNode));
    return farthestNode;
}

