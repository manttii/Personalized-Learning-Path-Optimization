#ifndef PATH_FINDER_HPP
#define PATH_FINDER_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "PriorityQueue.hpp"
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Abstract Base Class for Pathfinding Algorithms.
 * Demonstrates Object-Oriented Design (OOPs) with Polymorphism.
 */
class IPathFinder {
public:
    virtual ~IPathFinder() = default;

    /**
     * @brief Compute the optimal learning path from start to goal node.
     * @param graph The curriculum knowledge DAG.
     * @param learner The student's current proficiency profile.
     * @param startNode Starting topic identifier.
     * @param goalNode Target topic identifier.
     * @param[out] totalEstimatedHours Calculated cumulative learning hours.
     * @return Ordered list of topic IDs to study.
     */
    virtual std::vector<std::string> computePath(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& startNode,
        const std::string& goalNode,
        double& totalEstimatedHours
    ) = 0;

    virtual std::string getAlgorithmName() const = 0;
};

/**
 * @brief Dynamic Dijkstra's Algorithm with Min-Heap Priority Queue.
 * Uses real-time effective weights W'(v) and prerequisite relaxation.
 */
class DijkstraPathFinder : public IPathFinder {
public:
    std::vector<std::string> computePath(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& startNode,
        const std::string& goalNode,
        double& totalEstimatedHours
    ) override;

    std::string getAlgorithmName() const override {
        return "Adaptive Dijkstra (Min-Heap Priority Queue)";
    }
};

/**
 * @brief Topological Sort Pathfinder based on Kahn's in-degree algorithm.
 * Guarantees strict prerequisite order for all required ancestors.
 */
class TopologicalPathFinder : public IPathFinder {
public:
    std::vector<std::string> computePath(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& startNode,
        const std::string& goalNode,
        double& totalEstimatedHours
    ) override;

    std::string getAlgorithmName() const override {
        return "Adaptive Topological Sort (Kahn's In-Degree)";
    }
};

/**
 * @brief A* Pathfinder with Goal-Oriented Heuristic.
 */
class AStarPathFinder : public IPathFinder {
private:
    double calculateHeuristic(const KnowledgeGraph& graph, const std::string& current, const std::string& goal) const;

public:
    std::vector<std::string> computePath(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& startNode,
        const std::string& goalNode,
        double& totalEstimatedHours
    ) override;

    std::string getAlgorithmName() const override {
        return "Goal-Oriented A* Heuristic Search";
    }
};

/**
 * @brief Baseline Static Curriculum Pathfinder (No Dynamic Adaptation).
 * Used as a control baseline to benchmark adaptive gains.
 */
class StaticLinearPathFinder : public IPathFinder {
public:
    std::vector<std::string> computePath(
        const KnowledgeGraph& graph,
        const LearnerProfile& learner,
        const std::string& startNode,
        const std::string& goalNode,
        double& totalEstimatedHours
    ) override;

    std::string getAlgorithmName() const override {
        return "Static Rigid Linear Curriculum (Baseline)";
    }
};

#endif // PATH_FINDER_HPP
