#include <iostream>
#include <vector>
#include <unordered_set>
#include "pk_tree.h"
int main() {

  pk_tree pk("blablabla", 3, 9);
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//  std::unordered_set<int> s = pk.select(v);
//  for(auto u: s) {
//    std::cout << u << std::endl;
//  }


  return 0;
}
