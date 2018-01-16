#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include "node.h"
#include "tree.h"

class kd_tree : public tree {
friend std::ostream &operator<<(std::ostream &out, kd_tree &k) {
	out << k.print();
	return out;
}

private:
	std::shared_ptr<node> root = std::make_shared<node>();

	kd_tree() {}

	/**
 	* This function construct the kd-tree
 	*
 	*/
	void   construct(std::shared_ptr<node> &current, int index);

	/**
 	* this function search for the nearest points arround q.
 	*
 	*/
	void search(std::shared_ptr<node> &current, const point &q, double &R, int &res, double &pmed);

	std::string print();

public:
	/**
 	* A constructor
 	* it takes a file name that contains a set of points and creat
 	* a kd-tree.
 	*/
	kd_tree(const std::string file_name);

	kd_tree(const std::string file_name, double (*)(point &a, point &b));

	/**
	* @param: query is a vector of point that we want to search for
	* their nearest neighbor.
	*/
	std::vector<int> search(std::vector<point> &query);

};
