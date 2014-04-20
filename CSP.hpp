#include <stack>
#include <deque>
#include <vector>
#include "MoorePartial.hpp"


template<int MT>
void constraint_solve(MoorePartial<MT> &graph){

  std::stack<Variable> assigned;

  auto vect = graph.all_unassigned();
  std::deque<Variable> container(vect.begin(), vect.end());
  std::stack<Variable> unassigned(container);
  
  auto variable = unassigned.top();
  unassigned.pop();

  while (!unassigned.empty()){
    
    int group2 = variable.group2;
    int last_attempted = variable.last_attempted;

    //We iterate through all the elements in the group we are connected to
    bool mapping_found = false;
    for (int i = last_attempted + 1; i < group_size<MT>(); i++){

      variable.last_attempted = i;
      graph.apply_variable(variable);
      
      //Try to assign the variable an assignment in the group 
      if (!graph.conflict(i, group2)) {
	variable.last_attempted = i;
	mapping_found = true;
	break;
      }
      graph.unapply_variable(variable);
    }

    //If we can't find a mapping then one of our earlier assignments was wrong
    if (!mapping_found){

      //We remove the edge 
      graph.unapply_variable(variable);
      variable.last_attempted = 0;
      variable = assigned.top();
      assigned.pop();

    } else {
      graph.apply_variable(variable);
      //We found a valid assignment for this variable
      assigned.push(variable);
      variable = unassigned.top();
      unassigned.pop();
    }
    
    if (assigned.empty()) {
      return;
    }
  }
  return ;
}
