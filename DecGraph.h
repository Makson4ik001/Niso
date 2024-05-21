#pragma once
#include<vector>
#include "point.h"

struct DecGraph {
	std::vector<point_dec_d> coordinates;
	std::vector<std::vector<int>> adjList;

	int begin;
	int end;
};