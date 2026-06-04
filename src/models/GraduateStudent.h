#ifndef GRADUATESTUDENT_H
#define GRADUATESTUDENT_H

#include "Student.h"

class GraduateStudent final : public Student {
public:
    GraduateStudent(std::string name, int id, double gpa, std::string researchTopic);

    const std::string& getResearchTopic() const noexcept;
    void setResearchTopic(const std::string& researchTopic);

    std::string getType() const override;
    double computeScholarship() const override;
    void display() const override;
    std::string displayString() const override;
    std::string extraFieldLabel() const override;
    std::string extraFieldValue() const override;

private:
    std::string researchTopic_;
};

#endif // GRADUATESTUDENT_H
