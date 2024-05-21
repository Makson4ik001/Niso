#include<queue>
#include"GraphBuilder.h"
#include"Deikstra.h"

/*
std::vector<int> A_star(DecGraph& graph) {
	int begin = graph.begin, end = graph.end;
	size_t count_graph = graph.coordinates.size();
	std::vector<int> processed;
	std::vector < std::pair<float, int>> distance; //<расстояние, индекс предыдущего>
	std::priority_queue<std::pair<float, int>> q;
	
	distance.resize(count_graph);
//	for (int i = 1; i < count_graph; i++) {
//		distance[i].first = 100000;
//	}

	distance[begin].first = 0;
	q.push({ 0,begin });
	while (!q.empty()) {
		int a = q.top().second; q.pop();
		if (a == end) {
			return path_constructor(begin, a, distance);
		}


		for (auto u : graph.adjList[a]) {

			//вычислить расстояние w
			int w = h(graph.coordinates[a], graph.coordinates[u]);
			int b = u;
			if (distance[a].first + w < distance[b].first) {
				distance[b].first = distance[a].first + w;
				distance[b].second = a;
				q.push({ -distance[b].first + h(graph.coordinates[u],graph.coordinates[end]),b });
			}
		}
	}
	return path_constructor(begin, end, distance);;

}*/


std::vector<int> A_star(DecGraph& graph) {
    int begin = graph.begin, end = graph.end;
    size_t count_graph = graph.coordinates.size();
    std::vector<int> processed(count_graph, 0);
    std::vector<float> distance(count_graph, std::numeric_limits<float>::max());
    std::vector<int> parent(count_graph, -1);
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> q;

    distance[begin] = 0;
    q.push({ 0, begin });

    while (!q.empty()) {
        int a = q.top().second;
        q.pop();
        if (a == end) {
            return path_constructor(begin, end, parent);
        }

        if (processed[a]) continue; // Пропустить обработанные вершины

        processed[a] = 1;

        for (auto u : graph.adjList[a]) {
            float w = h(graph.coordinates[a], graph.coordinates[u]);
            int b = u;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                parent[b] = a;
                q.push({ distance[b] + h(graph.coordinates[u], graph.coordinates[end]), b });
            }
        }
    }
    // Путь не найден
    return std::vector<int>();
}