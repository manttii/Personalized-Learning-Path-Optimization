#ifndef LEARNER_PROFILE_HPP
#define LEARNER_PROFILE_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <ctime>

/**
 * @brief Record of an assessment or quiz attempt.
 */
struct AssessmentRecord {
    std::string topicId;
    double score;
    bool passed;
    std::string timestamp;
    double effectiveWeightBefore;
    double effectiveWeightAfter;
    std::string remarks;
};

/**
 * @brief Encapsulates a student's dynamic learning state and knowledge vector P(v).
 */
class LearnerProfile {
private:
    std::string studentId_;
    std::string name_;
    std::string email_;
    std::string targetGoalNode_;
    double alphaPenalty_; // Tuning parameter for weak prerequisite penalty (default 0.75)
    
    // Knowledge state vector P(v) in [0.0, 1.0]
    std::unordered_map<std::string, double> proficiencies_;
    std::unordered_map<std::string, bool> mastered_;
    std::vector<AssessmentRecord> history_;
    std::vector<std::string> currentOptimizedPath_;

public:
    LearnerProfile();
    LearnerProfile(std::string studentId, std::string name, std::string email, std::string targetGoal = "T15");

    const std::string& getId() const { return studentId_; }
    const std::string& getName() const { return name_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getTargetGoal() const { return targetGoalNode_; }
    double getAlpha() const { return alphaPenalty_; }

    void setName(const std::string& name) { name_ = name; }
    void setEmail(const std::string& email) { email_ = email; }
    void setTargetGoal(const std::string& goal) { targetGoalNode_ = goal; }
    void setAlpha(double a) { alphaPenalty_ = a; }

    double getProficiency(const std::string& topicId) const;
    void setProficiency(const std::string& topicId, double score);

    bool isMastered(const std::string& topicId) const;
    void setMastered(const std::string& topicId, bool status);

    void recordAssessment(const std::string& topicId, double score, bool passed, 
                          double wBefore, double wAfter, const std::string& remarks = "");

    const std::vector<AssessmentRecord>& getHistory() const { return history_; }
    const std::unordered_map<std::string, double>& getAllProficiencies() const { return proficiencies_; }

    void setCurrentPath(const std::vector<std::string>& path) { currentOptimizedPath_ = path; }
    const std::vector<std::string>& getCurrentPath() const { return currentOptimizedPath_; }

    double getAverageProficiency() const;
    int getMasteredCount() const;
    void initializeDefaultBaselines();
};

#endif // LEARNER_PROFILE_HPP
