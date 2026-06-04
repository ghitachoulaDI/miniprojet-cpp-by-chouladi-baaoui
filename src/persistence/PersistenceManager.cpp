#include "PersistenceManager.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../models/UndergraduateStudent.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

PersistenceManager::PersistenceManager(std::string path) : path_(std::move(path)) {}
const std::string& PersistenceManager::path() const noexcept { return path_; }

void PersistenceManager::save(const StudentManager& manager) const {
    const std::filesystem::path filePath(path_);
    if (filePath.has_parent_path()) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream out(path_, std::ios::trunc);
    if (!out) return;

    for (const auto& pair : manager.getAll()) {
        const auto& s = pair.second;
        out << s->getType() << '|' << s->getId() << '|' << escape(s->getName()) << '|' << s->getGpa() << '|';
        if (auto u = std::dynamic_pointer_cast<UndergraduateStudent>(s)) {
            out << escape(u->getMajor());
        } else if (auto g = std::dynamic_pointer_cast<GraduateStudent>(s)) {
            out << escape(g->getResearchTopic());
        } else if (auto p = std::dynamic_pointer_cast<PhDStudent>(s)) {
            out << escape(p->getSupervisor()) << '|' << p->getYearOfStudy();
        }
        out << '\n';
    }
}

void PersistenceManager::load(StudentManager& manager) const {
    std::ifstream in(path_);
    if (!in) return;

    StudentManager loaded;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        try {
            std::vector<std::string> parts;
            std::stringstream ss(line);
            std::string token;
            while (std::getline(ss, token, '|')) parts.push_back(unescape(token));
            if (parts.size() < 5) continue;

            const std::string type = parts[0];
            const int id = std::stoi(parts[1]);
            const std::string name = parts[2];
            const double gpa = std::stod(parts[3]);

            if ((type == "Licence" || type == "Undergraduate") && parts.size() == 5) {
                loaded.add(std::make_shared<UndergraduateStudent>(name, id, gpa, parts[4]));
            } else if ((type == "Master" || type == "Graduate") && parts.size() == 5) {
                loaded.add(std::make_shared<GraduateStudent>(name, id, gpa, parts[4]));
            } else if ((type == "Doctorat" || type == "PhD") && parts.size() == 6) {
                loaded.add(std::make_shared<PhDStudent>(name, id, gpa, parts[4], std::stoi(parts[5])));
            }
        } catch (...) {
            // Ligne corrompue ignoree : le programme reste robuste.
        }
    }
    manager = loaded;
}

std::string PersistenceManager::escape(std::string value) {
    for (auto& c : value) {
        if (c == '|') c = '/';
    }
    return value;
}

std::string PersistenceManager::unescape(std::string value) {
    return value;
}
