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


/**
 * Slope between this point and p
 *
 * If the points are the same, negative infinity is returned
 * If the line between the points is horizontal positive zero is returned
 * If the line between the points is vertical positive infinity is returned
 */
void getLines(map<double,set<Point>>& lines, const Point& startPoint,
              const vector<Point>& points) {
    lines.clear();
    double gradient;
    unsigned int samePointCount = 0;
    map<double,set<Point>> tempLines;
    for (vector<Point>::const_iterator pointIt = points.begin();
                 pointIt != points.end(); ++pointIt) {
        gradient = startPoint.slopeTo(*pointIt);
        if (gradient == -std::numeric_limits<double>::infinity()) {
            samePointCount++;
        } else {
            tempLines[gradient].insert(*pointIt);
        }
    }
    for (map<double,set<Point>>::iterator mapIt = tempLines.begin(); mapIt != tempLines.end(); mapIt++) {
        if (mapIt->second.size() >= (3 - samePointCount)) {
            lines[mapIt->first] = mapIt->second;
        }
    }
    /*for (map<double,set<Point>>::iterator mapIt = lines.begin(); mapIt != lines.end(); mapIt++) {
        if (mapIt->second.size() < (3 - samePointCount)) {
            lines.erase(mapIt);
        }
    }*/
    /*map<double,set<Point>>::iterator mapIt = lines.begin();
    while (mapIt != lines.end()) {
        if (mapIt->second.size() < (3 - samePointCount)) {
            lines.erase(mapIt);
        }
        mapIt++;
    }*/
}

bool lineDrawnByConnectingPoint(const double gradient,
                                const set<pair<double,Point>>& taken,
                                const Point& point) {
    for (set<pair<double,Point>>::const_iterator takenIt = taken.begin();
         takenIt != taken.end(); takenIt++) {
        if (takenIt->first == gradient && point.slopeTo(takenIt->second) == gradient) {
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

    // the vector of pointslines
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

    //set<pair<double,Point>> taken;
    map<double,set<Point>> lines;
    Point currPoint(0,0);
    string str;
    int num = 0;
    //double gradient;
    while (!points.empty()) {
        currPoint = points.at(0);
        points.erase(points.begin());
        getLines(lines, currPoint, points);
        for (map<double,set<Point>>::iterator lineIt = lines.begin();
             lineIt != lines.end(); lineIt++) {
            //cout << "line point size: " << lineIt->second.size() << endl;
            //gradient = lineIt->first;
            if (true) {
                //!lineDrawnByConnectingPoint(gradient, taken, currPoint)
                set<Point>::iterator pointIt = lineIt->second.begin();
                Point fromPoint = currPoint;
                Point toPoint = Point(0,0);
                while (pointIt != lineIt->second.end()) {
                    toPoint = *pointIt;
                    render_line(scene, fromPoint, toPoint);
                    a.processEvents();
                    //fromPoint = toPoint;
                    pointIt++;
                    num++;
                    //cin >> str;
                }
                cout << num << endl;
                num = 0;
                //pair<double,Point> newTaken(lineIt->first,currPoint);
                //taken.insert(newTaken);
            }
        }
        //points.erase(points.begin());
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
