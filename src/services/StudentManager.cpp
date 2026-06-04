#include "StudentManager.h"
#include "../exceptions/StudentExceptions.h"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <stdexcept>

void StudentManager::add(const StudentPtr& student) {
    if (!student) {
        throw std::invalid_argument("Impossible d'ajouter un etudiant nul.");
    }
    const int id = student->getId();
    if (students_.find(id) != students_.end()) {
        throw DuplicateIDException(id);
    }
    students_[id] = student;
}

void StudentManager::remove(int id) {
    if (students_.erase(id) == 0) {
        throw StudentNotFoundException(id);
    }
}

void StudentManager::update(int id, const StudentPtr& updated) {
    if (!updated) {
        throw std::invalid_argument("Impossible de modifier avec un etudiant nul.");
    }
    auto it = students_.find(id);
    if (it == students_.end()) {
        throw StudentNotFoundException(id);
    }
    if (updated->getId() != id && students_.find(updated->getId()) != students_.end()) {
        throw DuplicateIDException(updated->getId());
    }
    if (updated->getId() != id) {
        students_.erase(it);
        students_[updated->getId()] = updated;
    } else {
        it->second = updated;
    }
}

StudentManager::StudentPtr StudentManager::findById(int id) const {
    auto it = students_.find(id);
    if (it == students_.end()) {
        throw StudentNotFoundException(id);
    }
    return it->second;
}

std::vector<StudentManager::StudentPtr> StudentManager::findByName(const std::string& value) const {
    std::vector<StudentPtr> result;
    const std::string needle = lower(value);
    std::for_each(students_.begin(), students_.end(), [&](const auto& pair) {
        const auto& s = pair.second;
        const std::string haystack = lower(s->getName() + " " + s->getType() + " " + s->extraFieldValue() + " " + std::to_string(s->getId()));
        if (haystack.find(needle) != std::string::npos) {
            result.push_back(s);
        }
    });
    return result;
}

std::vector<StudentManager::StudentPtr> StudentManager::sortByGpa() const {
    auto result = getAllVector();
    std::sort(result.begin(), result.end(), [](const StudentPtr& a, const StudentPtr& b) {
        if (a->getGpa() == b->getGpa()) return lower(a->getName()) < lower(b->getName());
        return a->getGpa() > b->getGpa();
    });
    return result;
}

std::vector<StudentManager::StudentPtr> StudentManager::sortByName() const {
    auto result = getAllVector();
    std::sort(result.begin(), result.end(), [](const StudentPtr& a, const StudentPtr& b) {
        return lower(a->getName()) < lower(b->getName());
    });
    return result;
}

std::vector<StudentManager::StudentPtr> StudentManager::getAllVector() const {
    std::vector<StudentPtr> result;
    result.reserve(students_.size());
    std::transform(students_.begin(), students_.end(), std::back_inserter(result), [](const auto& pair) { return pair.second; });
    return result;
}

double StudentManager::averageGpa() const noexcept {
    if (students_.empty()) return 0.0;
    const double sum = std::accumulate(students_.begin(), students_.end(), 0.0,
        [](double acc, const auto& pair) { return acc + pair.second->getGpa(); });
    return sum / static_cast<double>(students_.size());
}

int StudentManager::count() const noexcept {
    return static_cast<int>(students_.size());
}

int StudentManager::countByType(const std::string& type) const {
    const auto t = lower(type);
    return static_cast<int>(std::count_if(students_.begin(), students_.end(), [&](const auto& pair) {
        return lower(pair.second->getType()) == t;
    }));
}

void StudentManager::clear() noexcept {
    students_.clear();
}

const std::map<int, StudentManager::StudentPtr>& StudentManager::getAll() const noexcept {
    return students_;
}

std::string StudentManager::lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
