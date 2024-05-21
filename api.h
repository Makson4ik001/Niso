#pragma once

#include <vector>
#include "point.h"

namespace PathFinder {

	using Circuits = std::vector<std::vector<point_dec>>;
	using Path = std::vector<point_dec>;

	Path findPath(const Circuits& circuits, point_dec begin, point_dec end);

};
