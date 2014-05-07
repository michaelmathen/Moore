#include <algorithm>
#include <iostream>

#include "Permutation.hpp"

auto initialPerm(int n) -> std::vector<int> {
  /*
    Generates the first permutation in lexicographic order.
   */
  std::vector<int> vectInit(n, 0);
  for (int i = 0; i < n; i++){
    vectInit[i] = i;
  }
  return vectInit;
}

bool nextPerm(std::vector<int>& permutation) {
  /*
    Generates the next permutation in lexicographic order
   */
  auto i = permutation.size() - 2;
  while (!((i < 0) || permutation[i] < (permutation[i + 1])))
    i--;
  //This was the last permutation
  if (i < 0) 
    return false;
  
  auto j = permutation.size() - 1;
  while (!(permutation[j] > permutation[i])) {
    j--;
  }

  //Swap the element
  auto tmp = permutation[i];
  permutation[i] = permutation[j];
  permutation[j] = tmp;

  //Reverse the last half of the array
  std::reverse(permutation.begin() + i + 1, permutation.end());
  return true;
}

 
auto permutationAtIndex(mpz_class& index, int permutation_size) -> std::vector<int> {
  /*
    Find the permutation at the ith lexicographic index
   */
  auto factorial = [] (int n){
    mpz_class fac_size;    
    fac_size = 1;
    for (int i = 1; i <= n; i++)
      fac_size = fac_size * i;
    return fac_size;
  };
  //set of numbers
  std::vector<int> setOfInts;
  for (int i = 0; i < permutation_size; i++) setOfInts.push_back(i);

  std::vector<int> indices;
  mpz_class remainder = index;
  for (int i = permutation_size - 1; i >= 0;  i--){
    mpz_class fac_size = factorial(i);
    mpz_class ithvalue = remainder / fac_size;
    indices.push_back(setOfInts[ithvalue.get_ui()]);
    setOfInts.erase(setOfInts.begin() + ithvalue.get_ui());
    remainder = remainder % fac_size;
  }
  return indices;
}

void printPermutation(std::vector<int>& vect){
  for (unsigned int i = 0; i < vect.size(); i++){
    std::cout << vect[i] << ", "; 
  }
  std::cout << std::endl;
}

std::vector<std::vector<int> > dearangementSets(int n){
  std::vector<std::vector<int> > vec(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      vec[i][j] = (j + i) % n;
    }
  }
  return vec;
}
    
