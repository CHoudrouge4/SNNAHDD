#include "rkd_tree.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <sstream>
#include <queue>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <set>
#include <random>
#include <memory>
#include "node.h"
#include <cmath>
#include <functional>

int rkd_tree::ND = 5;
std::random_device rkd_tree::rd;
std::mt19937 rkd_tree::gen(rd());
std::uniform_int_distribution<> rkd_tree::dis(0, rkd_tree::ND);

rkd_tree::rkd_tree(const std::string file_name) {
	construct(file_name);
}

rkd_tree::rkd_tree(const std::string file_name, int n, int l) {
	tree_num = n;
	limit = l;
	construct(file_name);
}

void rkd_tree::construct(const std::string file_name) {
	read_point(file_name);
	roots = std::vector<std::shared_ptr<node>>(tree_num);
	for(size_t i = 0; i < roots.size(); ++i) {

		roots[i] = std::make_shared<node>();
		for(size_t j = 0; j < points.size(); ++j)
		 	(roots[i]->pts).push_back(j);

		if(points.size() > 0) {
			dimension = points[0].size();
		}

		std::cout << "start constructing root " << i << '\n';
		int index = split_dimension(roots[i]);
		roots[i]->index = index;
		construct(roots[i], index);
		std::cout << "done constructiing root " << i << '\n';
	}
}

void rkd_tree::construct(std::shared_ptr<node> &current, int index) {
	if(current == nullptr) return;
	int n = (current->pts).size();
	// if we have one point, then we are done
	if(n <= 1) return;
	// get thr point of the current node
	std::vector<double> v(n);
	for(int i = 0; i < n; ++i) v[i] = points[current->pts[i]][index];

	// compute the median of the current node

	double med = median(v);
	current->median = med;
	current->index  = index;
	// create a left and right child
	//std::cout << med << "\n";// << v << std::endl;
	current->left   = std::make_shared<node>();
	current->right  = std::make_shared<node>();
	std::shared_ptr<node> l = current->left;
	std::shared_ptr<node> r = current->right;
	l->parent = current;
	r->parent = current;
	for(int i = 0; i < n; i++) {
//		std::cout << v << std::endl;
		if(points[current->pts[i]][index] <= med) {
			l->pts.push_back(current->pts[i]);
		} else {
			r->pts.push_back(current->pts[i]);
		}
	}
	//change here
	if(dimension < 5)
		index = (index + 1) % dimension;
	else
		index = split_dimension(current);

	construct(current->left, index);
	construct(current->right, index);
}

int rkd_tree::split_dimension(std::shared_ptr<node> &current) {
	point tmp(current->pts.size());
	std::vector<std::pair<double, int>> vars(dimension);
	for(int i = 0; i < dimension; ++i) {
		for(size_t j = 0; j < current->pts.size(); ++j)
			tmp[j] = points[current->pts[j]][i];
		double var = variance(tmp);
		vars[i]	   = {var, i};
	}
	std::sort(vars.rbegin(), vars.rend());
	int i = dis(gen);
	return vars[i].second;
}

std::string rkd_tree::print() {
	std::ostringstream o;
	for(int i = 0; i < roots.size(); ++i) {
		o << "\n rkd_tree nb " << i << '\n';
		o << print(roots[i]);
	}
	return o.str();
}

std::string rkd_tree::print(std::shared_ptr<node> &root) {
	if(root == nullptr) return std::string();
	std::ostringstream o;
	std::queue<std::shared_ptr<node>> Q;
	int nodes_in_current_level = 1;
	int nodes_in_next_level = 0;
	Q.push(root);
	int level = 0;
	o << level << '\t';
	while(!Q.empty()) {
		std::shared_ptr<node> current = Q.front();
		Q.pop();
		nodes_in_current_level--;
		if(current != nullptr) {
			o << *current << '\t';
			if(current->left  != nullptr) {
				Q.push(current->left);
				nodes_in_next_level++;
			}
			if(current->right != nullptr) {
				Q.push(current->right);
				nodes_in_next_level++;
			}
		}
		if(nodes_in_current_level == 0) {
			o << '\n';
			nodes_in_current_level = nodes_in_next_level;
			nodes_in_next_level = 0;
			level++;
			o << level << '\t';
		}
	}
	return o.str();
}

