#ifndef STUDENTEXCEPTIONS_H
#define STUDENTEXCEPTIONS_H

#include <exception>
#include <string>

class StudentException : public std::exception {
public:
    explicit StudentException(std::string message) noexcept : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }
    ~StudentException() noexcept override = default;
private:
    std::string message_;
};

class InvalidIDException final : public StudentException {
public:
    explicit InvalidIDException(int id) noexcept
        : StudentException("ID invalide : " + std::to_string(id) + ". L'identifiant doit etre superieur ou egal a 1000.") {}
};

class InvalidGradeException final : public StudentException {
public:
    explicit InvalidGradeException(double gpa) noexcept
        : StudentException("GPA invalide : " + std::to_string(gpa) + ". Le GPA doit etre compris entre 0.0 et 4.0.") {}
};

class StudentNotFoundException final : public StudentException {
public:
    explicit StudentNotFoundException(int id) noexcept
        : StudentException("Etudiant introuvable avec l'ID : " + std::to_string(id) + ".") {}
};

class DuplicateIDException final : public StudentException {
public:
    explicit DuplicateIDException(int id) noexcept
        : StudentException("ID duplique : " + std::to_string(id) + ". Un etudiant avec cet ID existe deja.") {}
};

#endif // STUDENTEXCEPTIONS_H
