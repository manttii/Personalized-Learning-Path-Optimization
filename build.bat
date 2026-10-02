@echo off
echo ==============================================================================
echo   Building Personalized Learning Path Optimization Engine (PBL-2)
echo ==============================================================================

if not exist bin mkdir bin
if not exist database mkdir database

echo [*] Compiling C++ Engine Sources with g++ (C++14/17)...
g++ -std=c++14 -O2 -Iinclude ^
    src/Graph.cpp ^
    src/LearnerProfile.cpp ^
    src/OptimizationEngine.cpp ^
    src/PathFinder.cpp ^
    src/AssessmentEngine.cpp ^
    src/DatabaseManager.cpp ^
    src/OSConcurrencySimulator.cpp ^
    src/AnalyticsEngine.cpp ^
    src/TerminalUI.cpp ^
    src/main.cpp ^
    -o bin/pbl_optimizer.exe

if %ERRORLEVEL% NEQ 0 (
    echo [!] Compilation FAILED!
    exit /b %ERRORLEVEL%
)

echo [*] Compiling Unit Tests and Benchmark Suites...
g++ -std=c++14 -O2 -Iinclude src/Graph.cpp src/LearnerProfile.cpp src/OptimizationEngine.cpp src/PathFinder.cpp tests/test_dijkstra.cpp -o bin/test_dijkstra.exe
g++ -std=c++14 -O2 -Iinclude src/Graph.cpp tests/test_topological.cpp -o bin/test_topological.exe
g++ -std=c++14 -O2 -Iinclude src/Graph.cpp src/LearnerProfile.cpp src/OptimizationEngine.cpp src/PathFinder.cpp tests/test_optimization.cpp -o bin/test_optimization.exe
g++ -std=c++14 -O2 -Iinclude src/Graph.cpp src/LearnerProfile.cpp src/OptimizationEngine.cpp src/PathFinder.cpp src/AnalyticsEngine.cpp tests/benchmark_comparison.cpp -o bin/benchmark_comparison.exe

echo ==============================================================================
echo   [✓] BUILD SUCCESSFUL! Executable generated at bin/pbl_optimizer.exe
echo   Run "run.bat" to start the interactive application.
echo ==============================================================================
