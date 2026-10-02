cmake_minimum_required(VERSION 3.10)
project(PersonalizedLearningPathOptimization CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIB_SOURCES
    src/Graph.cpp
    src/LearnerProfile.cpp
    src/OptimizationEngine.cpp
    src/PathFinder.cpp
    src/AssessmentEngine.cpp
    src/DatabaseManager.cpp
    src/OSConcurrencySimulator.cpp
    src/AnalyticsEngine.cpp
    src/TerminalUI.cpp
)

add_executable(pbl_optimizer src/main.cpp ${LIB_SOURCES})

add_executable(test_dijkstra tests/test_dijkstra.cpp ${LIB_SOURCES})
add_executable(test_topological tests/test_topological.cpp src/Graph.cpp)
add_executable(test_optimization tests/test_optimization.cpp ${LIB_SOURCES})
add_executable(benchmark_comparison tests/benchmark_comparison.cpp ${LIB_SOURCES})
