
#include <iostream>
#include <tuple>
#include "MooreGraph.hpp"

using std::tuple;



template<int MT>
struct bestDelta{
  tuple<int, Tdelta> operator()(MooreGraph<MT> &g, Tdelta delta, tuple<int, Tdelta> d) 
  {
    auto heuristic_score = g.heuristicScore();
    if (heuristic_score < std::get<0>(d))
      return tuple<int, Tdelta>(heuristic_score, delta);
    return d;
  }
};

template<int MT>
int localGreedySearch(MooreGraph<MT> &moore_g){
  int last_score = moore_g.heuristicScore();
  int new_score;
  while (1) {

    auto bd = foldDeltas<MT, bestDelta<MT>, tuple<int, Tdelta> >(bestDelta<MT>(), 
								 tuple<int, Tdelta>(pow(2, 30), 
										    Tdelta(-1,-1,-1,-1)), 
								 moore_g);
    new_score = std::get<0>(bd);
    auto new_delta = std::get<1>(bd);

    
    if (new_score == 0){
      moore_g.apply_delta(new_delta);
      return 0;
    }
    
    if (new_score >= last_score){
      moore_g.printHeuristicMatrix();
      return last_score;
    }
    else {
      last_score = new_score;
    }

    moore_g.apply_delta(new_delta);

  }
  return 0;
}
