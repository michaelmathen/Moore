#include <vector>
#include <random>
#include <tuple>
#include <functional>
#include <iostream>

using std::vector;
using std::tuple;

typedef tuple<int, int, int, int> Tdelta;

template<int MT>
class MooreGraph {
	//Empty adjacency list
	vector<vector<int> > graph_data;
	std::default_random_engine generator;
	int groupNumber(int vertex_number);
	MooreGraph(vector<vector<int> > graph);
public:
	MooreGraph();
	MooreGraph(std::default_random_engine generator);
	~MooreGraph();
	MooreGraph randomChild();
	void initialize();
	int heuristicScore();
	void printDelta(Tdelta &delta);
	void apply_delta(Tdelta &delta);
	void printAdjacency();
	void printHeuristicMatrix();
	void randomize();
	MooreGraph& operator=(MooreGraph &rhs);
	Tdelta random_delta();
};

template<int MT, typename K>
vector<tuple<Tdelta, K> > mapDeltas(std::function<K(MooreGraph<MT>&)> f,
		MooreGraph<MT> &g) {
	vector<tuple<Tdelta, K> > vec;
	for (int grpi = 1; grpi < MT - 1; grpi++) {
		for (int grpj = grpi + 1; grpj < MT; grpj++) {
			for (int vertex1 = 0; vertex1 < MT - 2; vertex1++) {
				for (int vertex2 = vertex1 + 1; vertex2 < MT - 1; vertex2++) {

					//Apply a delta
					Tdelta t(grpi, vertex1, vertex2, grpj);

					g.apply_delta(t);
					//Map the value
					vec.push_back(tuple<Tdelta, K>(t, f(g)));

					//Now reverse the applied delta
					g.apply_delta(t);
				}
			}
		}
	}
	return vec;
}

template<int MT, typename K>
vector<Tdelta> filterDeltas(std::function<bool(MooreGraph<MT>&)> f,
		MooreGraph<MT> &g) {
	vector<Tdelta> vec;
	for (int grpi = 1; grpi < MT - 1; grpi++) {
		for (int grpj = grpi + 1; grpj < MT; grpj++) {
			for (int vertex1 = 0; vertex1 < MT - 2; vertex1++) {
				for (int vertex2 = vertex1 + 1; vertex2 < MT - 1; vertex2++) {
					//Apply a delta
					Tdelta t(grpi, vertex1, vertex2, grpj);

					g.apply_delta(t);
					if (f(g))
						vec.push_back(t);

					//Now reverse the applied delta
					g.apply_delta(t);
				}
			}
		}
	}
	return vec;
}

template<int MT, typename fType, typename rType>
rType foldDeltas(fType f, rType initial, MooreGraph<MT> &g) {
	for (int grpi = 1; grpi < MT - 1; grpi++) {
		for (int grpj = grpi + 1; grpj < MT; grpj++) {
			for (int vertex1 = 0; vertex1 < MT - 2; vertex1++) {
				for (int vertex2 = vertex1 + 1; vertex2 < MT - 1; vertex2++) {
					//Apply a delta
					Tdelta t(grpi, vertex1, vertex2, grpj);
					g.apply_delta(t);
					initial = f(g, t, initial);
					//Now reverse the applied delta
					g.apply_delta(t);
				}
			}
		}
	}
	return initial;
}
