/*
 * MoorePartial.hpp
 *
 *  Created on: Apr 14, 2014
 *      Author: micmath
 */
#include <vector>


#ifndef MOOREPARTIAL_HPP_
#define MOOREPARTIAL_HPP_

typedef struct {
  int group1;
  int vertex1;
  int group2;
  int last_attempted;
} Variable;



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
  bool conflict(int vertex1, int group1);
  void apply_variable(Variable& var);
  void unapply_variable(Variable& var);
  std::vector<Variable> all_unassigned();
};

#endif /* MOOREPARTIAL_HPP_ */
