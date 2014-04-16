#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "MooreGraph.hpp"

template<int MT>
MooreGraph<MT>::MooreGraph(vector<vector<int> > vect) {
	graph_data = vect;
}

template<int MT>
MooreGraph<MT>::MooreGraph() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator = decltype(generator)(seed);
	graph_data = decltype(graph_data)(MT * MT + 1, vector<int>());
	initialize();
}

template<int MT>
MooreGraph<MT>::MooreGraph(std::default_random_engine generator) {
	this->generator = generator;
	graph_data = decltype(graph_data)(MT * MT + 1, vector<int>());
	initialize();
}


template<int MT>
void MooreGraph<MT>::initialize(){
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

	//Generating random k-regular graphs is very hard to do
	//so I generate a graph with approximately the structure
	//I think it should have at the beginning.

	//For groups at the base of which there are 57
	int init_index = MT + 1;
	for (int i = 0; i < MT; i++) {
		int group_index = init_index + (MT - 1) * i;
		//For vertices in each group which are of size 56
		for (int j = 0; j < MT - 1; j++) {
			int vertex_index = group_index + j;
			//For edge connections in each vertex
			for (int k = 1 + i; k < MT; k++) {
				int edge_index = init_index + k * (MT - 1) + j;
				//Each each of the 56 edges connect to one vertex in
				//each group
				graph_data[vertex_index].push_back(edge_index);
				graph_data[edge_index].push_back(vertex_index);
			}
		}
	}
}

template<int MT>
void MooreGraph<MT>::randomize(){
	/*
	 * In each group each vertex has to map to exactly one vertex in another group.
	 * Therefore we just have to permute the mapping between the vertices in each
	 * pair of groups.
	 */
	//This is a mapping from one group to the other
	vector<int> group_mapping(MT - 1);
	for (int i = 0; i < MT - 1; i++)
		group_mapping[i] = i;

	//Starting at group 1
	for (int grpi = 1; grpi < MT - 1; grpi++) {
		int group_index1 = grpi * (MT - 1) + MT + 1;

		//mapping each group after it.
		for (int grpj = grpi + 1; grpj < MT; grpj++) {
			int group_index2 = grpj * (MT - 1) + MT + 1;

			//Shuffle the mapping
			shuffle(group_mapping.begin(), group_mapping.end(), generator);
			for (int i = 0; i < MT - 1; i++){
				graph_data[group_index1 + i][grpj] = group_index2 + group_mapping[i];
				graph_data[group_index2 + group_mapping[i]][grpi + 1] = group_index1 + i;
			}
		}
	}
}

template<int MT>
MooreGraph<MT> MooreGraph<MT>::randomChild() {

	auto newGraph = *this;
	newGraph.randomize();
	return newGraph;
}

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
MooreGraph<MT>::~MooreGraph() {
}

template<int MT>
void MooreGraph<MT>::printDelta(Tdelta &delta) {

	std::cout << "Delta(" << std::get<0>(delta) << ", " << std::get<1>(delta)
			<< ", " << std::get<2>(delta) << ", " << std::get<3>(delta) << "); "
			<< std::endl;
}

template<int MT>
int MooreGraph<MT>::groupNumber(int vertex_number) {
	if (vertex_number < (MT + 1))
		return -1;
	else
		return (vertex_number - 1 - MT) % (MT - 1);
}

template<int MT>
MooreGraph<MT>& MooreGraph<MT>::operator=(MooreGraph<MT> &rhs) {
	if (&rhs == this)
		return *this;
	else {
		for (int i = 0; i < graph_data.size(); i++) {
			std::copy(graph_data[i].begin(), graph_data[i].end(),
					rhs.graph_data[i].begin());
		}
	}
	return rhs;
}

template<int MT>
void MooreGraph<MT>::apply_delta(tuple<int, int, int, int> &delta) {

	//First pick group i
	int prefix = MT + 1;
	int first_group_index = prefix + std::get<0>(delta) * (MT - 1);
	//int second_group_index = prefix + std::get<3>(delta) * (MT - 1);
	int first_vertex = first_group_index + std::get<1>(delta);
	int second_vertex = first_group_index + std::get<2>(delta);

	int edge_index1;
	int edge_index2;
	if (std::get<0>(delta) < std::get<3>(delta)) {
		edge_index1 = std::get<3>(delta);
		edge_index2 = std::get<0>(delta) + 1;
	} else {
		edge_index1 = std::get<3>(delta) + 1;
		edge_index2 = std::get<0>(delta);
	}

	int mappedv1 = graph_data[first_vertex][edge_index1];
	int mappedv2 = graph_data[second_vertex][edge_index1];

	graph_data[first_vertex][edge_index1] = mappedv2;
	graph_data[second_vertex][edge_index1] = mappedv1;

	graph_data[mappedv1][edge_index2] = second_vertex;
	graph_data[mappedv2][edge_index2] = first_vertex;
}

template<int MT>
tuple<int, int, int, int> MooreGraph<MT>::random_delta() {
	int group_i = std::uniform_int_distribution<int>(1, MT - 2)(generator);
	//Now Pick group j
	int group_j = std::uniform_int_distribution<int>(group_i + 1, MT - 1)(
			generator);
	//Now pick two vertices in group i
	int vertex_1 = std::uniform_int_distribution<int>(0, MT - 3)(generator);
	int vertex_2 = std::uniform_int_distribution<int>(vertex_1 + 1, MT - 2)(
			generator);

	return tuple<int, int, int, int>(group_i, vertex_1, vertex_2, group_j);
}

template<int MT>
void MooreGraph<MT>::printHeuristicMatrix() {
	vector<int> lookup_table(MT * MT + 1, 0);

	//Go through all vertices
	for (int i = 0; i < MT * MT + 1; i++) {
		//Go through all vertices that are adjacent to this vertex
		for (int j = 0; j < MT; j++) {
			int adj_vert = graph_data[i][j];
			lookup_table[adj_vert] += 1;
			for (int k = 0; k < MT; k++) {
				lookup_table[graph_data[adj_vert][k]] += 1;
			}
		}
		for (int j = 0; j < MT * MT + 1; j++) {
			std::cout << lookup_table[j] << ", ";
			lookup_table[j] = 0;
		}
		std::cout << std::endl;
	}

}

template<int MT>
int MooreGraph<MT>::heuristicScore() {
	vector<int> lookup_table(MT * MT + 1, 0);

	//We start off with a negative value since we overcount
	//by one on each vertex.
	int heuristic_value = -(MT - 1) * (MT * MT + 1);
	//Go through all vertices
	for (int i = 0; i < MT * MT + 1; i++) {
		//Go through all vertices that are adjacent to this vertex
		for (int j = 0; j < MT; j++) {
			int adj_vert = graph_data[i][j];
			lookup_table[adj_vert] += 1;
			for (int k = 0; k < MT; k++) {
				lookup_table[graph_data[adj_vert][k]] += 1;
			}
		}
		for (int j = 0; j < MT * MT + 1; j++) {
			//std::cout << lookup_table[j] << ", " ;
			heuristic_value += abs(lookup_table[j] - 1);
			lookup_table[j] = 0;
		}
	}
	return heuristic_value;
}

template class MooreGraph<3>;
template class MooreGraph<7>;
template class MooreGraph<57>;

