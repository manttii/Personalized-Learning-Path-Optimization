#include "TerminalUI.hpp"
#include "OptimizationEngine.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

void TerminalUI::printHeader() {
    std::cout << "\n";
    printDivider('=', 78);
    std::cout << "  PROJECT-BASED LEARNING (PBL) -- PHASE I & II EVALUATION ENGINE\n";
    std::cout << "  Project: Personalized Learning Path Optimization\n";
    std::cout << "  Dept. of Computer Science & Engineering | Graphic Era University (Dehradun)\n";
    std::cout << "  Team ID: DSCPP-III-2026-T284 | Academic Session: 2026-27 | Mentor: Ram ji Chauhan\n";
    std::cout << "  Team: Priyanshi Saini (2510011893), Ishita Doval, Navdeep Singh Pundir (251037038)\n";
    printDivider('=', 78);
    std::cout << "\n";
}

void TerminalUI::printBanner(const std::string& title) {
    std::cout << "\n";
    printDivider('-', 78);
    std::cout << "  >>> " << title << "\n";
    printDivider('-', 78);
}

void TerminalUI::printDivider(char ch, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << ch;
    }
    std::cout << "\n";
}

void TerminalUI::printMenu() {
    std::cout << "\n+----------------------------------------------------------------------------+\n";
    std::cout << "|                          INTERACTIVE PBL CONTROL PANEL                     |\n";
    std::cout << "+----------------------------------------------------------------------------+\n";
    std::cout << "|  [1] View Full Curriculum Knowledge DAG & Dynamic Effective Weights        |\n";
    std::cout << "|  [2] Compute & Display Optimal Learning Path (Dijkstra vs Topological)     |\n";
    std::cout << "|  [3] Take Interactive Diagnostic Baseline Assessment                       |\n";
    std::cout << "|  [4] Interactive Topic Study & Quiz Evaluation (Simulate Pass/Fail)        |\n";
    std::cout << "|  [5] Trigger Concept Failure & Observe Real-Time Dynamic Re-routing        |\n";
    std::cout << "|  [6] Run Static vs Adaptive Benchmark (Time Reduction & Retention Efficacy)|\n";
    std::cout << "|  [7] Run OS Concurrent Learner Multithreading Simulation                   |\n";
    std::cout << "|  [8] View Student Knowledge State & Analytics Dashboard                    |\n";
    std::cout << "|  [9] Export Data & Launch Interactive Web Visualizer Dashboard             |\n";
    std::cout << "|  [0] Exit System                                                           |\n";
    std::cout << "+----------------------------------------------------------------------------+\n";
    std::cout << "  Select an option [0-9]: ";
}

void TerminalUI::printTopicTable(const KnowledgeGraph& graph, const LearnerProfile& learner) {
    printBanner("CURRICULUM DAG TOPICS & EFFECTIVE WEIGHTS W'(v) = W(v)*[1 + a*(1 - P(v))]");
    std::cout << std::left 
              << std::setw(6)  << "ID" 
              << std::setw(30) << "Topic Name" 
              << std::setw(15) << "Category" 
              << std::setw(8)  << "Base(h)" 
              << std::setw(8)  << "Diff" 
              << std::setw(9)  << "Prof P(v)" 
              << std::setw(9)  << "Eff W'(v)" 
              << "Status\n";
    printDivider('-', 95);

    const auto& order = graph.getNodeOrder();
    for (const std::string& tid : order) {
        const TopicNode& n = graph.getNode(tid);
        double prof = learner.getProficiency(tid);
        double effW = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, tid);
        bool isMast = learner.isMastered(tid);

        std::string statusStr = isMast ? "[✓] Mastered" : (prof > 0.0 ? "[~] In-Prog" : "[ ] Pending");

        std::cout << std::left 
                  << std::setw(6)  << n.id 
                  << std::setw(30) << (n.title.length() > 28 ? n.title.substr(0, 26) + ".." : n.title)
                  << std::setw(15) << (n.category.length() > 13 ? n.category.substr(0, 12) + ".." : n.category)
                  << std::setw(8)  << std::fixed << std::setprecision(1) << n.baseHours
                  << std::setw(8)  << std::fixed << std::setprecision(1) << n.difficulty
                  << std::setw(9)  << std::fixed << std::setprecision(2) << prof
                  << std::setw(9)  << std::fixed << std::setprecision(2) << effW
                  << statusStr << "\n";
    }
    printDivider('-', 95);
}

void TerminalUI::printPath(const KnowledgeGraph& graph, const std::vector<std::string>& path, double totalHours) {
    std::cout << "\n=== COMPUTED ADAPTIVE TRAJECTORY ===\n";
    std::cout << "Steps: " << path.size() << " topics | Estimated Total Study Time: " 
              << std::fixed << std::setprecision(2) << totalHours << " Hours\n\n";

    for (size_t i = 0; i < path.size(); ++i) {
        const std::string& tid = path[i];
        const TopicNode& node = graph.getNode(tid);
        std::cout << "  [" << (i + 1) << "] " << tid << " : " << node.title 
                  << " (" << node.category << ", Base: " << node.baseHours << "h)\n";
        if (i + 1 < path.size()) {
            std::cout << "       |\n       v\n";
        }
    }
    std::cout << "\n";
}

