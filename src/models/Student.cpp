#include "Student.h"
#include "../exceptions/StudentExceptions.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

Student::Student(std::string name, int id, double gpa)
    : name_(trim(name)), id_(id), gpa_(gpa) {
    validateId(id_);
    validateGpa(gpa_);
    if (name_.empty()) {
        throw std::invalid_argument("Le nom de l'etudiant ne peut pas etre vide.");
    }
}

const std::string& Student::getName() const noexcept { return name_; }
int Student::getId() const noexcept { return id_; }
double Student::getGpa() const noexcept { return gpa_; }

void Student::setName(const std::string& name) {
    const auto cleaned = trim(name);
    if (cleaned.empty()) {
        throw std::invalid_argument("Le nom de l'etudiant ne peut pas etre vide.");
    }
    name_ = cleaned;
}

void Student::setGpa(double gpa) {
    validateGpa(gpa);
    gpa_ = gpa;
}

void Student::validateId(int id) {
    if (id < 1000) {
        throw InvalidIDException(id);
    }
}

void Student::validateGpa(double gpa) {
    if (gpa < 0.0 || gpa > 4.0) {
        throw InvalidGradeException(gpa);
    }
}

std::string Student::trim(const std::string& value) {
    auto begin = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) { return std::isspace(ch); });
    auto end = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();
    if (begin >= end) return "";
    return std::string(begin, end);
}

std::string Student::formatMoney(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << amount << " EUR";
    return oss.str();
}

std::string Student::baseDisplay() const {
    std::ostringstream oss;
    oss << "[" << getType() << "] ID : " << id_ << " | Nom : " << name_
        << " | GPA : " << std::fixed << std::setprecision(2) << gpa_;
    return oss.str();
}
