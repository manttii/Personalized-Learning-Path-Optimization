#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "OptimizationEngine.hpp"
#include <iostream>
#include <cassert>

void testWeightCalculation() {
    std::cout << "[TEST] Running Dynamic Effective Weight Calculation Tests...\n";
    KnowledgeGraph graph;
    graph.loadDefaultCurriculum();

    LearnerProfile learner("2510011893", "Test Student", "test@geu.ac.in", "T05");
    
    // Baseline: P(T01) = 0.0 -> highest weight
    learner.setProficiency("T01", 0.0);
    double wZero = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, "T01");

    // Mastered: P(T01) = 1.0 -> lowest weight
    learner.setProficiency("T01", 1.0);
    double wFull = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, "T01");

    assert(wZero > wFull);
    std::cout << "  [✓] Effective weight properly scales from " << wZero << "h (P=0) down to " << wFull << "h (P=1.0)!\n";
}

void testDynamicRerouteOnFailure() {
    std::cout << "[TEST] Running Dynamic Reroute on Failure Tests...\n";
    KnowledgeGraph graph;
    graph.loadDefaultCurriculum();

    LearnerProfile learner("2510011893", "Test Student", "test@geu.ac.in", "T12");
    
    // Student fails T05
    RerouteDecision decision = OptimizationEngine::evaluateAndReroute(graph, learner, "T05", 0.30);
    assert(decision.rerouted);
    assert(!decision.newPath.empty());

    std::cout << "  [✓] Rerouting correctly triggered upon assessment failure!\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "  RUNNING DYNAMIC WEIGHT & REROUTE TESTS   \n";
    std::cout << "===========================================\n";
    testWeightCalculation();
    testDynamicRerouteOnFailure();
    std::cout << "[ALL TESTS PASSED SUCCESSFULLY]\n";
    return 0;
}
