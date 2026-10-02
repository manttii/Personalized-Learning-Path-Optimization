#ifndef OPTIMIZATION_ENGINE_HPP
#define OPTIMIZATION_ENGINE_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Result structure for dynamic rerouting events.
 */
struct RerouteDecision {
    bool rerouted;
    std::string triggeredTopicId;
    std::string rootCausePrerequisiteId;
    double oldPathEstimatedHours;
    double newPathEstimatedHours;
    std::vector<std::string> oldPath;
    std::vector<std::string> newPath;
    std::string explanation;
};

/**
 * @brief Mathematical Optimization Engine for Adaptive Curricula.
 * 
 * Computes Dynamic Effective Weights:
 *   W'(v_i) = W(v_i) * [1 + alpha * (1 - P(v_i))] + Prereq_Penalty(v_i)
 */
class OptimizationEngine {
public:
    // Compute effective weight for a single node based on learner's proficiency P(v)
    static double calculateEffectiveNodeWeight(const KnowledgeGraph& graph,
                                              const LearnerProfile& learner,
                                              const std::string& topicId);

    // Compute effective edge weight (combining transition difficulty and target node load)
    static double calculateEffectiveEdgeWeight(const KnowledgeGraph& graph,
                                              const LearnerProfile& learner,
                                              const std::string& fromId,
                                              const std::string& toId);

    // Evaluate diagnostic or quiz submission, determine if rerouting is needed
    static RerouteDecision evaluateAndReroute(const KnowledgeGraph& graph,
                                            LearnerProfile& learner,
                                            const std::string& currentTopicId,
                                            double testScore);

    // Check if all prerequisites of a node are sufficiently mastered (P(u) >= threshold)
    static bool arePrerequisitesSatisfied(const KnowledgeGraph& graph,
                                          const LearnerProfile& learner,
                                          const std::string& topicId,
                                          double threshold = 0.60);

    // Identify which prerequisite is the weakest bottleneck
    static std::string findWeakestPrerequisite(const KnowledgeGraph& graph,
                                             const LearnerProfile& learner,
                                             const std::string& topicId);
};

#endif // OPTIMIZATION_ENGINE_HPP
