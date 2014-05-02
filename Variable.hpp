#include <unordered_set>

class Variable {
  int group1;
  int vertex1;
  int group2;
  int assignment;
  int degree;
  std::unordered_set<int> possible_values;
public:
  Variable(int group1, int vertex1, int group2, int degree);
  Variable(int group1, int vertex1, int group2, int vertex2, int degree);
  ~Variable();
  int getFirstGroup();
  int getSecondGroup();
  int getFirstVertex();
  int getSecondVertex();
  void unassign();
  void assign(int i);
  //Variable<MT>& operator=(Variable<MT> &rhs);
  void removePossible(int i);
};
