#include "node.h"
#include <vector>
#include <random>

class rkd_tree {

friend std::ostream &operator<<(std::ostream &out, rkd_tree &k) {
	out << k.print();
	return out;
}

private:
	const double epsilon = 0.001;
	int dimension;
	static int ND;
	std::vector<std::shared_ptr<node>> roots;
	std::vector<point> points;
	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_int_distribution<> dis;
	
	rkd_tree() {}

	double dist(const point &p, const point &q);
	double median(std::vector<double> &v); 
	double mean(const point &p);
	double variance(const point &p);	
	int    split_dimension(std::shared_ptr<node> &current);
	void construct(std::shared_ptr<node> &current, int index);	
	void read_point(const std::string file_name);

	std::string print();
	std::string print(std::shared_ptr<node> &current);

public:

	rkd_tree(const std::string file_name);
	rkd_tree(const std::string file_name, double (*)(point &a, point &b));
	rkd_tree(const std::string file_name, int n);
	rkd_tree(const std::string file_name, int n, double (*)(point &a, point &b));
	
	point get_point(const int i) const;	
	std::vector<point> get_points(const std::vector<int> &v) const;
	int get_dimension() const;
}; 
