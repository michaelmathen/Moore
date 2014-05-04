#include <iostream>
#include "CSP.hpp"


int main(int argc, char* argv[]){

  MoorePartial<7> graph;
  std::cout << constraint_solve<7>(graph) << std::endl;
  graph.printAdjacency();
  return 0;
}
