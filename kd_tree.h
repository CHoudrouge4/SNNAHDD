#pragma once 
#include <fstream>
#include <vector>
#include <memory>

typedef std::vector<double> point 

struct node {
	double r; 
	std::vector<int> pts;
	std::unique_ptr<node> left;
	std::unique_ptr<node> right;
	std::unique_ptr<node> parent;
};


class kd_tree {
	
private: 
	static std::vector<point> pts;
	std::unique_ptr<node> root = nullptr;
	void read_point(const std::string file_name);
public:
	kd_tree() {}
	kd_tree(const std::string file_name);
};
