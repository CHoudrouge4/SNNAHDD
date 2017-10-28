#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "kd_tree.h"

int main() {
//	std::cout << "hello kd_tree!" << '\n';
	kd_tree k("data.txt");
	std::vector<point> query = {{1.5, 1.5}, {100.0, 2.5}};
	std::vector<std::vector<int>> result = k.search(query); 
	for(auto&& v: result) {
		for(auto&& e : v) {
			std::cout << e << ' ';
		}
		std::cout << '\n';
	}

	return 0;
}
