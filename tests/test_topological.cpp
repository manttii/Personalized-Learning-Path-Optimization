#include "Graph.hpp"
#include <iostream>
#include <cassert>
#include <unordered_map>

void testTopologicalSortAndCycleDetection() {
    std::cout << "[TEST] Running Topological Sort & Cycle Detection Tests...\n";
    KnowledgeGraph graph;
    graph.loadDefaultCurriculum();

    assert(!graph.hasCycle());
    auto topoOrder = graph.getTopologicalOrder();
    assert(topoOrder.size() == 15);

    // Verify prerequisite constraints: if u is prerequisite of v, u must appear before v in topoOrder
    std::unordered_map<std::string, int> position;
    for (size_t i = 0; i < topoOrder.size(); ++i) {
        position[topoOrder[i]] = static_cast<int>(i);
    }

    for (const std::string& u : topoOrder) {
        for (const auto& edge : graph.getOutgoingEdges(u)) {
            assert(position[edge.fromId] < position[edge.toId]);
        }
    }
    std::cout << "  [✓] Topological sorting strictly satisfies all DAG prerequisite constraints!\n";

    // Test cycle detection on synthetic cyclic graph
    KnowledgeGraph cyclicGraph;
    cyclicGraph.addNode(TopicNode("A", "Node A", "Test", 1, 1, ""));
    cyclicGraph.addNode(TopicNode("B", "Node B", "Test", 1, 1, ""));
    cyclicGraph.addNode(TopicNode("C", "Node C", "Test", 1, 1, ""));
    cyclicGraph.addEdge("A", "B", 1.0);
    cyclicGraph.addEdge("B", "C", 1.0);
    cyclicGraph.addEdge("C", "A", 1.0); // Cycle!

    assert(cyclicGraph.hasCycle());
    std::cout << "  [✓] Cycle detection correctly identified cycle A -> B -> C -> A!\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "  RUNNING TOPOLOGICAL SORT & CYCLE TESTS   \n";
    std::cout << "===========================================\n";
    testTopologicalSortAndCycleDetection();
    std::cout << "[ALL TESTS PASSED SUCCESSFULLY]\n";
    return 0;
}
