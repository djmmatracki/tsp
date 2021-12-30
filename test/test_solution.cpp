#include "gtest/gtest.h"
#include "TSP.hpp"

TEST(Solution, test1)
{
    cost_matrix_t cm = {{INF, 10, 8, 19, 12},
                        {10, INF, 20, 6, 3},
                        {8, 20, INF, 4, 2},
                        {19, 6, 4, INF, 7},
                        {12, 3, 2, 7, INF}};

    tsp_solutions_t solutions = solve_tsp(cm);
    path_t solution({1, 3, 4, 5, 2});

    ASSERT_EQ(solutions[0].lower_bound, 32);
    ASSERT_EQ(solutions[0].path, solution);
    std::cout << "Tests passed 1!" << std::endl;
}

TEST(Solution, test2)
{
    cost_matrix_t cm = {{INF, 12, 3, 45, 6},
                        {78, INF, 90, 21, 3},
                        {5, 56, INF, 23, 98},
                        {12, 6, 8, INF, 34},
                        {3, 98, 3, 2, INF}};

    tsp_solutions_t solutions = solve_tsp(cm);
    path_t solution({5, 4, 3, 1, 2});

    ASSERT_EQ(solutions[0].lower_bound, 30);
    ASSERT_EQ(solutions[0].path, solution);
    std::cout << "Tests passed 2!" << std::endl;
}