#include <iostream>

#include "CSP.hpp"
#include "MoorePartial.hpp"

int main(int argc, char* argv[]){

  MoorePartial<7> graph;
  constraint_solve<7>(graph);
  return 0;
}
