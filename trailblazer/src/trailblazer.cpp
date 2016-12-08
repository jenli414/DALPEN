// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"

using namespace std;

void depthFirstSearchHelper(vector<Vertex*>& path, Vertex* start,
                                     Vertex* end, bool& foundEnd) {
    path.push_back(start);
    start->setColor(YELLOW);
    if (start != end) {
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
        } else {
            start->setColor(GREEN);
        }
    } else {
        foundEnd = true;
        start->setColor(GREEN);
    }
}

vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    bool foundEnd = false;
    vector<Vertex*> path;
    depthFirstSearchHelper(path, start, end, foundEnd);
    graph.resetData();
    return path;
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}
