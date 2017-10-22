#pragma once 
#include <fstream>
#include <vector>
#include <memory>

typedef std::vector<double> point; 

struct node {
	double r; 
	std::vector<int> pts;
	std::shared_ptr<node> left; // change unique to shared
	std::shared_ptr<node> right;
	std::shared_ptr<node> parent;
};

class kd_tree {	
private: 
	int dimension;
	std::shared_ptr<node> root = std::make_shared<node>();
	std::vector<point> points;
	double dist(const point p, const point q);
	// v is not sorted 
	double median(std::vector<double> &v);  
	void   construct(std::shared_ptr<node> &current, int index);
	void   read_point(const std::string file_name);
public:
	kd_tree() {}
	kd_tree(const std::string file_name);
};
