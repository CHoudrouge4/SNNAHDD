# SNNAHDD

Scalable Nearest Neighbor Algorithms for High Dimensional Data

The main goal of this project is to implement several Data Structures(Trees) that are efficient for Nearest Neighbour Search in higher dimensional data.
A detailed description of these Data Structures is presented in [This paper](https://github.com/CHoudrouge4/SNNAHDD/blob/master/06809191.pdf).



## Step Zero
This step is implementing **The k-d Tree**.

1. Current Status:
	* Constructing The Tree: **done**
	* Querying  The tree: **done**
	* Allowing The user to specify the distance function.
2. Testing:
	we started testing our algorithm by comparing it to the naive algorithm in order to show its correctness and to compare its efficiency.The testing is done on a random set of points with different dimensionality chosen from a uniform distribution.In addition, we computed the construction time for the tree with respect to the number of points and to space's dimension. (for more details check main.cpp)

Graphs for some results:
Construction time(in seconds) with respect to the dimensionality:
![Construction time with respect to the dimensionality](./imges/const_dim.png)
Construction time(in seconds) with respect to the number of points with fixed dimension equals to two:
![](./imges/const-pts.png)

Comment on the results: k-d tree performs very well in low dimensions. This result is obvious from the first and the second graph. However, the performance of the k-d tree decreases drastically when the dimension is high. The third and the fourth graph demonstrate this fact. This will lead us to start implementing step one and two to get a better performance in higher dimensions.

Comparison between kd-tree search algorithm (in green) and the naive algorithm (in red).
For dimension equals 2 and number of points ranges between 10 and 100000.(time in seconds/number of points)
![](./imges/100000_2.png)

For dimension equals 3 and number of points ranges between 10 and 10000.(time in seconds/number of points)
![](./imges/10000_3.png)

For dimension equals 15 and number of points equals 10000.(time in seconds/number of points)
![](./imges/10000_15.png)

For dimension equals 15 and number of points equals 100000.(time in seconds/number of points)
![](./imges/100000_15.png)





## Step One
This step is implementing **The Randomized k-d Tree.**

1. Current Status:
	* Constructing The Tree: **done**
	* Querying  The tree: **Need Improvement**
2. Testing:
	Similarly to the k-d tree, we tested the randomized k-d tree algorithm by comparing it to the naive algorithm in order to show its correctness and to compare its efficiency. the testing is done over a random set of points chosen from a uniform distribution.
	We computed the construction time for a randomized k-d forest of size 20 over a set of points of dimension equals to 25.
	Furthermore, we tested the search algorithm for different forest size in a fixed dimension, and we computed the precision for each size.

Graphs for some results:
Construction time(in seconds) for 20 trees with respect to the number of points(with dimension equals to 25).
![](./imges/rconst10000_25_20.png)

Construction time(in seconds) for 20 trees with respect to the dimension (with 1000 points for each dimension).
![](./imges/dim1000.png)

Search time(in seconds) for 15 trees with respect to the number of points(with dimension equals to 10). The search precision is equal to 81.5% (the number of explored leafs is 17).
![](./imges/test1/15_trees_7_leafs_10000_81.5_.png)

Search time(in seconds) for 20 trees with respect to the number of points(with dimension equals to 10). The search precision is equal to 73.62% (the number of explored leafs is 15).
![](./imges/test1/20_trees_15_leafs_10000_73.62_.png)

Search time(in seconds) for 25 trees with respect to the number of points(with dimension equals to 10). The search precision is equal to 74.38% (the number of explored leafs is 15).
![](./imges/test1/25_trees_15_leafs_10000_74.38_.png)

Search time(in seconds) for 25 trees with respect to the number of points(with dimension equals to 10). The search precision is equal to 92.99% (the number of explored leafs is 20).
![](./imges/test1/25_trees_20_leafs_10000_92_99_.png)

In addition, we tested the relation between the number of trees/number of leafs and the precision/performance.

In all of these tests, the number of points (space size) is equal to 10000, the query size equals to 100, and the dimension equals to 10.
For the performance/number of trees and precision/number of trees, the number of leafs was fixed to 16.
For the performance/number of leafs and precision/number of leafs, the number of trees was fixed to 20.

The following graph represents the change of the performance with respect to the number of leafs.
![](./imges/leaf_trees/performance_number_of_leafs.png)

The following graph represents the change of the performance with respect to the number of trees;
![](./imges/leaf_trees/performance_number_of_trees.png)

The following graph represents the change of precision with respect to the number of leafs.
![](./imges/leaf_trees/precision_per_leaf.png)

The following graph represents the change of precision with respect to the number of trees.
![](./imges/leaf_trees/precision_trees.png)

Now, we ll present the performance and the precision with respect to number of visited leafs on the same graph.
![](./imges/test1/precision_performance_leafs.png)

and the following one combines the performance and the precision with respect to number of trees.
![](./imges/test1/precision_performance_trees.png)

## Step Two
This Step is implementing **The Priority Search K-Means Tree**

## Step Three
TBA.
