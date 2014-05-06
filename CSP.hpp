#include <stack>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include "MoorePartial.hpp"


template<int MT>
bool constraint_solve(MoorePartial<MT> &graph){

  std::stack<Variable<MT>> assigned;

  auto vect = graph.all_unassigned();
  std::reverse(vect.begin(), vect.end());
  std::deque<Variable<MT> > container(vect.begin(), vect.end());
  std::stack<Variable<MT> > unassigned(container);
  
  auto variable = unassigned.top();
  unassigned.pop();
  unsigned long long int j = 0;
  while (true){
    int last_attempted = variable.getSecondVertex();

    //We iterate through all the elements in the group we are connected to
    bool assignment_found = false;
    for (int i = last_attempted + 1; i < MT - 1; i++){

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
    //If we can't find a mapping then one of our earlier assignments was wrong
    if (!assignment_found){

      //Put it back on the unassigned pile
      variable.unassign();
      unassigned.push(variable);
      if (assigned.empty()) 
	return false;
      variable = assigned.top();
      assigned.pop();
      graph.unapply_variable(variable);
    } else {
      
      //We found a valid assignment for this variable
      assigned.push(variable);
      if (unassigned.empty()) {
	std::cout << "Total number of iterations: " << j << std::endl;
	return true;
      }
      variable = unassigned.top();
      unassigned.pop();
    }
    j++;
    if (j > 1000000) {
      //graph.printAdjacency();
      std::cout << assigned.size() << std::endl;
      j = 0;
    }
    
  }
  return false;
}
