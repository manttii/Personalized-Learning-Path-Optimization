#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <memory>

/**
 * @brief Representation of a curriculum topic node in the DAG.
 */
struct TopicNode {
    std::string id;
    std::string title;
    std::string category;
    double baseHours;
    double difficulty;
    std::string description;

    TopicNode() : baseHours(0.0), difficulty(1.0) {}
    TopicNode(std::string id_, std::string title_, std::string category_,
              double baseHours_, double difficulty_, std::string desc_)
        : id(std::move(id_)), title(std::move(title_)), category(std::move(category_)),
          baseHours(baseHours_), difficulty(difficulty_), description(std::move(desc_)) {}
};

/**
 * @brief Directed edge representing prerequisite dependency.
 * fromNode -> toNode means 'fromNode' is a prerequisite for 'toNode'.
 */
struct PrereqEdge {
    std::string fromId;
    std::string toId;
    double weight;

    PrereqEdge() : weight(1.0) {}
    PrereqEdge(std::string from_, std::string to_, double w = 1.0)
        : fromId(std::move(from_)), toId(std::move(to_)), weight(w) {}
};

/**
 * @brief Directed Acyclic Graph (DAG) for Curriculum Knowledge Representation.
 * Implemented using Adjacency Lists and Reverse-Adjacency Predecessor Lists.
 */
class KnowledgeGraph {
private:
    std::unordered_map<std::string, TopicNode> nodes_;
    std::unordered_map<std::string, std::vector<PrereqEdge>> adj_;   // from -> list of (to, weight)
    std::unordered_map<std::string, std::vector<std::string>> pred_; // to -> list of from (prerequisites)
    std::vector<std::string> nodeInsertionOrder_;

public:
    KnowledgeGraph() = default;

    void addNode(const TopicNode& node);
    void addEdge(const std::string& fromId, const std::string& toId, double weight = 1.0);

    bool hasNode(const std::string& id) const;
    const TopicNode& getNode(const std::string& id) const;
    const std::unordered_map<std::string, TopicNode>& getAllNodes() const;
    const std::vector<std::string>& getNodeOrder() const;

    const std::vector<PrereqEdge>& getOutgoingEdges(const std::string& id) const;
    const std::vector<std::string>& getPrerequisites(const std::string& id) const;

    bool hasCycle() const;
    std::vector<std::string> getTopologicalOrder() const;

    // Retrieve all transitive prerequisite ancestors required to reach goalNode
    std::unordered_set<std::string> getRequiredAncestors(const std::string& goalNode) const;

    // Load curriculum from JSON or built-in default data
    bool loadDefaultCurriculum();
    bool loadFromFile(const std::string& jsonPath);

    size_t nodeCount() const { return nodes_.size(); }
    size_t edgeCount() const;
};

#endif // GRAPH_HPP
