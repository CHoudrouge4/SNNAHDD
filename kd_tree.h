#pragma once 
#include <fstream>
#include <vector>
#include <memory>

/**
 * TODO: overide << yes (I need to improve it). 
 * Testing. 50%
 * add aditional constructors.
 * Make the code modular concerning the cluctiring algorithm, not yet
 * and the distance algorithm.	not yet
 * change the way of getting the result. almost done
 * */

typedef std::vector<double> point; 
template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	if(v.size() == 0) {
		out << "()";
		return out;
	} else  {
		out << '(';
		out << ' ' << v[0] ;
		for(size_t i = 1; i < v.size(); ++i)
			out << " , " << v[i];
		out << ')';
	return out;
	}
}

struct node {
	double r; 
	double median;
	int index; // the index that we computed the median with respect to it.
	std::vector<int> pts;
	std::shared_ptr<node> left;   // pointer to the left child
	std::shared_ptr<node> right;  // pointer to the right child
	std::shared_ptr<node> parent; // pointer to the parent
	friend std::ostream &operator<<(std::ostream &out, node& n) {
		out << "NODE"
			<< " M: " <<  n.median
			<< " i: " <<  n.index
		    << ' '    <<  n.pts;
		return out;
	}
};


class kd_tree {	
friend std::ostream &operator<<(std::ostream &out, kd_tree &k) {
	out << k.print();
	return out;
}

private: 
	const double epsilon = 0.001;
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
	void search(std::shared_ptr<node> &current, const point &q, double &R, int &res, double &pmed);

	std::string print();
	std::string print(std::shared_ptr<node> &current);

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
	std::vector<int> search(std::vector<point> &query);

	/**
 	* this function takes an index and return 
 	* its corresponding point.
 	*/
	point get_point(const int i) const;

	/**
 	* this funtion take a set of indeces and return
 	* their corresponding points.
 	*/
	std::vector<point> get_points(const std::vector<int> &v) const;

	/**
 	* this functions return the data stored
 	* in points
 	*/
	std::vector<point> get_points() const;
	
	/*
 	* This function returns the number of the points 
 	* in the tree
	*/
	size_t size() const;

	/*
 	* This function return the dimension of the points 
 	* stored
 	*/
	int get_dimension() const;
};
