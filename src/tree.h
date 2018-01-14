#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <unordered_set>
#include "node.h"
#include <set>

class tree {
  protected:
  	const double epsilon = 0.001;
  	int dimension;
  	int limit;
  	std::vector<point> points;

    /**
   	* this function computes the square of the ecludian distance
   	* between point p and q
   	*/
    double dist(const point &p, const point &q);

    /**
  	* this function computes the median
   	* for a vector of values.
   	* NOTE: v is not sorted
   	*/
    double median(std::vector<double> &v);

    double mean(const point &p);
  	double variance(const point &p);
    int    compute_dimension() const;
  	int    split_dimension(std::shared_ptr<node> &current);

    /**
    * this function reads points from a file
    * and stors them in "points"
    *
    */
  	void read_point(const std::string file_name);
    std::string print(std::shared_ptr<node> &root);
  public:
    tree() {}

    /**
   	* this functions return the data stored
   	* in points
   	*/
  	std::vector<point> get_points() const;

    /**
   	* this function takes an index and return
   	* its corresponding point.
   	*/
  	point get_point(const size_t i) const;

    /**
   	* this funtion take a set of indeces and return
   	* their corresponding points.
   	*/
  	std::vector<point> get_points(const std::vector<int> &v) const;
    std::vector<point> get_points(const std::unordered_set<int> &) const;
    template <typename Iter>
    std::vector<point> get_points(Iter, Iter);

    /*
   	* This function return the dimension of the points
   	* stored
   	*/
    int get_dimension() const;

    /*
   	* This function returns the number of the points
   	* in the tree
  	*/
  	size_t size();
};
