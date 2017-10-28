#pragma once 
#include <fstream>
#include <vector>
#include <memory>

/**
 * TODO: overide <<
 * Testing.
 * add aditional constructors.
 * Make the code modular concerning the cluctiring algorithm
 * and the distance algorithm.	
 * change the way of getting the result.
 * */

typedef std::vector<double> point; 

struct node {
	double r; 
	double median;
	int index; // the index that we computed the median with respect to it.
	std::vector<int> pts;
	std::shared_ptr<node> left;   // pointer to the left child
	std::shared_ptr<node> right;  // pointer to the right child
	std::shared_ptr<node> parent; // pointer to the parent
};

class kd_tree {	
private: 
	int dimension; // store the dimension of the space
	std::shared_ptr<node> root = std::make_shared<node>();
	std::vector<point> points; // stors all the points in the space

	/**
 	* this function computes the square of the ecludian distance
 	* between point p and q 
 	*/
	double dist(const point p, const point q);

	/**
	* this function computes the median 
 	* for a vector of values.
 	* NOTE: v is not sorted
 	*/
	double median(std::vector<double> &v);  
	/**
 	* This function construct the kd-tree
 	*
 	*/
	void   construct(std::shared_ptr<node> &current, int index);
	/**
 	* this function reads points from a file 
 	* and stors them in "points"
 	*
 	*/
	void   read_point(const std::string file_name);
	/**
 	* this function search for the nearest points arround q.
 	*
 	*/
	void search(std::shared_ptr<node> &current, const point &q, double R, std::vector<int> &res);    
public:
	kd_tree() {}
	/**
 	* A constructor 
 	* it takes a file name that contains a set of points and creat 
 	* a kd-tree.
 	*/
	kd_tree(const std::string file_name);
	/**
	* @param: query is a vector of point that we want to search for 
	* their nearest neighbor.
	*/
	std::vector<std::vector<int>> search(std::vector<point> &query);
};
