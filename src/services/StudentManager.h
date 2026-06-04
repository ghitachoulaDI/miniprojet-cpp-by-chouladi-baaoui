#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "../models/Student.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class StudentManager {
public:
    using StudentPtr = std::shared_ptr<Student>;

    void add(const StudentPtr& student);
    void remove(int id);
    void update(int id, const StudentPtr& updated);

    StudentPtr findById(int id) const;
    std::vector<StudentPtr> findByName(const std::string& value) const;
    std::vector<StudentPtr> sortByGpa() const;
    std::vector<StudentPtr> sortByName() const;
    std::vector<StudentPtr> getAllVector() const;

    double averageGpa() const noexcept;
    int count() const noexcept;
    int countByType(const std::string& type) const;
    void clear() noexcept;

    const std::map<int, StudentPtr>& getAll() const noexcept;

private:
    std::map<int, StudentPtr> students_;
    static std::string lower(std::string value);
};

#endif // STUDENTMANAGER_H
