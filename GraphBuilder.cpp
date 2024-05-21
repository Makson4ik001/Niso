#include "GraphBuilder.h" 
#include "CircuitConnector.h"

using namespace std;
std::ostream& operator<<(std::ostream& out, const DecGraph& graph) {
	for (size_t i = 0; i < graph.coordinates.size(); i++)
		out << i << ": {" << graph.coordinates[i].x << ", " << graph.coordinates[i].y << "}\n";
	out << "\n";

	for (size_t i = 0; i < graph.adjList.size(); i++) {
		out << i << " -> [";
		for (auto vertex : graph.adjList[i])
			out << vertex << ", ";
		out << "]\n";
	}
	out << "\n";

	return out;
}

bool segments_on_axis_intersects(double al, double ar, double bl, double br) {
	return !(min(al, ar) > max(bl, br) || min(bl, br) > max(al, ar));
}

bool segments_intersect(segment a, segment b) {
	constexpr double EPS = 1e-6;
	//zero vectors
	if ((a.first - b.first).length() < EPS || (a.second - b.second).length() < EPS ||
		(a.first - b.second).length() < EPS || (a.second - b.first).length() < EPS)
		return false;
	//have common point
	if ((a.first - a.second).length() < EPS || (b.first - b.second).length() < EPS)
		return false;
	//AABB collision check
	if (!segments_on_axis_intersects(a.first.x, a.second.x, b.first.x, b.second.x) &&
		!segments_on_axis_intersects(a.first.y, a.second.y, b.first.y, b.second.y))
		return false;
	auto tana = a.second - a.first;
	auto tanb = b.second - b.first;
	point_dec_d norma = { tana.y, -tana.x }, normb = { tanb.y, -tanb.x };
	auto mCa = (norma.dotProduct(a.first)), mCb = (normb.dotProduct(b.first));
	auto det = norma.x * normb.y - normb.x * norma.y; // |norma.x norma.y|
							  // |normb.x normb.y|
	if (fabs(det) < EPS)
		return norma.dotProduct(b.first) - mCa < EPS;
	auto detx = mCa * normb.y - mCb * norma.y; // |mCa norma.y|
						   // |mCb normb.y|

	auto dety = norma.x * mCb - normb.x * mCa; // |norma.x mCa|
						   // |normb.x mCb|
	point_dec_d intersection = { detx / det, dety / det };
	auto la = a.first.x, ra = a.second.x, lb = b.first.x, rb = b.second.x;
	if (la > ra)
		swap(la, ra);
	if (lb > rb)
		swap(lb, rb);
	return intersection.x >= la && intersection.x <= ra &&
		intersection.x >= lb && intersection.x <= rb;
}

bool external_collision(segment seg, const vector<point_dec_d>& circuit) {
	for (size_t i = 0; i + 1 < circuit.size(); i++) {
		auto beg = i;
		auto end = i + 1;
		if (segments_intersect(seg, { circuit[beg], circuit[end] }))
			return true;
	}
	return false;
}

bool point_inside(point_dec_d point, const vector<point_dec_d>& circuit) {
	constexpr double EPS = 1e-6;
	size_t cnt = 0;
	for (size_t i = 0; i + 1 < circuit.size(); i++) {
		auto on_circuit = segment{ circuit[i], circuit[i + 1] };

		// outside of aabb collision
		if (max(on_circuit.first.x, on_circuit.second.x) < point.x ||
			min(on_circuit.first.x, on_circuit.second.x) > point.x)
			continue;
		auto diff = on_circuit.second - on_circuit.first;

		//vertical
		if (fabs(diff.x) < EPS) {
			if (point.y >= min(on_circuit.second.y, on_circuit.first.y) &&
				point.y <= max(on_circuit.first.y, on_circuit.second.y))
				return true;
			continue;
		}

		auto k = diff.y / diff.x;
		auto b = on_circuit.first.y - on_circuit.first.x * k;
		auto intersection = point_dec_d{ point.x, k * point.x + b };
		if (intersection.y < point.y)
			continue;
		if ((intersection - point).length() < EPS)
			return true;
		if ((intersection - on_circuit.first).length() < EPS ||
			(intersection - on_circuit.second).length() < EPS)
			cnt += 1;
		else
			cnt += 2;
	}
	return (cnt / 2) % 2;
}

bool self_collision(segment seg, const vector<point_dec_d>& circuit) {
	return external_collision(seg, circuit) ||
		point_inside(seg.first + (seg.second - seg.first) * 0.5, circuit);
}

