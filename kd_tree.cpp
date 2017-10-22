#include "kd_tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>

kd_tree::kd_tree(const std::string file_name) {
	read_point(file_name);
	for(size_t i = 0; i < points.size(); ++i) {
			(root->pts).push_back(i);
	}
	if(points.size() > 0) dimension = points[0].size();
	construct(root, 0);
}

void kd_tree::construct(std::shared_ptr<node> &current, int index) {
	int n = (current->pts).size(); 
	if(n <= 1) return;
	std::vector<double> v(n);
	for(int i = 0; i < n; ++i) {
		v[i] = points[current->pts[i]][index];
	}
	double med = median(v);
	current->left  = std::make_shared<node>();
	current->right = std::make_shared<node>();
	std::shared_ptr<node> l  = current->left;
	std::shared_ptr<node> r = current->right;
	l->parent = current;
	r->parent = current;
	std::cout << "median " << med << '\n';
	for(int i = 0; i < n; i++) {
		if(points[(current->pts)[i]][index] < med) {
			l->pts.push_back((current->pts)[i]);
		} else {
			r->pts.push_back((current->pts)[i]);
		}
	}
	index = (index + 1) % dimension;
	std::cout << "left: ";	
	for(size_t i = 0; i < l->pts.size(); ++i) {
		std::cout << l->pts[i] << ' ';
	}
	std::cout << '\n';
	std::cout << "right: ";	
	for(size_t i = 0; i < r->pts.size(); ++i) {
		std::cout << r->pts[i] << ' ';
	}
	std::cout << '\n';
	construct(l,  index);
	construct(r, index);

} 

void kd_tree::read_point(const std::string file_name) {
	std::ifstream in(file_name);
	int n, m; in >> n >> m;
	double t; 
	for(int i = 0; i < n; ++i) {
		point p;
		for(int j = 0; j < m; ++j) {
			in >> t;
			p.push_back(t);		
		}
		points.push_back(p);
	}
	in.close();
}

double kd_tree::dist(const point p, const point q) {
	// need to add sqrt.
	double result = 0.0;
	for(size_t i = 0; i < p.size(); ++i) {
		result += (p[i] - q[i]) * (p[i] - q[i]);
	}
	return result;
}

double kd_tree::median(std::vector<double> &v) {	
	std::sort(v.begin(), v.end());
	if(v.size() % 2 == 0)
		return (v[v.size()/2] + v[v.size()/2 - 1])/2;
	else 
		return v[v.size()/2];	
}
