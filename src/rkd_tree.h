#include "node.h"
#include "tree.h"
#include <vector>
#include <random>
#include <memory>
#include <set>
#include <thread>
#include <mutex>

class rkd_tree : public tree {

friend std::ostream &operator<<(std::ostream &out, rkd_tree &k) {
	out << k.print();
	return out;
}

private:
	int limit;
	static int ND;
	std::vector<std::shared_ptr<node>> roots;
	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_int_distribution<> dis;
	int tree_num = 20;
	rkd_tree() {}

	int    split_dimension(std::shared_ptr<node> &current);
	void construct(std::shared_ptr<node> &current, int index);
	void construct(const std::string file_name);
	//void read_point(const std::string file_name);

	std::string print(std::shared_ptr<node> &current);
	void explore(std::shared_ptr<node> &current, const point &q, double &R, int &res, double pmed, int &count,
						std::set<std::pair<double, int>> &pq, int m, std::vector<bool> &visited);

//	void explore_best(std::shared_ptr<node> &current, const point &q, double &R, int &res, std::set<std::pair<double, int>> &pq, int m, int rp);
	int  search(const point &q);
	void search(std::shared_ptr<node> &current, const point &q, double &R, int &res, std::set<std::pair<double, int>> pq, int m,
	int& count, int rp, std::vector<bool> &visited);
	std::string print();

public:

	rkd_tree(const std::string file_name);
	rkd_tree(const std::string file_name, double (*)(point &a, point &b));
	rkd_tree(const std::string file_name, int n, int l);
	rkd_tree(const std::string file_name, int n, double (*)(point &a, point &b));

	std::vector<int> search(const std::vector<point> &query);
};
