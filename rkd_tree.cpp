#include "rkd_tree.h"
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

int rkd_tree::ND = 0;
std::random_device rkd_tree::rd;
std::mt19937 rkd_tree::gen(rd());
std::uniform_int_distribution<> rkd_tree::dis(0, rkd_tree::ND);


rkd_tree::rkd_tree(const std::string file_name) {
	ND = 5;
}

void rkd_tree::read_point(const std::string file_name) {
	srand(static_cast<unsigned> (time(0)));
	std::ifstream in(file_name);
	int n, m; in >> n >> m;
	double t; 
	std::set<point> s;
	for(int i = 0; i < n; ++i) {
		point p;
		for(int j = 0; j < m; ++j) {
			in >> t;
			double e = static_cast<double> (rand()) / static_cast<float>(RAND_MAX);
			t += e;
			p.push_back(t);		
		}
		s.insert(p);
	}
	
	for(auto&& e : s) {
		points.push_back(e);
	}

	in.close();
}

double rkd_tree::dist(const point &p, const point &q) {
	// need to add sqrt.
	double result = 0.0;
	for(size_t i = 0; i < p.size(); ++i) {
		result += (p[i] - q[i]) * (p[i] - q[i]);
	}
	return result;
}

double rkd_tree::median(std::vector<double> &v) {
	std::sort(v.begin(), v.end());
	if(v.size() % 2 == 0)
		return (v[v.size()/2] + v[v.size()/2 - 1])/2.0;
	else 
		return v[v.size()/2];
}

std::string rkd_tree::print() {
	return std::string(); //print(root);
}

double rkd_tree::mean(const point &p) {
	double sum = 0.0;
	for(auto&& e: p) sum += e;
	return sum/p.size();
}

double rkd_tree::variance(const point &p) {
	double var = 0.0;
	double m = mean(p);
	for(auto&& e: p) var += (e - m) * (e - m);
	return var/p.size(); 
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

point rkd_tree::get_point(const int i) const {
	if(i >= 0 && i < points.size()) return points[i];
	throw std::invalid_argument("Index is not in range!!\n");
}

std::vector<point> rkd_tree::get_points(const std::vector<int> &v) const {
	std::vector<point> result;
	for(auto&& e: v) {
		result.push_back(get_point(e));
	}
	return result;
}

