// jenli414 och sabse455

#include "costs.h"
#include "trailblazer.h"
#include "pqueue.h"
#include <queue>
#include <algorithm>

using namespace std;

/*
 * Recursive help-function for depthFirstSearch.
 */
void depthFirstSearchHelper(vector<Vertex*>& path, Vertex* start,
                                     Vertex* end, bool& foundEnd) {
    path.push_back(start);
    start->setColor(GREEN);
    if (start == end) {
        foundEnd = true;
    } else {
        Set<Arc*> arcs = start->arcs;
        Vertex* nextVertex;
        start->visited = true;
        for (Set<Arc*>::iterator arcIt = arcs.begin(); arcIt != arcs.end(); arcIt++) {
            nextVertex = (*arcIt)->finish;
            if (foundEnd) {
                break;
            } else if (!nextVertex->visited) {
                depthFirstSearchHelper(path, nextVertex, end, foundEnd);
            }
        }
        if (!foundEnd) {
            path.pop_back();
            start->setColor(GRAY);
        }
    }
}

/*
 * Finds a path between start and end using depth-first search.
 */
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    bool foundEnd = false;
    vector<Vertex*> path;
    depthFirstSearchHelper(path, start, end, foundEnd);
    return path;
}

/*
 * Backtracks from end to start and returns resulting path.
 */
vector<Vertex*> getPath(Vertex* start, Vertex* end) {
    vector<Vertex*> path;
    bool foundEnd = end->previous != nullptr;
    if (foundEnd) {
        Vertex* backtracker = end;
        while (backtracker != start) {
            path.push_back(backtracker);
            backtracker = backtracker->previous;
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }
    return path;
}

/*
 * Finds a path between start and end using breadth-first search.
 */
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    queue<Vertex*> vertexQueue;
    start->visited = true;
    vertexQueue.push(start);
    Vertex* currVertex;
    Set<Vertex*> currNeighbors;
    while (!vertexQueue.empty()) {
        currVertex = vertexQueue.front();
        vertexQueue.pop();
        currVertex->setColor(GREEN);
        if (currVertex == end) {
            break;
        } else {
            // Queues unvisited neighbors of currVertex
            currNeighbors = graph.getNeighbors(currVertex);
            for (Set<Vertex*>::iterator neighborIt = currNeighbors.begin();
                 neighborIt != currNeighbors.end(); neighborIt++) {
                if (!(*neighborIt)->visited) {
                    (*neighborIt)->previous = currVertex;
                    (*neighborIt)->visited = true;
                    vertexQueue.push(*neighborIt);
                    (*neighborIt)->setColor(YELLOW);
                }
            }
        }
    }
    return getPath(start, end);
}

/*
 * Finds the cheapest path between start and end using the dijikstra algorithm.
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    PriorityQueue<Vertex*> vertexQueue;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Set<Vertex*>::iterator vertexIt = vertices.begin();
         vertexIt != vertices.end(); vertexIt++) {
        (*vertexIt)->cost = numeric_limits<double>::infinity();
    }
    start->cost = 0;
    vertexQueue.enqueue(start, 0);
    Vertex* currVertex;
    Set<Vertex*> currNeighbors;
    double altCost;
    while (!vertexQueue.isEmpty()) {
        currVertex = vertexQueue.dequeue();
        currVertex->setColor(GREEN);
        if (currVertex == end) {
            break;
        }
        currNeighbors = graph.getNeighbors(currVertex);
        // Calculates and updates lowest costs and changes
        // the priority of currVertex neighbors if needed.
        for (Set<Vertex*>::iterator neighborIt = currNeighbors.begin();
             neighborIt != currNeighbors.end(); neighborIt++) {
            altCost = currVertex->cost + graph.getEdge(currVertex, *neighborIt)->cost;
            if (altCost < (*neighborIt)->cost) {
                (*neighborIt)->cost = altCost;
                (*neighborIt)->previous = currVertex;
                if ((*neighborIt)->visited) {
                    vertexQueue.changePriority(*neighborIt, altCost);
                } else {
                    vertexQueue.enqueue(*neighborIt, altCost);
                    (*neighborIt)->visited = true;
                    (*neighborIt)->setColor(YELLOW);
                }
            }
        }
    }
    return getPath(start, end);
}

/*
 * Finds a path between start and end using a* search.
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    PriorityQueue<Vertex*> vertexQueue;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Set<Vertex*>::iterator vertexIt = vertices.begin();
         vertexIt != vertices.end(); vertexIt++) {
        (*vertexIt)->cost = numeric_limits<double>::infinity();
    }
    double priority;
    double altCost;
    Vertex* currVertex;
    Set<Vertex*> currNeighbors;
    start->cost = 0;
    start->visited = true;
    vertexQueue.enqueue(start, 0);
    while (!vertexQueue.isEmpty()) {
        currVertex = vertexQueue.dequeue();
        currVertex->setColor(GREEN);
        if (currVertex == end){
            break;
        }
        currNeighbors = graph.getNeighbors(currVertex);
        // Calculates and updates lowest costs and changes
        // the priority of currVertex neighbors if needed.
        for (Set<Vertex*>::iterator neighborIt = currNeighbors.begin();
             neighborIt != currNeighbors.end(); neighborIt++) {
            altCost = currVertex->cost + graph.getEdge(currVertex, *neighborIt)->cost;
            if (altCost < (*neighborIt)->cost) {
                priority = altCost + (*neighborIt)->heuristic(end);
                (*neighborIt)->cost = altCost;
                (*neighborIt)->previous = currVertex;
                if ((*neighborIt)->visited) {
                    vertexQueue.changePriority(*neighborIt, priority);
                } else {
                    vertexQueue.enqueue(*neighborIt,priority);
                    (*neighborIt)->visited = true;
                    (*neighborIt)->setColor(YELLOW);
                }
            }
        }
    }
    return getPath(start,end);
}
