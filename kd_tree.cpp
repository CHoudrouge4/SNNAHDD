#include "kd_tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

kd_tree::kd_tree(const std::string file_name) {
	read_point(file_name);
	for(size_t i = 0; i < points.size(); ++i) {
		(root->pts).push_back(i);
	}
	if(points.size() > 0) dimension = points[0].size();
	construct(root, 0);
}

//90 % is ok
void kd_tree::construct(std::shared_ptr<node> &current, int index) {
	int n = (current->pts).size(); 
	if(n <= 1) return;
	std::vector<double> v(n);
	for(int i = 0; i < n; ++i) {
		v[i] = points[current->pts[i]][index];
	}
	double med = median(v);
	current->median = med;
	current->index  = index;
	current->left   = std::make_shared<node>();
	current->right  = std::make_shared<node>();
	std::shared_ptr<node> l  = current->left;
	std::shared_ptr<node> r = current->right;
	l->parent = current;
	r->parent = current;
	for(int i = 0; i < n; i++) {
		if(points[(current->pts)[i]][index] < med) {
			l->pts.push_back((current->pts)[i]);
		} else {
			r->pts.push_back((current->pts)[i]);
		}
	}
	index = (index + 1) % dimension;
	construct(l, index);
	construct(r, index);
} 

std::vector<std::vector<int>> kd_tree::search(std::vector<point> &query) {
	std::vector<std::vector<int>> result(query.size());
	double R = std::numeric_limits<double>::max();
	int i = 0;
	for(auto&& q: query) {
		search(root, q, R, result[i]);
		i++;
	}	
	return result;
}
// need revision and improvement + testing 
void kd_tree::search(std::shared_ptr<node> &current, const point &q, double R, std::vector<int> &res) {
	if(current->left == nullptr && current->right == nullptr) {		
		double d = dist(points[current->pts[0]], q);  
		if(d < R) {
			R = d;
			res.push_back(current->pts[0]);
		}	
	} else {
		double med   = current->median;
		int index = current->index;
		bool exp;
		if(q[index] <= med) {
			exp = true;
			search(current->left,  q, R, res);
		} else {
			exp = false;
			search(current->right, q, R, res);
		}
		for(auto&& p: current->pts) {
			if(exp && points[p][index] > med && dist(q, points[p]) < R) {
				search(current->right, q, R, res);
				break;
			} 

			if(!exp && points[p][index] <= med && dist(q, points[p]) < R) {
				search(current->left, q, R, res);
				break;
			}
		}
	}
	
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
