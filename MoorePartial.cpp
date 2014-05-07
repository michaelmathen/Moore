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

  //For each of the groups
  for (int i = 0; i < MT; i++){
    //For all the elements in the groups
    for (int j = 0; j < MT - 1; j++){
      for (int k = 0; k < MT - 1; k++){
	graph_data[1 + MT + (MT - 1) * i + k].push_back(-1);
      }
    }
  }
}


template<int MT>
void MoorePartial<MT>::setInitialGrouping1(){
  for (int i = 0; i < MT - 1; i++){
    for (int j = 0; j < MT - 1; j++){
      addEdge(i, 0, i, j + 1);
    }
  }
}



template<int MT>
void MoorePartial<MT>::setInitialGrouping2(){
  //grab the first four groups
  //for (int i = 0; i < MT / 2; i++){
  //  addMapping(2 * i, 2 * i + 1);
  //}
}

template<int MT> 
void MoorePartial<MT>::addMapping(int g1, int g2, std::vector<int>& mapping){
  for (int i = 0; i < MT - 1; i++){
    addEdge(i, g1, mapping[i], g2);
  }
}

template<int MT> 
void MoorePartial<MT>::addMapping(int g1, int g2){
  for (int i = 0; i < MT - 1; i++){
    addEdge(i, g1, i, g2);
  }
}

template<int MT>
void MoorePartial<MT>::addEdge(int v1, int g1, int v2, int g2){
  auto fst_index = g1 < g2 ? g2 : g2 + 1;
  auto snd_index = g1 < g2 ? g1 + 1 : g1;
  graph_data[1 + MT + (MT - 1) * g1 + v1][fst_index] = 1 + MT + (MT - 1) * g2 + v2;
  graph_data[1 + MT + (MT - 1) * g2 + v2][snd_index] = 1 + MT + (MT - 1) * g1 + v1;
}

