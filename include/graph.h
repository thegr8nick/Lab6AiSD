
#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>

using namespace std;

const int INF = 100000000000;

enum class Color {
	White, 
	Black,
	Grey 
};

template<typename Vertex, typename Distance = double>
class Graph {
public:
	struct Edge {
		Vertex from, to;
		Distance distance;
	};
private:
	vector<Vertex> _vertices;
	unordered_map<Vertex, vector<Edge>> _edges;
public:
	bool has_vertex(const Vertex& v) const {
		return find(_vertices.begin(), _vertices.end(), v) != _vertices.end();
	}

	bool add_vertex(const Vertex& v) {
		if (!has_vertex(v)) {
			_vertices.push_back(v);
			_edges[v] = {};
			return true;
		}
		return false;
	}

	bool remove_vertex(const Vertex& v) {
		auto it = find(_vertices.begin(), _vertices.end(), v);
		if (it == _vertices.end())
			return false;
		_vertices.erase(it);
		_edges.erase(v);
		for (auto& pair : _edges) {
			auto& edges = pair.second;
			edges.remove_if([v](const Edge& edge) {return edge.to == v; });
		}
		return true;
	}

	vector<Vertex> vertices() const {
		return _vertices;
	}

	bool add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
		_edges[from].push_back({ from, to, d });
		return true;
	}

	bool remove_edge(const Vertex& from, const Vertex& to) {
		auto it = find_if(_edges[from].begin(), _edges[from].end(), [to](const Edge& edge) {return edge.to == to; });
		if (it == _edges[from].end())
			return false;
		_edges[from].erase(it);
		return true;
	}

	bool remove_edge(const Edge& e) {
		auto it = find(_edges[e.from].begin(), _edges[e.from].end(), e);
		if (it == _edges[e.from].end())
			return false;
		_edges[e.from].erase(it);
		return true;
	}

	bool has_edge(const Vertex& from, const Vertex& to) {
		auto it = find_if(_edges[from].begin(), _edges[from].end(), [to](const Edge& edge) {return edge.to == to; });
		if (it != _edges[from].end())
			return false;
		return true;
	}

	bool has_edge(const Edge& e) {
		const auto& edges = _edges[e.from];
		return find(edges.begin(), edges.end(), e) != edges.end();
	}

	vector<Edge> edges(const Vertex& vertex) {
		if (!has_vertex(vertex))
			out_of_range("Don't have vertex.");
		return _edges[vertex];
	}

	size_t order() const {
		return _vertices.size();
	}

	size_t degree(const Vertex& v) const {
		if (!has_vertex(v))
			throw out_of_range("Vertex is not found.");
		return _edges.at(v).size();
	}

	vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
		if (!has_vertex(from) || !has_vertex(to)) {
			return {};
		}
		for (const auto& edges_pair : _edges) {
			for (const auto& edge : edges_pair.second) {
				if (edge.distance < 0) {
					throw runtime_error("Graph contains an edge with negative weight.");
				}
			}
		}
		unordered_map<Vertex, Distance> distance;
		unordered_map<Vertex, Vertex> prev;
		priority_queue<pair<Distance, Vertex>, vector<pair<Distance, Vertex>>, greater<pair<Distance, Vertex>>> pq;
		vector<Edge> sp;
		for (const auto& vertex : _vertices) {
			distance[vertex] = INF;
			prev[vertex] = Vertex();
		}
		distance[from] = 0;
		pq.push({ 0, from });
		while (!pq.empty()) {
			Vertex u = pq.top().second;
			pq.pop();
			for (auto& edge : _edges.at(u)) {
				Vertex v = edge.to;
				Distance alt = distance[u] + edge.distance;
				if (alt < distance[v]) {
					distance[v] = alt;
					prev[v] = u;
					pq.push({ alt, v });
				}
			}
		}
		if (prev.find(to) == prev.end()) {
			return {};
		}
		Vertex current = to;
		while (current != from) {
			Vertex parent = prev[current];
			if (parent == Vertex())
				return {};
			for (auto& edge : _edges.at(parent)) {
				if (edge.to == current) {
					sp.push_back(edge);
					break;
				}
			}
			current = parent;
		}
		reverse(sp.begin(), sp.end());
		return sp;
	}

	vector<Vertex> bfs(const Vertex& start_vertex, function<void(const Vertex&)> action) const {
		queue<Vertex> q;
		unordered_map<Vertex, Color> color;
		unordered_map<Vertex, Vertex> parent;
		unordered_map<Vertex, Distance> distance;
		vector<Vertex> visited_vertices;
		for (const auto& vertex : _vertices) {
			color[vertex] = Color::White;
			parent[vertex] = Vertex();
			distance[vertex] = INF;
		}
		q.push(start_vertex);
		color[start_vertex] = Color::Grey;
		distance[start_vertex] = 0;
		while (!q.empty()) {
			Vertex u = q.front();
			q.pop();
			visited_vertices.push_back(u);
			if (action) {
				action(u);
			}
			for (const auto& edge : _edges.at(u)) {
				Vertex v = edge.to;
				if (color[v] == Color::White) {
					color[v] = Color::Grey;
					distance[v] = distance[u] + 1;
					parent[v] = u;
					q.push(v);
				}
			}
			color[u] = Color::Black;
		}
		return visited_vertices;
	}

	void print() const {
		for (const auto& vertex : _vertices) {
			cout << "Vertex: " << vertex << endl;
			cout << "Edges: " << endl;
			if (_edges.find(vertex) != _edges.end()) {
				const auto& edges = _edges.at(vertex);
				if (!edges.empty()) {
					for (const auto& edge : edges) {
						cout << "From: " << edge.from << " To: " << edge.to << " Distance: " << edge.distance << endl;
					}
				}
				else {
					cout << "No outgoing edges." << endl;
				}
			}
			else {
				cout << "No outgoing edges." << endl;
			}
			cout << "" << endl;
		}
	}

	Vertex find_warehouse() const {
		Vertex warehouse;
		Distance min_max_distance = INF;
		for (const auto& from : _vertices) {
			Distance max_distance = 0;
			for (const auto& to : _vertices) {
				if (from != to) {
					vector<Edge> path = shortest_path(from, to);
					Distance d = 0;
					for (const auto& edge : path) {
						d += edge.distance;
					}
					if (d > max_distance) {
						max_distance = d;
					}
				}
			}
			if (max_distance < min_max_distance) {
				min_max_distance = max_distance;
				warehouse = from;
			}
		}
		return warehouse;
	}
};