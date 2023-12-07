#ifndef HEADER_ONLY_GRAPH_HPP
#define HEADER_ONLY_GRAPH_HPP

#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <climits>
#include <cstdio>
#include <cstring>

/*
 * Wrapper struct of the int* buffer of the graph
 */
struct graph {
	int vertices = 0;
	int* buffer = NULL;

	graph(int v);
	graph(std::istream& file);	
	graph(const graph& copy);	
	~graph();

	int& at(int u, int v);
};

/*
 * Print the flow network from a residual product graph
 */
void print_flow_network(graph G, graph residual);

/* C++ Program to Implement Network Flow Problem
 * Gara Pruesse, starting with code from sanfoundry.com/cpp-program/network-flow-problem/
 * Oct 2023
 */
bool bfs(graph& residual, int s, int t, std::vector<int>& parent);
graph fordFulkerson(graph G, int s, int t);

#endif



#ifdef IMPLEMENT_GRAPH_HPP

/*
 *	Initialize to the empty edge set
 */
graph::graph(int v) {
	vertices = v;
	buffer = new int[vertices*vertices];
	std::memset(buffer, 0, sizeof(int) * vertices*vertices);
}

/*
 *	Initialize by the edge set in a file
 */
graph::graph(std::istream& file) {

	std::string line;
	std::getline(file, line);

	if(std::sscanf(line.c_str(), "%d", &vertices) != 1) {
		vertices = -1;
		return;
	}

	buffer = new int[vertices*vertices];
	std::memset(buffer, 0, sizeof(int) * vertices*vertices);

	while(!file.eof() && file.good()) {
		int u, v, flow;
		
		std::getline(file, line);

		if(std::sscanf(line.c_str(), "%d %d %d", &u, &v, &flow) != 3)
			break;

		at(u, v) = flow;
	}
}

/*
 * Copy contents of graph
 */
graph::graph(const graph& copy) {
	
	vertices = copy.vertices;
	buffer = new int[vertices*vertices];
	std::memcpy(buffer, copy.buffer, sizeof(int) * vertices*vertices);
}

graph::~graph() {
	if(buffer)
		delete [] buffer;
}

int& graph::at(int u, int v) {
	return buffer[u + v * vertices];
}

void print_flow_network(graph G, graph residual) {
	int max_flow = 0;

	for(int u = 0; u < G.vertices; u ++) {
		for(int v = 0; v < G.vertices; v ++) {
			
			if(G.at(u, v) > 0) {
				std::cout << "u: " << u << " v: " << v << " ";
				std::cout << residual.at(u, v) << "/" << G.at(u, v) << "\n";
			}

			if(v == G.vertices-1)
				max_flow += residual.at(u, v);
		}
	}
	std::cout << "Total flow: " << max_flow << "\n";	
}

int get_max_flow(graph G, graph residual) {
	int maxflow = 0;

	for(int u = 0; u < G.vertices; u ++) 
		maxflow += residual.at(u, G.vertices-1);

	return maxflow;
}
 
/*
 * Returns true if there is a path from source 's' to sink 't' in
 * residual graph. Also fills parent[] to store the path *
 */
bool bfs(graph& residual, int s, int t, std::vector<int>& parent) {

	std::vector<bool> visited;
	visited.resize(residual.vertices, false);

	std::queue <int> q;
	q.push(s);

	visited[s] = true;
	parent[s] = -1;

	while (!q.empty()) {
		int u = q.front();
       	q.pop();

	    for (int v = 0; v < residual.vertices; v ++) {

			if (!visited[v] && residual.at(u, v) > 0) {
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}
	return visited[t];
}
 
/*
 * Returns the maximum flow from s to t in the given graph
 */
graph fordFulkerson(graph G, int s, int t) {

	// Error checking graph is OK
	if(s == t || G.vertices <= 0)
		return graph(0);

	int u, v;

	std::vector<int> parent;
	parent.resize(G.vertices);
		 
	graph residual(G);

	while(bfs(residual, s, t, parent)) {
		int path_flow = INT_MAX;

		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = std::min(path_flow, residual.at(u,v));
		}

		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			residual.at(u, v) -= path_flow;
			residual.at(v, u) += path_flow;
		}
	}

	graph flow(G.vertices);

	for(int u = 0; u < G.vertices; u ++) {
		for(int v = 0; v < G.vertices; v ++) {
			flow.at(u, v) = G.at(u, v) - residual.at(u, v);
		}
	}

    return flow;
}

#endif
