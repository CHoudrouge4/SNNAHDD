#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "node.h"


template <typename Iter>
std::vector<int> bar (Iter it, Iter end) {
    std::vector<int> res;
    for (; it!=end; ++it) {
      res.push_back(*it);
    }
    return res;
}

int main() {


  return 0;
}
