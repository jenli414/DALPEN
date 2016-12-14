/*
 * jenli414 och sabse455
 */

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:

    /*
     * Constructs an empty tour
     */
    Tour();

    /*
     * Frees the memory that was allocated internally by the tour
     */
    ~Tour();

    /*
     * Prints the coordinates of each node's point, format: (x, y)
     */
    void show() const;

    /*
     * Draws the tour to given scene by drawing lines between each node's
     * point and the point the node points to.
     */
    void draw(QGraphicsScene* scene) const;

    /*
     * Returns the number of nodes in the tour.
     */
    int size() const;

    /*
     * Returns the total distance of the tour.
     */
    double distance() const;

    /*
     * Creates a new node with the given point and inserts it after the
     * node with the point that is closest to the given point.
     */
    void insertNearest(const Point& p);


    /*
     * Creates a new node with the given point and inserts it after the
     * node with the point which in relation to the given point results
     * in the least distance added to the tour.
     */
    void insertSmallest(const Point& p);


    /*
     * Gets the two nodes in remainingNodes that has the farthest distance from
     * each other and inserts them as the tour. It also removes these two nodes
     * from the remainingNodes vector.
     */
    void insertFarthestNodes(vector<Node*>& remainingNodes);


    /*
     * Returns the node in remainingNodes with the longest distance from the
     * nearest tour node. It also erases that node from the remainingNodes vector.
     */
    Node* getFarthestNode(vector<Node*>& remainingNodes);


    /*
     * Assumes the Tour is empty at start. Inserts all nodes in remainingNodes in
     * the tour by fallowing the farthest insertion algorithm, i.e. starting with
     * the two nodes that has the farthest distance from each other, then adding the
     * node that has the farthest distance from the nearest tour node
     * in the tour position that gives the minimun increase in tour length.
     */
    void farthestInsertion(vector<Node*>& remainingNodes);




private:
    Node* m_firstNode = nullptr; // Pointer to the front node in the Tour
};

#endif // TOUR_H
