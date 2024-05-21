
#include<queue>
#include"GraphBuilder.h"


std::vector<int> path_constructor(int start, int a, const std::vector<int>& parent) {
	std::vector<int> res;
	res.push_back(a);
	while (a != start) {
		a = parent[a];
		res.push_back(a);
	}
	std::reverse(res.begin(), res.end());
	return res;
}

double h(const point_dec_d& a, const point_dec_d& b) {
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

std::priority_queue;


std::vector<int> Deikstra(DecGraph& graph) {
    int begin = graph.begin, end = graph.end;
    size_t count_graph = graph.coordinates.size();
    std::vector<bool> processed(count_graph, false);
    std::vector<float> distance(count_graph, std::numeric_limits<float>::max());
    std::vector<int> parent(count_graph, -1);
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> q;

    distance[begin] = 0;
    q.push({ 0, begin });

    while (!q.empty()) {
        int a = q.top().second;
        q.pop();

        if (processed[a]) continue; // Пропуск уже обработанных вершин
        processed[a] = true;

        if (a == end) {
            return path_constructor(begin, a, parent);
        }

        for (auto u : graph.adjList[a]) {
            float w = h(graph.coordinates[a], graph.coordinates[u]);
            int b = u;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                parent[b] = a;
                q.push({ distance[b], b });
            }
        }
    }
    // Путь не найден
    return std::vector<int>();
}
