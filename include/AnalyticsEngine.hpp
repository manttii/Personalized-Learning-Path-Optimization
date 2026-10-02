#ifndef ANALYTICS_ENGINE_HPP
#define ANALYTICS_ENGINE_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "PathFinder.hpp"
#include <string>
#include <vector>

/**
 * @brief Detailed comparative analysis metrics between Static and Adaptive learning.
 */
struct CurriculumComparisonResult {
    std::string goalTopicId;
    std::string goalTopicName;
    
    std::vector<std::string> staticPath;
    double staticEstimatedHours;
    double staticRetentionScore;

    std::vector<std::string> dynamicPath;
    double dynamicEstimatedHours;
    double dynamicRetentionScore;

    double hoursSaved;
    double percentageTimeReduction;
    int redundantTopicsSkipped;
    int remedialTopicsInjected;
    double cognitiveLoadReductionPercent;
};

/**
 * @brief Analytics and Performance Evaluation Engine.
 */
class AnalyticsEngine {
public:
    static CurriculumComparisonResult compareCurricula(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& goalNodeId
    );

    static void printComparisonReport(const CurriculumComparisonResult& res);
};

#endif // ANALYTICS_ENGINE_HPP
