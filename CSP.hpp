#include <stack>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include "MoorePartial.hpp"


template<int MT>
bool constraint_solve(MoorePartial<MT> &graph){

  std::stack<Variable> assigned;

  auto vect = graph.all_unassigned();
  std::reverse(vect.begin(), vect.end());
  std::deque<Variable> container(vect.begin(), vect.end());
  std::stack<Variable> unassigned(container);
  
  auto variable = unassigned.top();
  unassigned.pop();
  int j = 0;
  while (true){
    int last_attempted = variable.last_attempted;
    //We iterate through all the elements in the group we are connected to
    bool assignment_found = false;
    for (int i = last_attempted + 1; i < MT - 1; i++){

      variable.last_attempted = i;

      //Try to assign the variable an assignment in the group 
      if (graph.apply_variable(variable)) {
	assignment_found = true;
	break;
      }
    }
    //If we can't find a mapping then one of our earlier assignments was wrong
    if (!assignment_found){
      //Put it back on the unassigned pile
      variable.last_attempted = -1;
      unassigned.push(variable);
      if (assigned.empty()) 
	return false;
      variable = assigned.top();
      assigned.pop();
      graph.unapply_variable(variable);
    } else {
      //We found a valid assignment for this variable
      assigned.push(variable);
      if (unassigned.empty())
	return true;
      variable = unassigned.top();
      unassigned.pop();
    }
    j++;
    if (j > 10000000){
      graph.printAdjacency();
      std::cout << assigned.size() << std::endl;
      j = 0;
      
    }
    
  }
  return true;
}
