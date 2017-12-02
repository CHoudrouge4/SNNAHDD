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

rkd_tree::rkd_tree(const std::string file_name, int n) {
	ND = n;
	construct(file_name);
}

void rkd_tree::construct(const std::string file_name) {
	read_point(file_name);
	roots = std::vector<std::shared_ptr<node>>(ND);
	for(size_t i = 0; i < roots.size(); ++i) {

		roots[i] = std::make_shared<node>();
		for(size_t j = 0; j < points.size(); ++j)
		 	(roots[i]->pts).push_back(j);

		if(points.size() > 0) dimension = points[0].size();
	
		std::cout << "start constructing root " << i << '\n';
		int index = split_dimension(roots[i]);
		roots[i]->index = index;
		construct(roots[i], index);
		std::cout << "done constructiing root " << i << '\n';
	}
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
		points.push_back(p);
	}
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

size_t rkd_tree::size() { return points.size(); }
int rkd_tree::get_dimension() const { return dimension; }

std::vector<int> rkd_tree::search(const std::vector<point> &query) {
	std::vector<int> results(query.size());
	for(size_t i = 0; i < query.size(); ++i)
		results[i] = search(query[i]);
	return results;
}

int rkd_tree::search(const point &q) {
	std::set<std::pair<double, int>> pq;
	int res = 0;
	int count = 0;
	double R = std::numeric_limits<double>::max(); 
	for(size_t i = 0; i < roots.size(); ++i) {
		double pmed = roots[i]->median;
		explore(roots[i], q, R, res, pmed, count, pq, i);
	}
	
//	std::cout << "pq size = " << pq.size() << std::endl;
	double betterR = std::numeric_limits<double>::max();
	while(!pq.empty() && betterR > R) {
		auto p = *pq.begin();
		pq.erase(pq.begin());
		int rp = p.first;
		int m  = p.second;
		explore_best(roots[m], q, betterR, res, pq, m, rp);	
		if(betterR < R) {
			R = betterR;
		}
	}
	return res;
}

void rkd_tree::explore_best(std::shared_ptr<node> &current, const point &q, double &R, int &res, std::set<std::pair<double, int>> &pq, int m, int rp) {
	if(current == nullptr) return;
	if(current-> left == nullptr && current->right == nullptr && current->pts.size() != 0) {
		double d = dist(points[current->pts[0]], q);
		if(d < R) {
			R = d;
			res  = current->pts[0];

			double pmed = current->parent->median;
			int index = current->parent->index;
			point plane = q;
			plane[index] = pmed;
			double r = dist(plane, q);
			pq.insert({r, m});
		}
		
	} else {
		double med = current->median;
		int index  = current->index;
		point plane = q;
		plane[index] = med;
		double r = dist(plane, q);
		if(rp <= r) {
			if(q[index] <= med)
				explore_best(current->right, q, r, res, pq, m, rp);
			else 
				explore_best(current->left, q, r, res, pq, m, rp);
		} else {
			if(q[index] <= med)
				explore_best(current->right, q, r, res, pq, m, rp);
			else 
				explore_best(current->left, q, r, res, pq, m, rp);	
		}
	
			//if(q[index] <= med && rp <= r)
			//	explore_best(current->right, q, R, res, pq, explored, m, rp);
			 
			//if(q[index] > med && rp <= r) {
			//	explore_best(current->left , q, R, res, pq, explored, m, rp);
			//}	
	}	
}

void rkd_tree::explore(std::shared_ptr<node> &current, const point &q, double &R, int &res, double pmed, 							   
			int &count, std::set<std::pair<double, int>> &pq, int m) {

	if(current == nullptr) return;
	if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0) {
		double d = dist(points[current->pts[0]], q);  //compure the distance between p and q where p is the only node in the leaf node
		if(d < R) {
			R = d;
			res = current->pts[0];
		    double pmed = current->parent->median;
			int index = current->parent->index;
			point plane = q;
			plane[index] = pmed;
			double r = dist(plane, q);
			pq.insert({r, m});
		//	std::cout << "done inserting ........" << std::endl;
		}
	} else {
		double med = current->median;
		int index  = current->index;
		if(q[index] <= med)
			explore(current->left,  q, R, res, pmed, count, pq, m);
		 else
			explore(current->right, q, R, res, pmed, count, pq, m);		
	}
}

std::vector<point> rkd_tree::get_points() const { return points; } 
