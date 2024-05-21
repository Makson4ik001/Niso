#pragma once
#include "point.h"
#include <vector>
using Vector2 = point_dec_d;

int contains(std::vector<Vector2> const& circuit, Vector2 point);
