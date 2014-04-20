#include <iostream>
#include "CSP.hpp"
#include "MoorePartial.hpp"

int main(int argc, char* argv[]){

  MooreGraph<7> graph;
  localGreedySearch(graph);
  std::cout << "Finished Local greedy search" << std::endl;
  graph.printHeuristicMatrix();
  simmulated_anneal(graph, 100000);
  localGreedySearch(graph);
  simmulated_anneal(graph, 100000);
  std::cout << "Done" << std::endl;
  return 0;
}
