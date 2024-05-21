#pragma once
#include "point.h"
#include <vector>
struct PointInCircuit{
   int circuit;
   int point;
};

struct Join {
   PointInCircuit begin;
   PointInCircuit end;
};

std::vector<Join> findMatches(std::vector<std::vector<point_dec_d>> const &circuits);

std::vector<std::vector<size_t>> create_graph(const std::vector<Join> &matches, size_t circuits_amount);

std::vector<std::vector<size_t>>
cycled_connectivity_components(const std::vector<std::vector<size_t>> &graph);

std::vector<std::vector<point_dec_d>> connectedCircuits(
      std::vector<std::vector<point_dec_d>> const &circuits);