std::vector<int> rkd_tree::search(const std::vector<point> &query) {
	std::cout << limit  << std::endl;
	std::vector<int> results(query.size());
	for(size_t i = 0; i < query.size(); ++i)
		results[i] = search(query[i]);
	return results;
}

int rkd_tree::search(const point &q) {
	std::set<std::pair<double, int>> pq;
	std::vector<bool> visited(points.size());
	int res = 0;
	int count = 0;
	double R = std::numeric_limits<double>::max();
	double betterR = std::numeric_limits<double>::max();

	for(size_t i = 0; i < roots.size(); ++i) {
		double pmed = roots[i]->median;
		explore(roots[i], q, R, res, pmed, count, pq, i, visited);
	//	std::cout << "/* pq size */" << pq.size() << '\n';
	}
	//std::cout << "/* pq size */" << pq.size() << '\n';
	while(!pq.empty() && count <= limit) {
	//	std::cout << pq.size() << std::endl;
		auto p = *pq.begin();
		pq.erase(pq.begin());
		int rp = p.first;
		int m  = p.second;
		search(roots[m], q, R, res, pq, m, count, rp, visited);
	}
	return res;
}

void rkd_tree::explore(std::shared_ptr<node> &current, const point &q, double &R, int &res, double pmed, int &count,
	 																	std::set<std::pair<double, int>> &pq, int m, std::vector<bool> & visited) {
	if(current == nullptr) return;
	if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0 && !visited[current->pts[0]]) {
		visited[current->pts[0]] = true;
		count++;
		double d = dist(points[current->pts[0]], q);  //compure the distance between p and q where p is the only node in the leaf node
		if(d < R) {
			R = d;
			res = current->pts[0];
			point plane = q;
			plane[current->parent->index] = current->parent->median;
			double r = dist(plane, q);
			if(r < R){
				 //std::cout << r << " < " << R << std::endl;
				 pq.insert({r, m});
			}
		}
	} else {
		double med = current->median;
		int index  = current->index;
		if(q[index] <= med)
			explore(current->left,  q, R, res, pmed, count, pq, m, visited);
		else
			explore(current->right, q, R, res, pmed, count, pq, m, visited);
	}
}

// need revision and improvement + testing
void rkd_tree::search(std::shared_ptr<node> &current, const point &q, double &R, int &res, std::set<std::pair<double, int>> pq,
int m, int &count, int rp, std::vector<bool> &visited) {
	if(current == nullptr) return;
	if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0 && !visited[current->pts[0]]) {
		visited[current->pts[0]] = true;
		count++;
		double d = dist(points[current->pts[0]], q);  //compure the distance between p and q where p is the only node in the leaf node
		if(d < R) {
			R = d;
			res = current->pts[0];
		}
	} else {
		double med = current->median;
		int index  = current->index;
		bool exp;
		point plane = q;
		plane[index] = med;
		double r = dist(plane, q);
	  if(q[index] <= med) {
			exp = true;
			search(current->left, q, R, res, pq, m, count, rp, visited);
		} else {
			exp = false;
			search(current->right, q, R, res, pq, m, count, rp, visited);
		}

		auto p = *pq.begin();
		if(r <= rp) {
			if(r < R) { // if the distance to the plane is less than the reduis
					for(auto&& p: current->pts) {
						if(exp && points[p][index] > med && dist(q, points[p]) <= R) {
							search(current->right, q, R, res, pq, m, count, rp, visited);
							break;
						}
						if(!exp && points[p][index] <= med && dist(q, points[p]) <= R) {
							search(current->left, q, R, res, pq, m, count, rp, visited);
							break;
						}
				}
			}
		}
		if(r < R) {
			pq.insert({r, m});
			return;
		}
	}
}
