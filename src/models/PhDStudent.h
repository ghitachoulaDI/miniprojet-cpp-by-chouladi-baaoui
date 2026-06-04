#ifndef PHDSTUDENT_H
#define PHDSTUDENT_H

#include "Student.h"

class PhDStudent final : public Student {
public:
    PhDStudent(std::string name, int id, double gpa, std::string supervisor, int yearOfStudy);

    const std::string& getSupervisor() const noexcept;
    int getYearOfStudy() const noexcept;
    void setSupervisor(const std::string& supervisor);
    void setYearOfStudy(int yearOfStudy);

    std::string getType() const override;
    double computeScholarship() const override;
    void display() const override;
    std::string displayString() const override;
    std::string extraFieldLabel() const override;
    std::string extraFieldValue() const override;

private:
    std::string supervisor_;
    int yearOfStudy_;
};

#endif // PHDSTUDENT_H
