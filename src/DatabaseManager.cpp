#include "DatabaseManager.hpp"
#include "OptimizationEngine.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

DatabaseManager::DatabaseManager(std::string dbPath)
    : dbPath_(std::move(dbPath)), logFilePath_("database/transactions.sql"), isConnected_(true) {
}

bool DatabaseManager::initializeDatabase() {
    std::ofstream ofs(logFilePath_, std::ios::app);
    if (ofs.is_open()) {
        ofs << "-- Session Initialized: " << std::time(nullptr) << "\n";
        ofs << "BEGIN TRANSACTION;\n";
        ofs << "-- System Ready.\n";
        ofs << "COMMIT;\n\n";
        return true;
    }
    return false;
}

void DatabaseManager::logTransaction(const std::string& sqlQuery) {
    std::ofstream ofs(logFilePath_, std::ios::app);
    if (ofs.is_open()) {
        ofs << sqlQuery << "\n";
    }
}

bool DatabaseManager::saveStudent(const LearnerProfile& learner) {
    std::ostringstream ss;
    ss << "INSERT OR REPLACE INTO students (student_id, name, email, target_goal_node, updated_at) "
       << "VALUES ('" << learner.getId() << "', '" << learner.getName() << "', '"
       << learner.getEmail() << "', '" << learner.getTargetGoal() << "', CURRENT_TIMESTAMP);";
    logTransaction(ss.str());
    return true;
}

bool DatabaseManager::saveEvaluationLog(const std::string& studentId, const std::string& topicId,
                                       double score, bool passed, bool rerouted) {
    std::ostringstream ss;
    ss << "INSERT INTO evaluation_logs (student_id, topic_id, score_obtained, total_score, passed, reroute_triggered) "
       << "VALUES ('" << studentId << "', '" << topicId << "', " << score << ", 1.0, "
       << (passed ? 1 : 0) << ", " << (rerouted ? 1 : 0) << ");";
    logTransaction(ss.str());
    return true;
}

bool DatabaseManager::saveTrajectory(const std::string& studentId, const std::string& goalTopicId,
                                     const std::string& algorithm, const std::vector<std::string>& path,
                                     double estimatedHours) {
    std::ostringstream pathStr;
    for (size_t i = 0; i < path.size(); ++i) {
        pathStr << path[i] << (i + 1 < path.size() ? "->" : "");
    }

    std::ostringstream ss;
    ss << "INSERT INTO learning_trajectories (student_id, goal_topic_id, algorithm_used, computed_path, total_estimated_hours) "
       << "VALUES ('" << studentId << "', '" << goalTopicId << "', '" << algorithm << "', '"
       << pathStr.str() << "', " << estimatedHours << ");";
    logTransaction(ss.str());
    return true;
}

bool DatabaseManager::exportDashboardData(const KnowledgeGraph& graph, const LearnerProfile& learner,
                                         const std::vector<std::string>& currentPath,
                                         double totalHours, const std::string& outputPath) {
    std::ofstream ofs(outputPath);
    if (!ofs.is_open()) return false;

    ofs << "{\n";
    ofs << "  \"student\": {\n";
    ofs << "    \"id\": \"" << learner.getId() << "\",\n";
    ofs << "    \"name\": \"" << learner.getName() << "\",\n";
    ofs << "    \"email\": \"" << learner.getEmail() << "\",\n";
    ofs << "    \"goal_node\": \"" << learner.getTargetGoal() << "\",\n";
    ofs << "    \"avg_proficiency\": " << std::fixed << std::setprecision(3) << learner.getAverageProficiency() << ",\n";
    ofs << "    \"mastered_count\": " << learner.getMasteredCount() << ",\n";
    ofs << "    \"total_estimated_hours\": " << std::fixed << std::setprecision(2) << totalHours << "\n";
    ofs << "  },\n";

    // Nodes export
    ofs << "  \"nodes\": [\n";
    const auto& allNodes = graph.getAllNodes();
    const auto& order = graph.getNodeOrder();
    for (size_t i = 0; i < order.size(); ++i) {
        const std::string& tid = order[i];
        const TopicNode& n = graph.getNode(tid);
        double prof = learner.getProficiency(tid);
        bool mast = learner.isMastered(tid);
        double effWeight = OptimizationEngine::calculateEffectiveNodeWeight(graph, learner, tid);

        bool inCurrentPath = false;
        int pathIndex = -1;
        for (size_t p = 0; p < currentPath.size(); ++p) {
            if (currentPath[p] == tid) {
                inCurrentPath = true;
                pathIndex = static_cast<int>(p) + 1;
                break;
            }
        }

        ofs << "    {\n";
        ofs << "      \"id\": \"" << n.id << "\",\n";
        ofs << "      \"title\": \"" << n.title << "\",\n";
        ofs << "      \"category\": \"" << n.category << "\",\n";
        ofs << "      \"base_hours\": " << n.baseHours << ",\n";
        ofs << "      \"difficulty\": " << n.difficulty << ",\n";
        ofs << "      \"proficiency\": " << std::fixed << std::setprecision(2) << prof << ",\n";
        ofs << "      \"effective_weight\": " << std::fixed << std::setprecision(2) << effWeight << ",\n";
        ofs << "      \"is_mastered\": " << (mast ? "true" : "false") << ",\n";
        ofs << "      \"in_path\": " << (inCurrentPath ? "true" : "false") << ",\n";
        ofs << "      \"path_order\": " << pathIndex << ",\n";
        ofs << "      \"description\": \"" << n.description << "\"\n";
        ofs << "    }" << (i + 1 < order.size() ? "," : "") << "\n";
    }
    ofs << "  ],\n";

    // Edges export
    ofs << "  \"edges\": [\n";
    size_t edgeCounter = 0;
    size_t totalEdges = graph.edgeCount();
    for (const std::string& fromId : order) {
        for (const auto& edge : graph.getOutgoingEdges(fromId)) {
            edgeCounter++;
            ofs << "    {\n";
            ofs << "      \"from\": \"" << edge.fromId << "\",\n";
            ofs << "      \"to\": \"" << edge.toId << "\",\n";
            ofs << "      \"weight\": " << edge.weight << "\n";
            ofs << "    }" << (edgeCounter < totalEdges ? "," : "") << "\n";
        }
    }
    ofs << "  ],\n";

    // Active Path export
    ofs << "  \"current_path\": [";
    for (size_t i = 0; i < currentPath.size(); ++i) {
        ofs << "\"" << currentPath[i] << "\"" << (i + 1 < currentPath.size() ? ", " : "");
    }
    ofs << "],\n";

    // History export
    ofs << "  \"evaluation_history\": [\n";
    const auto& hist = learner.getHistory();
    for (size_t i = 0; i < hist.size(); ++i) {
        const auto& h = hist[i];
        ofs << "    {\n";
        ofs << "      \"topic_id\": \"" << h.topicId << "\",\n";
        ofs << "      \"score\": " << h.score << ",\n";
        ofs << "      \"passed\": " << (h.passed ? "true" : "false") << ",\n";
        ofs << "      \"timestamp\": \"" << h.timestamp << "\",\n";
        ofs << "      \"weight_before\": " << h.effectiveWeightBefore << ",\n";
        ofs << "      \"weight_after\": " << h.effectiveWeightAfter << "\n";
        ofs << "    }" << (i + 1 < hist.size() ? "," : "") << "\n";
    }
    ofs << "  ]\n";
    ofs << "}\n";

    return true;
}
