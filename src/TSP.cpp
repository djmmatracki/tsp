#include "TSP.hpp"

#include <algorithm>
#include <stack>
#include <optional>
#include <iostream>
#include <vector>
#include <map>

std::ostream &operator<<(std::ostream &os, const CostMatrix &cm)
{
    for (std::size_t r = 0; r < cm.size(); ++r)
    {
        for (std::size_t c = 0; c < cm.size(); ++c)
        {
            const auto &elem = cm[r][c];
            os << (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path()
{
    // Get two last nodes
    reduce_cost_matrix();
    cost_matrix_t matrix = get_matrix().get_matrix();
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix[i].size(); ++j)
        {
            if (matrix[i][j] == 0)
            {
                unsorted_path_.push_back(vertex_t(i, j));
            }
        }
    }
    // Sort unsorted path
    std::map<size_t, size_t> m;
    size_t second;

    for (auto el : unsorted_path_)
    {
        m.insert(std::pair<size_t, size_t>(el.row, el.col));
        if (el.row == 0)
        {
            second = el.col;
        }
    }
    path_t result({0, second});

    while (m[second] != 0)
    {
        result.push_back(m[second] + 1);
        second = m[second];
    }
    result[0] += 1;
    result[1] += 1;

    return result;
}

/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const
{
    std::vector<cost_t> result;
    for (auto v : matrix_)
    {
        result.push_back(*std::min_element(v.begin(), v.end()));
    };
    return result;
}

/**
 * Reduce rows so that in each row at least one zero value is present.
 * @return Sum of values reduced in rows.
 */
cost_t CostMatrix::reduce_rows()
{
    const std::vector<cost_t> min_values = get_min_values_in_rows();
    for (size_t j = 0; j < matrix_.size(); ++j)
    {
        for (int &el : matrix_[j])
        {
            if (el != INF)
            {
                el -= min_values[j];
            }
        }
    }
    return std::accumulate(min_values.cbegin(), min_values.cend(), 0);
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_cols() const
{
    std::vector<cost_t> min_values(matrix_.size(), INF);
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_[i].size(); ++j)
        {
            if (matrix_[j][i] < min_values[i])
            {
                min_values[i] = matrix_[j][i];
            }
        }
    };
    return min_values;
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols()
{
    const std::vector<cost_t> min_values = get_min_values_in_cols();
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_[i].size(); ++j)
        {
            if (matrix_[j][i] != INF)
            {
                matrix_[j][i] -= min_values[i];
            }
        }
    }
    return std::accumulate(min_values.cbegin(), min_values.cend(), 0);
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const
{
    cost_t min_row_val = INF;
    cost_t min_col_val = INF;
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        if (i != row)
        {
            if (matrix_[i][col] < min_col_val)
            {
                min_col_val = matrix_[i][col];
            }
        }
    }

    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        if (i != col)
        {
            if (matrix_[row][i] < min_row_val)
            {
                min_row_val = matrix_[row][i];
            }
        }
    }
    return min_row_val + min_col_val;
}

/* PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it.
 * @param cm
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex()
{
    CostMatrix matrix = get_matrix();
    cost_matrix_t m = matrix.get_matrix();
    std::pair<vertex_t, cost_t> result({0, 0}, -INF);
    // New Vertex - cords, cost
    for (size_t i = 0; i < m.size(); ++i)
    {
        for (size_t j = 0; j < m.size(); ++j)
        {
            // std::cout << m[i][j] << " ";
            if (m[i][j] == 0)
            {
                cost_t vert_cost = matrix.get_vertex_cost(i, j);
                if (vert_cost > result.second)
                {
                    result = std::pair<vertex_t, cost_t>(vertex_t(i, j), vert_cost);
                }
            }
        }
        // std::cout << std::endl;
    }
    return NewVertex(vertex_t(result.first.row, result.first.col), result.second);
}

std::pair<size_t, size_t> get_begin_end_from_path(unsorted_path_t unsorted_path_)
{
    std::vector<size_t> begins({});
    std::vector<size_t> ends({});
    // std::cout << "Start function!" << std::endl;

    // Wyznaczamy poczatek i koniec sciezki
    for (auto vert : unsorted_path_)
    {
        // Sprawdzamy czy poczatek iterowanego wektora jest w koncach
        auto res1 = std::find(ends.cbegin(), ends.cend(), vert.row);

        // Jezeli poczatek jest w koncach usuwamy go
        if (res1 != ends.end())
        {
            ends.erase(res1);
        }
        else
        {
            begins.push_back(vert.row);
        }
        // Sprawdzamy czy koniec iterowanego wektora jest w poczatkach
        auto res2 = std::find(begins.cbegin(), begins.cend(), vert.col);

        // Jezeli koniec jest w poczatkach usuwamy go
        if (res2 != begins.end())
        {
            begins.erase(res2);
        }
        else
        {
            ends.push_back(vert.col);
        }
        // std::cout << "End of loop!" << std::endl;
    }
    // std::cout << "Begin: " << begins[0] << std::endl;
    // std::cout << "End: " << ends[0] << std::endl;
    // std::cout << "Begin size: " << begins.size() << std::endl;
    // std::cout << "End size: " << ends.size() << std::endl;
    std::pair<size_t, size_t> res(begins[0], ends[0]);
    return res;
}

/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex)
{
    cost_matrix_t matrix = matrix_.get_matrix();
    // Blokujemy przejescie przez odpowiednia kolumne i wiersz
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        matrix[i][new_vertex.col] = INF;
        matrix[new_vertex.row][i] = INF;
    }

    std::pair<size_t, size_t> result = get_begin_end_from_path(unsorted_path_);

    // [(0, 2), (3, 0)] -> 3, 2
    size_t begin = result.first;
    size_t end = result.second;

    // Jezeli poczatek nowego wektora to koniec naszego path'a to znaczy ze dodajemy go na koniec.
    // Czyli usuwamy przejscie z konca naszego nowego wektora na poczatek path'a
    if (new_vertex.row == end)
    {
        // Dodajemy na koniec
        matrix[new_vertex.col][begin] = INF;
    }
    else if (new_vertex.col == begin)
    {
        // Dodajemy na poczatek
        matrix[end][new_vertex.row] = INF;
    }
    matrix[new_vertex.col][new_vertex.row] = INF;
    matrix[end][begin] = INF;
    // Update Cost Matrix
    matrix_ = CostMatrix(matrix);
}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix()
{
    return matrix_.reduce_cols() + matrix_.reduce_rows();
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
cost_t get_optimal_cost(const path_t &optimal_path, const cost_matrix_t &m)
{
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx)
    {
        cost += m[optimal_path[idx - 1] - 1][optimal_path[idx] - 1];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path[optimal_path.size() - 1] - 1][optimal_path[0] - 1];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb)
{
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
tsp_solutions_t filter_solutions(tsp_solutions_t solutions)
{
    cost_t optimal_cost = INF;
    for (const auto &s : solutions)
    {
        optimal_cost = (s.lower_bound < optimal_cost) ? s.lower_bound : optimal_cost;
    }

    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t &s)
                 { return s.lower_bound == optimal_cost; });

    return optimal_solutions;
}

/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t &cm)
{

    StageState left_branch(cm);

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch); // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty())
    {

        left_branch = tree_lifo.top();
        tree_lifo.pop();

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb)
        {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0)
            {
                left_branch.reset_lower_bound();
            }

            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = left_branch.reduce_cost_matrix();

            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb)
            {
                break;
            }

            // 3. Get new vertex and the cost of not choosing it.

            NewVertex new_vertex = left_branch.choose_new_vertex();

            // 4. @TODO Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);

            // std::cout << new_vertex.coordinates.row << " " << new_vertex.coordinates.col << std::endl;
            // 5. @TODO (KROK 3) Update the cost matrix of the left branch.
            left_branch.update_cost_matrix(new_vertex.coordinates);

            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb)
        {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();
            path_t new_path = left_branch.get_path();
            // for (auto el : new_path)
            // {
            //     std::cout << el << std::endl;
            // }
            solutions.push_back({get_optimal_cost(new_path, cm), new_path});
        }
    }

    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
