#include "OptimizationEngine.hpp"
#include "PathFinder.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

double OptimizationEngine::calculateEffectiveNodeWeight(const KnowledgeGraph& graph,
                                                       const LearnerProfile& learner,
                                                       const std::string& topicId) {
    if (!graph.hasNode(topicId)) return 5.0;
    const TopicNode& node = graph.getNode(topicId);

    double baseW = node.baseHours;
    double p = learner.getProficiency(topicId);
    double alpha = learner.getAlpha();

    // Exact Mathematical Model from Phase-I Report (Slide 7):
    // W'(v_i) = W(v_i) * [1 + alpha * (1 - P(v_i))]
    double effectiveW = baseW * (1.0 + alpha * (1.0 - p));

    // Prerequisite bottleneck penalty (if direct prerequisites are unmastered)
    for (const std::string& prereqId : graph.getPrerequisites(topicId)) {
        double pPrereq = learner.getProficiency(prereqId);
        if (pPrereq < 0.60 && !learner.isMastered(prereqId)) {
            effectiveW += 1.5 * (0.60 - pPrereq);
        }
    }

    return effectiveW;
}

double OptimizationEngine::calculateEffectiveEdgeWeight(const KnowledgeGraph& graph,
                                                       const LearnerProfile& learner,
                                                       const std::string& /*fromId*/,
                                                       const std::string& toId) {
    return calculateEffectiveNodeWeight(graph, learner, toId);
}

bool OptimizationEngine::arePrerequisitesSatisfied(const KnowledgeGraph& graph,
                                                  const LearnerProfile& learner,
                                                  const std::string& topicId,
                                                  double threshold) {
    for (const std::string& prereqId : graph.getPrerequisites(topicId)) {
        if (learner.getProficiency(prereqId) < threshold && !learner.isMastered(prereqId)) {
            return false;
        }
    }
    return true;
}

std::string OptimizationEngine::findWeakestPrerequisite(const KnowledgeGraph& graph,
                                                       const LearnerProfile& learner,
                                                       const std::string& topicId) {
    std::string weakest = "";
    double minScore = 2.0;

    for (const std::string& prereqId : graph.getPrerequisites(topicId)) {
        double score = learner.getProficiency(prereqId);
        if (score < minScore) {
            minScore = score;
            weakest = prereqId;
        }
    }
    return weakest;
}

RerouteDecision OptimizationEngine::evaluateAndReroute(const KnowledgeGraph& graph,
                                                     LearnerProfile& learner,
                                                     const std::string& currentTopicId,
                                                     double testScore) {
    RerouteDecision decision;
    decision.triggeredTopicId = currentTopicId;
    decision.oldPath = learner.getCurrentPath();

    DijkstraPathFinder pathFinder;
    double oldHours = 0.0;
    pathFinder.computePath(graph, learner, "T01", learner.getTargetGoal(), oldHours);
    decision.oldPathEstimatedHours = oldHours;

    double wBefore = calculateEffectiveNodeWeight(graph, learner, currentTopicId);

    // Update learner state with new score
    learner.setProficiency(currentTopicId, testScore);
    if (testScore >= 0.70) {
        learner.setMastered(currentTopicId, true);
    }

    double wAfter = calculateEffectiveNodeWeight(graph, learner, currentTopicId);

    // Recompute path with updated proficiencies
    double newHours = 0.0;
    std::vector<std::string> newPath = pathFinder.computePath(graph, learner, "T01", learner.getTargetGoal(), newHours);
    decision.newPath = newPath;
    decision.newPathEstimatedHours = newHours;
    learner.setCurrentPath(newPath);

    std::ostringstream ss;
    if (testScore < 0.60) {
        decision.rerouted = true;
        std::string weakestPrereq = findWeakestPrerequisite(graph, learner, currentTopicId);
        decision.rootCausePrerequisiteId = weakestPrereq;

        ss << "[!] Assessment Failure Detected on topic " << currentTopicId << " (" 
           << graph.getNode(currentTopicId).title << ") with score " << std::fixed << std::setprecision(1) << (testScore * 100) << "%.\n";
        
        if (!weakestPrereq.empty() && learner.getProficiency(weakestPrereq) < 0.60) {
            ss << "    -> Root Cause Identified: Foundational gap in prerequisite " 
               << weakestPrereq << " (" << graph.getNode(weakestPrereq).title << ") [Proficiency: " 
               << (learner.getProficiency(weakestPrereq) * 100) << "%].\n"
               << "    -> Optimization Engine has dynamically re-routed the trajectory to reinforce foundational sub-modules first.";
        } else {
            ss << "    -> Dynamic weight increased from " << std::setprecision(2) << wBefore << "h to " << wAfter 
               << "h. Trajectory recalculated to prioritize supplementary review modules.";
        }
    } else {
        decision.rerouted = false;
        decision.rootCausePrerequisiteId = "";
        ss << "[✓] Concept Mastery Verified on topic " << currentTopicId << " (" 
           << graph.getNode(currentTopicId).title << ") with score " << std::fixed << std::setprecision(1) << (testScore * 100) << "%.\n"
           << "    -> Effective weight optimized down to " << std::setprecision(2) << wAfter 
           << "h. Trajectory dynamically progressing toward goal " << learner.getTargetGoal() << ".";
    }

    decision.explanation = ss.str();
    learner.recordAssessment(currentTopicId, testScore, testScore >= 0.60, wBefore, wAfter, decision.explanation);

    return decision;
}
