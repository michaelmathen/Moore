#include <iostream>
#include <string>
#include "CSP.hpp"


using namespace std;

/*
 */

int main(int argc, char* argv[]){
  if (argc > 1) {
    if (argv[1] == string("3")) {
      MoorePartial<3> graph;
      cout << constraint_solve(graph) << endl;
      graph.printReached();
      graph.printMappings();
      return 0;
    } else if (argv[1] == string("7")) {
      MoorePartial<7> graph;
      cout << constraint_solve_pair(graph) << endl;
      //graph.printReached();
      graph.printMappings();
      return 0;
    } else if (argv[1] == string("57")) {
      MoorePartial<57> graph;
      cout << constraint_solve_pair(graph) << endl;
      graph.printMappings();
      return 0;
    } 
  }
  cout << "This attempts to find the Moore Graphs with girth 5. " << endl
       << "From Wikipedia:" << endl
       << "The Hoffman–Singleton theorem states that any Moore graph with girth 5 "
       << "must have degree 2, 3, 7, or 57. The Moore graphs are:" << endl
       << "   The complete graphs  K_n  on n > 2 nodes. (diameter 1, girth 3, degree n-1, order n)" << endl
       << "   The odd cycles  C_{2n+1} . (diameter n, girth 2n+1, degree 2, order 2n+1)" << endl
       << "   The Petersen graph. (diameter 2, girth 5, degree 3, order 10)" << endl
       << "   The Hoffman–Singleton graph. (diameter 2, girth 5, degree 7, order 50)" << endl
       << "   A hypothetical graph of diameter 2, girth 5, degree 57 and order 3250; it is currently " 
       << "unknown whether such a graph exists." << endl
       << "Unlike all other Moore graphs, Higman proved that the unknown Moore graph cannot be vertex-transitive." << endl
       << endl
       << "This code uses CSP to find the 3 and 7 graph very quickly. " << endl
       << "The 57 graph is unfortunately much harder to find (if it exists) and " << endl
       << "while you are welcome to attempt to find it using this code you will probably have to wait for the heat death of the"
       << " universe. " << endl << endl;

  cout << "The Hoffman-Singleton Theorem states that a Moore graph with girth 5" << endl
       << "must have degree 2, 3, 7, or 57." << endl
       << "Please enter either 3, 7, or 57" << endl;

  return 0;
}
