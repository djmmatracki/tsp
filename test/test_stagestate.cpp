#include "gtest/gtest.h"
#include "TSP.hpp"
#include <vector>

TEST(StageStateTest, reduce_cost_matrix)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 10, 8, 19, 12};
    std::vector<int> v2{10, INF, 20, 6, 3};
    std::vector<int> v3{8, 20, INF, 4, 2};
    std::vector<int> v4{19, 6, 4, INF, 7};
    std::vector<int> v5{12, 3, 2, 7, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    // Declare Stage State
    StageState stage(matrix);

    // Check if reduced value is correct
    cost_t reduced_cost = stage.reduce_cost_matrix();
    ASSERT_EQ(reduced_cost, 28);

    // Get reduced matrix
    cost_matrix_t new_matrix = stage.get_matrix().get_matrix();

    // Check if reduced matrix is correct
    std::vector<int> v1_new{INF, 1, 0, 9, 4};
    std::vector<int> v2_new{1, INF, 17, 1, 0};
    std::vector<int> v3_new{0, 17, INF, 0, 0};
    std::vector<int> v4_new{9, 1, 0, INF, 3};
    std::vector<int> v5_new{4, 0, 0, 3, INF};

    ASSERT_EQ(new_matrix[0], v1_new);
    ASSERT_EQ(new_matrix[1], v2_new);
    ASSERT_EQ(new_matrix[2], v3_new);
    ASSERT_EQ(new_matrix[3], v4_new);
    ASSERT_EQ(new_matrix[4], v5_new);
}

TEST(StageStateTest, choose_new_vertex)
{
    // Deklaracja macierzy
    cost_matrix_t cm = {{INF, 10, 8, 19, 12},
                        {10, INF, 20, 6, 3},
                        {8, 20, INF, 4, 2},
                        {19, 6, 4, INF, 7},
                        {12, 3, 2, 7, INF}};

    CostMatrix matrix(cm);

    // Declare Stage State
    StageState stage(matrix);
    stage.reduce_cost_matrix();

    // Check if cost of NewVertex is correct
    NewVertex vert = stage.choose_new_vertex();
    ASSERT_EQ(vert.cost, 1);
    // std::cout << vert.coordinates.row << " " << vert.coordinates.col << std::endl;
}

TEST(StageStateTest, update_cost_matrix)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, INF, INF, INF, INF};
    std::vector<int> v2{0, INF, INF, 1, 0};
    std::vector<int> v3{INF, 17, INF, 0, 0};
    std::vector<int> v4{7, 0, INF, INF, 2};
    std::vector<int> v5{3, 0, INF, 3, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    // Declare Stage State
    StageState stage(matrix, {vertex_t(0, 2)});

    // Update Cost Matrix
    stage.update_cost_matrix(vertex_t(1, 0));

    // Get Cost Matrix
    cost_matrix_t new_matrix = stage.get_matrix().get_matrix();

    // Assert new matrix
    std::vector<int> v1_new{INF, INF, INF, INF, INF};
    std::vector<int> v2_new{INF, INF, INF, INF, INF};
    std::vector<int> v3_new{INF, INF, INF, 0, 0};
    std::vector<int> v4_new{INF, 0, INF, INF, 2};
    std::vector<int> v5_new{INF, 0, INF, 3, INF};

    ASSERT_EQ(new_matrix[0], v1_new);
    ASSERT_EQ(new_matrix[1], v2_new);
    ASSERT_EQ(new_matrix[2], v3_new);
    ASSERT_EQ(new_matrix[3], v4_new);
    ASSERT_EQ(new_matrix[4], v5_new);
}

TEST(StageStateTest, get_path)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, INF, INF, INF, INF};
    std::vector<int> v2{INF, INF, INF, INF, INF};
    std::vector<int> v3{INF, INF, INF, INF, INF};
    std::vector<int> v4{INF, INF, INF, INF, 2};
    std::vector<int> v5{INF, 0, INF, INF, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    // Declare Stage State
    StageState stage(matrix, {vertex_t(0, 2)});

    // Update Cost Matrix
    // path_t p = stage.get_path();
}