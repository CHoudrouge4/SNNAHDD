#include "pk_tree.h"
#include "node.h"
#include <iostream>
#include <unordered_set>
#include <random>
#include <limits>
#include <set>

std::random_device pk_tree::rd;
std::mt19937 pk_tree::gen(rd());

pk_tree::pk_tree(std::string file_name, unsigned int K, int I_max) {
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

int pk_tree::get_centroid_index(const std::vector<point> &centroid, const point& p) {
  double max_distance = std::numeric_limits<double>::max();
  size_t index = 0;
  for(size_t i = 0; i < centroid.size(); ++i) {
    double d = dist(p, centroid[i]);
    if(d <= max_distance) {
      max_distance = d;
      index = i;
    }
  }
  return index;
}

std::vector<cluster> pk_tree::clustering(const std::vector<point> &centroid, const std::vector<int> &pts) {
  //std::cout << "the centroid size is " << centroid.size() << std::endl;
  std::vector<cluster> c(centroid.size());
  for(auto&& e : pts) {
    auto index = get_centroid_index(centroid, points[e]);
    c[index].push_back(e);
  }
  return c;
}

double pk_tree::average_dim_coordinates(cluster& c, int d) {
  double sum = 0;
  for(size_t j = 0; j < c.size(); ++j)
    sum += points[c[j]][d];
  return sum;
}

point pk_tree::get_mean_point(cluster& c) {
  point p(dimension);
  for(int i = 0; i < dimension; ++i) {
    auto sum = average_dim_coordinates(c, i);
    p[i] = sum/c.size();
  }
  return p;
}

std::vector<point> pk_tree::get_P(std::vector<cluster> &C) {
  std::vector<point> P;
  for(size_t i = 0; i < C.size();  i++) P.push_back(get_mean_point(C[i]));
  return P;
}

void pk_tree::construct(std::shared_ptr<node> &current) {

  if(current->pts.size() < k) return;

  std::unordered_set<int> p = select(current->pts);
  std::vector<point> P = get_points(p);
  bool coveraged = false;
  int i = 0;
  std::vector<cluster> C;
  while(!coveraged && i < i_max) {
    C = clustering(P, current->pts);
    std::vector<point> P_new = get_P(C);
    if(P == P_new) coveraged = true;
    P = P_new;
    i++;
  }

  int j = 0;
  for(auto&& c: C) {
    if(c.size() == 0) continue;
    std::shared_ptr<node> child = std::make_shared<node>();
    child->center = P[j];
    child->pts = c;
    current->children.push_back(child);
    construct(current->children.back());
    j++;
  }
}

std::vector<int> pk_tree::get_top_k(std::set<std::pair<double, int>> &R) {
  std::vector<int> res(k);
  unsigned int i = 0;
  for(auto&& e : R) {
    if(i >= k) break;
    res[i++] = e.second;
  }
  return res;
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
  return get_top_k(R);
}

void pk_tree::traverse(std::shared_ptr<node> &current,
                            std::set<std::pair<double, std::shared_ptr<node>>> &PQ,
                            std::set<std::pair<double, int>> &R, int &count, point &q) {
  if(current == nullptr) return;
  if(current->children.size() == 0 && current->pts.size() != 0) {
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
  for(auto&& q: query) res[i++] = search(q, l);
  return res;
}
