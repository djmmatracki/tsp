# Traveling Salesman Problem

Problem determines the following task: having a list of cities and the distance between each pair, which is the shortest path to visit all cities and return to the starting?


### Get closer to the problem

The TSP problem is considered a NP-hard problem, which means that there are no known algorithms with polynomial complexity to solve this problem. Therefore, usually only algorithms approximating the optimal solution are used. Nevertheless, for a small number of cities, it is possible to use methods capable of determining directly optimal routes. Such an algorithm is, for example, Little's algorithm.


### Algoritm steps

1. First, we reduce the values we find for each row minimum value and we subtract it from all elements in that row. Then we check if there is at least one 0 in each column and row, if not then we reduce the values in the columns. The sum of all the values we substracted is our lower bound.

2. Then we check all possible transitions: we select the pairs of vertices at the intersection of which in the table there is a value of 0. Then we calculate the sum of the minimum values in the designated row and column. We select the edge with the maximum of the determined values and add it to the solution we are looking for.

3. Now we need to remove the row and the column we have chosen from the cost matrix. We should also check if there is a possibility to create another cycle in the current solution.

4. Then we go back to point 1. We proceed the algorithm untill we get our path.