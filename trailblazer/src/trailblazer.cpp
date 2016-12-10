// jenli414 och sabse455

#include "costs.h"
#include "trailblazer.h"
#include "pqueue.h"
#include <queue>
#include <algorithm>

using namespace std;

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
            if (!foundEnd && !nextVertex->visited) {
                depthFirstSearchHelper(path, nextVertex, end, foundEnd);
            } else if (foundEnd) {
                break;
            }
        }
        if (!foundEnd) {
            path.pop_back();
            start->setColor(GRAY);
        }
    }
}

vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    bool foundEnd = false;
    vector<Vertex*> path;
    depthFirstSearchHelper(path, start, end, foundEnd);
    graph.resetData();
    return path;
}

void getPath(vector<Vertex*>& path, Vertex* start, Vertex* end) {
    Vertex* backtracker = end;
    while (backtracker != start) {
        path.push_back(backtracker);
        backtracker = backtracker->previous;
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
}

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
            while (!vertexQueue.empty()) {
                vertexQueue.pop();
            }
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
    vector<Vertex*> path;
    bool foundEnd = end->previous != nullptr;
    if (foundEnd) {
        getPath(path, start, end);
    }
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    PriorityQueue<Vertex*> vertexQueue;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Set<Vertex*>::iterator vertexIt = vertices.begin();
         vertexIt != vertices.end(); vertexIt++) {
        vertexQueue.enqueue(*vertexIt, numeric_limits<double>::infinity());
        (*vertexIt)->cost = numeric_limits<double>::infinity();
    }
    start->cost = 0;
    vertexQueue.changePriority(start, 0);
    Vertex* currVertex;
    Set<Vertex*> currNeighbors;
    double altCost;
    while (!vertexQueue.isEmpty()) {
        currVertex = vertexQueue.dequeue();
        if (currVertex->cost < end->cost) {
            currVertex->visited = true;
            currVertex->setColor(GREEN);
            currNeighbors = graph.getNeighbors(currVertex);
            // Calculates and updates lowest costs and changes
            // the priority of currVertex neighbors if needed.
            for (Set<Vertex*>::iterator neighborIt = currNeighbors.begin();
                 neighborIt != currNeighbors.end(); neighborIt++) {
                altCost = currVertex->cost + graph.getEdge(currVertex, *neighborIt)->cost;
                if (!(*neighborIt)->visited && (altCost < (*neighborIt)->cost)) {
                    (*neighborIt)->cost = altCost;
                    (*neighborIt)->previous = currVertex;
                    vertexQueue.changePriority(*neighborIt, (*neighborIt)->cost);
                    (*neighborIt)->setColor(YELLOW);
                }
            }
        } else {
            vertexQueue.clear();
        }
    }
    vector<Vertex*> path;
    bool foundEnd = end->previous != nullptr;
    if (foundEnd) {
        end->setColor(GREEN);
        getPath(path, start, end);
    }
    return path;
}



/*
 * Ganska säker på att den inte alltid ger den bästa vägen,
 * men den följer A*-algoritmen (Fö23)
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    PriorityQueue<Vertex*> vertexQueue;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Set<Vertex*>::iterator vertexIt = vertices.begin();
         vertexIt != vertices.end(); vertexIt++) {
        vertexQueue.enqueue(*vertexIt, numeric_limits<double>::infinity());
        (*vertexIt)->cost = numeric_limits<double>::infinity();
    }
    double priority;
    double altCost;
    Vertex* currVertex;
    Set<Vertex*> currNeighbors;
    start->cost = 0;
    vertexQueue.changePriority(start, 0);
    while (!vertexQueue.isEmpty()) {
        currVertex = vertexQueue.dequeue();
        currVertex->visited = true;
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
            if (!(*neighborIt)->visited && (altCost < (*neighborIt)->cost)) {
                (*neighborIt)->cost = altCost;
                (*neighborIt)->previous = currVertex;
                priority = altCost + (*neighborIt)->heuristic(end);
                vertexQueue.changePriority(*neighborIt,priority);
                (*neighborIt)->setColor(YELLOW);
            }
        }
    }
    vector<Vertex*> path;
    bool foundEnd = end->previous != nullptr;
    if (foundEnd) {
        getPath(path, start, end);
    }
    return path;
}



/*
 * Söker alla vägar och retunerar den bästa!
 */
vector<Node *> aStarBEST(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    PriorityQueue<Vertex*> vertexQueue;
    Vertex* currVertex;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Set<Vertex*>::iterator vertexIt = vertices.begin();
         vertexIt != vertices.end(); vertexIt++) {
        (*vertexIt)->cost = numeric_limits<double>::infinity();
        vertexQueue.enqueue(*vertexIt, numeric_limits<double>::infinity());
    }
    Set<Vertex*> currNeighbors;
    double altCost;
    double priority;
    start->cost =0;
    vertexQueue.changePriority(start,0);
    while (!vertexQueue.isEmpty()){
        currVertex = vertexQueue.dequeue();
        if ((currVertex->cost < end->cost)){
            currVertex->visited = true;
            (currVertex)->setColor(GREEN);
            currNeighbors = graph.getNeighbors(currVertex);
            for (Set<Vertex*>::iterator neighborIt = currNeighbors.begin();
                 neighborIt != currNeighbors.end(); neighborIt++){
                altCost = currVertex->cost + graph.getEdge(currVertex, *neighborIt)->cost;
                if ((altCost < (*neighborIt)->cost)) {
                    (*neighborIt)->cost = altCost;
                    (*neighborIt)->previous = currVertex;
                    if (!(*neighborIt)->visited){
                        priority = altCost + (*neighborIt)->heuristic(end);
                        vertexQueue.changePriority(*neighborIt, priority);
                        (*neighborIt)->setColor(YELLOW);
                    }
                }
            }
       }
    }
    vector<Vertex*> path;
    bool foundEnd = end->previous != nullptr;
    if (foundEnd) {
        //end->setColor(GREEN);
        getPath(path, start, end);
    }
    return path;
}









