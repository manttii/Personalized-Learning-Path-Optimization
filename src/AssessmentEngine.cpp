#include "AssessmentEngine.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

AssessmentEngine::AssessmentEngine() {
    loadDefaultQuestions();
}

void AssessmentEngine::addQuestion(const QuizQuestion& q) {
    questionBank_.push_back(q);
    topicQuestionMap_[q.topicId].push_back(q);
}

bool AssessmentEngine::loadDefaultQuestions() {
    questionBank_.clear();
    topicQuestionMap_.clear();

    // T01: Time & Space Complexity
    addQuestion(QuizQuestion(
        "T01",
        "What is the worst-case time complexity of linear search on an unsorted array of size N?",
        {"O(1)", "O(log N)", "O(N)", "O(N log N)"},
        2,
        "Linear search must check every element in the worst case, taking O(N) comparisons."
    ));
    addQuestion(QuizQuestion(
        "T01",
        "Which asymptotic notation describes both the tight upper and lower bounds?",
        {"Big-O (O)", "Big-Omega (Ω)", "Big-Theta (Θ)", "Little-o (o)"},
        2,
        "Big-Theta (Θ) formally bounds a function from above and below within constant factors."
    ));

    // T02: Arrays & Memory Layout
    addQuestion(QuizQuestion(
        "T02",
        "Why is random access in an array O(1)?",
        {
            "Because elements are stored in contiguous memory with direct index offset calculation",
            "Because the CPU caches the entire array in RAM",
            "Because array indices are stored in a hash table",
            "Because the array is pre-sorted"
        },
        0,
        "Array indexing uses base address + index * element_size to calculate memory offset in O(1)."
    ));

    // T03: Linked Lists
    addQuestion(QuizQuestion(
        "T03",
        "What is the time complexity to insert an element at the beginning (head) of a singly linked list?",
        {"O(1)", "O(N)", "O(log N)", "O(N^2)"},
        0,
        "Inserting at head requires only updating head pointer and new node's next pointer: O(1)."
    ));

    // T04: Stacks & Queues
    addQuestion(QuizQuestion(
        "T04",
        "Which data structure follows the Last-In-First-Out (LIFO) principle?",
        {"Queue", "Stack", "Priority Queue", "Deque"},
        1,
        "A stack operates strictly on LIFO order."
    ));

    // T05: Recursion & Backtracking
    addQuestion(QuizQuestion(
        "T05",
        "What is the primary risk of a recursive function lacking a valid base condition?",
        {"Memory Leak", "Stack Overflow error due to infinite call frames", "Deadlock", "CPU Overheating"},
        1,
        "Each recursive call pushes an activation record onto the call stack; missing base cases trigger Stack Overflow."
    ));

    // T06: Binary Trees & Traversals
    addQuestion(QuizQuestion(
        "T06",
        "Which tree traversal visits the root node AFTER visiting both left and right subtrees?",
        {"Preorder", "Inorder", "Postorder", "Level-order"},
        2,
        "Postorder traversal visits Left, Right, then Root."
    ));

    // T07: BST & Balanced Trees
    addQuestion(QuizQuestion(
        "T07",
        "What is the time complexity of searching in a perfectly balanced Binary Search Tree (AVL tree)?",
        {"O(1)", "O(log N)", "O(N)", "O(N log N)"},
        1,
        "Balanced BST maintains height O(log N), guaranteeing O(log N) search time."
    ));

    // T08: Binary Heaps & Priority Queues
    addQuestion(QuizQuestion(
        "T08",
        "In a Binary Min-Heap of size N, what is the time complexity to extract the minimum element?",
        {"O(1)", "O(log N)", "O(N)", "O(N log N)"},
        1,
        "Extracting root requires heapify-down to restore heap invariant, taking O(log N)."
    ));

    // T09: Graph Representations
    addQuestion(QuizQuestion(
        "T09",
        "What is the space complexity of an Adjacency List for a graph with V vertices and E edges?",
        {"O(V^2)", "O(V + E)", "O(E^2)", "O(V * E)"},
        1,
        "Adjacency list stores V vertex headers and E edge node entries: O(V + E)."
    ));

    // T10: Graph Traversals (BFS & DFS)
    addQuestion(QuizQuestion(
        "T10",
        "Which data structure is fundamentally utilized to implement Breadth-First Search (BFS)?",
        {"Stack", "Queue", "Priority Queue", "Binary Search Tree"},
        1,
        "BFS explores nodes level-by-level using a First-In-First-Out (FIFO) queue."
    ));

    // T11: Topological Sorting & DAGs
    addQuestion(QuizQuestion(
        "T11",
        "Which of the following conditions is REQUIRED to perform a Topological Sort?",
        {"Graph must be undirected", "Graph must be a Directed Acyclic Graph (DAG)", "Graph must be complete", "Graph must be a tree"},
        1,
        "Topological sorting is only well-defined for Directed Acyclic Graphs (DAGs)."
    ));

    // T12: Dijkstra's Shortest Path
    addQuestion(QuizQuestion(
        "T12",
        "What is the overall time complexity of Dijkstra's algorithm implemented with a Min-Heap priority queue?",
        {"O(V^2)", "O((V + E) log V)", "O(V * E)", "O(E^2)"},
        1,
        "Using a binary min-heap priority queue, Dijkstra runs in O((V + E) log V)."
    ));

    // T13: Minimum Spanning Trees
    addQuestion(QuizQuestion(
        "T13",
        "Which data structure allows Kruskal's algorithm to efficiently detect cycles when adding edges?",
        {"Min-Heap", "Disjoint Set Union (Union-Find)", "Adjacency Matrix", "Binary Search Tree"},
        1,
        "Disjoint Set Union (DSU) with path compression and rank detects cycles in near O(1) amortized time."
    ));

    // T14: Dynamic Programming
    addQuestion(QuizQuestion(
        "T14",
        "What are the two foundational properties required to apply Dynamic Programming?",
        {
            "Greedy property and Heap structure",
            "Optimal Substructure and Overlapping Subproblems",
            "Topological ordering and Acyclic structure",
            "Divide and conquer with independent subproblems"
        },
        1,
        "Dynamic Programming solves problems having optimal substructure and overlapping subproblems."
    ));

    // T15: Graph Dynamic Programming
    addQuestion(QuizQuestion(
        "T15",
        "What is the time complexity of the Floyd-Warshall All-Pairs Shortest Path dynamic programming algorithm?",
        {"O(V^2)", "O(V^3)", "O(V log V)", "O(E log V)"},
        1,
        "Floyd-Warshall checks all pairs for each intermediate node k: 3 nested loops resulting in O(V^3)."
    ));

    return true;
}

