#include "../src/models/GraduateStudent.h"
#include "../src/models/PhDStudent.h"
#include "../src/models/UndergraduateStudent.h"
#include "../src/services/StudentManager.h"
#include "../src/persistence/PersistenceManager.h"
#include "../src/exceptions/StudentExceptions.h"
#include <cassert>
#include <cmath>
#include <filesystem>
#include <iostream>

static bool almost(double a, double b) { return std::fabs(a - b) < 1e-9; }

int main() {
    UndergraduateStudent licence("Amira Benali", 1001, 3.80, "Informatique");
    GraduateStudent master("Cherif Omar", 1002, 3.50, "IA");
    PhDStudent doctorat("Meziane Riad", 1003, 3.90, "Dr. Hadj", 2);

    assert(licence.getType() == "Licence");
    assert(master.getType() == "Master");
    assert(doctorat.getType() == "Doctorat");
    assert(almost(licence.computeScholarship(), 500.0));
    assert(almost(master.computeScholarship(), 1050.0));
    assert(almost(doctorat.computeScholarship(), 1200.0));
    assert(licence.displayString().find("[Licence]") != std::string::npos);

    bool thrown = false;
    try { UndergraduateStudent bad("Bad", 42, 3.0, "Info"); } catch (const InvalidIDException&) { thrown = true; }
    assert(thrown);
    thrown = false;
    try { GraduateStudent bad("Bad", 1004, 5.5, "IA"); } catch (const InvalidGradeException&) { thrown = true; }
    assert(thrown);

    StudentManager manager;
    manager.add(std::make_shared<UndergraduateStudent>(licence));
    manager.add(std::make_shared<GraduateStudent>(master));
    manager.add(std::make_shared<PhDStudent>(doctorat));
    assert(manager.count() == 3);
    assert(manager.countByType("Licence") == 1);
    assert(manager.findById(1002)->getName() == "Cherif Omar");
    assert(manager.findByName("amira").size() == 1);
    assert(manager.sortByGpa().front()->getId() == 1003);
    assert(manager.sortByName().front()->getId() == 1001);
    assert(almost(manager.averageGpa(), (3.8 + 3.5 + 3.9) / 3.0));

    thrown = false;
    try { manager.add(std::make_shared<GraduateStudent>("Doublon", 1001, 3.3, "Cloud")); } catch (const DuplicateIDException&) { thrown = true; }
    assert(thrown);

    manager.update(1002, std::make_shared<GraduateStudent>("Cherif Omar", 1002, 3.70, "Cloud"));
    assert(almost(manager.findById(1002)->getGpa(), 3.70));
    manager.remove(1001);
    assert(manager.count() == 2);
    thrown = false;
    try { manager.remove(9999); } catch (const StudentNotFoundException&) { thrown = true; }
    assert(thrown);

    std::filesystem::create_directories("tests/tmp");
    PersistenceManager pm("tests/tmp/students_test.txt");
    pm.save(manager);
    StudentManager loaded;
    pm.load(loaded);
    assert(loaded.count() == 2);
    assert(loaded.findById(1003)->getType() == "Doctorat");

    std::cout << "Tous les tests sont reussis.\n";
    return 0;
}
