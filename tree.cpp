#include "tree.h"
#include "node.h"
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
#include <cmath>
#include <functional>
#include <unordered_set>

int tree::compute_dimension() const {
	if(points.size() > 0) return points[0].size();
	throw std::invalid_argument("Incorrect dimension\n");
}

void tree::read_point(const std::string file_name) {
	srand(static_cast<unsigned> (time(0)));
	std::ifstream in(file_name);
	int n, m; in >> n >> m;
	double t;
//	std::set<point> s;
	for(int i = 0; i < n; ++i) {
		point p;
		for(int j = 0; j < m; ++j) {
			in >> t;
			double e = static_cast<double> (rand()) / static_cast<float>(RAND_MAX);
			t += e;
			p.push_back(t);
		}
		points.push_back(p);
	}
}

double tree::dist(const point &p, const point &q) {
	// need to add sqrt.
	double result = 0.0;
	for(size_t i = 0; i < p.size(); ++i) {
		result += (p[i] - q[i]) * (p[i] - q[i]);
	}
	return result;
}

double tree::median(std::vector<double> &v) {
	std::sort(v.begin(), v.end());
	if(v.size() % 2 == 0)
		return (v[v.size()/2] + v[v.size()/2 - 1])/2.0;
	else
		return v[v.size()/2];
}

double tree::mean(const point &p) {
	double sum = 0.0;
	for(auto&& e: p) sum += e;
	return sum/p.size();
}

double tree::variance(const point &p) {
	double var = 0.0;
	double m = mean(p);
	for(auto&& e: p) var += (e - m) * (e - m);
	return var/p.size();
}

std::string tree::print(std::shared_ptr<node> &root) {
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

point tree::get_point(const int i) const {
	if(i >= 0 && i < points.size()) return points[i];
	throw std::invalid_argument("Index is not in range!!\n");
}

std::vector<point> tree::get_points(const std::vector<int> &v) const {
	std::vector<point> result;
	for(auto&& e: v) {
		result.push_back(get_point(e));
	}
	return result;
}

std::vector<point> tree::get_points(const std::unordered_set<int> &s) const {
	std::vector<point> result;
	for(auto&& e: s) {
		result.push_back(get_point(e));
	}
	return result;
}

template <typename Iter>
std::vector<point> tree::get_points(Iter it, Iter end) {
    std::vector<point> res;
    for (; it != end; ++it) res.push_back(get_point(*it));
    return res;
}

size_t tree::size() { return points.size(); }
int tree::get_dimension() const { return dimension; }

std::vector<point> tree::get_points() const { return points; }
