#include <vector>
#include <algorithm>
#include "point.h"

using namespace std;

double PerpendicularDistance(const point_dec_d& p1, const point_dec_d& p2, const point_dec_d& p) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // Вычисление параметра t
    double t = ((p.x - p1.x) * dx + (p.y - p1.y) * dy) / (dx * dx + dy * dy);

    // Вычисление расстояния в зависимости от значения t
    if (t < 0.0) {
        return sqrt((p.x - p1.x) * (p.x - p1.x) + (p.y - p1.y) * (p.y - p1.y));
    }
    else if (t > 1.0) {
        return sqrt((p.x - p2.x) * (p.x - p2.x) + (p.y - p2.y) * (p.y - p2.y));
    }
    else {
        return sqrt((p.x - (p1.x + t * dx)) * (p.x - (p1.x + t * dx)) + (p.y - (p1.y + t * dy)) * (p.y - (p1.y + t * dy)));
    }
}


vector<point_dec_d> DouglasPeucker(vector<point_dec_d> contour, double epsilon) {
    vector<bool> visited(contour.size(), false);
    vector<int> stack = { 0, static_cast<int>(contour.size() - 1) };

    while (!stack.empty()) {
        int i = stack.back();
        stack.pop_back();

        if (!visited[i]) {
            visited[i] = true;
        }

        for (int j = i + 1; j < contour.size() - 1; j++) {
            if (!visited[j]) {
                double d = PerpendicularDistance(contour[i], contour[i + 1], contour[j]);
                if (d > epsilon) {
                    stack.push_back(j);
                }
            }
        }
    }

    vector<point_dec_d> simplifiedContour;

    for (int i = 0; i < contour.size(); i++) {
        if (visited[i]) {
            simplifiedContour.push_back(contour[i]);
        }
    }

    return simplifiedContour;
}
