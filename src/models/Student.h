#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
public:
    Student(std::string name, int id, double gpa);
    virtual ~Student() = default;

    const std::string& getName() const noexcept;
    int getId() const noexcept;
    double getGpa() const noexcept;

    void setName(const std::string& name);
    void setGpa(double gpa);

    virtual std::string getType() const = 0;
    virtual double computeScholarship() const = 0;
    virtual void display() const = 0;
    virtual std::string displayString() const = 0;
    virtual std::string extraFieldLabel() const = 0;
    virtual std::string extraFieldValue() const = 0;

protected:
    static void validateId(int id);
    static void validateGpa(double gpa);
    static std::string trim(const std::string& value);
    static std::string formatMoney(double amount);
    std::string baseDisplay() const;

private:
    std::string name_;
    int id_;
    double gpa_;
};

#endif // STUDENT_H