void TerminalUI::printProgressBar(const std::string& label, double percentage, int width) {
    if (percentage < 0.0) percentage = 0.0;
    if (percentage > 1.0) percentage = 1.0;

    int pos = static_cast<int>(width * percentage);
    std::cout << std::left << std::setw(20) << label << " [";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << (percentage * 100.0) << "%\n";
}

void TerminalUI::printAsciiDAG(const KnowledgeGraph& graph, const LearnerProfile& learner, const std::string& goalNode) {
    printBanner("KNOWLEDGE GRAPH (DAG) PREREQUISITE STRUCTURE TOWARDS GOAL [" + goalNode + "]");
    std::cout << "\n";
    std::cout << "  (T01: Complexity) ---> (T02: Arrays) ----+---> (T03: Linked Lists) ---> (T06: Trees) --+-> (T07: BST)\n";
    std::cout << "         |                      |          |                                   |          |\n";
    std::cout << "         v                      v          +---> (T08: Heaps/Priority Q) <----+          v\n";
    std::cout << "  (T05: Recursion) <----+ (T04: Stacks)                |                               (T14: DP)\n";
    std::cout << "         |              |       |                      v                                  |\n";
    std::cout << "         +--------------+       +---------------> (T10: Traversals BFS/DFS)               v\n";
    std::cout << "         |                      |                      |                             (T15: Graph DP)\n";
    std::cout << "         v                      v                      v                                  ^\n";
    std::cout << "  (T09: Graph Rep) ---------> (T10: BFS/DFS) ---> (T12: Dijkstra Shortest Path) ---------+\n";
    std::cout << "\n";
    std::cout << "  Node Status Legend: [✓]=Mastered, [~]=In-Progress, [!]=Bottleneck / Penalty Active\n\n";

    const auto& ancestors = graph.getRequiredAncestors(goalNode);
    std::cout << "  Required Ancestor Modules for " << goalNode << " (" << graph.getNode(goalNode).title << "):\n  ";
    for (const std::string& anc : ancestors) {
        double p = learner.getProficiency(anc);
        char sym = learner.isMastered(anc) ? '+' : (p < 0.6 ? '!' : '~');
        std::cout << "[" << sym << " " << anc << "] ";
    }
    std::cout << "\n";
}

void TerminalUI::printLearnerDashboard(const KnowledgeGraph& graph, const LearnerProfile& learner) {
    printBanner("STUDENT KNOWLEDGE STATE & ANALYTICS DASHBOARD");
    std::cout << "  Student Name : " << learner.getName() << " | ID: " << learner.getId() << "\n";
    std::cout << "  Email        : " << learner.getEmail() << "\n";
    std::cout << "  Target Goal  : " << learner.getTargetGoal() << " (" << graph.getNode(learner.getTargetGoal()).title << ")\n";
    std::cout << "  Alpha Penalty: " << learner.getAlpha() << "\n";
    std::cout << "------------------------------------------------------------------------------\n";

    double avgProf = learner.getAverageProficiency();
    int mastCount = learner.getMasteredCount();
    int totalNodes = static_cast<int>(graph.nodeCount());

    printProgressBar("Mastery Progress", static_cast<double>(mastCount) / totalNodes);
    printProgressBar("Average Knowledge", avgProf);

    std::cout << "\n  Mastered Topics (" << mastCount << "/" << totalNodes << "): ";
    for (const auto& pair : learner.getAllProficiencies()) {
        if (learner.isMastered(pair.first)) {
            std::cout << pair.first << " ";
        }
    }
    std::cout << "\n";

    const auto& hist = learner.getHistory();
    if (!hist.empty()) {
        std::cout << "\n  Recent Assessment Logs (" << hist.size() << " attempts):\n";
        for (int i = static_cast<int>(hist.size()) - 1; i >= 0 && i >= static_cast<int>(hist.size()) - 3; --i) {
            const auto& h = hist[i];
            std::cout << "   - [" << h.timestamp << "] Topic: " << h.topicId 
                      << " | Score: " << (h.score * 100) << "% | Result: " << (h.passed ? "PASSED" : "FAILED")
                      << " | Weight: " << h.effectiveWeightBefore << "h -> " << h.effectiveWeightAfter << "h\n";
        }
    }
}

void TerminalUI::clearScreen() {
    // Cross-platform ANSI clear
    std::cout << "\033[2J\033[1;1H";
}

void TerminalUI::pause() {
    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(10000, '\n');
}