DecGraph makeGraph(std::vector<std::vector<point_dec_d>> const& circuits,
	point_dec_d begin, point_dec_d end) {
	DecGraph res;
	//point mapping:
	int vertex_amount = 2; //begin, end
	for (auto& circuit : circuits)
		vertex_amount += circuit.size() > 0 ? circuit.size() - 1 : 0;
	res.adjList.resize(vertex_amount);
	res.coordinates.resize(vertex_amount);

	size_t i = 0;
	//every point with start, end
	for (size_t circuit = 0; circuit < circuits.size(); circuit++) {
		for (size_t point = 0; point + 1 < circuits[circuit].size(); point++) {
			bool begin_ok = true, end_ok = true;
			segment begin_seg = { begin, circuits[circuit][point] },
				end_seg = { end, circuits[circuit][point] };
			for (size_t collide_check = 0; collide_check < circuits.size(); collide_check++) {
				if (collide_check == circuit) {
					if (begin_ok)
						begin_ok = (!self_collision(begin_seg, circuits[collide_check]));
					if (end_ok)
						end_ok = (!self_collision(end_seg, circuits[collide_check]));
				}
				else {
					if (begin_ok)
						begin_ok = (!external_collision(begin_seg, circuits[collide_check]));
					if (end_ok)
						end_ok = (!external_collision(end_seg, circuits[collide_check]));
				}
				if (!(begin_ok || end_ok))
					break;
			}

			if (begin_ok) {
				res.adjList[vertex_amount - 2].push_back(i);
				res.adjList[i].push_back(vertex_amount - 2);
			}
			if (end_ok) {
				res.adjList[vertex_amount - 1].push_back(i);
				res.adjList[i].push_back(vertex_amount - 1);
			}
			i++;
		}
	}

	//begin to end
	segment begin_to_end = { begin, end };
	bool begin_to_end_ok = true;
	for (auto&& circuit : circuits)
		if (external_collision(begin_to_end, circuit)) {
			begin_to_end_ok = false;
			break;
		}

	if (begin_to_end_ok) {
		res.adjList[vertex_amount - 2].push_back(vertex_amount - 1);
		res.adjList[vertex_amount - 1].push_back(vertex_amount - 2);
	}

	//every point with every other point on other circuits:
	i = 0;
	for (size_t left = 0; left + 1 < circuits.size(); left++) {
		if (circuits[left].size() == 0)
			continue;
		size_t j = i + circuits[left].size() - 1;
		for (size_t right = left + 1; right < circuits.size(); right++) {
			if (circuits[right].size() == 0)
				continue;
			for (size_t point_on_left = 0; point_on_left + 1 < circuits[left].size(); point_on_left++) {
				for (size_t point_on_right = 0; point_on_right + 1 < circuits[right].size(); point_on_right++) {
					segment seg = { circuits[left][point_on_left], circuits[right][point_on_right] };
					bool seg_ok = true;
					for (size_t to_collide = 0; to_collide < circuits.size(); to_collide++) {
						if (to_collide == left || to_collide == right)
							seg_ok = self_collision(seg, circuits[to_collide]);
						else
							seg_ok = external_collision(seg, circuits[to_collide]);
						if (!seg_ok)
							break;
					}
					if (seg_ok) {
						res.adjList[i + point_on_left].push_back(j + point_on_right);
						res.adjList[j + point_on_right].push_back(i + point_on_left);
					}
				}
			}
			j += circuits[right].size() - 1;
		}
		i += circuits[left].size() - 1;
	}

	//every circuit within itself
	i = 0;
	for (size_t target = 0; target < circuits.size(); target++) {
		auto& circuit = circuits[target];
		if (circuit.size() == 0)
			continue;
		auto sz = circuit.size() - 1;
		for (size_t point_l = 0; point_l + 2 < circuit.size(); point_l++) {
			for (size_t point_r = point_l + 1; point_r < circuit.size(); point_r++) {
				bool seg_ok = true;
				if (point_l + 1 != point_r) {
					segment seg = { circuit[point_l], circuit[point_r] };
					for (size_t to_collide = 0; to_collide < circuits.size(); to_collide++) {
						if (to_collide == target)
							seg_ok = self_collision(seg, circuits[to_collide]);
						else
							seg_ok = external_collision(seg, circuits[to_collide]);
						if (!seg_ok)
							break;
					}
				}
				if (seg_ok) {
					res.adjList[i + point_l].push_back(i + point_r);
					res.adjList[i + point_r].push_back(i + point_l);
				}
			}
		}
		i += sz;
	}


	i = 0;
	for (auto&& circuit : circuits) {
		for (size_t j = 0; j + 1 < circuit.size(); j++)
			res.coordinates[i++] = circuit[j];
	}
	res.coordinates[vertex_amount - 2] = begin;
	res.coordinates[vertex_amount - 1] = end;
	res.begin = vertex_amount - 2;
	res.end = vertex_amount - 1;

	return res;
}