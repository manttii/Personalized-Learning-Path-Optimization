#include "OSConcurrencySimulator.hpp"
#include "PathFinder.hpp"
#include "OptimizationEngine.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

ConcurrencyBenchmarkResult OSConcurrencySimulator::runConcurrentEvaluationSimulation(
    const KnowledgeGraph& graph,
    int numStudents,
    int numThreads
) {
    ConcurrencyBenchmarkResult result;
    result.totalStudents = numStudents;
    result.threadCount = numThreads;
    result.lockContentionEvents = 0;
    result.successfulRecalculations = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    // Create synthetic student pool
    std::vector<LearnerProfile> studentPool;
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> scoreDist(0.2, 0.95);
    std::uniform_int_distribution<int> goalDist(8, 15);

    for (int i = 0; i < numStudents; ++i) {
        std::string sid = "25100" + std::to_string(1000 + i);
        std::string sname = "Learner_" + std::to_string(i + 1);
        int gVal = goalDist(rng);
        std::string goal = std::string("T") + (gVal < 10 ? "0" : "") + std::to_string(gVal);
        LearnerProfile p(sid, sname, sid + "@geu.ac.in", goal);

        // Assign randomized prior knowledge vector
        for (int t = 1; t <= 15; ++t) {
            std::string tid = "T" + (t < 10 ? std::string("0") : std::string("")) + std::to_string(t);
            double score = scoreDist(rng);
            p.setProficiency(tid, score);
            if (score >= 0.70) p.setMastered(tid, true);
        }
        studentPool.push_back(p);
    }

    // Process all student adaptive trajectories through multi-stage Dijkstra evaluations
    DijkstraPathFinder pathFinder;
    double totalLatencyAccum = 0.0;

    for (int i = 0; i < numStudents; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        
        double hours = 0.0;
        std::vector<std::string> path = pathFinder.computePath(graph, studentPool[i], "T01", studentPool[i].getTargetGoal(), hours);
        studentPool[i].setCurrentPath(path);
        
        // Simulate dynamic evaluation event and reroute
        OptimizationEngine::evaluateAndReroute(graph, studentPool[i], "T05", 0.45); // simulate recursion struggle
        
        auto t1 = std::chrono::high_resolution_clock::now();
        double latencyMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
        totalLatencyAccum += latencyMs;
        result.successfulRecalculations++;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.totalExecutionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    result.throughputOpsPerSec = (result.totalStudents / (result.totalExecutionTimeMs / 1000.0));
    result.averageLatencyPerStudentMs = totalLatencyAccum / result.totalStudents;

    return result;
}
