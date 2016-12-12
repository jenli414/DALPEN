/*
 * jenli414 och sabse455
 */

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
#include <iterator>

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

/*
 * Iterates over a set of Points and keeps only the "smallest" and "largest".
 */
void keepOnlyEndPoints(set<Point>& points) {
    Point startPoint = *points.begin();
    Point endPoint = *points.begin();
    for (set<Point>::iterator pointIt = points.begin();
         pointIt != points.end(); pointIt++) {
        if (*pointIt < startPoint) {
            startPoint = *pointIt;
        } else if (*pointIt > endPoint) {
            endPoint = *pointIt;
        }
    }
    points.clear();
    points.insert(startPoint);
    points.insert(endPoint);
}

/*
 * Puts lines with 4 or more points in lines by pairing the gradient with
 * the start and end point of that line.
 */
void getLines(map<double,set<Point>>& lines, vector<Point>& points) {
    lines.clear();
    Point refPoint = points.at(0);
    points.erase(points.begin());
    double gradient;
    unsigned int samePointCount = 0;
    // Sort all remaining points by gradient relative refPoint
    for (vector<Point>::const_iterator pointIt = points.begin();
                 pointIt != points.end(); ++pointIt) {
        gradient = refPoint.slopeTo(*pointIt);
        if (gradient == -numeric_limits<double>::infinity()) {
            samePointCount++;
        } else {
            lines[gradient].insert(*pointIt);
        }
    }
    // Erase lines that are made up of less than 4 points in total
    // and only keep end points for remaining lines.
    map<double,set<Point>>::iterator mapIt = lines.begin();
    while (mapIt != lines.end()) {
        if (mapIt->second.size() < (3 - samePointCount)) {
            mapIt = lines.erase(mapIt);
        } else {
            mapIt->second.insert(refPoint);
            keepOnlyEndPoints(mapIt->second);
            mapIt++;
        }
    }
}

/*
 * UTÖKNING - E8:
 * Returns true if the given line (gradient from point) has already been drawn.
 */
bool alreadyDrawn(const set<pair<double,Point>>& taken, const double gradient,
                  const Point& point) {
    for (set<pair<double,Point>>::const_iterator takenIt = taken.begin();
         takenIt != taken.end(); takenIt++) {
        if (takenIt->first == gradient &&
                point.slopeTo(takenIt->second) == gradient) {
            return true;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input12800.txt";
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
    view->setWindowTitle("Fast Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

    set<pair<double,Point>> taken;
    map<double,set<Point>> lines;
    Point fromPoint(0,0);
    Point toPoint(0,0);
    double gradient;
    while (!points.empty()) {
        getLines(lines, points);
        for (map<double,set<Point>>::iterator lineIt = lines.begin();
             lineIt != lines.end(); lineIt++) {
            gradient = lineIt->first;
            if (!alreadyDrawn(taken, gradient, *lineIt->second.begin())) {
                fromPoint = *(lineIt->second.begin());
                toPoint = *(++lineIt->second.begin());
                render_line(scene, fromPoint, toPoint);
                a.processEvents();
                pair<double,Point> newTaken(gradient,fromPoint);
                taken.insert(newTaken);
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
