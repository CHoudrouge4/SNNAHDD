#include "node.h"
#include <vector>
#include <random>
#include <memory>
#include <set>
#include <thread>
#include <mutex>

class rkd_tree {

friend std::ostream &operator<<(std::ostream &out, rkd_tree &k) {
	out << k.print();
	return out;
}

private:
	const double epsilon = 0.001;
	int dimension;
	int limit;
	static int ND;
	std::vector<std::shared_ptr<node>> roots;
	std::vector<point> points;
	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_int_distribution<> dis;
	std::mutex mx;	

	rkd_tree() {}

	double dist(const point &p, const point &q);
	double median(std::vector<double> &v); 
	double mean(const point &p);
	double variance(const point &p);	
	int    split_dimension(std::shared_ptr<node> &current);
	void construct(std::shared_ptr<node> &current, int index);	
	void read_point(const std::string file_name);

	std::string print(std::shared_ptr<node> &current);
	void explore(std::shared_ptr<node> &current, const point &q, double &R, int &res, double pmed, int &count, std::set<std::pair<double, int>> &pq, std::vector<bool> &explored);		
	int search(const point &q);
	std::string print();

public:

	rkd_tree(const std::string file_name);
	rkd_tree(const std::string file_name, double (*)(point &a, point &b));
	rkd_tree(const std::string file_name, int n);
	rkd_tree(const std::string file_name, int n, double (*)(point &a, point &b));
	
	std::vector<int> search(const std::vector<point> &query);
	std::vector<point> get_points() const;
	point get_point(const int i) const;	
	std::vector<point> get_points(const std::vector<int> &v) const;
	int get_dimension() const;
	size_t size();

}; 
