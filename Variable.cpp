#include <unordered_set>
#include "Variable.hpp"



Variable::Variable(int group1, int vertex1, int group2, int degree) {
  this->group1 = group1;
  this->vertex1 = vertex1;
  this->group2 = group2;
  this->assignment = -1;
  this->degree = degree;
  for (int i = 0; i < degree; i++){
    possible_values.insert(i);
  }
}


Variable::Variable(int group1, int vertex1, int group2, int vertex2, int degree) {
  this->group1 = group1;
  this->vertex1 = vertex1;
  this->group2 = group2;
  this->assignment = vertex2;
  this->degree = degree;
  for (int i = 1; i < degree - 2; i++){
    possible_values.insert(i);
  }
}


Variable::~Variable(){
}
void Variable::assign(int i){
  this->assignment = i;
}

int Variable::getFirstGroup(){
  return this->group1;
}



int Variable::getSecondGroup(){
  return this->group2;
}



int Variable::getFirstVertex(){
  return this->vertex1;
}


int Variable::getSecondVertex(){
  return this->vertex1;
}

/*
Variable& Variable::operator=(Variable &rhs){
  if (this == &rhs){
    return *this;
  } else {
    rhs.group1 = group1;
    rhs.group2 = group2;
    rhs.vertex1 = vertex1;
    rhs.assignment = assignment;
    //rhs.possible_values = this.possible_values;
    return *this;
  }
}
*/

void Variable::removePossible(int i){
  possible_values.erase(i);
}


void Variable::unassign(){
  assignment = -1;
}
//template class Variable<3>;
//template class Variable<7>;
//template class Variable<57>;
