#include <iostream>
#include <vector>

#define IMPLEMENT_GRAPH_HPP
#include "graph.hpp"

using std::vector;

struct push_relabel {
	graph capacity;
	graph flow;
	vector<int> label;
	vector<int> excess;
	int s, t;

	push_relabel(graph& G, int _s, int _t) : capacity(G), flow(G.vertices), s(_s), t(_t) {

		// Create pre-flow
		excess.resize(flow.vertices, 0);

		for(int v = 0; v < flow.vertices; v ++) {
			flow.at(s, v) += capacity.at(s, v);
			flow.at(s, v) += -capacity.at(s, v);
			excess[v] = capacity.at(s, v);
		}

		// Create labels
		label.resize(flow.vertices, 0);
		label[s] = flow.vertices;

		// Iterate through push / relabel operations
		int u = -1;

		do{
			u = active();

			// Push to all posible residual edges
			for(int v = 0; v < flow.vertices; v ++)
				if(residual(u, v) > 0)
					push(u, v);

			// Relabel if label[u] <= every label[v]
			relabel(u);

		}while(u != -1);
	}

	int active() {

		for(int u = 0; u < flow.vertices; u ++) {

			if(u == s || u == t)
				continue;

			if(can_push(u) || can_relabel(u))
				return u;
		}
		return -1;
	}

	int residual(int u, int v) {
		return capacity.at(u, v) - flow.at(u, v);
	}

	bool can_push(int u) {

		if(excess[u] <= 0)
			return false;

		for(int v = 0; v < flow.vertices; v ++) {

			if(residual(u, v) > 0 && label[u] == label[v] + 1)
				return true;
		}
		return false;
	}

	void push(int u, int v) {

		if(excess[u] <= 0 || label[u] != label[v] + 1)
			return;

		int move = std::min(excess[u], residual(u, v));
		flow.at(u, v) += move;
		flow.at(v, u) += -move;
		excess[u] -= move;
		excess[v] += move;
	}	

	bool can_relabel(int u) {

		if(excess[u] <= 0 || label[u] > flow.vertices-1)
			return false;

		int edges = 0;

		for(int v = 0; v < flow.vertices; v ++) {

			if(residual(u, v) > 0){
				edges ++;

				if(label[u] <= label[v])
					continue;
				else
					return false;
			}
		}
		return (edges > 0);
	}

	void relabel(int u) {

		if(excess[u] <= 0)
			return ;

		int min = -1;

		for(int v = 0; v < flow.vertices; v ++) {

			if(residual(u, v) > 0) {

				if(label[u] <= label[v]) {

					if(min == -1 || label[v] < min)
						min = label[v];

				}else
					return;
			}
		}

		if(min != -1)
			label[u] = 1 + min;
	}

	static graph max_flow(graph G, int s, int t) {
		push_relabel pr(G, s, t);
		return pr.flow;
	}
};

int main(int argc, char** argv) {
		
	if(argc <= 1){
		std::cout << "Usage: " << argv[0] << " graph_file\n";

	}else if(argc >= 2) {
		graph G(argv[1]);
		graph residual = push_relabel::max_flow(G, 0, G.vertices-1);

		int max_flow = 0;

		for(int u = 0; u < G.vertices; u ++) {
			for(int v = 0; v < G.vertices; v ++) {
				
				if(G.at(u, v) > 0)
					std::cout << "u: " << u << " v: " << v << " " << residual.at(u, v) << "\n";

				if(v == G.vertices-1)
					max_flow += residual.at(u, v);
			}
		}

		std::cout << "Total flow: " << max_flow << "\n";
	}
	return 0;
}
