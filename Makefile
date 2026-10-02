CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -Iinclude
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests

SRCS = $(SRC_DIR)/Graph.cpp \
       $(SRC_DIR)/LearnerProfile.cpp \
       $(SRC_DIR)/OptimizationEngine.cpp \
       $(SRC_DIR)/PathFinder.cpp \
       $(SRC_DIR)/AssessmentEngine.cpp \
       $(SRC_DIR)/DatabaseManager.cpp \
       $(SRC_DIR)/OSConcurrencySimulator.cpp \
       $(SRC_DIR)/AnalyticsEngine.cpp \
       $(SRC_DIR)/TerminalUI.cpp

MAIN_TARGET = $(BIN_DIR)/pbl_optimizer.exe

.PHONY: all clean test benchmark run web

all: $(MAIN_TARGET) test_binaries

$(MAIN_TARGET): $(SRCS) $(SRC_DIR)/main.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

test_binaries: $(BIN_DIR)/test_dijkstra.exe $(BIN_DIR)/test_topological.exe $(BIN_DIR)/test_optimization.exe $(BIN_DIR)/benchmark_comparison.exe

$(BIN_DIR)/test_dijkstra.exe: $(SRC_DIR)/Graph.cpp $(SRC_DIR)/LearnerProfile.cpp $(SRC_DIR)/OptimizationEngine.cpp $(SRC_DIR)/PathFinder.cpp $(TEST_DIR)/test_dijkstra.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/test_topological.exe: $(SRC_DIR)/Graph.cpp $(TEST_DIR)/test_topological.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/test_optimization.exe: $(SRC_DIR)/Graph.cpp $(SRC_DIR)/LearnerProfile.cpp $(SRC_DIR)/OptimizationEngine.cpp $(SRC_DIR)/PathFinder.cpp $(TEST_DIR)/test_optimization.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/benchmark_comparison.exe: $(SRC_DIR)/Graph.cpp $(SRC_DIR)/LearnerProfile.cpp $(SRC_DIR)/OptimizationEngine.cpp $(SRC_DIR)/PathFinder.cpp $(SRC_DIR)/AnalyticsEngine.cpp $(TEST_DIR)/benchmark_comparison.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: test_binaries
	$(BIN_DIR)/test_dijkstra.exe
	$(BIN_DIR)/test_topological.exe
	$(BIN_DIR)/test_optimization.exe

benchmark: $(BIN_DIR)/benchmark_comparison.exe
	$(BIN_DIR)/benchmark_comparison.exe

run: $(MAIN_TARGET)
	$(MAIN_TARGET)

demo: $(MAIN_TARGET)
	$(MAIN_TARGET) --demo

web:
	python web/server.py

clean:
	rm -rf $(BIN_DIR) *.o
