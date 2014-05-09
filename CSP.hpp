#include <stack>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <queue>
#include "MoorePartial.hpp"

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
    //auto gmap = graph.getMapping(first_group, second_group);    
    //auto vmap = graph.getVertexMapping(first_vertex, first_group);
    
    //std::set<int> possible_vertices(gmap.begin(), gmap.end());
    //possible_vertices.insert(vmap.begin(), vmap.end());
    
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

    // int first_group = variable.getFirstGroup();
    // int second_group = variable.getFirstGroup();
    // int first_vertex = variable.getFirstVertex();
    int last_attempted = variable.getSecondVertex();

    // auto gmap = graph.getMapping(first_group, second_group);    
    // auto vmap = graph.getVertexMapping(first_vertex, first_group);
    
    // std::set<int> possible_vertices(gmap.begin(), gmap.end());
    // possible_vertices.insert(vmap.begin(), vmap.end());
    
    //We iterate through all the elements in the group we are
    //connected to
    bool assignment_found = false;
    for (int i = last_attempted + 1; i < MT - 1; i++){
      // if (possible_vertices.find(i) != possible_vertices.end()) {
      //   //Have to be a derangement in the columns
      //   continue;
      // }
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
    //std::cout<< unassigned.size() << std::endl;

    j++;
    if (j > 10000) {
      graph.printMapping(variable.getFirstGroup(),variable.getSecondGroup());
     
      //graph.printAdjacency();
      std::cout << variable.getFirstGroup() << std::endl;
      std::cout << variable.getSecondGroup() << std::endl;
      std::cout << vect.size() / 2 << std::endl;
      std::cout << assigned.size() << std::endl;
      j = 0;
    }
    
  }
  return false;
}
/*

  template<int MT>
bool constraint_solve_pair(MoorePartial<MT> &graph){
  //Make sure the graph is set up so that all matching pairs are set.
  graph.setInitialGrouping1();
  graph.setPairs();

  auto vect = graph.all_unassigned();
  unsigned int variable_number = vect.size();

  std::deque<Variable<MT> > assigned;
  std::deque<Variable<MT> > unassigned(vect.begin(), vect.end());
  using setType = std::unordered_set<Variable<MT>, HashFunction<MT> >;
  setType assigned_variable_set;

  //Define a pop front function that could come in handy.
  auto pop_front = [](decltype(unassigned_variables)& set){
    auto front_iterator = set.begin();
    auto tmp = *front_iterator;
    set.erase(front_iterator);
    return tmp;
  };

  auto variable = unassigned.front();
  unassigned.pop_front();

  unsigned long long int j = 0;
  while (true){
    int last_attempted = variable.getSecondVertex();
    
    //We iterate through all the elements in the group we are connected to
    bool assignment_found = false;
    //graph.printMappings();
    
    for (int i = last_attempted + 1; i < MT - 1; i++){
      variable.assign(i);
      auto paired_variable = pairedVar<MT>(variable);
      //Try to assign the variables an assignment in the group 
      if (!graph.apply_variable(variable))	  
	continue;
      //This should fail if we have already found its paired variable before.
      if (!graph.apply_variable(paired_variable)){
	graph.unapply_variable(variable);
	continue;
      }

      // if (!graph.conflict(variable.getSecondVertex(), 
      // 			  variable.getSecondGroup()) &&
      // 	  !graph.conflict(paired_variable.getSecondVertex(), 
      // 			  paired_variable.getSecondGroup())) {
      // 	  unassigned_variables.erase(paired_variable);
      // 	  assignment_found = true;
      // 	  break;
      // }

      assignment_found = true;
      break;
      graph.unapply_variable(variable);
      graph.unapply_variable(paired_variable);
    }
    //If we can't find a mapping then one of our earlier assignments was wrong
    if (!assignment_found){

      //Put it back on the unassigned pile.
      variable.unassign();
      unassigned.push_front(variable);

      if (assigned.empty()) 
	return false;

      variable = assigned.front();
      assigned.pop_front();
      auto paired_variable = pairedVar<MT>(variable);
      graph.unapply_variable(variable);
      graph.unapply_variable(paired_variable);
    } else {
      graph.printMapping(5, 6);
      //We found a valid assignment for this variable
      assigned.push_front(variable);
      if (unassigned_variables.empty()) {
	std::cout << "Total number of iterations: " << j << std::endl;
	return true;
      }
      variable = pop_front(unassigned_variables);
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
*/
