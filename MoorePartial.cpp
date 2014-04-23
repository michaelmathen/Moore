/*
 * MoorePartial.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: micmath
 */
#include <vector>
#include <tuple>
#include <iostream>
#include "MoorePartial.hpp"

using std::vector;

template<int MT>
MoorePartial<MT>::MoorePartial() {
  current_vertex = 2 * MT;
  //Define the mappings between each pair of the MT - 1 groups
  //We can think of each mapping as a variable with a set of 56 possible values to
  //map too.


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

  //Set all the first vertices to -1
  for (int i = 0; i < (MT - 1) * (MT - 1); i++){
    for (int j = 0; j < MT - 2; j++){
      graph_data[2 * MT + i].push_back(-1);
    }
  }
}

template<int MT>
MoorePartial<MT>::~MoorePartial() {}

template<int MT>
void MoorePartial<MT>::printAdjacency() {
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
MoorePartial<MT>& MoorePartial<MT>::operator=(MoorePartial<MT> &rhs) {
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
bool MoorePartial<MT>::conflict(int vertex1, int group1){
  std::array<int, MT * MT + 1>  mapped_check;
  mapped_check.fill(0);
  int vertex_index = MT + 1 + (MT - 1) * group1 + vertex1;
  for (int i = 0; i < MT; i++) {
    int adjacent = graph_data[vertex_index][i];
    if (adjacent != -1) {
      mapped_check[adjacent] += 1;
      for (int j = 0; j < MT; j++) {
	if (graph_data[adjacent][j] != -1)
	  mapped_check[graph_data[adjacent][j]] += 1;
      }
    }
  }
  for (int i = 0; i < MT * MT  + 1; i++){
    if (i != vertex_index && mapped_check[i] > 1)
      return true;
  }
  return false;
}


template<int MT>
bool MoorePartial<MT>::apply_variable(Variable& var){
  
  int init = MT + 1;
  //First check to make sure this variable is not already assigned
  int gindex1 = group_index<MT>(var.group1, var.group2);
  int gindex2 = group_index<MT>(var.group2, var.group1);

  if (graph_data[init + (MT - 1) * var.group1 + var.vertex1][gindex1] != -1)
    return false;

  if (graph_data[init + (MT - 1) * var.group2 + var.last_attempted][gindex2] != -1)
    return false;

  graph_data[init + (MT - 1) * var.group1 + var.vertex1][gindex1] = init + (MT - 1) * var.group2 + var.last_attempted;
  
  graph_data[init + (MT - 1) * var.group2 + var.last_attempted][gindex2] = init + (MT - 1) * var.group1 + var.vertex1;
  //this->printAdjacency();

  if (conflict(var.group2, var.last_attempted)) {
    unapply_variable(var);
    return false;
  }
  return true;
}

template<int MT>
void MoorePartial<MT>::unapply_variable(Variable& var){
  int init = MT + 1;
  int gindex = group_index<MT>(var.group1, var.group2);
  graph_data[init + (MT - 1) * var.group1 + var.vertex1][gindex] = -1;
  
  gindex = group_index<MT>(var.group2, var.group1);
  graph_data[init + (MT - 1) * var.group2 + var.last_attempted][gindex] = -1;
}


template<int MT>
std::vector<Variable> MoorePartial<MT>::all_unassigned(){

  Variable var;
  vector<Variable> vectVariables;
  for (int grpi = 1; grpi < MT - 1; grpi++) {
    for (int vertex1 = 0; vertex1 < MT - 1; vertex1++) {
      for (int grpj = grpi + 1; grpj < MT; grpj++) {
	var.group1 = grpi;
	var.vertex1 = vertex1;
	var.group2 = grpj;
	var.last_attempted = -1;
	vectVariables.push_back(var);
      }
    }
  }
  std::cout << vectVariables.size() << std::endl;
  return vectVariables;
}


template class MoorePartial<3>;
template class MoorePartial<7>;
template class MoorePartial<57>;
