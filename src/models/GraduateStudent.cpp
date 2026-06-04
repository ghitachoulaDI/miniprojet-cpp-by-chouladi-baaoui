#include "GraduateStudent.h"
#include <iostream>
#include <stdexcept>

GraduateStudent::GraduateStudent(std::string name, int id, double gpa, std::string researchTopic)
    : Student(std::move(name), id, gpa), researchTopic_(trim(researchTopic)) {
    if (researchTopic_.empty()) {
        throw std::invalid_argument("Le sujet de recherche ne peut pas etre vide.");
    }
}

const std::string& GraduateStudent::getResearchTopic() const noexcept { return researchTopic_; }

void GraduateStudent::setResearchTopic(const std::string& researchTopic) {
    const auto cleaned = trim(researchTopic);
    if (cleaned.empty()) {
        throw std::invalid_argument("Le sujet de recherche ne peut pas etre vide.");
    }
    researchTopic_ = cleaned;
}

std::string GraduateStudent::getType() const { return "Master"; }

double GraduateStudent::computeScholarship() const {
    return getGpa() * 300.0;
}

std::string GraduateStudent::displayString() const {
    return baseDisplay() + " | Sujet : " + researchTopic_ + " | Bourse : " + formatMoney(computeScholarship());
}

void GraduateStudent::display() const {
    std::cout << displayString() << std::endl;
}

std::string GraduateStudent::extraFieldLabel() const { return "Sujet"; }
std::string GraduateStudent::extraFieldValue() const { return researchTopic_; }
