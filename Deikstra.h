#pragma once

#include<queue>

#include"GraphBuilder.h"

std::vector<int> Deikstra(DecGraph& graph);
double h(const point_dec_d& a, const point_dec_d& b);
std::vector<int> path_constructor(int start, int a, const std::vector<int>& parent);