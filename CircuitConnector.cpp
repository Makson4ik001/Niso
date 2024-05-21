#include "CircuitConnector.h"
#include <iostream>
#include <stack>
using namespace std;

/*
vector<vector<point_dec>> simple_connector(vector<vector<point_dec>> const &circuits) {
   vector<vector<point_dec>> circuits_copy = circuits;
   vector<vector<point_dec>> result;
   unordered_set<size_t> used;
   for (size_t i = 0; i < circuits_copy.size(); i++) {
      if (circuits[i].size() <= 1)
	 continue;
      vector<point_dec> &current = circuits_copy[i];
      auto min_distance = (current[0] - current[current.size() - 1]);

   }
   

   return result;
}
*/
vector<Join> findMatches(vector<vector<point_dec_d>> const &circuits) {
  constexpr float EPS = 1e-2;
  vector<Join> matches;
  // O(n^2), n circuits
  ////possible to reimplement in O(nlogn) with AABB tree
  if (!circuits.size())
    return {};
  for (int i = 0; i < circuits.size() - 1; i++) {

    if (circuits[i].size() <= 1)
      continue;
    int lend = circuits[i].size() - 1;

    // closed
    if ((circuits[i][0] - circuits[i][circuits[i].size() - 1]).length() < EPS) {
      matches.push_back({{i, 0}, {i, lend}});
      continue;
    }

    for (int k = i + 1; k < circuits.size(); k++) {
      if (circuits[k].size() <= 1)
        continue;
      int rend = circuits[k].size() - 1;
      // assuming only first and last points of circuit are connected

      if ((circuits[i][0] - circuits[k][0]).length() < EPS)
        matches.push_back({{i, 0}, {k, 0}});

      if ((circuits[i][lend] - circuits[k][0]).length() < EPS)
        matches.push_back({{i, lend}, {k, 0}});

      if ((circuits[i][0] - circuits[k][rend]).length() < EPS)
        matches.push_back({{i, 0}, {k, rend}});

      if ((circuits[i][lend] - circuits[k][rend]).length() < EPS)
        matches.push_back({{i, lend}, {k, rend}});
    }
  }
  return matches;
}

vector<vector<size_t>> create_graph(const vector<Join> &matches,
                                    size_t circuits_amount) {
  // begin - index
  // end - size(circuits) + index
  vector<vector<size_t>> result(2 * circuits_amount);
  for (auto &&match : matches) {
    size_t first = match.begin.circuit, last = match.end.circuit;
    if (match.begin.point)
      first += circuits_amount;
    if (match.end.point)
      last += circuits_amount;
    result[first].push_back(last);
    result[last].push_back(first);
  }

  return result;
}

inline constexpr size_t switch_begin_with_end(size_t target,
                                              size_t nodes_amount) {
  if (target < nodes_amount)
    return target + nodes_amount;
  return target - nodes_amount;
}

vector<size_t> restore_path(const vector<size_t> &visited, size_t finish) {
   vector<size_t> res = {};
   size_t current = finish;
   while (visited[current] != current) {
      res.push_back(current);
      current = visited[current];
   }
   res.push_back(current);
   return res;
}

void show_connector_error_message() {
   std::cerr << "VERY BAD THING IN CIRCUIT CONNECTOR HAPPENED! SOME "
      "CIRCUIT GRAPH IS LIKE THAT:\n";
   std::cerr 
      << "*-*-*-*\n"
      << "  |   |\n"
      << "  *   *\n"
      << "  |   |\n"
      << "  *-*-*\n";
   std::cerr << "\nBut it's expected to either be\n__This__\n*\n__This__\n";
   std::cerr 
      << " *-*-*\n"
      << " |   |\n"
      << " *   *\n"
      << " |   |\n"
      << " *-*-*\n";
   std::cerr << "\n__This__\n*-*-*-*-*\n";
   std::cerr << "current situation is indicative of bad circuit data source" << std::endl;
}

