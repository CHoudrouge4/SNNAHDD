#include "kd_tree.h"
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

kd_tree::kd_tree(const std::string file_name) {
	read_point(file_name);
	for(size_t i = 0; i < points.size(); ++i) {
		(root->pts).push_back(i);
	}
	if(points.size() > 0) dimension = points[0].size();
	std::cout << "start constructing" << std::endl;
	construct(root, 0);
	std::cout << "done constructiogn" << std::endl;
}

void kd_tree::construct(std::shared_ptr<node> &current, int index) {

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
	current->left   = std::make_shared<node>();
	current->right  = std::make_shared<node>();
	std::shared_ptr<node> l = current->left;
	std::shared_ptr<node> r = current->right;
	l->parent = current;
	r->parent = current;
	for(int i = 0; i < n; i++) {
		if(points[current->pts[i]][index] <= med) {
			l->pts.push_back(current->pts[i]);
		} else {
			r->pts.push_back(current->pts[i]);
		}
	}
	index = (index + 1) % dimension;
	construct(current->left, index);
	construct(current->right, index);
}

std::vector<int> kd_tree::search(std::vector<point> &query) {
	std::vector<int> result(query.size());
	int i = 0;
	for(auto&& q: query) {
		double pmed = 0;
		double R = std::numeric_limits<double>::max();
		search(root, q, R, result[i], pmed);
		i++;
	}
	return result;
}

// need revision and improvement
void kd_tree::search(std::shared_ptr<node> &current, const point &q, double &R, int &res, double &pmed) {
	if(current == nullptr) return;
	if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0) {
		double d = dist(points[current->pts[0]], q);  //compure the distance between p and q where p is the only node in the leaf node
		if(d < R) {
			R = d;
			res = current->pts[0];
			pmed = current->parent->median;
		}
	} else {
		double med = current->median;
		int index  = current->index;
		bool exp;
		if(q[index] <= med) {
			exp = true;
			search(current->left,  q, R, res, pmed);
		} else {
			exp = false;
			search(current->right, q, R, res, pmed);
		}

		point plane = q;
		plane[index] = med;
		double r = dist(plane, q);
		if(r < R) {
				for(auto&& p: current->pts) {
				if(exp && points[p][index] > med && dist(q, points[p]) <= R) {
					search(current->right, q, R, res, pmed);
					break;
				}
				if(!exp && points[p][index] <= med && dist(q, points[p]) <= R) {
					search(current->left, q, R, res, pmed);
					break;
				}
			}
		}
	}
}
