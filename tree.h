#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include "node.h"

class tree{
  protected:
  	const double epsilon = 0.001;
  	int dimension;
  	int limit;
  	std::vector<point> points;

    double dist(const point &p, const point &q);
  	double median(std::vector<double> &v);
  	double mean(const point &p);
  	double variance(const point &p);
  	int    split_dimension(std::shared_ptr<node> &current);
  	void read_point(const std::string file_name);
    std::string print(std::shared_ptr<node> &root);
  public:
    tree() {}
  	std::vector<point> get_points() const;
  	point get_point(const int i) const;
  	std::vector<point> get_points(const std::vector<int> &v) const;
  	int get_dimension() const;
  	size_t size();
};
