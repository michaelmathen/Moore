#include <iostream>

template<int MT>
class Variable {
  int group1;
  int vertex1;
  int group2;
  int assignment;
  long possible;
public:
  Variable(int group1, int vertex1, int group2) {
    this->group1 = group1;
    this->vertex1 = vertex1;
    this->group2 = group2;
    this->assignment = -1; 
    possible = 0;
  }


  Variable(int group1, int vertex1, int group2, int vertex2){
    this->group1 = group1;
    this->vertex1 = vertex1;
    this->group2 = group2;
    this->assignment = vertex2;
    possible = 0;
  }

  ~Variable(){
  }

  int getFirstGroup() const {
    return group1;
  }

  int getSecondGroup() const {
    return group2;
  }

  int getFirstVertex() const {
    return vertex1;
  }
  int getSecondVertex() const {
    return assignment;
  }
  
  void removePossible(int i){
    //Set this particular bit so we know that 
    //vertex is not available 
    possible = possible | (0x01 << i);
  }

  bool isOpen(int i){
    //Check to see if a vertex is available
    return possible & (0x01 << i);
  }

  int numberPossible(){
    int count = 0;
    for(int i = 0; i < MT - 1; i++)
        count += (possible >> i) & 0x01;
    return MT - 1 - count;
  }

  void assign(int i){
    assignment = i;
  }

  void print() const {
    std::cout << "Variable{" << std::endl;
    std::cout << "group1=" << this->group1 << std::endl;
    std::cout << "vertex1=" << this->vertex1 << std::endl;
    std::cout << "group2=" << this->group2 << std::endl;
    std::cout << "assignment=" << this->assignment << std::endl;
    std::cout << "}" << std::endl;
  }
  void unassign(){
    assignment = -1;
  }

};
