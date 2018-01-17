#include <iostream>
#include <vector>
#include <set>
#include <algorithm>


template <typename Iter>
std::vector<int> bar (Iter it, Iter end) {
    std::vector<int> res;
    for (; it!=end; ++it) {
      res.push_back(*it);
    }
    return res;
}

int main() {

  //pk_tree pk("blablabla", 3, 9);
//  std::set<int> v = {1, 2, 3, 14, 15, 16, 7, 8, 9};
//  std::vector<int> u = bar(v.begin(), v.end());
  //v.erase(v.begin() + 1);
//  std::set<int> s;
//  s.insert(v.begin(), v.end());
//  std::vector<int> u(s.begin(), s.begin() + 3);
//  std::unordered_set<int> s = pk.select(v);
//  for(auto u: s) {
//    std::cout << u << std::endl;
//  }
//  for(auto&& e : s) std::cout << e << ' ';
//  std::cout << '\n';
//  int i = 0;
//  while(i < v.size()) {
//    v[i++] = 0;
//  }
//  for(auto&& e: u) std::cout << e << ' ';
//  std::cout << '\n';
  std::vector<int> v(10);
  std::generate(v.begin(), v.end(), [n = 0] () mutable { return n++; });
  for(auto e : v) {
    std::cout << e << std::endl;
  }

  return 0;
}
