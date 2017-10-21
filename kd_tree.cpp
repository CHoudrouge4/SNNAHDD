#include "kd_tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<point> kd_tree::points = std::vector<point>();

kd_tree::kd_tree(const std::string file_name) {
	read_point(file_name);
	for(size_t i = 0; i < points.size(); ++i) {
		if(root != nullptr) {
			(root->pts).push_back(i);
			std::cout << root->pts[i] << '\n';
		}
	}
	// call construct
}

void kd_tree::construct(std::unique_ptr<node> current, int index) {
	if((current->pts).size() == 1) return;
//	int n = points[0].size();
} 

void kd_tree::read_point(const std::string file_name) {
	std::ifstream in(file_name);
	int n, m; in >> n >> m;
	double t; 
	for(int i = 0; i < n; ++i) {
		point p;
		for(int j = 0; j < m; ++j) {
			in >> t;
			std::cout << t << ' ';
			p.push_back(t);		
		}
		std::cout << '\n';
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
		return v[v.size()/2] + v[v.size()/2 + 1];
	else 
		return v[v.size()/2];	
}
