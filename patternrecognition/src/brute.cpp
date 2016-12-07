#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"
#include <set>

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for (const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

void getSlopes(map<double,set<Point>>& slopes, const Point& startPoint, const vector<Point>& points) {
    double gradient;
    for (vector<Point>::const_iterator pointIt = points.begin();
                 pointIt != points.end(); ++pointIt) {
        gradient = startPoint.slopeTo(*pointIt);
        slopes[gradient].insert(*pointIt);
    }
    for (map<double,set<Point>>::iterator mapIt = slopes.begin();
         mapIt != slopes.end(); mapIt++) {
        if (mapIt->second.size() < 4) {
            slopes.erase(mapIt);
        }
    }
}

bool slopeDrawnByConnectingPoint(const double slope, const set<pair<double,Point>>& taken, const Point& point) {
    for (set<pair<double,Point>>::const_iterator takenIt = taken.begin();
         takenIt != taken.end(); takenIt++) {
        if (takenIt->first == slope && point.slopeTo(takenIt->second) == slope) {
            return true;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input100.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Brute Force Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

    set<pair<double,Point>> taken;
    map<double,set<Point>> slopes;
    while (!points.empty()) {
        getSlopes(slopes, points.at(0), points);
        for (map<double,set<Point>>::iterator slopeIt = slopes.begin(); slopeIt != slopes.end(); slopeIt++) {
            if (!slopeDrawnByConnectingPoint(slopeIt->first, taken, points.at(0))) {
                set<Point>::iterator pointIt = slopeIt->second.begin();
                Point fromPoint = Point(0,0);
                Point toPoint = Point(0,0);
                if (pointIt != slopeIt->second.end()) {
                    fromPoint = *pointIt;
                    pointIt++;
                }
                while (pointIt != slopeIt->second.end()) {
                    toPoint = *pointIt;
                    render_line(scene, fromPoint, toPoint);
                    a.processEvents();
                    fromPoint = toPoint;
                    pointIt++;
                }
                pair<double,Point> newTaken(slopeIt->first,points.at(0));
                taken.insert(newTaken);
            }
        }
        points.erase(points.begin());
        slopes.clear();
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
