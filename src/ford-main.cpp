#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>

#define IMPLEMENT_GRAPH_HPP
#include "graph.hpp"

int main(int argc, char** argv) {

	using clock = std::chrono::steady_clock;
	using time = clock::time_point;

	if(argc <= 1){
		std::cout << "Usage: " << argv[0] << " graph_file\n";

	}else if(argc >= 2) {

		std::ifstream file(argv[1]);

		if(file.fail()){
			std::cerr << "Failed to open graph file\n";
			return 1;
		}

		// Initialize graph
		graph G(file);

		file.close();

		time start, end;
		double delta;

		// Runtime analysis of Ford Fulkerson O(VE^2)
		start = clock::now();
		graph residual = fordFulkerson(G, 0, G.vertices-1);
		end = clock::now();
		delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		std::cout << get_max_flow(G, residual) << ", " << delta << "\n";

	}
	return 0;
}