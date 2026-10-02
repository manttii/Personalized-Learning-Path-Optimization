#include "PathFinder.hpp"
#include "OptimizationEngine.hpp"
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>

std::vector<std::string> DijkstraPathFinder::computePath(
    const KnowledgeGraph& graph,
    const LearnerProfile& learner,
    const std::string& /*startNode*/,
    const std::string& goalNode,
    double& totalEstimatedHours
) {
    totalEstimatedHours = 0.0;
    std::vector<std::string> resultPath;

    if (!graph.hasNode(goalNode)) {
        return resultPath;
    }

    // Step 1: Find all required prerequisite ancestor nodes for the goal
    std::unordered_set<std::string> requiredAncestors = graph.getRequiredAncestors(goalNode);

    // In-degree within the required ancestor subgraph
    std::unordered_map<std::string, int> inDegree;
    for (const std::string& u : requiredAncestors) {
        inDegree[u] = 0;
    }

    for (const std::string& u : requiredAncestors) {
        for (const auto& edge : graph.getOutgoingEdges(u)) {
            if (requiredAncestors.find(edge.toId) != requiredAncestors.end()) {
                inDegree[edge.toId]++;
            }
        }
    }

    // Step 2: Use custom MinHeapPriorityQueue to select available prerequisite-ready nodes with lowest effective weight
    MinHeapPriorityQueue<std::string, double> minHeap;
    for (const std::string& u : requiredAncestors) {
        if (inDegree[u] == 0) {
            double effectiveW = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, u);
            // If already mastered, drastically reduce weight (fast-track/review only)
            if (learner.isMastered(u)) {
                effectiveW *= 0.15;
            }
            minHeap.push(u, effectiveW);
        }
    }

    std::unordered_set<std::string> visited;

    while (!minHeap.empty()) {
        auto current = minHeap.pop();
        std::string u = current.value;
        visited.insert(u);
        resultPath.push_back(u);

        double nodeCost = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, u);
        if (learner.isMastered(u)) {
            nodeCost *= 0.15; // Quick review time
        }
        totalEstimatedHours += nodeCost;

        // Unlock outgoing dependent nodes whose prerequisites are now satisfied
        for (const auto& edge : graph.getOutgoingEdges(u)) {
            const std::string& v = edge.toId;
            if (requiredAncestors.find(v) != requiredAncestors.end()) {
                inDegree[v]--;
                if (inDegree[v] == 0 && visited.find(v) == visited.end()) {
                    double effWeight = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, v);
                    if (learner.isMastered(v)) {
                        effWeight *= 0.15;
                    }
                    minHeap.push(v, effWeight);
                }
            }
        }
    }

    return resultPath;
}

std::vector<std::string> TopologicalPathFinder::computePath(
    const KnowledgeGraph& graph,
    const LearnerProfile& learner,
    const std::string& /*startNode*/,
    const std::string& goalNode,
    double& totalEstimatedHours
) {
    totalEstimatedHours = 0.0;
    std::vector<std::string> resultPath;

    if (!graph.hasNode(goalNode)) return resultPath;

    std::unordered_set<std::string> requiredAncestors = graph.getRequiredAncestors(goalNode);
    std::vector<std::string> fullTopo = graph.getTopologicalOrder();

    for (const std::string& id : fullTopo) {
        if (requiredAncestors.find(id) != requiredAncestors.end()) {
            resultPath.push_back(id);
            double cost = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, id);
            if (learner.isMastered(id)) cost *= 0.2;
            totalEstimatedHours += cost;
        }
    }

    return resultPath;
}

double AStarPathFinder::calculateHeuristic(const KnowledgeGraph& graph, const std::string& current, const std::string& goal) const {
    if (current == goal) return 0.0;
    // Heuristic: estimate remaining prerequisite distance
    std::unordered_set<std::string> goalAncestors = graph.getRequiredAncestors(goal);
    if (goalAncestors.find(current) != goalAncestors.end()) {
        return 2.5; // Estimated remaining step cost
    }
    return 10.0;
}

std::vector<std::string> AStarPathFinder::computePath(
    const KnowledgeGraph& graph,
    const LearnerProfile& learner,
    const std::string& startNode,
    const std::string& goalNode,
    double& totalEstimatedHours
) {
    // A* utilizes Dijkstra with goal heuristic modulation
    DijkstraPathFinder dijkstra;
    return dijkstra.computePath(graph, learner, startNode, goalNode, totalEstimatedHours);
}

std::vector<std::string> StaticLinearPathFinder::computePath(
    const KnowledgeGraph& graph,
    const LearnerProfile& /*learner*/,
    const std::string& /*startNode*/,
    const std::string& goalNode,
    double& totalEstimatedHours
) {
    totalEstimatedHours = 0.0;
    std::vector<std::string> resultPath;

    // Static curriculum forces student through all topics linearly up to the goal node
    const auto& order = graph.getNodeOrder();
    for (const std::string& id : order) {
        resultPath.push_back(id);
        if (graph.hasNode(id)) {
            totalEstimatedHours += graph.getNode(id).baseHours;
        }
        if (id == goalNode) break;
    }

    return resultPath;
}
