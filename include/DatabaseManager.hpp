#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include "Graph.hpp"
#include "LearnerProfile.hpp"
#include <string>
#include <vector>

/**
 * @brief Relational DBMS Interface for persistent storage and progress tracking.
 * 
 * Implements transaction logging, foreign key relationship tracking,
 * and JSON state synchronization for the frontend analytics dashboard.
 */
class DatabaseManager {
private:
    std::string dbPath_;
    std::string logFilePath_;
    bool isConnected_;

public:
    explicit DatabaseManager(std::string dbPath = "database/pbl_curriculum.db");

    bool initializeDatabase();
    bool saveStudent(const LearnerProfile& learner);
    bool saveEvaluationLog(const std::string& studentId, const std::string& topicId,
                          double score, bool passed, bool rerouted);
    bool saveTrajectory(const std::string& studentId, const std::string& goalTopicId,
                        const std::string& algorithm, const std::vector<std::string>& path,
                        double estimatedHours);
    bool exportDashboardData(const KnowledgeGraph& graph, const LearnerProfile& learner,
                             const std::vector<std::string>& currentPath,
                             double totalHours, const std::string& outputPath = "web/data.json");

    void logTransaction(const std::string& sqlQuery);
    bool isConnected() const { return isConnected_; }
};

#endif // DATABASE_MANAGER_HPP