vector<vector<size_t>>
cycled_connectivity_components(const vector<vector<size_t>> &graph) {
   vector<vector<size_t>> paths;
   constexpr size_t NOT_VISITED = (size_t)(-1);
   vector<size_t> visited(graph.size(), NOT_VISITED);
   size_t nodes_amount = graph.size() / 2;

   size_t cur_last = 0;
   size_t last[] = {NOT_VISITED, NOT_VISITED};
   auto dfs = [&cur_last, &last, &visited, &graph,&nodes_amount](stack<size_t> &s, size_t start) -> void {
      while (!s.empty()) {
	 size_t current = s.top();
	 s.pop();
	 last[cur_last] = current;
	 auto switched_current = switch_begin_with_end(current, nodes_amount);
	 if (visited[switched_current] != NOT_VISITED) {
	    if (visited[switched_current] == start) {
	       return;
	    }
	    cout << "first problem!" << endl;
	    show_connector_error_message();
	    last[cur_last] = NOT_VISITED;
	    break;
	 }
	 last[cur_last] = switched_current;
	 visited[switched_current] = current;
	 for (auto &&child : graph[switched_current]) {
	    if (visited[child] == NOT_VISITED) {
	       visited[child] = switched_current;
	       s.push(child);
	       continue;
	    }
	    if (child == start) {
	       return;
	    }
	    cout << "second problem!" << endl;
	    show_connector_error_message();
	    last[cur_last] = NOT_VISITED;
	    return;
	 }
      }
      cur_last++;
   };
   for (size_t start = 0; start < visited.size(); start++) {
      if (visited[start] != NOT_VISITED || graph[start].size() == 0)
	 continue;

      auto switched_start = switch_begin_with_end(start, nodes_amount);
      // if component is closed by itself
      if (graph[start][0] == switched_start) {
	 visited[start] = start;
	 visited[switched_start] = start;
	 paths.push_back({start, switched_start});
	 continue;
      }
      cur_last = 0;
      last[0] = NOT_VISITED;
      last[1] = NOT_VISITED;
      stack<size_t> s;
      s.push(start);
      visited[start] = start;
      cout << start << endl;
      dfs(s, start);
      if (last[cur_last] == NOT_VISITED)
	 continue;
      if (cur_last == 0) {
	 paths.push_back(restore_path(visited, last[0]));
	 continue;
      }
      auto lpath = restore_path(visited, last[0]);
      visited[switched_start] = switched_start;
      if (graph[start].size() > 0)
	 dfs(s, switched_start);
      auto rpath = restore_path(visited, last[1]);
      auto last_path = paths.size();
      paths.push_back(lpath);
      paths[last_path].insert(paths[last_path].end(), rpath.begin() + 1, rpath.end());
   }
   return paths;
}


vector<vector<point_dec_d>> connectedCircuits(
    vector<vector<point_dec_d>> const& circuits) {
    auto matches = findMatches(circuits);
    auto graph = create_graph(matches, circuits.size());
    auto paths = cycled_connectivity_components(graph);
    vector<vector<point_dec_d>> result;
    for (auto& path : paths) {
        if (path.size() == 0)
            continue;
        result.push_back({});
        auto& to_append = result[result.size() - 1];
        size_t first = 0, last = path.size() - 1;
        size_t half_nodes = circuits.size();
        if (path[first] == path[last] + half_nodes || path[last] == path[first] + half_nodes) {
            for (size_t cur = 0; cur + 1 <= last; cur++)
                swap(path[cur], path[cur + 1]);
        }
        for (size_t cur = 0; cur < path.size(); cur += 2) {
            if (path[cur] < half_nodes)
                to_append.insert(to_append.end(), circuits[path[cur]].begin(), circuits[path[cur]].end() - 1);
            else
                to_append.insert(to_append.end(), circuits[path[cur + 1]].rbegin(), circuits[path[cur + 1]].rend() - 1);
        }
        to_append.push_back(to_append[0]);
    }
    return result;
}

