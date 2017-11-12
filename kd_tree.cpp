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

//90 % is ok
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
// need revision and improvement + testing 
void kd_tree::search(std::shared_ptr<node> &current, const point &q, double &R, int &res, double &pmed) {
	if(current == nullptr) return;
	if(current->left == nullptr && current->right == nullptr && current->pts.size() != 0) {		
		double d = dist(points[current->pts[0]], q);  //compure the distance between p and q where p is the only node in the leaf node
//		double d = 0;
	//	std::cout << "leaf is equal to: " << current->pts << std::endl;
		if(d < R) {
			R = d;
			res = current->pts[0]; 
			pmed = current->parent->median;
		//	std::cout << "pmed = " << pmed << std::endl;
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
		//std::cout << "for : " << med << " r is equal to " << r << std::endl;
		if(r < R){//&& med == pmed) {	
				for(auto&& p: current->pts) {
				if(exp && points[p][index] > med && dist(q, points[p]) <= R) {
		//			std::cout << "right" << std::endl;
					search(current->right, q, R, res, pmed);
					break;
				} 
				if(!exp && points[p][index] <= med && dist(q, points[p]) <= R) { 
		//			std::cout << "left" << std::endl;
					search(current->left, q, R, res, pmed);
					break;
				}
			}
		}	
	}
}

void kd_tree::read_point(const std::string file_name) {
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
//		points.push_back(p);
		s.insert(p);
	}
	
	for(auto&& e : s) {
		points.push_back(e);
	}

//	std::cout << "set size: " << s.size() << " " << "points size " << points.size() << std::endl;
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
		return (v[v.size()/2] + v[v.size()/2 - 1])/2.0;
	else 
		return v[v.size()/2];
}

std::string kd_tree::print() {
	return print(root);
}

std::string kd_tree::print(std::shared_ptr<node> &root) {
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

point kd_tree::get_point(const int i) const {
	if(i >= 0 && i < points.size()) return points[i];
	throw std::invalid_argument("Index is not in range!!\n");
}

std::vector<point> kd_tree::get_points(const std::vector<int> &v) const {
	std::vector<point> result;
	for(auto&& e: v) {
		result.push_back(get_point(e));
	}
	return result;
}

std::vector<point> kd_tree::get_points() const { return points; } 

size_t kd_tree::size() const { return points.size(); }
int    kd_tree::get_dimension() const { return dimension; }
