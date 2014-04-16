/*
 * MoorePartial.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: micmath
 */
#include <vector>
#include <tuple>
#include "MoorePartial.hpp"

using std::vector;

template<int MT>
MoorePartial<MT>::MoorePartial() {
	current_vertex = 2 * MT;
	//Define the mappings between each pair of the MT - 1 groups
	//We can think of each mapping as a variable with a set of 56 possible values to
	//map too.
	mappings = decltype(mappings)();


	graph_data = decltype(graph_data)(MT * MT + 1, vector<int>());

	//Starting at vertex 0 has a link to vertices 1 to 57
	for (int i = 0; i < MT; i++) {
		graph_data[0].push_back(i + 1);
		graph_data[i + 1].push_back(0);
	}

	for (int i = 0; i < MT; i++) {
		for (int j = 0; j < MT - 1; j++) {
			//Vertex 1 to 57 have edges to vertex 58 to 58 + 57^2
			graph_data[i + 1].push_back(MT + 1 + i * (MT - 1) + j);
			//Vertex 58 to 58 + 57^2 have edges with 1 to 57
			graph_data[MT + 1 + i * (MT - 1) + j].push_back(i + 1);
		}
	}

	for (int j = 0; j < MT - 1; j++) {
		int vertex_index = MT + 1 + j;
		//For edge connections in each vertex
		for (int k = 1; k < MT; k++) {
			int edge_index = MT + 1 + k * (MT - 1) + j;
			//Each each of the 56 edges connect to one vertex in
			//each group
			graph_data[vertex_index].push_back(edge_index);
			graph_data[edge_index].push_back(vertex_index);
		}
	}
}

template<int MT>
MoorePartial<MT>::~MoorePartial() {}

template<int MT>
void MooreGraph<MT>::printAdjacency() {
	/*
	 Prints the adjacency list out for debugging purposes.
	 */
	for (int i = 0; i < MT * MT + 1; i++) {
		std::cout << i << ") ";
		if (graph_data[i].size() != 0) {
			for (int j = 0; j < graph_data[i].size() - 1; j++) {
				std::cout << graph_data[i][j] << ", ";
			}
			std::cout << graph_data[i][graph_data[i].size() - 1] << std::endl;
		} else
			std::cout << "Empty row" << std::endl;
	}
	std::cout << std::endl;
}

template<int MT>
void MoorePartial<MT>::possible_Values(Edge &edge){

}

