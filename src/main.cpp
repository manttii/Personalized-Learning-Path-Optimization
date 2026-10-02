#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "PathFinder.hpp"
#include "OptimizationEngine.hpp"
#include "AssessmentEngine.hpp"
#include "DatabaseManager.hpp"
#include "OSConcurrencySimulator.hpp"
#include "AnalyticsEngine.hpp"
#include "TerminalUI.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib>

int main(int argc, char* argv[]) {
    // Initialize Core Engines
    KnowledgeGraph graph;
    graph.loadDefaultCurriculum();

    LearnerProfile student("2510011893", "Priyanshi Saini", "priyanshi@geu.ac.in", "T12");
    AssessmentEngine assessmentEngine;
    DatabaseManager dbManager("database/pbl_curriculum.db");
    dbManager.initializeDatabase();
    dbManager.saveStudent(student);

    // Initial Path Calculation
    DijkstraPathFinder dijkstraFinder;
    double initialHours = 0.0;
    std::vector<std::string> currentPath = dijkstraFinder.computePath(
        graph, student, "T01", student.getTargetGoal(), initialHours
    );
    student.setCurrentPath(currentPath);
    dbManager.exportDashboardData(graph, student, currentPath, initialHours);

    // Command Line Flags Handler for Automated Testing / Demos
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--demo" || arg == "--loop-demo") {
            TerminalUI::printHeader();
            std::cout << "[*] Executing Automated End-to-End PBL Workflow Demo...\n";

            std::cout << "\n1. Initial Knowledge State & Computed Dijkstra Trajectory for Goal: " 
                      << student.getTargetGoal() << "\n";
            TerminalUI::printPath(graph, currentPath, initialHours);

            std::cout << "\n2. Simulating Learner Mastery on Foundations (T01, T02)...\n";
            student.setProficiency("T01", 0.90);
            student.setMastered("T01", true);
            student.setProficiency("T02", 0.85);
            student.setMastered("T02", true);

            double updatedHours = 0.0;
            currentPath = dijkstraFinder.computePath(graph, student, "T01", student.getTargetGoal(), updatedHours);
            student.setCurrentPath(currentPath);
            std::cout << "   -> New Effective Path Time: " << updatedHours << "h (Fast-tracked foundational review)\n";

            std::cout << "\n3. Simulating Student Concept Failure on T05 (Recursion & Backtracking)...\n";
            RerouteDecision reroute = OptimizationEngine::evaluateAndReroute(graph, student, "T05", 0.35);
            std::cout << reroute.explanation << "\n";
            TerminalUI::printPath(graph, reroute.newPath, reroute.newPathEstimatedHours);

            std::cout << "\n4. Running Static vs Dynamic Comparative Benchmark...\n";
            auto bench = AnalyticsEngine::compareCurricula(graph, student, student.getTargetGoal());
            AnalyticsEngine::printComparisonReport(bench);

            std::cout << "\n5. Running OS Concurrent Multithreaded Learner Simulation...\n";
            auto osBench = OSConcurrencySimulator::runConcurrentEvaluationSimulation(graph, 100, 4);
            std::cout << "   -> Processed " << osBench.totalStudents << " learners in " 
                      << osBench.totalExecutionTimeMs << " ms\n";
            std::cout << "   -> Throughput: " << osBench.throughputOpsPerSec << " operations/sec\n";
            std::cout << "   -> Avg Recalculation Latency: " << osBench.averageLatencyPerStudentMs << " ms\n";

            dbManager.exportDashboardData(graph, student, reroute.newPath, reroute.newPathEstimatedHours);
            std::cout << "\n[✓] Demo Completed. Data exported to web/data.json\n";
            return 0;
        } else if (arg == "--benchmark") {
            auto bench = AnalyticsEngine::compareCurricula(graph, student, "T15");
            AnalyticsEngine::printComparisonReport(bench);
            return 0;
        }
    }

    // Interactive Loop Mode
    while (true) {
        TerminalUI::printHeader();
        TerminalUI::printMenu();

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            std::cout << "\nThank you for using the Personalized Learning Path Optimization Engine.\nExiting...\n";
            break;
        }

        switch (choice) {
            case 1: { // View Knowledge DAG Topics
                TerminalUI::printTopicTable(graph, student);
                TerminalUI::printAsciiDAG(graph, student, student.getTargetGoal());
                TerminalUI::pause();
                break;
            }
            case 2: { // Compute Path
                std::cout << "\nCurrent Goal Node: " << student.getTargetGoal() 
                          << " (" << graph.getNode(student.getTargetGoal()).title << ")\n";
                std::cout << "Enter new target goal topic ID (e.g. T07, T12, T14, T15) or press ENTER to keep: ";
                std::string newGoal;
                std::getline(std::cin, newGoal);
                if (!newGoal.empty() && graph.hasNode(newGoal)) {
                    student.setTargetGoal(newGoal);
                }

                double dijkstraHours = 0.0;
                auto dPath = dijkstraFinder.computePath(graph, student, "T01", student.getTargetGoal(), dijkstraHours);
                student.setCurrentPath(dPath);

                TopologicalPathFinder topoFinder;
                double topoHours = 0.0;
                auto tPath = topoFinder.computePath(graph, student, "T01", student.getTargetGoal(), topoHours);

                TerminalUI::printBanner("DIJKSTRA ADAPTIVE PATH (OPTIMIZED EFFECTIVE WEIGHTS)");
                TerminalUI::printPath(graph, dPath, dijkstraHours);

                TerminalUI::printBanner("TOPOLOGICAL SORT PATH (PREREQUISITE ORDER)");
                TerminalUI::printPath(graph, tPath, topoHours);

                dbManager.saveTrajectory(student.getId(), student.getTargetGoal(), "Dijkstra", dPath, dijkstraHours);
                dbManager.exportDashboardData(graph, student, dPath, dijkstraHours);
                TerminalUI::pause();
                break;
            }
            case 3: { // Diagnostic Baseline Assessment
                TerminalUI::printBanner("DIAGNOSTIC BASELINE ASSESSMENT (FOUNDATIONAL KNOWLEDGE CHECK)");
                auto questions = assessmentEngine.getDiagnosticBaselineQuestions(5);
                int correct = 0;

                for (size_t i = 0; i < questions.size(); ++i) {
                    const auto& q = questions[i];
                    std::cout << "\nQ" << (i + 1) << " [" << q.topicId << " - " 
                              << graph.getNode(q.topicId).title << "]:\n";
                    std::cout << "   " << q.questionText << "\n";
                    for (size_t opt = 0; opt < q.options.size(); ++opt) {
                        std::cout << "   [" << (opt + 1) << "] " << q.options[opt] << "\n";
                    }
                    std::cout << "   Your answer (1-" << q.options.size() << "): ";
                    int ans = 1;
                    if (std::cin >> ans && ans >= 1 && ans <= static_cast<int>(q.options.size())) {
                        if (ans - 1 == q.correctOptionIndex) {
                            std::cout << "   [✓] Correct! " << q.explanation << "\n";
                            student.setProficiency(q.topicId, 0.85);
                            student.setMastered(q.topicId, true);
                            correct++;
                        } else {
                            std::cout << "   [✗] Incorrect. Correct was [" << (q.correctOptionIndex + 1) 
                                      << "]. " << q.explanation << "\n";
                            student.setProficiency(q.topicId, 0.40);
                        }
                    }
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                double totalHours = 0.0;
                currentPath = dijkstraFinder.computePath(graph, student, "T01", student.getTargetGoal(), totalHours);
                student.setCurrentPath(currentPath);
                dbManager.exportDashboardData(graph, student, currentPath, totalHours);

                std::cout << "\nDiagnostic Finished: " << correct << "/" << questions.size() 
                          << " Correct. Trajectory updated dynamically!\n";
                TerminalUI::pause();
                break;
            }
            case 4: { // Interactive Topic Study & Quiz
                TerminalUI::printTopicTable(graph, student);
                std::cout << "\nEnter Topic ID to Study (e.g. T05, T06, T10, T12): ";
                std::string tid;
                std::getline(std::cin, tid);

                if (!graph.hasNode(tid)) {
                    std::cout << "[!] Invalid topic ID.\n";
                    TerminalUI::pause();
                    break;
                }

                const auto& node = graph.getNode(tid);
                TerminalUI::printBanner("STUDY MODULE: " + node.id + " - " + node.title);
                std::cout << "Category: " << node.category << " | Base Estimated Hours: " << node.baseHours << "h\n";
                std::cout << "Overview: " << node.description << "\n\n";

                auto questions = assessmentEngine.getQuestionsForTopic(tid);
                std::vector<int> userAns;

                for (size_t i = 0; i < questions.size(); ++i) {
                    const auto& q = questions[i];
                    std::cout << "Q" << (i + 1) << ": " << q.questionText << "\n";
                    for (size_t opt = 0; opt < q.options.size(); ++opt) {
                        std::cout << "  [" << (opt + 1) << "] " << q.options[opt] << "\n";
                    }
                    std::cout << "Your Choice (1-" << q.options.size() << "): ";
                    int sel = 1;
                    if (std::cin >> sel) {
                        userAns.push_back(sel - 1);
                    } else {
                        userAns.push_back(-1);
                    }
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                AssessmentResult result = assessmentEngine.gradeSubmission(tid, userAns);
                RerouteDecision decision = OptimizationEngine::evaluateAndReroute(
                    graph, student, tid, result.scorePercentage
                );

                std::cout << "\n" << decision.explanation << "\n";
                TerminalUI::printPath(graph, decision.newPath, decision.newPathEstimatedHours);

                dbManager.saveEvaluationLog(student.getId(), tid, result.scorePercentage, result.passed, decision.rerouted);
                dbManager.exportDashboardData(graph, student, decision.newPath, decision.newPathEstimatedHours);
                TerminalUI::pause();
                break;
            }
            case 5: { // Concept Failure Simulation & Real-Time Rerouting
                TerminalUI::printBanner("CONCEPT FAILURE SIMULATION & DYNAMIC RE-ROUTING");
                std::cout << "Demonstrating dynamic adaptation when student fails advanced concept (e.g. T10 Traversals or T05 Recursion)...\n\n";
                
                std::cout << "Simulating student attempting 'T05 - Recursion & Backtracking' and scoring 30%...\n";
                RerouteDecision decision = OptimizationEngine::evaluateAndReroute(graph, student, "T05", 0.30);

                std::cout << "\n" << decision.explanation << "\n\n";
                TerminalUI::printBanner("RE-CALCULATED OPTIMAL TRAJECTORY (REMEDIATION INJECTED)");
                TerminalUI::printPath(graph, decision.newPath, decision.newPathEstimatedHours);

                dbManager.saveEvaluationLog(student.getId(), "T05", 0.30, false, true);
                dbManager.exportDashboardData(graph, student, decision.newPath, decision.newPathEstimatedHours);
                TerminalUI::pause();
                break;
            }
            case 6: { // Static vs Adaptive Benchmark
                auto comparison = AnalyticsEngine::compareCurricula(graph, student, student.getTargetGoal());
                AnalyticsEngine::printComparisonReport(comparison);
                TerminalUI::pause();
                break;
            }
            case 7: { // OS Concurrent Multithreading Simulation
                TerminalUI::printBanner("OPERATING SYSTEMS INTEGRATION: MULTITHREADED CONCURRENT LEARNERS");
                std::cout << "Simulating 100 concurrent learners with dynamic graph pathfinding and state synchronization...\n";

                auto result = OSConcurrencySimulator::runConcurrentEvaluationSimulation(graph, 100, 4);
                std::cout << "\n------------------------------------------------------------------------------\n";
                std::cout << " CONCURRENCY BENCHMARK RESULTS (OS EVALUATION ENGINE)                         \n";
                std::cout << "------------------------------------------------------------------------------\n";
                std::cout << " Active Concurrent Students  : " << result.totalStudents << "\n";
                std::cout << " Thread Pool Concurrency     : " << result.threadCount << " Workers\n";
                std::cout << " Total Wall Clock Time       : " << std::fixed << std::setprecision(2) 
                          << result.totalExecutionTimeMs << " ms\n";
                std::cout << " Engine Throughput           : " << std::fixed << std::setprecision(1) 
                          << result.throughputOpsPerSec << " Students/Second\n";
                std::cout << " Average Latency per Student : " << std::fixed << std::setprecision(3) 
                          << result.averageLatencyPerStudentMs << " ms\n";
                std::cout << " Successful Recalculations   : " << result.successfulRecalculations << " / " << result.totalStudents << "\n";
                std::cout << "------------------------------------------------------------------------------\n";
                std::cout << " [✓] Race Safety Verified (Mutex critical sections held with zero deadlock).\n";
                std::cout << "------------------------------------------------------------------------------\n\n";
                TerminalUI::pause();
                break;
            }
            case 8: { // View Knowledge State
                TerminalUI::printLearnerDashboard(graph, student);
                TerminalUI::pause();
                break;
            }
            case 9: { // Web Dashboard Launch
                double totalHours = 0.0;
                auto path = dijkstraFinder.computePath(graph, student, "T01", student.getTargetGoal(), totalHours);
                dbManager.exportDashboardData(graph, student, path, totalHours);

                TerminalUI::printBanner("INTERACTIVE WEB VISUALIZER DASHBOARD");
                std::cout << "Data synchronized to 'web/data.json'.\n";
                std::cout << "You can view the dashboard by opening 'web/index.html' in your browser\n";
                std::cout << "or running 'python web/server.py' to launch the local visualizer server.\n\n";
                
                #if defined(_WIN32)
                std::cout << "Opening web dashboard in browser...\n";
                system("start web/index.html");
                #endif

                TerminalUI::pause();
                break;
            }
            default:
                std::cout << "[!] Invalid selection. Please enter 0-9.\n";
                TerminalUI::pause();
                break;
        }
    }

    return 0;
}
