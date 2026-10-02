#ifndef OS_CONCURRENCY_SIMULATOR_HPP
#define OS_CONCURRENCY_SIMULATOR_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include <string>
#include <vector>
#include <chrono>

/**
 * @brief Benchmark results from concurrent multi-threaded execution.
 */
struct ConcurrencyBenchmarkResult {
    int totalStudents;
    int threadCount;
    double totalExecutionTimeMs;
    double throughputOpsPerSec;
    double averageLatencyPerStudentMs;
    int successfulRecalculations;
    int lockContentionEvents;
};

/**
 * @brief Concurrent Processing Engine for Multiple Active Learners.
 * 
 * Integrates Operating Systems concepts (Multithreading, Mutex Synchronization,
 * Critical Sections, and Throughput Benchmarking) into the PBL project.
 */
class OSConcurrencySimulator {
public:
    static ConcurrencyBenchmarkResult runConcurrentEvaluationSimulation(
        const KnowledgeGraph& graph,
        int numStudents = 50,
        int numThreads = 4
    );
};

#endif // OS_CONCURRENCY_SIMULATOR_HPP