bool AssessmentEngine::loadFromFile(const std::string& /*jsonPath*/) {
    return loadDefaultQuestions();
}

std::vector<QuizQuestion> AssessmentEngine::getQuestionsForTopic(const std::string& topicId) const {
    auto it = topicQuestionMap_.find(topicId);
    if (it != topicQuestionMap_.end()) {
        return it->second;
    }
    // Fallback: return general question
    return {
        QuizQuestion(topicId, "Do you understand the foundational theory of this module?", {"Yes", "Partially", "No", "Uncertain"}, 0, "Self-assessment.")
    };
}

std::vector<QuizQuestion> AssessmentEngine::getDiagnosticBaselineQuestions(int count) const {
    std::vector<QuizQuestion> selected;
    std::vector<std::string> baselineTopics = {"T01", "T02", "T04", "T05", "T06", "T10"};

    for (const std::string& tid : baselineTopics) {
        if (static_cast<int>(selected.size()) >= count) break;
        auto qs = getQuestionsForTopic(tid);
        if (!qs.empty()) {
            selected.push_back(qs.front());
        }
    }
    return selected;
}

AssessmentResult AssessmentEngine::gradeSubmission(const std::string& topicId,
                                                  const std::vector<int>& userAnswers) const {
    AssessmentResult res;
    res.topicId = topicId;
    auto questions = getQuestionsForTopic(topicId);
    res.totalQuestions = static_cast<int>(questions.size());
    res.correctAnswers = 0;

    for (size_t i = 0; i < questions.size() && i < userAnswers.size(); ++i) {
        if (userAnswers[i] == questions[i].correctOptionIndex) {
            res.correctAnswers++;
        }
    }

    res.scorePercentage = (res.totalQuestions > 0) ? 
        (static_cast<double>(res.correctAnswers) / res.totalQuestions) : 0.0;
    res.passed = (res.scorePercentage >= 0.60);

    std::ostringstream ss;
    if (res.passed) {
        ss << "Passed with score " << std::fixed << std::setprecision(1) << (res.scorePercentage * 100) 
           << "%. Prerequisites satisfied!";
    } else {
        ss << "Score " << std::fixed << std::setprecision(1) << (res.scorePercentage * 100) 
           << "% is below passing threshold (60%). Adaptive rerouting triggered.";
    }
    res.feedback = ss.str();

    return res;
}

void AssessmentEngine::applyResultToLearner(LearnerProfile& learner, const AssessmentResult& result,
                                           double wBefore, double wAfter) {
    learner.recordAssessment(result.topicId, result.scorePercentage, result.passed, wBefore, wAfter, result.feedback);
}
