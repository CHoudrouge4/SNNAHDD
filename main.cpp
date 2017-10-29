#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kd_tree.h"
#include <cmath>
#include <random>

std::vector<point> generate_data() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-100, 100);

	std::ofstream out("data.txt");

	int num = std::abs(dis(gen));
	out << num << ' ' << 2 << '\n';
	for(int i = 0; i < num; i++) {
		int x = dis(gen);
		int y = dis(gen);
		out << x << ' ' << y << '\n';
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
		q <<  x << ' ' << y;
		Q[i] = {(double) x, (double)y};
	}	
	q.close();
	return Q;
}


int main() {
//	std::cout << "hello kd_tree!" << '\n';
//	std::cout << k << '\n';
//	std::vector<point> query = generate_data();
	kd_tree k("data.txt");
//	std::cout << k << '\n';
//	std::cout << "Start Searching " << std::endl;
//	std::vector<int> result = k.search(query); 
//	std::cout << "End Searching " << std::endl;
//	for(auto&& v: result) {
//		std::cout << v << ": " << k.get_point(v) << ' ';
//		std::cout << '\n';
//	}

	return 0;
}
