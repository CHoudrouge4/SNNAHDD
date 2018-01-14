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
  unsigned int k;
  int i_max; // maximum number of iteration
  static std::random_device rd;
	static std::mt19937 gen;

  pk_tree(){}
  /**
  * This function select k integr from the
  * vector of integer and return them by unordered_set
  */
  std::unordered_set<int> select(const std::vector<int>&) const;

  /**
  * This function computes the point whose coordinates are
  * the mean value of the the points' coordinates in a given cluster
  */
  point get_mean_point(cluster&);

  /**
  * This function computes the mean points for each
  * cluser in the given vector of cluseters
  */
  std::vector<point> get_P(std::vector<cluster>&);


  int get_centroid_index(const std::vector<point> &, const point&);
  double average_dim_coordinates(cluster&, int);
  std::vector<int> get_top_k(std::set<std::pair<double, int>>&);
  /**
  * This function assigns for each point (given by there index in a vector on integer)
  * to a centroid point (i.e it clusser the vector of point arround the centroids).
  *
  */
  std::vector<cluster> clustering(const std::vector<point>&, const std::vector<int>&);

  /**
  * This function construct the priority search k-means tree.
  */
  void construct(std::shared_ptr<node>&);

  void traverse(std::shared_ptr<node> &,
                std::set<std::pair<double, std::shared_ptr<node>>> &,
                std::set<std::pair<double, int>> &, int&, point &);
public:
  pk_tree(std::string, unsigned int, int);
  std::vector<int> search(point&, int);
  std::vector<std::vector<int>> search(std::vector<point> &, int);
};
