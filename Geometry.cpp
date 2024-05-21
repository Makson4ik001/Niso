#include "Geometry.h"
#include <cmath>
constexpr double EPS = 1e-11;
#define double float
inline double fixZero(double d) {
    if (fabs(d) < EPS)
        d = 0;
    return d;
}


// 1 - intersection but not on edge or point
//-1 - we are on edge
//-2 - we are on point
// 0 - no intersection
Vector2 intersection(double x0, double x1, double y0, double y1, Vector2 p,
    int* flag) {
    Vector2 res;

    if (fixZero(p.x - x0) == 0 && fixZero(p.y - y0) == 0 ||
        fixZero(p.x - x1) == 0 && fixZero(p.y - y1) == 0) {
        *flag = -2;
        res.x = p.x;
        res.y = p.y;
        return res;
    }
    if (fixZero(x0 - x1) == 0) {
        if (fixZero(x0 - p.x) != 0) {
            *flag = 0;
            return res;
        }
        if (y0 > y1)
            std::swap(y0, y1);
        if (p.y > y0 && p.y < y1) {
            *flag = -1;
            res.x = x0;
            res.y = p.y;
            return res;
        }
        *flag = 0;
        return res;
    }
    if (p.x >= x0 && p.x <= x1) {
        double x = p.x - x0;
        double k = x / (x1 - x0);
        double y = (y1 - y0) * k;
        res.x = p.x;
        res.y = y + y0;
        *flag = 1;
        if (fixZero(p.y - res.y) == 0 && fixZero(p.x - res.x) == 0)
            *flag = -1;
        return res;
    }
    *flag = 0;
    return res;
}

bool isInside(std::pair<const Vector2*, const Vector2*> line, int direction) {
    Vector2 diff = *line.second - *line.first;
    Vector2 normalDiff(diff.y, -diff.x);
    Vector2 vdirection;
    if (direction == 0)
        vdirection = Vector2(0, 1);
    if (direction == 1)
        vdirection = Vector2(1, 0);
    if (direction == 2)
        vdirection = Vector2(0, -1);
    if (direction == 3)
        vdirection = Vector2(-1, 0);

    return normalDiff.dotProduct(vdirection) > 0;
}

//-1 if on edge, 0 if outside, 1 if inside, -2 if on point
int contains(std::vector<Vector2> const& circuit, Vector2 point) {
    // counter-clockwise
    std::pair<const Vector2*, const Vector2*> leftL = { nullptr, nullptr },
        rightL = { nullptr, nullptr },
        topL = { nullptr, nullptr },
        bottomL = { nullptr, nullptr };
    Vector2 left, right, top, bottom;
    double x0, x1, y0, y1;
    for (size_t i = 1; i < circuit.size(); i++) {
        x0 = circuit[i - 1].x;
        x1 = circuit[i].x;
        y0 = circuit[i - 1].y;
        y1 = circuit[i].y;
        if (x1 < x0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int status = 0;
        Vector2 resX = intersection(x0, x1, y0, y1, point, &status);
        if (status == -1 || status == -2)
            return status;

        if (status == 1) {
            if (resX.y > point.y) {
                if (topL.first && top.y > resX.y || !topL.first) {
                    topL.first = &circuit[i - 1];
                    topL.second = &circuit[i];
                    top = resX;
                }
            }
            else {
                if (bottomL.first && bottom.y < resX.y || !bottomL.first) {
                    bottomL.first = &circuit[i - 1];
                    bottomL.second = &circuit[i];
                    bottom = resX;
                }
            }
        }
        if (y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        status = 0;
        std::swap(point.x, point.y);
        Vector2 resY = intersection(y0, y1, x0, x1, point, &status);
        std::swap(point.x, point.y);
        std::swap(resY.x, resY.y);
        if (status == -1 || status == -2)
            return status;
        if (status == 0)
            continue;
        if (resY.x > point.x) {
            if (rightL.first && right.x > resY.x || !rightL.first) {
                rightL.first = &circuit[i - 1];
                rightL.second = &circuit[i];
                right = resY;
            }
        }
        else {
            if (leftL.first && left.x < resY.x || !leftL.first) {
                leftL.first = &circuit[i - 1];
                leftL.second = &circuit[i];
                left = resY;
            }
        }
    }
    if (!leftL.first || !rightL.first || !topL.first || !bottomL.first)
        return 0;
    return isInside(leftL, 3) && isInside(rightL, 1) && isInside(topL, 0) &&
        isInside(bottomL, 2);
}
