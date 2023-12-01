#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>

#define IMPLEMENT_GRAPH_HPP
#include "graph.hpp"

using std::vector;
using std::queue;

struct push_relabel {
	graph capacity;
	graph flow;
	vector<int> label;
	vector<int> excess;
	queue<int> queue;
	int s, t, vertices;

	push_relabel(graph& G, int _s, int _t) : capacity(G), flow(G.vertices), s(_s), t(_t), vertices(G.vertices) {

		// Initialize excess(x), x[s] = infinity
		excess.resize(vertices, 0);
		excess[s] = std::numeric_limits<int>::max();

		// Initialize label(l), l[s] = vertices
		label.resize(vertices, 0);
		label[s] = vertices;

		// Create Pre-Flow, s to v edges
		for(int v = 0; v < vertices; v ++) 
			push(s, v);

		while(queue.size() > 0){

			int u = queue.front();
			queue.pop();

			// Try push / relabel operations
			for(int v = 0; v < vertices; v ++)
				push(u, v);

			relabel(u);

			// Re-add to queue if there is still excess
			if(excess[u] > 0)
				queue.push(u);
		}
	}

	int residual(int u, int v) {
		return capacity.at(u, v) - flow.at(u, v);
	}

	bool push(int u, int v) {

		if(excess[u] <= 0)
			return false;

		// Except s, for Pre-Flow
		if(label[u] != label[v] + 1 && u != s)
			return false;

		if(residual(u, v) <= 0)
			return false;

		// Push flow through and create residial edge
		int move = std::min(excess[u], residual(u, v));
		flow.at(u, v) += move;
		flow.at(v, u) += -move;
		excess[u] -= move;
		excess[v] += move;

		// Add to queue
		if(v != s && v != t)
			queue.push(v);

		return true;
	}	

	bool relabel(int u) {

		if(excess[u] <= 0)
			return false;

		// Look for the min adjacent label
		int min = 2 * vertices;

		for(int v = 0; v < vertices; v ++) {

			if(residual(u, v) > 0) {

				if(label[v] < min)
					min = label[v];
			}
		}

		// Check for invalid labelling
		if(min < label[u])
			return false;

		label[u] = 1 + min;
		return true;
	}

	static graph max_flow(graph G, int s, int t) {
		push_relabel pr(G, s, t);
		return pr.flow;
	}
};

int main(int argc, char** argv) {

	using clock = std::chrono::steady_clock;
	using time = clock::time_point;

	if(argc <= 1){
		std::cout << "Usage: " << argv[0] << " graph_file\n";

	}else if(argc >= 2) {

		// Initialize graph
		graph G(argv[1]);

		time start, end;
		double delta1, delta2;

		// Runtime analysis of Push-Relabel O(V^3)
		start = clock::now();
		graph residual1 = push_relabel::max_flow(G, 0, G.vertices-1);
		end = clock::now();
		delta1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		// Runtime analysis of Ford-Fulkerson O(VE^2) ~ O(V^5)
		start = clock::now();		
		graph residual2 = fordFulkerson(G, 0, G.vertices-1);
		end = clock::now();
		delta2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		std::cout << "Showing Network Flows: \n";
		print_flow_network(G, residual1);
		print_flow_network(G, residual2);

		std::cout << "-------------------------\n";

		std::cout << "Algorithm\tRuntime\n";
		std::cout << "Push Relabel\t" << delta1 << "\n";
		std::cout << "Ford Fulkerson\t" << delta2 << "\n";
		double pdiff = (1 - delta1 / delta2) * 100;
		std::cout << "Push Relabel was " << pdiff << "% " << ((pdiff < 0) ? "slower\n" : "faster\n");
	}
	return 0;
}
