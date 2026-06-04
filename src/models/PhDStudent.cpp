#include "PhDStudent.h"
#include <iostream>
#include <stdexcept>

PhDStudent::PhDStudent(std::string name, int id, double gpa, std::string supervisor, int yearOfStudy)
    : Student(std::move(name), id, gpa), supervisor_(trim(supervisor)), yearOfStudy_(yearOfStudy) {
    if (supervisor_.empty()) {
        throw std::invalid_argument("Le superviseur ne peut pas etre vide.");
    }
    if (yearOfStudy_ < 1 || yearOfStudy_ > 8) {
        throw std::invalid_argument("L'annee de doctorat doit etre comprise entre 1 et 8.");
    }
}

const std::string& PhDStudent::getSupervisor() const noexcept { return supervisor_; }
int PhDStudent::getYearOfStudy() const noexcept { return yearOfStudy_; }

void PhDStudent::setSupervisor(const std::string& supervisor) {
    const auto cleaned = trim(supervisor);
    if (cleaned.empty()) {
        throw std::invalid_argument("Le superviseur ne peut pas etre vide.");
    }
    supervisor_ = cleaned;
}

void PhDStudent::setYearOfStudy(int yearOfStudy) {
    if (yearOfStudy < 1 || yearOfStudy > 8) {
        throw std::invalid_argument("L'annee de doctorat doit etre comprise entre 1 et 8.");
    }
    yearOfStudy_ = yearOfStudy;
}

std::string PhDStudent::getType() const { return "Doctorat"; }

double PhDStudent::computeScholarship() const { return 1200.0; }

std::string PhDStudent::displayString() const {
    return baseDisplay() + " | Superviseur : " + supervisor_ + " | Annee : "
        + std::to_string(yearOfStudy_) + " | Bourse : " + formatMoney(computeScholarship());
}

void PhDStudent::display() const {
    std::cout << displayString() << std::endl;
}

std::string PhDStudent::extraFieldLabel() const { return "Superviseur"; }
std::string PhDStudent::extraFieldValue() const { return supervisor_ + " | Annee " + std::to_string(yearOfStudy_); }
