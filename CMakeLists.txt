cmake_minimum_required(VERSION 3.12)
project(implementation)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include
)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

set(SOURCE_FILES
        src/tsp_setup.cpp
        src/TSP.cpp
        )

set(SOURCE_FILES_TESTS
        test/test_solution.cpp
        test/test_costmatrix.cpp
        test/test_stagestate.cpp
        )

add_executable(tsp ${SOURCE_FILES} main.cpp)

set(EXEC_TEST test)
add_executable(test ${SOURCE_FILES} ${SOURCE_FILES_TESTS} test/main_gtest.cpp)

# == Uwzględnij pliki frameworku Google Testing Framework (GTF) ==

# Przyjmij, że główny katalog z plikami frameworku GTF znajduje się
# dwa katalogi wyżej względem katalogu projektu.
set(GTEST_ROOT ../../googletest-master)

# Dodaj katalogi z plikami nagłówkowymi GTF.
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )

add_subdirectory(${GTEST_ROOT} googletest-master)

# Dołącz bibliotekę Google Mock.
target_link_libraries(${EXEC_TEST} gmock)