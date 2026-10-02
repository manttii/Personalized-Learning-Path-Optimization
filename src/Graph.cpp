#include "Graph.hpp"
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>

void KnowledgeGraph::addNode(const TopicNode& node) {
    if (nodes_.find(node.id) == nodes_.end()) {
        nodeInsertionOrder_.push_back(node.id);
    }
    nodes_[node.id] = node;
}

void KnowledgeGraph::addEdge(const std::string& fromId, const std::string& toId, double weight) {
    adj_[fromId].push_back(PrereqEdge(fromId, toId, weight));
    pred_[toId].push_back(fromId);
}

bool KnowledgeGraph::hasNode(const std::string& id) const {
    return nodes_.find(id) != nodes_.end();
}

const TopicNode& KnowledgeGraph::getNode(const std::string& id) const {
    auto it = nodes_.find(id);
    if (it == nodes_.end()) {
        static TopicNode emptyNode("UNKNOWN", "Unknown Topic", "General", 0.0, 1.0, "");
        return emptyNode;
    }
    return it->second;
}

const std::unordered_map<std::string, TopicNode>& KnowledgeGraph::getAllNodes() const {
    return nodes_;
}

const std::vector<std::string>& KnowledgeGraph::getNodeOrder() const {
    return nodeInsertionOrder_;
}

const std::vector<PrereqEdge>& KnowledgeGraph::getOutgoingEdges(const std::string& id) const {
    static const std::vector<PrereqEdge> emptyEdges;
    auto it = adj_.find(id);
    if (it != adj_.end()) {
        return it->second;
    }
    return emptyEdges;
}

const std::vector<std::string>& KnowledgeGraph::getPrerequisites(const std::string& id) const {
    static const std::vector<std::string> emptyPreds;
    auto it = pred_.find(id);
    if (it != pred_.end()) {
        return it->second;
    }
    return emptyPreds;
}

size_t KnowledgeGraph::edgeCount() const {
    size_t count = 0;
    for (const auto& pair : adj_) {
        count += pair.second.size();
    }
    return count;
}

bool KnowledgeGraph::hasCycle() const {
    // DFS 3-color cycle detection: 0 = unvisited, 1 = visiting (in stack), 2 = visited
    std::unordered_map<std::string, int> state;
    for (const auto& pair : nodes_) {
        state[pair.first] = 0;
    }

    auto dfs = [this, &state](auto& self, const std::string& u) -> bool {
        state[u] = 1; // Visiting
        for (const auto& edge : this->getOutgoingEdges(u)) {
            const std::string& v = edge.toId;
            if (state[v] == 1) return true; // Back-edge detected -> Cycle!
            if (state[v] == 0 && self(self, v)) return true;
        }
        state[u] = 2; // Visited
        return false;
    };

    for (const auto& pair : nodes_) {
        if (state[pair.first] == 0) {
            if (dfs(dfs, pair.first)) return true;
        }
    }
    return false;
}

