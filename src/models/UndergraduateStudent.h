#ifndef UNDERGRADUATESTUDENT_H
#define UNDERGRADUATESTUDENT_H

#include "Student.h"

class UndergraduateStudent final : public Student {
public:
    UndergraduateStudent(std::string name, int id, double gpa, std::string major);

    const std::string& getMajor() const noexcept;
    void setMajor(const std::string& major);

    std::string getType() const override;
    double computeScholarship() const override;
    void display() const override;
    std::string displayString() const override;
    std::string extraFieldLabel() const override;
    std::string extraFieldValue() const override;

private:
    std::string major_;
};

#endif // UNDERGRADUATESTUDENT_H
