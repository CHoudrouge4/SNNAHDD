#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kd_tree.h"
#include <cmath>
#include <random>
#include <limits>
#include <chrono>

std::vector<point> generate_data() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-1000, 1000);

	std::ofstream out("data.txt");

	int num = std::abs(dis(gen));
	out << num << ' ' << 4 << '\n';
	for(int i = 0; i < num; i++) {
		int x = dis(gen);
		int y = dis(gen);
		int z = dis(gen);
		int a = dis(gen);
		out << x << ' ' << y << ' ' << z << ' ' << a << '\n';
	}
	out.close();

	std::ofstream q("query.txt");
	std::uniform_int_distribution<> d(0, num);
	int size = d(gen);
	std::cout << size << std::endl;
	q << size << '\n';
	std::vector<point> Q(size);
	for(size_t i = 0; i < Q.size(); ++i) {
		int x = dis(gen);
		int y = dis(gen);
		int z = dis(gen);
		int a = dis(gen);
		q <<  x << ' ' << y << ' ' << z << ' ' << a << '\n';
		Q[i] = {(double) x, (double)y, (double)z};
	}	
	q.close();
	return Q;
}


double dist(const point p, const point q) {
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

int main() {

	std::ofstream in1("kd_result.txt" , std::ios_base::app);
	std::ofstream in2("naive.txt"	  , std::ios_base::app);
	std::ofstream in3("const_time.txt", std::ios_base::app);

	std::vector<point> query = generate_data();
	
	auto start = std::chrono::system_clock::now();
	kd_tree k("data.txt");
	auto end   = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;

	in3 << k.size() << ' ' << k.get_dimension() << ' ' << elapsed_time.count() << '\n';

	start = std::chrono::system_clock::now();
	std::vector<int> result = k.search(query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in1 << query.size() <<  ' ' << k.get_dimension() << ' ' << elapsed_time.count() << '\n';
		
   	const std::vector<point> p = k.get_points();

	start = std::chrono::system_clock::now();
	std::vector<int> result_naive = naive(p, query);
	end   = std::chrono::system_clock::now();
	elapsed_time = end - start;
	in2 << query.size() << ' ' <<  k.get_dimension() <<' ' << elapsed_time.count() << '\n';

	for(size_t i = 0; i < result.size(); ++i) {
		if(result[i] == result_naive[i])
			std::cout << result[i] << " == " << result_naive[i] <<  " [OK] " << '\n';
		else
			std::cout << "[FAILED]\n" << '\n';
	}
	
	in1.close();
	in2.close();
	in3.close();
	return 0;
}
