#pragma once
#include<vector>
#include "point.h"

std::vector<point_dec_d> DouglasPeucker(std::vector<point_dec_d> contour , double epsilon);
