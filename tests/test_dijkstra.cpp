#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "PathFinder.hpp"
#include "PriorityQueue.hpp"
#include <iostream>
#include <cassert>

void testMinHeap() {
    std::cout << "[TEST] Running MinHeapPriorityQueue unit tests...\n";
    MinHeapPriorityQueue<std::string, double> pq;
    assert(pq.empty());

    pq.push("T01", 5.0);
    pq.push("T02", 2.0);
    pq.push("T03", 8.0);
    pq.push("T04", 1.0);

    assert(pq.size() == 4);
    assert(pq.top().value == "T04");
    assert(pq.top().priority == 1.0);

    auto el1 = pq.pop();
    assert(el1.value == "T04");

    auto el2 = pq.pop();
    assert(el2.value == "T02");

    pq.decreaseKey("T03", 0.5);
    assert(pq.top().value == "T03");

    std::cout << "  [✓] MinHeap priority queue tests passed!\n";
}

void testDijkstraPath() {
    std::cout << "[TEST] Running Dijkstra Pathfinding tests on KnowledgeGraph...\n";
    KnowledgeGraph graph;
    graph.loadDefaultCurriculum();
    assert(graph.nodeCount() == 15);
    assert(graph.edgeCount() > 10);

    LearnerProfile learner("2510011893", "Test Student", "test@geu.ac.in", "T12");
    DijkstraPathFinder finder;
    double totalHours = 0.0;
    auto path = finder.computePath(graph, learner, "T01", "T12", totalHours);

    assert(!path.empty());
    assert(path.back() == "T12");
    assert(totalHours > 0.0);

    std::cout << "  [✓] Dijkstra computed path of length " << path.size() 
              << " with total estimated hours: " << totalHours << "h\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  RUNNING DIJKSTRA & PRIORITY QUEUE TESTS\n";
    std::cout << "========================================\n";
    testMinHeap();
    testDijkstraPath();
    std::cout << "[ALL TESTS PASSED SUCCESSFULLY]\n";
    return 0;
}
