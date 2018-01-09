#include "pk_tree.h"
#include "node.h"
#include <iostream>
#include <unordered_set>
#include <random>
#include <limits>
#include <set>

std::random_device pk_tree::rd;
std::mt19937 pk_tree::gen(rd());

pk_tree::pk_tree(std::string file_name, int K, int I_max) {
  k = K;
  i_max = I_max;
  read_point(file_name);
  dimension = compute_dimension();
  root = std::make_shared<node>();
  for(size_t i = 0; i < points.size(); ++i) root->pts.push_back(i);
  construct(root);
}

std::unordered_set<int> pk_tree::select(const std::vector<int> &v) const {
  std::uniform_int_distribution<> dis(0, v.size() - 1);
  std::unordered_set<int> s;
  while(s.size() != k) {
    int index = dis(gen);
    if(s.find(v[index]) == s.end()) s.insert(v[index]);
  }
  return s;
}

std::vector<cluster> pk_tree::clustering(const std::vector<point> &centroid, const std::vector<int> &pts) {
  //std::cout << "the centroid size is " << centroid.size() << std::endl;
  std::vector<cluster> c(centroid.size());
  for(auto&& e : pts) {
    double max_distance = std::numeric_limits<double>::max();
    int index = 0;
    for(int i = 0; i < centroid.size(); ++i) {
      double d = dist(points[e], centroid[i]);
      if(d <= max_distance) {
        max_distance = d;
        index = i;
      }
    }
    c[index].push_back(e);
  }
  return c;
}

point pk_tree::get_mean_point(cluster& c) {
  point p(dimension);
  for(int i = 0; i < dimension; ++i) {
    double sum = 0;
    for(int j = 0; j < c.size(); ++j)
      sum += points[c[j]][i];
    p[i] = sum/c.size();
  }
  return p;
}

std::vector<point> pk_tree::get_P(std::vector<cluster> &C) {
  std::vector<point> P;
  for(int i = 0; i < C.size();  i++) P.push_back(get_mean_point(C[i]));
  return P;
}

void pk_tree::construct(std::shared_ptr<node> &current) {
  if(current->pts.size() < k) return;

  std::unordered_set<int> p = select(current->pts);
  std::vector<point> P = get_points(p);
//  std::cout << "the initial centroid are: " << P << std::endl;

  bool coveraged = false;
  int i = 0;
  std::vector<cluster> C;

  while(!coveraged && i < i_max) {
    C = clustering(P, current->pts);
    std::vector<point> P_new = get_P(C);
    if(P == P_new) {
      coveraged = true;
    }
    P = P_new;
    i++;
  }
  int j = 0;
//  std::cout << "*********************" << std::endl;
  for(auto&& c: C) {
    if(c.size() == 0) continue;
    std::shared_ptr<node> child = std::make_shared<node>();
    child->center = P[j];
    child->pts = c;
    root->children.push_back(child);
  //  std::cout << (root->children.back())->pts << std::endl;
    construct(root->children.back());
    j++;
  }
}

std::vector<int> pk_tree::search(point &q, int L) {
  int count = 0;
  std::set<std::pair<double, std::shared_ptr<node>>> PQ;
  std::set<std::pair<double, int>> R;
  traverse(root, PQ, R, count, q);
  while(!PQ.empty() && count < L) {
    auto N = *PQ.begin();
    PQ.erase(PQ.begin());
    traverse(N.second, PQ, R, count, q);
  }
  std::vector<int> res(k);
  int i = 0;
  for(auto&& e : R) {
    if(i >= k) break;
    res[i++] = e.second;
  }
  return res;
}

void pk_tree::traverse(std::shared_ptr<node> &current,
                            std::set<std::pair<double, std::shared_ptr<node>>> &PQ,
                            std::set<std::pair<double, int>> &R, int &count, point &q) {

  if(current == nullptr) return;
  if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0) {
    for(auto&& e: current->pts) {
      double d = dist(points[e], q);
      R.insert({d, e});
    }
    count += current->pts.size();
  } else {
      auto C = current->children;
      std::vector<std::pair<double, std::shared_ptr<node>>> cp;
      std::shared_ptr<node> cq;
      double mind = std::numeric_limits<double>::max();
      int i = 0; int index = 0;
      for(auto&& e: C) {
        double d = dist(e->center, q);
        cp.push_back({d, e});
        if(mind < d) {
          mind = d;
          cq = e;
          index = i;
        }
        i++;
      }
      cp.erase(cp.begin() + index);
      PQ.insert(cp.begin(), cp.end());
      traverse(cq, PQ, R, count, q);
  }
}

std::vector<std::vector<int>> pk_tree::search(std::vector<point> &query, int l) {
  std::vector<std::vector<int>> res(query.size());
  int i = 0;
  for(auto&& q: query) {
    res[i++] = search(q, l);
  }
  return res;
}
