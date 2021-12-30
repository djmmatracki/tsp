#include "gtest/gtest.h"
#include "TSP.hpp"
#include <vector>

TEST(CostMatrixTest, get_min_values_in_rows)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 2, 8, 4};
    std::vector<int> v2{1, 1, 3, 4};
    std::vector<int> v3{3, 8, INF, 7};
    std::vector<int> v4{INF, INF, INF, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4};
    CostMatrix matrix(cost_matrix);

    // Sprawdzenie minimalnych wartosci w macierzy w wierszach
    std::vector<int> v = matrix.get_min_values_in_rows();
    ASSERT_EQ(v[0], 2);
    ASSERT_EQ(v[1], 1);
    ASSERT_EQ(v[2], 3);
    ASSERT_EQ(v[3], INF);
}

TEST(CostMatrixTest, get_min_values_in_cols)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 2, 8, INF};
    std::vector<int> v2{1, 1, 3, INF};
    std::vector<int> v3{3, 8, INF, INF};
    std::vector<int> v4{8, 3, INF, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4};
    CostMatrix matrix(cost_matrix);

    // Sprawdzenie minimalnych wartosci w macierzy w kolumnach
    std::vector<int> v = matrix.get_min_values_in_cols();
    ASSERT_EQ(v[0], 1);
    ASSERT_EQ(v[1], 1);
    ASSERT_EQ(v[2], 3);
    ASSERT_EQ(v[3], INF);
}

TEST(CostMatrixTest, reduce_rows)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 10, 8, 19, 12};
    std::vector<int> v2{10, INF, 20, 6, 3};
    std::vector<int> v3{8, 20, INF, 4, 2};
    std::vector<int> v4{19, 6, 4, INF, 7};
    std::vector<int> v5{12, 3, 2, 7, INF};
    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    cost_t v = matrix.reduce_rows();

    // Sprawdzenie lower bound (LB)
    ASSERT_EQ(v, 19);

    cost_matrix_t m = matrix.get_matrix();

    // Sprawdzenie każdej wartości zaktulizowanej macierzy
    std::vector<int> v1_new{INF, 2, 0, 11, 4};
    std::vector<int> v2_new{7, INF, 17, 3, 0};
    std::vector<int> v3_new{6, 18, INF, 2, 0};
    std::vector<int> v4_new{15, 2, 0, INF, 3};
    std::vector<int> v5_new{10, 1, 0, 5, INF};

    ASSERT_EQ(m[0], v1_new);
    ASSERT_EQ(m[1], v2_new);
    ASSERT_EQ(m[2], v3_new);
    ASSERT_EQ(m[3], v4_new);
    ASSERT_EQ(m[4], v5_new);
}

TEST(CostMatrixTest, reduce_cols)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 2, 0, 11, 4};
    std::vector<int> v2{7, INF, 17, 3, 0};
    std::vector<int> v3{6, 18, INF, 2, 0};
    std::vector<int> v4{15, 2, 0, INF, 3};
    std::vector<int> v5{10, 1, 0, 5, INF};

    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    cost_t v = matrix.reduce_cols();

    // Sprawdzenie lower bound (LB)
    ASSERT_EQ(v, 9);

    cost_matrix_t m = matrix.get_matrix();

    // Sprawdzenie każdej wartości zaktulizowanej macierzy
    std::vector<int> v1_new{INF, 1, 0, 9, 4};
    std::vector<int> v2_new{1, INF, 17, 1, 0};
    std::vector<int> v3_new{0, 17, INF, 0, 0};
    std::vector<int> v4_new{9, 1, 0, INF, 3};
    std::vector<int> v5_new{4, 0, 0, 3, INF};

    ASSERT_EQ(m[0], v1_new);
    ASSERT_EQ(m[1], v2_new);
    ASSERT_EQ(m[2], v3_new);
    ASSERT_EQ(m[3], v4_new);
    ASSERT_EQ(m[4], v5_new);
}

TEST(CostMatrixTest, get_vertex_cost)
{
    // Deklaracja macierzy
    std::vector<int> v1{INF, 1, 0, 9, 4};
    std::vector<int> v2{1, INF, 17, 1, 0};
    std::vector<int> v3{0, 17, INF, 0, 0};
    std::vector<int> v4{9, 1, 0, INF, 3};
    std::vector<int> v5{4, 0, 0, 3, INF};

    cost_matrix_t cost_matrix{v1, v2, v3, v4, v5};
    CostMatrix matrix(cost_matrix);

    ASSERT_EQ(1, matrix.get_vertex_cost(0, 2));
    ASSERT_EQ(1, matrix.get_vertex_cost(2, 0));
    ASSERT_EQ(0, matrix.get_vertex_cost(2, 4));
    ASSERT_EQ(1, matrix.get_vertex_cost(4, 1));
    ASSERT_EQ(1, matrix.get_vertex_cost(1, 4));
    ASSERT_EQ(1, matrix.get_vertex_cost(2, 3));
    ASSERT_EQ(1, matrix.get_vertex_cost(3, 2));
    ASSERT_EQ(0, matrix.get_vertex_cost(4, 2));
}