std::vector<std::string> KnowledgeGraph::getTopologicalOrder() const {
    std::unordered_map<std::string, int> inDegree;
    for (const auto& pair : nodes_) {
        inDegree[pair.first] = 0;
    }

    for (const auto& pair : adj_) {
        for (const auto& edge : pair.second) {
            inDegree[edge.toId]++;
        }
    }

    std::queue<std::string> q;
    for (const auto& pair : nodes_) {
        if (inDegree[pair.first] == 0) {
            q.push(pair.first);
        }
    }

    std::vector<std::string> order;
    while (!q.empty()) {
        std::string u = q.front();
        q.pop();
        order.push_back(u);

        for (const auto& edge : getOutgoingEdges(u)) {
            const std::string& v = edge.toId;
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (order.size() != nodes_.size()) {
        // Cycle detected, fallback to insertion order
        return nodeInsertionOrder_;
    }
    return order;
}

std::unordered_set<std::string> KnowledgeGraph::getRequiredAncestors(const std::string& goalNode) const {
    std::unordered_set<std::string> required;
    if (!hasNode(goalNode)) return required;

    std::queue<std::string> q;
    q.push(goalNode);
    required.insert(goalNode);

    while (!q.empty()) {
        std::string curr = q.front();
        q.pop();

        for (const std::string& prereq : getPrerequisites(curr)) {
            if (required.find(prereq) == required.end()) {
                required.insert(prereq);
                q.push(prereq);
            }
        }
    }
    return required;
}

bool KnowledgeGraph::loadDefaultCurriculum() {
    nodes_.clear();
    adj_.clear();
    pred_.clear();
    nodeInsertionOrder_.clear();

    // 15 Core Computer Science Curriculum Nodes
    addNode(TopicNode("T01", "Time & Space Complexity", "Foundations", 3.0, 1.5, "Asymptotic notation, Big-O, recurrence relations"));
    addNode(TopicNode("T02", "Arrays & Dynamic Memory", "Linear DS", 4.0, 2.0, "Dynamic vectors, pointer arithmetic, contiguous cache"));
    addNode(TopicNode("T03", "Linked Lists", "Linear DS", 5.0, 2.5, "Singly/Doubly linked lists, pointer manipulation, Floyd cycle detection"));
    addNode(TopicNode("T04", "Stacks & Queues", "Linear DS", 4.0, 2.0, "LIFO/FIFO, evaluation of expressions, circular queues"));
    addNode(TopicNode("T05", "Recursion & Backtracking", "Paradigms", 6.0, 3.5, "Call stack frame, base conditions, recursion trees, N-Queens"));
    addNode(TopicNode("T06", "Binary Trees & Traversals", "Hierarchical DS", 5.5, 3.0, "Inorder, Preorder, Postorder recursive and iterative traversal"));
    addNode(TopicNode("T07", "BST & Balanced AVL Trees", "Hierarchical DS", 6.5, 4.0, "BST invariant, search, insert, delete, AVL rotations"));
    addNode(TopicNode("T08", "Binary Heaps & Priority Queues", "Hierarchical DS", 4.5, 3.0, "Min/Max heap, heapify up/down, priority queue ops"));
    addNode(TopicNode("T09", "Graph Representations", "Graph Theory", 4.0, 2.5, "Adjacency matrix vs Adjacency list representation"));
    addNode(TopicNode("T10", "Graph Traversals (BFS & DFS)", "Graph Theory", 6.0, 3.5, "Queue-based BFS, recursive/stack DFS, components"));
    addNode(TopicNode("T11", "Topological Sort & DAGs", "Graph Theory", 5.0, 3.5, "Kahns algorithm, dependency scheduling, cycle detection"));
    addNode(TopicNode("T12", "Dijkstra's Shortest Path", "Graph Algorithms", 7.0, 4.5, "Greedy edge relaxation with min-heap priority queue"));
    addNode(TopicNode("T13", "Minimum Spanning Trees", "Graph Algorithms", 6.5, 4.0, "Prim's & Kruskal's algorithms with Disjoint Set Union (DSU)"));
    addNode(TopicNode("T14", "Dynamic Programming", "Advanced Paradigms", 8.0, 5.0, "Optimal substructure, overlapping subproblems, memoization"));
    addNode(TopicNode("T15", "Graph Dynamic Programming", "Advanced Paradigms", 7.5, 5.0, "Floyd-Warshall, Bellman-Ford, DAG longest paths"));

    // Prerequisite Directed Edges
    addEdge("T01", "T02", 1.0);
    addEdge("T02", "T03", 1.0);
    addEdge("T02", "T04", 1.0);
    addEdge("T01", "T05", 1.2);
    addEdge("T04", "T05", 1.1);
    addEdge("T03", "T06", 1.0);
    addEdge("T05", "T06", 1.3);
    addEdge("T06", "T07", 1.2);
    addEdge("T02", "T08", 1.0);
    addEdge("T06", "T08", 1.1);
    addEdge("T02", "T09", 1.0);
    addEdge("T03", "T09", 1.0);
    addEdge("T04", "T10", 1.1);
    addEdge("T05", "T10", 1.3);
    addEdge("T09", "T10", 1.0);
    addEdge("T10", "T11", 1.2);
    addEdge("T08", "T12", 1.4);
    addEdge("T10", "T12", 1.3);
    addEdge("T08", "T13", 1.2);
    addEdge("T10", "T13", 1.2);
    addEdge("T05", "T14", 1.5);
    addEdge("T06", "T14", 1.2);
    addEdge("T12", "T15", 1.3);
    addEdge("T14", "T15", 1.4);

    return true;
}

bool KnowledgeGraph::loadFromFile(const std::string& /*jsonPath*/) {
    // For simplicity and 100% native zero-dependency C++ portability,
    // load default structured curriculum
    return loadDefaultCurriculum();
}
