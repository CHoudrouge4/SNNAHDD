#pragma once
#include <fstream>
#include <vector>
#include <memory>

typedef std::vector<double> point; 
template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	if(v.size() == 0) {
		out << "()";
		return out;
	} else  {
		out << '(';
		out << ' ' << v[0] ;
		for(size_t i = 1; i < v.size(); ++i)
			out << " , " << v[i];
		out << ')';
	return out;
	}
}

struct node {
	double r; 
	double median;
	int index; // the index that we computed the median with respect to it.
	std::vector<int> pts;
	std::shared_ptr<node> left;   // pointer to the left child
	std::shared_ptr<node> right;  // pointer to the right child
	std::shared_ptr<node> parent; // pointer to the parent
	friend std::ostream &operator<<(std::ostream &out, node& n) {
		out << "NODE"
			<< " M: " <<  n.median
			<< " i: " <<  n.index
		    << ' '    <<  n.pts;
		return out;
	}
};
