#pragma once
#include <vector>
#include "point.h"
#include <ostream>

struct DecGraph {
    std::vector<point_dec_d> coordinates;
    std::vector<std::vector<int>> adjList;
    int begin;
    int end;
};

std::ostream& operator<<(std::ostream& out, const DecGraph& graph);

using segment = std::pair<point_dec_d, point_dec_d>;
bool segments_intersect(segment a, segment b);

DecGraph makeGraph(std::vector<std::vector<point_dec_d>> const& circuits,
    point_dec_d begin, point_dec_d end);