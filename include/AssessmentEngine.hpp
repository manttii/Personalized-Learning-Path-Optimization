#ifndef ASSESSMENT_ENGINE_HPP
#define ASSESSMENT_ENGINE_HPP

#include "LearnerProfile.hpp"
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Representation of a multiple-choice diagnostic/quiz question.
 */
struct QuizQuestion {
    std::string topicId;
    std::string questionText;
    std::vector<std::string> options;
    int correctOptionIndex;
    std::string explanation;

    QuizQuestion() : correctOptionIndex(0) {}
    QuizQuestion(std::string topicId_, std::string text_, std::vector<std::string> opts,
                 int correctIdx, std::string exp_)
        : topicId(std::move(topicId_)), questionText(std::move(text_)),
          options(std::move(opts)), correctOptionIndex(correctIdx),
          explanation(std::move(exp_)) {}
};

/**
 * @brief Assessment result summary for an evaluation event.
 */
struct AssessmentResult {
    std::string topicId;
    int totalQuestions;
    int correctAnswers;
    double scorePercentage;
    bool passed;
    std::string feedback;
};

/**
 * @brief Assessment and Diagnostic Engine for evaluating student proficiencies.
 */
class AssessmentEngine {
private:
    std::vector<QuizQuestion> questionBank_;
    std::unordered_map<std::string, std::vector<QuizQuestion>> topicQuestionMap_;

public:
    AssessmentEngine();

    void addQuestion(const QuizQuestion& q);
    bool loadDefaultQuestions();
    bool loadFromFile(const std::string& jsonPath);

    std::vector<QuizQuestion> getQuestionsForTopic(const std::string& topicId) const;
    std::vector<QuizQuestion> getDiagnosticBaselineQuestions(int count = 5) const;

    AssessmentResult gradeSubmission(const std::string& topicId,
                                    const std::vector<int>& userAnswers) const;

    void applyResultToLearner(LearnerProfile& learner, const AssessmentResult& result,
                             double wBefore, double wAfter);
};

#endif // ASSESSMENT_ENGINE_HPP
