#include <vector>

#include <gmpxx.h>



auto initialPerm(int n) -> std::vector<int>;

bool nextPerm(std::vector<int>& permutation);
 
auto permutationAtIndex(mpz_class& index, int permutation_size) -> std::vector<int>;

void printPermutation(std::vector<int>& vect);

std::vector<std::vector<int> > dearangementSets(int n);
