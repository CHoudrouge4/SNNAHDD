#pragma once
#include "node.h"
#include "tree.h"
#include <memory>
#include <random>
#include <unordered_set>

typedef std::vector<int> cluster;

class pk_tree : public tree {
private:
  std::shared_ptr<node> root;
  int k;
  int i_max;
  static std::random_device rd;
	static std::mt19937 gen;

  pk_tree(){}
  std::unordered_set<int> select(const std::vector<int>&) const;
  point get_mean_point(cluster&);
  std::vector<point> get_P(std::vector<cluster>&);
  std::vector<cluster> clustering(const std::vector<point>&, const std::vector<int>&);
  void construct(std::shared_ptr<node>&);
  std::vector<int> search(point&, int);
  void traverse(std::shared_ptr<node> &,
                std::set<std::pair<double, std::shared_ptr<node>>> &,
                std::set<std::pair<double, int>> &, int&, point &);
public:
  pk_tree(std::string, int, int);

};
