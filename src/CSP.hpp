#include <stack>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <queue>
#include "MoorePartial.hpp"
//#include "Permutation.hpp"
/*
template<int MT>
void addMapping(MoorePartial<MT> &graph){
  auto vec = dearangementSets(MT - 1);

  for (auto j = 1; j < MT; j++){
    graph.addMapping(0, j, vec[0]);
  }


  for (auto j = 2; j < MT; j++){
    graph.addMapping(1, j, vec[j - 1]);
  }
}
*/

template<int MT>
bool constraint_solve(MoorePartial<MT> &graph) {

  auto vect = graph.all_unassigned();
  //std::reverse(vect.begin(), vect.end());
  std::deque<Variable<MT> > assigned;
  std::deque<Variable<MT> > unassigned(vect.begin(), vect.end());
  
  auto variable = unassigned.front();

  unassigned.pop_front();
  unsigned long long int j = 0;
  while (true){
    int last_attempted = variable.getSecondVertex();
    //We iterate through all the elements in the group we are
    //connected to
    bool assignment_found = false;

    for (int i = last_attempted + 1; i < MT - 1; i++){
      //if (possible_vertices.end() != possible_vertices.find(i))
      //continue;
      variable.assign(i);
      //Try to assign the variable an assignment in the group 
      if (graph.apply_variable(variable)) {
	if (!graph.conflict(i, variable.getSecondGroup())) {
	  assignment_found = true;
	  break;
	}
	graph.unapply_variable(variable);
      }
    }
    //If we can't find a mapping then one of our earlier assignments
    //was wrong
    if (!assignment_found){

      //Put it back on the unassigned pile
      variable.unassign();
      unassigned.push_front(variable);
      if (assigned.empty()) 
	return false;
      variable = assigned.front();
      assigned.pop_front();
      graph.unapply_variable(variable);
    } else {
      
      //We found a valid assignment for this variable
      assigned.push_front(variable);
      if (unassigned.size() == 0) {
	std::cout << "Total number of iterations: " << j << std::endl;
	return true;
      }
      variable = unassigned.front();
      unassigned.pop_front();
    }
    j++;
    if (j > 10000) {
      //graph.printAdjacency();
      std::cout << assigned.size() << std::endl;
      j = 0;
    }
    
  }
  return false;
}



template<int MT>
Variable<MT> pairedVar(Variable<MT>& variable) {
  int first_group = variable.getFirstGroup();
  int first_vertex = variable.getFirstVertex();
  int second_group = variable.getSecondGroup();
  int last_attempted = variable.getSecondVertex();
  Variable<MT> paired_variable(first_group, last_attempted, 
			       second_group, first_vertex);

  return paired_variable;
}

template<int MT>
struct HashFunction {
  int operator()(const Variable<MT>& var) const {
    int ver1 = var.getFirstGroup() * (MT - 1) + var.getFirstVertex();
    return ver1 * (MT - 1) + var.getSecondGroup();
  }
};

template<int MT>
bool operator==(const Variable<MT>& lhs, const Variable<MT>& rhs) {
  return (lhs.getFirstGroup() == rhs.getFirstGroup() &&
	  lhs.getFirstVertex() == rhs.getFirstVertex() &&
	  lhs.getSecondGroup() == rhs.getSecondGroup());
}

template<int MT>
struct CompFunc {
  bool operator()(const Variable<MT>& lhs, 
                  const Variable<MT>& rhs) const {
    if (lhs.getFirstGroup() < rhs.getFirstGroup())
      return false;
    if (lhs.getSecondGroup() < rhs.getSecondGroup())
      return false;
    if (lhs.getFirstVertex() < rhs.getFirstVertex())
      return false;
    return true;
  }
};

template<int MT>
bool constraint_solve_pair(MoorePartial<MT> &graph) {
  graph.setInitialGrouping1();
  graph.setPairs();

  auto vect = graph.all_unassigned();
  //std::reverse(vect.begin(), vect.end());
  std::deque<Variable<MT> > assigned;
  //std::deque<Variable<MT> > unassigned(vect.begin(), vect.end());
  using queueType = std::priority_queue<int, std::vector<Variable<MT> >, CompFunc<MT> >;
  queueType unassigned(vect.begin(), vect.end());

  using setType = std::unordered_set<Variable<MT>, HashFunction<MT> >;
  setType assigned_variable_set;

  auto variable = unassigned.top();
  unassigned.pop();

  unsigned long long int j = 0;
  while (true){

    int last_attempted = variable.getSecondVertex();
    
    //We iterate through all the elements in the group we are
    //connected to
    bool assignment_found = false;
    for (int i = last_attempted + 1; i < MT - 1; i++){
      variable.assign(i);
      auto paired_variable = pairedVar<MT>(variable);

      if (graph.is_not_assigned(variable) && 
          graph.is_not_assigned(paired_variable)) {
        graph.apply_variable(variable);
        graph.apply_variable(paired_variable);
        if (!graph.conflict(variable.getSecondVertex(), 
                            variable.getSecondGroup()) && 
            !graph.conflict(paired_variable.getSecondVertex(), 
                            paired_variable.getSecondGroup())) {
          assigned_variable_set.insert(variable);
          assigned_variable_set.insert(paired_variable);
          assigned.push_front(variable);
          
          do {
            if (unassigned.size() == 0) {
              std::cout << "Total number of iterations: " << j << std::endl;
              return true;
            }
            //keep grabbing variables untill we find one that has not
            //yet been assigned.
            variable = unassigned.top();
            unassigned.pop();
          } while (assigned_variable_set.find(variable) != assigned_variable_set.end());
          assignment_found = true;
          break;
        }
        graph.unapply_variable(variable);
        graph.unapply_variable(paired_variable);
      }
    }
    //If we can't find a mapping then one of our earlier assignments
    //was wrong
    if (!assignment_found){
      //Put it back on the unassigned pile
      variable.unassign();
      unassigned.push(variable);
      if (assigned.empty()) 
        return false;

      //Grab an already assigned variable 
      variable = assigned.front();
      assigned.pop_front();
      graph.unapply_variable(variable);
      //Grab its pair which we had set earlier
      auto paired_variable = pairedVar<MT>(variable);
      //Remove its pair from our assigned set
      assigned_variable_set.erase(paired_variable);
      assigned_variable_set.erase(variable);
      //Add its pair to the list of unassigned variables
      graph.unapply_variable(paired_variable);
      paired_variable.unassign();
      unassigned.push(paired_variable);
    } 
    j++;
    if (j > 10000) {
      graph.printMapping(variable.getFirstGroup(),variable.getSecondGroup());

      std::cout << variable.getFirstGroup() << std::endl;
      std::cout << variable.getSecondGroup() << std::endl;
      std::cout << vect.size() / 2 << std::endl;
      std::cout << assigned.size() << std::endl;
      j = 0;
    }
    
  }
  return false;
}
