#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../persistence/PersistenceManager.h"
#include "../services/StudentManager.h"
#include <QMainWindow>
#include <memory>
#include <vector>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void addStudent();
    void editStudent();
    void deleteStudent();
    void refreshTable();
    void applyFilters();
    void sortByGpa();
    void sortByName();
    void exportCsv();
    void showGpaHistogram();
    void toggleTheme();

private:
    void buildUi();
    void populateTable(const std::vector<StudentManager::StudentPtr>& students);
    void updateStats();
    void autoSave();
    int selectedStudentId() const;
    void applyTheme();
    std::vector<StudentManager::StudentPtr> filteredStudents() const;

    StudentManager manager_;
    PersistenceManager persistence_;
    QTableWidget* table_{};
    QLineEdit* searchEdit_{};
    QComboBox* typeFilter_{};
    QDoubleSpinBox* minGpa_{};
    QDoubleSpinBox* maxGpa_{};
    QLabel* statsLabel_{};
    bool darkMode_{false};
};

#endif // MAINWINDOW_H
