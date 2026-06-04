#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include "../services/StudentManager.h"
#include <string>

class PersistenceManager {
public:
    explicit PersistenceManager(std::string path = "data/students.txt");
    void save(const StudentManager& manager) const;
    void load(StudentManager& manager) const;
    const std::string& path() const noexcept;
private:
    std::string path_;
    static std::string escape(std::string value);
    static std::string unescape(std::string value);
};

#endif // PERSISTENCEMANAGER_H