template<int MT>
void MoorePartial<MT>::addEdge(int v1, int v2){
  auto g1 = (v1 - MT - 1) / (MT - 1);
  auto g2 = (v2 - MT - 1) / (MT - 1);
  auto fst_index = g1 < g2 ? g2 : g2 + 1;
  auto snd_index = g1 < g2 ? g1 + 1 : g1;
  graph_data[v1][fst_index] = v2;
  graph_data[v2][snd_index] = v1;
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
      for (unsigned int j = 0; j < graph_data[i].size() - 1; j++) {
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
    for (unsigned int i = 0; i < graph_data.size(); i++) {
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
bool MoorePartial<MT>::isFinished(){
  std::array<int, MT * MT + 1>  mapped_check;
  for (int i = 0; i < MT * MT + 1; i++) {
    mapped_check.fill(0);
    for (int j = 0; j < MT; j++) {
      int adjacent = graph_data[i][j];
      if (adjacent == -1) 
	return false;
      mapped_check[adjacent] += 1;
      for (int k = 0; k < MT; k++) {
	if (graph_data[adjacent][k] == -1)
	  return false;
	mapped_check[graph_data[adjacent][k]] += 1;
      }
    }
    for (int j = 0; j < MT * MT  + 1; j++){
      if ((i != j) && (mapped_check[j] != 1))
	return false;
    }
  }
  return true;
}


template<int MT>
void MoorePartial<MT>::printReached(){
  std::array<int, MT * MT + 1>  mapped_check;
  for (int i = 0; i < MT * MT + 1; i++)
    std::cout << i << ", " ;
  std::cout<<std::endl;
  for (int i = 0; i < MT * MT + 1; i++) {
    mapped_check.fill(0);
    for (int j = 0; j < MT; j++) {
      int adjacent = graph_data[i][j];
      if (adjacent == -1) 
	continue;
      mapped_check[adjacent] += 1;
      for (int k = 0; k < MT; k++) {
	if (graph_data[adjacent][k] == -1)
	  continue;
	mapped_check[graph_data[adjacent][k]] += 1;
      }
    }
    for (int j = 0; j < MT * MT  + 1; j++){
       std::cout << mapped_check[j] << ", ";
    } 
    std::cout << std::endl;
  }
}

template<int MT>
bool MoorePartial<MT>::is_not_assigned(Variable<MT>& var){
  /*
    Check to see if this variable will introduce a triangle between 
    the groups in the graph.
   */
  int init = MT + 1;
  //First check to make sure this variable is not already assigned
  int gindex1 = group_index<MT>(var.getFirstGroup(), var.getSecondGroup());
  int gindex2 = group_index<MT>(var.getSecondGroup(), var.getFirstGroup());

  if (graph_data[init + (MT - 1) * var.getFirstGroup() + var.getFirstVertex()][gindex1] != -1)
    return false;

  if (graph_data[init + (MT - 1) * var.getSecondGroup() + var.getSecondVertex()][gindex2] != -1)
    return false;

  return true;
}

template<int MT>
void MoorePartial<MT>::printMappings(){
  for (auto i = 0; i < MT - 1; i++) {
    for (auto j = i + 1; j < MT; j++){
      std::cout << i << ", " << j << "   "; 
      for (auto el : getMapping(i, j)){
	std::cout << el <<", ";
      }
      std::cout << std::endl;
    }
  }
}

template<int MT>
std::vector<int> MoorePartial<MT>::getMapping(int g1, int g2){
  int gindex1 = group_index<MT>(g1, g2);
  vector<int> vect(MT - 1, 0);
  for (int i = 0; i < MT - 1; i++){
    vect[i] = graph_data[MT + 1 + (MT - 1) * g1 + i][gindex1] - MT - 1 - (MT - 1) * g2 ;
  }
  return vect;
}

template<int MT>
bool MoorePartial<MT>::apply_variable(Variable<MT>& var){
  /*
    Check to see if the assignement is not possible and then assign the variable if not.
   */
  if (!is_not_assigned(var))
    return false;
  addEdge(var.getFirstVertex(), var.getFirstGroup(), var.getSecondVertex(), var.getSecondGroup());
  return true;
}

template<int MT>
void MoorePartial<MT>::removeEdge(int v1, int g1, int v2, int g2){
  auto fst_index = g1 < g2 ? g2 : g2 + 1;
  auto snd_index = g1 < g2 ? g1 + 1 : g1;
  graph_data[1 + MT + (MT - 1) * g1 + v1][fst_index] = -1;
  graph_data[1 + MT + (MT - 1) * g2 + v2][snd_index] = -1;

}

template<int MT>
void MoorePartial<MT>::removeEdge(int v1, int v2){
  auto g1 = (v1 - MT - 1) / (MT - 1);
  auto g2 = (v2 - MT - 1) / (MT - 1);
  auto fst_index = g1 < g2 ? g2 : g2 + 1;
  auto snd_index = g1 < g2 ? g1 + 1 : g1;
  graph_data[v1][fst_index] = -1;
  graph_data[v2][snd_index] = -1;
}

template<int MT>
void MoorePartial<MT>::unapply_variable(Variable<MT>& var){
  removeEdge(var.getFirstVertex(), var.getFirstGroup(), var.getSecondVertex(), var.getSecondGroup());
}

template<int MT>
void MoorePartial<MT>::removePossible(Variable<MT>& var){

  int vertex_index = MT + 1 + (MT - 1) * var.getFirstGroup() + var.getFirstVertex();

  for (int i = 0; i < MT; i++) {
    int adjacent = graph_data[vertex_index][i];
    if (adjacent != -1) {
      if ((adjacent - MT - 1) / (MT - 1) == var.getSecondGroup())
	var.removePossible((adjacent - MT - 1) % (MT - 1));
      for (int j = 0; j < MT; j++) {
	if ((graph_data[adjacent][j] - MT - 1) / (MT - 1) == var.getSecondGroup())
	  var.removePossible((adjacent - MT - 1) % (MT - 1));
      }
    }
  }
}
/*
//Figure out how constricted this variable is.
template<int MT>
void MoorePartial<MT>::constrain(std::vector<Variable<MT> >& vars){
  
}
*/
template<int MT>
std::vector<Variable<MT> > MoorePartial<MT>::all_unassigned(){
  vector<Variable<MT> > vectVariables;
  for (int groupi = 0; groupi < MT; groupi++){
    for (int v1 = 0; v1 < MT - 1; v1++){
      for (int groupj = groupi + 1; groupj < MT; groupj++) {
	int vertex_index1 = 1 + MT + groupi * (MT - 1) + v1;
	if (graph_data[vertex_index1][groupj] == -1)	  
	  vectVariables.push_back(Variable<MT>(groupi, v1, groupj));
      }
    }
  }
  return vectVariables;
}


//template class MoorePartial<2>;
template class MoorePartial<3>;
//template class MoorePartial<4>;
template class MoorePartial<7>;
//template class MoorePartial<8>;
template class MoorePartial<57>;
