#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kd_tree.h"
#include "rkd_tree.h"
#include <cmath>
#include <random>
#include <limits>
#include <chrono>
#include <sstream>
#define RANGE 1000

static std::random_device rd;
static std::mt19937 gen(rd());

point generate_point(const int dim) {
	static std::uniform_int_distribution<> dis(-RANGE, RANGE);
	point p(dim);
	for(int j = 0; j < dim; ++j) p[j] = dis(gen);
	return p;
}

std::string point_to_string(const point p) {
	std::ostringstream o;
	for(int i = 0; i < p.size() - 1; ++i)
		o << p[i] << ' ';
	o << p[p.size() - 1];
	return o.str();
}

std::vector<point> generate_query(const int size, const int dim) {	
	std::ofstream out("query.txt");
	std::uniform_int_distribution<> d(1, size);
	int qsize = size;//d(gen);
	std::vector<point> Q(qsize);
	out << qsize << '\n';
	for(size_t i = 0; i < Q.size(); ++i) {
		Q[i] = generate_point(dim);
		out << point_to_string(Q[i]) << '\n';
	}
	out.close();
	return Q;
}

void generate_data(const int size, const int dim) {
	std::ofstream out("data.txt");
	out << size << ' ' << dim << '\n';
	for(int i = 0; i < size; i++) {
		point p = generate_point(dim);
		out << point_to_string(p) << '\n';
	}
	out.close();
}

double dist(const point &p, const point &q) {
	// need to add sqrt.
	double result = 0.0;
	for(size_t i = 0; i < p.size(); ++i) {
		result += (p[i] - q[i]) * (p[i] - q[i]);
	}
	return result;
}

std::vector<int> naive(const std::vector<point> &pts, std::vector<point> &query) {
	std::vector<int> res(query.size());
	int i = 0;
	for(auto e: query) {
		double bsf = std::numeric_limits<double>::max();
		int sol = 0;
		for(size_t j = 0; j < pts.size(); ++j) {
			double distance = dist(e, pts[j]);
			if(distance < bsf) {
				bsf = distance;
				sol = j;
			}
		}
		res[i] = sol;
		i++;
	}
	return res;
}
/*
void test0() {
//	int num = std::abs(dis(gen));

	std::ofstream in1("kd_result.txt" , std::ios_base::app);
	std::ofstream in2("naive.txt"	  , std::ios_base::app);
	std::ofstream in3("const_time.txt", std::ios_base::app);
	
	std::uniform_int_distribution<> dis(1000, 10000);
	int size; //dis(gen);
	int dim = 10;
//	std::cin >> size >> dim;
	for(size = 0; size < 10000; size += 100) {
	generate_data(size, dim);
	std::vector<point> query = generate_query(100, dim);
	//std::vector<point> query = {{4, 2}};	
		
	auto start = std::chrono::system_clock::now();
	kd_tree k("data.txt");
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	in3 << k.size() << ' ' << k.get_dimension() << ' ' << elapsed_time.count() << '\n';

//	std::cout << k << '\n';
i
	start = std::chrono::system_clock::now();
	std::vector<int> result = k.search(query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in1 << k.size() << ' ' << query.size() <<  ' ' << k.get_dimension() << ' ' << elapsed_time.count() << '\n';
	
	std::cout << "Done kd search" << std::endl;

   	const std::vector<point> pts = k.get_points();

	start = std::chrono::system_clock::now();
	std::vector<int> result_naive = naive(pts, query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in2 << k.size() << ' ' <<  query.size() << ' ' <<  k.get_dimension() <<' ' << elapsed_time.count() << '\n';
	
	std::cout << "Done naive search" << std::endl;

	for(size_t i = 0; i < result.size(); ++i) {
		if(result[i] == result_naive[i])
			std::cout << result[i] << " == " << result_naive[i] <<  " [OK] " << '\n';
		else
			std::cout << "[FAILED]" << '\n'
					  << "ANALYSIS:" << '\n'
					  << "Query: "    << i << ' ' << result[i] << " != " << result_naive[i] << '\n'
					  << "distance:" << '\n'
					  << "d(query[" << i << "]," << " result[" << i << "]) = " << dist(query[i], pts[result[i]])
					  << " d(query[" << i << "]," << " result_naive[" << i << "]) = " << dist(query[i], pts[result_naive[i]]) << '\n';
	}
	}
	in1.close();
	in2.close();
	in3.close();
}*/

void test1() {
	std::ofstream in1("rkd_result.txt" , std::ios_base::app);
	std::ofstream in2("rnaive.txt"	  , std::ios_base::app);
	std::ofstream in3("rconst_time.txt", std::ios_base::app);
	
	std::uniform_int_distribution<> dis(1000, 10000);
	for(int i = 10; i < 1000; i += 5) {
	int size = 1000; //dis(gen);
	int dim  = i;
//	std::cin >> size >> dim;
	generate_data(size, dim);
	std::vector<point> query = generate_query(size, dim);
	//std::vector<point> query = {{4, 2}};	
		
	auto start = std::chrono::system_clock::now();
	rkd_tree rk("data.txt");
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	in3 << rk.size() << ' ' << rk.get_dimension() << ' ' << elapsed_time.count() << '\n';
	}
//	std::cout << rk << '\n';
/*	start = std::chrono::system_clock::now();
	std::vector<int> result = rk.search(query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in1 << rk.size() << ' ' << query.size() <<  ' ' << rk.get_dimension() << ' ' << elapsed_time.count() << '\n';
	
	std::cout << "Done randomized kd search" << std::endl;

   	const std::vector<point> pts = rk.get_points();

	start = std::chrono::system_clock::now();
	std::vector<int> result_naive = naive(pts, query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in2 << rk.size() << ' ' <<  query.size() << ' ' <<  rk.get_dimension() <<' ' << elapsed_time.count() << '\n';
	
	std::cout << "Done naive search" << std::endl;

	int count = 0;
	for(size_t i = 0; i < result.size(); ++i) {
		if(result[i] == result_naive[i]) {
			std::cout << result[i] << " == " << result_naive[i] <<  " [OK] " << '\n';
			count++;
		} else
			std::cout << "[FAILED]" << '\n'
					  << "ANALYSIS:" << '\n'
					  << "Query: "    << i << ' ' << result[i] << " != " << result_naive[i] << '\n'
					  << "distance:" << '\n'
					  << "d(query[" << i << "]," << " result[" << i << "]) = " << dist(query[i], pts[result[i]])
					  << " d(query[" << i << "]," << " result_naive[" << i << "]) = " << dist(query[i], pts[result_naive[i]]) << '\n';
	}		

	std::cout << "\n success rate is: " << (count /((double) query.size())) * 100 << '\n';*/
	in1.close();
	in2.close();
	in3.close();
}

int main() {
	test1();
	return 0;
}
