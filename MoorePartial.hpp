/*
 * MoorePartial.hpp
 *
 *  Created on: Apr 14, 2014
 *      Author: micmath
 */
#include <vector>
#include "Variable.hpp"

#ifndef MOOREPARTIAL_HPP_
#define MOOREPARTIAL_HPP_


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
  void addEdge(int v1, int g1, int v2, int g2);
  void addEdge(int v1, int v2);
  void removeEdge(int v1, int v2);
  void removeEdge(int v1, int g1, int v2, int g2);

public:
  MoorePartial();
  virtual ~MoorePartial();
  void printAdjacency();
  MoorePartial<MT>& operator=(MoorePartial<MT> &rhs);
  void addMapping(int g1, int g2);
  void addMapping(int g1, int g2, std::vector<int>& mapping);
  bool conflict(int vertex1, int group1);
  bool apply_variable(Variable<MT>& var);
  void unapply_variable(Variable<MT>& var);
  bool is_not_assigned(Variable<MT>& var);
  std::vector<Variable<MT> > all_unassigned();
  void removePossible(Variable<MT>& var);
  void setInitialGrouping1();
  void setInitialGrouping2();
  bool isFinished();
  void printReached();
  std::vector<int> getMapping(int g1, int g2);
  void printMappings();
};

#endif /* MOOREPARTIAL_HPP_ */
