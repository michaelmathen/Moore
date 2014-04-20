/*
 * MoorePartial.hpp
 *
 *  Created on: Apr 14, 2014
 *      Author: micmath
 */
#include <vector>
#include <tuple>


#ifndef MOOREPARTIAL_HPP_
#define MOOREPARTIAL_HPP_

//An edge consists of the first vertex and the seccond vertex
//Vertices are numbered from 0 to MT * MT
typedef std::tuple<int, int> Edge;

typedef struct {
  int group1;
  int vertex1;
  int group2;
  int last_attempted;
} Variable;

//In partial graph we only care about the groups 1...MT - 1
//The group 0 must link to all groups so we leave it the way it is.
int firstVertex(Edge &edg){
  return std::get<0>(edg);
}

int secondVertex(Edge &edg){
  return get<1>(edg);
}

template<int MT>
int firstGroup(Edge &edg){
  return (std::get<0>(edg) - 2 * (MT  - 1)) / (MT - 1);
}

template<int MT>
int secondGroup(Edge &edg){
  return (std::get<1>(edg) - 2 * (MT  - 1)) / (MT - 1);
}

template<int MT>
int elGroup1(Edge &edg){
  return (std::get<0>(edg) - 2 * (MT  - 1)) % (MT - 1);
}

template<int MT>
int elGroup2(Edge &edg){
  return (std::get<1>(edg) - 2 * (MT  - 1)) % (MT - 1);
}

template<int MT>
Edge makeEdge(int group1, int vertex1, int group2, int vertex2){
  int vertex1_num = (group1 + 1) * (MT - 1) + MT + 1 + vertex1;
  int vertex2_num = (group2 + 1) * (MT - 1) + MT + 1 + vertex2;
  return Edge(vertex1_num, vertex2_num);
}

template<int MT>
int number_of_groups(){
  return MT - 1;
}

template<int MT>
int group_size(){
  return MT - 1;
}


template<int MT>
int group_index(int group1, int group2){
  //The index in vertices that maps group1 to group2
  if (group1 < group2)
    return group2;
  else
    return group2 + 1;
}

template<int MT>
int vertex_index(int vertex1, int group2){

  int group1 = (vertex1 - MT - 1) % (MT - 1);
  //The index in vertices that maps group1 to group2
  if (group1 < group2)
    return group2;
  else
    return group2 + 1;

}

template<int MT>
class MoorePartial {
  int current_vertex;
  std::vector<std::vector<int> > graph_data;
public:
  MoorePartial();
  virtual ~MoorePartial();
  void printAdjacency();
  MoorePartial<MT>& operator=(MoorePartial<MT> &rhs);
  bool conflict(int vertex1, int group1, int vertex2, int group2);
  void apply_variable(Variable& var);
  void unapply_variable(Variable& var);
  std::vector<Variable> all_unassigned();
};

#endif /* MOOREPARTIAL_HPP_ */
