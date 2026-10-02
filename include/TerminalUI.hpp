#ifndef TERMINAL_UI_HPP
#define TERMINAL_UI_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include "AnalyticsEngine.hpp"
#include <string>
#include <vector>

/**
 * @brief Terminal User Interface utility for interactive console rendering.
 */
class TerminalUI {
public:
    static void printHeader();
    static void printBanner(const std::string& title);
    static void printMenu();
    static void printTopicTable(const KnowledgeGraph& graph, const LearnerProfile& learner);
    static void printPath(const KnowledgeGraph& graph, const std::vector<std::string>& path, double totalHours);
    static void printProgressBar(const std::string& label, double percentage, int width = 30);
    static void printAsciiDAG(const KnowledgeGraph& graph, const LearnerProfile& learner, const std::string& goalNode);
    static void printLearnerDashboard(const KnowledgeGraph& graph, const LearnerProfile& learner);
    static void printDivider(char ch = '=', int length = 75);
    static void clearScreen();
    static void pause();
};

#endif // TERMINAL_UI_HPP
