#include "UndergraduateStudent.h"
#include <iostream>
#include <stdexcept>

UndergraduateStudent::UndergraduateStudent(std::string name, int id, double gpa, std::string major)
    : Student(std::move(name), id, gpa), major_(trim(major)) {
    if (major_.empty()) {
        throw std::invalid_argument("La filiere ne peut pas etre vide.");
    }
}

const std::string& UndergraduateStudent::getMajor() const noexcept { return major_; }

void UndergraduateStudent::setMajor(const std::string& major) {
    const auto cleaned = trim(major);
    if (cleaned.empty()) {
        throw std::invalid_argument("La filiere ne peut pas etre vide.");
    }
    major_ = cleaned;
}

std::string UndergraduateStudent::getType() const { return "Licence"; }

double UndergraduateStudent::computeScholarship() const {
    return getGpa() >= 3.5 ? 500.0 : 0.0;
}

std::string UndergraduateStudent::displayString() const {
    return baseDisplay() + " | Filiere : " + major_ + " | Bourse : " + formatMoney(computeScholarship());
}

void UndergraduateStudent::display() const {
    std::cout << displayString() << std::endl;
}

std::string UndergraduateStudent::extraFieldLabel() const { return "Filiere"; }
std::string UndergraduateStudent::extraFieldValue() const { return major_; }
