#include <iostream>
#include "Permutation.hpp"
#include "CSP.hpp"




int main(int argc, char* argv[]){
  //mpz_class i = 10000000;
  //std::vector<int> permutation = permutationAtIndex(i, 20);
  //printPermutation(permutation);

  MoorePartial<7> graph;
  //addMapping<7>(graph);
  //graph.printAdjacency();
  //graph.setInitialGrouping1();
  //graph.printAdjacency();
  std::cout << constraint_solve<7>(graph) << std::endl;
  //graph.printAdjacency();
  graph.printReached();
  graph.printMappings();
  return 0;
}
