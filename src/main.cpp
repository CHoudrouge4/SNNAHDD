#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kd_tree.h"
#include "rkd_tree.h"
#include "pk_tree.h"
#include <cmath>
#include <random>
#include <limits>
#include <chrono>
#include <sstream>
#include <algorithm>

#define RANGE 20
const double epsilon = 0.3;

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
	for(size_t i = 0; i < p.size() - 1; ++i)
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

std::vector<std::vector<int>> knaive(const std::vector<point> &pts, std::vector<point> &query, int k) {
	std::vector<std::vector<int>> res;
	for(auto q: query) {
		std::vector<std::pair<double, int>> tmp(pts.size());
		for(size_t i = 0; i < pts.size(); ++i) {
			double distance = dist(pts[i], q);
			tmp[i] = {distance, i};
		}
		std::sort(tmp.begin(), tmp.end());
		int i = 0;
		std::vector<int> result(k);
		for(auto&& e: tmp) {
			if(i > k) break;
			result[i++] = e.second;
		}
		res.push_back(result);
	}
	return res;
}


void test0() {
	std::ofstream in1("kd_result.txt" , std::ios_base::app);
	std::ofstream in2("naive.txt"	  , std::ios_base::app);
	std::ofstream in3("const_time.txt", std::ios_base::app);

	std::uniform_int_distribution<> dis(1000, 10000);
	int size; //dis(gen);
	int dim = 10;
	for(size = 0; size < 10000; size += 100) {
	generate_data(size, dim);
	std::vector<point> query = generate_query(100, dim);

	auto start = std::chrono::system_clock::now();
	kd_tree k("data.txt");
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	in3 << k.size() << ' ' << k.get_dimension() << ' ' << elapsed_time.count() << '\n';

//	std::cout << k << '\n';

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
}

void test1() {
	std::ofstream in1("rkd_result.txt" , std::ios_base::app);
	std::ofstream in2("rnaive.txt"	  , std::ios_base::app);
	std::ofstream in3("rconst_time.txt", std::ios_base::app);
	std::ofstream in4("performance_trees.txt", std::ios_base::app);

	int total = 0;
	std::uniform_int_distribution<> dis(1000, 10000);
	int count = 0;

	for(int i = 1; i <= 40; i += 1) {
		int trees = i;
		int size = 10000; //dis(gen);
		int dim  = 10;
		//	std::cin >> size >> dim;
		generate_data(size, dim);
		std::vector<point> query = generate_query(100, dim);
		//std::vector<point> query = {{4, 2}};

		auto start = std::chrono::system_clock::now();
		rkd_tree rk("data.txt", trees, 16);
		auto end   = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_time = end - start;
		in3 << rk.size() << ' ' << rk.get_dimension() << ' ' << elapsed_time.count() << '\n';

	//	std::cout << rk << '\n';
		start = std::chrono::system_clock::now();
		std::vector<int> result = rk.search(query);
		end   = std::chrono::system_clock::now();
		elapsed_time = end - start;
		in1 << rk.size() << ' ' << query.size() <<  ' ' << rk.get_dimension() << ' ' << elapsed_time.count() << '\n';

		in4 << i << ' ' << elapsed_time.count() << '\n';
		std::cout << "Done randomized kd search" << std::endl;

	  const std::vector<point> pts = rk.get_points();

		start = std::chrono::system_clock::now();
		std::vector<int> result_naive = naive(pts, query);
		end   = std::chrono::system_clock::now();
		elapsed_time = end - start;
		in2 << rk.size() << ' ' <<  query.size() << ' ' <<  rk.get_dimension() <<' ' << elapsed_time.count() << '\n';

		std::cout << "Done naive search" << std::endl;

	 	for(size_t i = 0; i < result.size(); ++i) {
			if(dist(pts[result[i]], query[i]) <= (1 + epsilon) * dist(pts[result_naive[i]], query[i])) {
				std::cout << result[i] << " == " << result_naive[i] <<  " [OK] " << '\n';
				count++;
			} else {
				std::cout << "[FAILED]" << '\n'
						  << "ANALYSIS:" << '\n'
						  << "Query: "    << i << ' ' << result[i] << " != " << result_naive[i] << '\n'
						  << "distance:" << '\n'
						  << "d(query[" << i << "]," << " result[" << i << "]) = " << dist(query[i], pts[result[i]])
						  << " d(query[" << i << "]," << " result_naive[" << i << "]) = " << dist(query[i], pts[result_naive[i]]) << '\n';
			}
			total += query.size();
		}
	}
	std::cout << "\n success rate is: " << (count /((double) total) * 100) << '\n';
	in1.close();
	in2.close();
	in3.close();
}

template<typename T>
int count_equal(std::vector<T> &u, std::vector<T> &v, std::vector<point> &pts, point& q) {
	int count = 0;
	for(auto&& e: u){
		for(auto&& ee: v) {
			std::cout << e << ' ' << ee << '\n';
			if(dist(q, pts[ee]) <= (1 + epsilon) * dist(q, pts[e]))
				count++;
		}
	}
	return count;
}

void test2() {
	std::ofstream in1("pk_result.txt" , std::ios_base::app);
	std::ofstream in2("pk_naive.txt"	  , std::ios_base::app);
	std::ofstream in3("pk_const_time.txt", std::ios_base::app);
	//std::ofstream in4("performance_trees.txt", std::ios_base::app);
	//for(int i = 10; i <= 10000; i += 100) {
		unsigned int k = 4;
		int imax = 100;
		int size = 10000;
		int dim  = 10;
		generate_data(size, dim);


		auto start = std::chrono::system_clock::now();
		pk_tree pk("data.txt", k, imax);
		auto end   = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_time = end - start;
		in3 << pk.size() << ' ' << pk.get_dimension() << ' ' << elapsed_time.count() << '\n';

		std::vector<point> query = generate_query(1, dim);

		start = std::chrono::system_clock::now();
		auto pk_result = pk.search(query, 7);
		end = std::chrono::system_clock::now();
		elapsed_time = end - start;
		in1 << pk.size() << ' ' << query.size() << ' ' << pk.get_dimension() << ' ' << elapsed_time.count() << '\n';

		auto pts = pk.get_points();

		start = std::chrono::system_clock::now();
		auto naive_result = knaive(pts, query, k);
		end = std::chrono::system_clock::now();
		elapsed_time = end - start;
		in2 << pk.size() << ' ' << query.size() << ' ' << pk.get_dimension() << ' ' << elapsed_time.count() << '\n';
		std::cout << "the number of equal points = " << count_equal(naive_result[0], pk_result[0], pts, query[0]) <<  '\n';

	in1.close();
	in2.close();
	in3.close();
}

/*
* construct a file name: ./sift/<i>.txt
*
*/
std::string get_file_name(int i) {
	std::ostringstream o;
	o << "../sift/" << i << ".txt";
	return o.str();
}

void read_write_sift_point(std::ifstream &in, std::ofstream& out) {
	double x;
	for(int i = 0; i < 128; ++i) {
		in >> x; out << x << ' ';
	}
	out << '\n';
}

/*
*
*
*/
void creat_sift_data_file(int size) {
	std::ifstream read;
	std::ofstream out("sift_data.txt");
	int file_number = 0;
	auto file_name = get_file_name(0);
	read.open(file_name);
	int a, b;
	read >> a >> b;
	out << size <<  ' ' << b << '\n';
	for(int i = 0; i < size; i++) {
		read_write_sift_point(read, out);
		if(read.eof()) {
			read.close();
			file_number++;
			file_name = get_file_name(file_number);
			read.open(file_name, std::ifstream::in);
			if(!read.is_open()) std::cout << "bad file" << '\n';
			read >> a >> b;
		//	std::cout << a << ' ' << b << '\n';
			std::cout << file_name << '\n';
		}
	}
	out.close();
	read.close();
}

pk_tree construct_pk_tree(std::string file_name, int k, int imax, std::ofstream& out) {
	auto start = std::chrono::system_clock::now();
	pk_tree pk(file_name, k, imax);
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	out << pk.size() << ' ' << pk.get_dimension() << ' ' << elapsed_time.count() << '\n';
	return pk;
}

kd_tree construct_kd_tree(std::string file_name, std::ofstream &out) {
	auto start = std::chrono::system_clock::now();
	kd_tree kd(file_name);
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	out << kd.size() << ' ' << kd.get_dimension() << ' ' << elapsed_time.count() << '\n';
	return kd;
}

rkd_tree construct_rkd_register_time(std::string file_name, int tree, int k, std::ofstream& out) {
	auto start = std::chrono::system_clock::now();
	rkd_tree rk(file_name, tree, k);
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
  out << rk.size() << ' ' << rk.get_dimension() << ' ' << elapsed_time.count() << '\n';
	return rk;
}

/**
* test against sift features
*
*/
void test_construction_sift() {
	std::ofstream out_kd("kdct.txt", std::ios_base::app);
	std::ofstream out_rkd("rkdct.txt", std::ios_base::app);
	std::ofstream out_pk("pkct.txt", std::ios_base::app);

	std::string file_name = "sift_data.txt";
	int tree = 10;
	int k    = 10;
	unsigned int k_means = 4;
	int imax = 30;
	for(int i = 10; i < 10000; i += 10) {
	//	std::cout << i << std::endl;
		creat_sift_data_file(i);
		//auto kd = construct_kd_tree(file_name, out_kd);
		//auto rk = construct_rkd_register_time(file_name, tree, k, out_rkd);
		auto pk = construct_pk_tree(file_name, k_means, imax, out_pk);
		std::cout << pk.size() << ' ' << pk.get_dimension() << std::endl;
	}
}

int main() {
	//test2();
	test_construction_sift();
	return 0;
}
