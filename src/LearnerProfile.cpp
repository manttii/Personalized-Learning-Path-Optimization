#include "LearnerProfile.hpp"
#include <iomanip>
#include <sstream>
#include <ctime>

LearnerProfile::LearnerProfile()
    : studentId_("2510011893"), name_("Priyanshi Saini"), email_("priyanshi@geu.ac.in"),
      targetGoalNode_("T12"), alphaPenalty_(0.35) {
    initializeDefaultBaselines();
}

LearnerProfile::LearnerProfile(std::string studentId, std::string name, std::string email, std::string targetGoal)
    : studentId_(std::move(studentId)), name_(std::move(name)), email_(std::move(email)),
      targetGoalNode_(std::move(targetGoal)), alphaPenalty_(0.35) {
    initializeDefaultBaselines();
}

void LearnerProfile::initializeDefaultBaselines() {
    proficiencies_.clear();
    mastered_.clear();
    history_.clear();

    // Default baseline proficiencies (unmastered = 0.0)
    for (int i = 1; i <= 15; ++i) {
        std::ostringstream oss;
        oss << "T" << std::setw(2) << std::setfill('0') << i;
        std::string tid = oss.str();
        proficiencies_[tid] = 0.0;
        mastered_[tid] = false;
    }
}

double LearnerProfile::getProficiency(const std::string& topicId) const {
    auto it = proficiencies_.find(topicId);
    if (it != proficiencies_.end()) {
        return it->second;
    }
    return 0.0;
}

void LearnerProfile::setProficiency(const std::string& topicId, double score) {
    if (score < 0.0) score = 0.0;
    if (score > 1.0) score = 1.0;
    proficiencies_[topicId] = score;
    if (score >= 0.70) {
        mastered_[topicId] = true;
    }
}

bool LearnerProfile::isMastered(const std::string& topicId) const {
    auto it = mastered_.find(topicId);
    if (it != mastered_.end()) {
        return it->second;
    }
    return false;
}

void LearnerProfile::setMastered(const std::string& topicId, bool status) {
    mastered_[topicId] = status;
}

void LearnerProfile::recordAssessment(const std::string& topicId, double score, bool passed,
                                      double wBefore, double wAfter, const std::string& remarks) {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    AssessmentRecord rec;
    rec.topicId = topicId;
    rec.score = score;
    rec.passed = passed;
    rec.timestamp = std::string(buf);
    rec.effectiveWeightBefore = wBefore;
    rec.effectiveWeightAfter = wAfter;
    rec.remarks = remarks;

    history_.push_back(rec);
    setProficiency(topicId, score);
}

double LearnerProfile::getAverageProficiency() const {
    if (proficiencies_.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& pair : proficiencies_) {
        sum += pair.second;
    }
    return sum / static_cast<double>(proficiencies_.size());
}

int LearnerProfile::getMasteredCount() const {
    int count = 0;
    for (const auto& pair : mastered_) {
        if (pair.second) count++;
    }
    return count;
}
