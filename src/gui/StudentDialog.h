#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <memory>

class Student;
class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QLabel;
class QSpinBox;

class StudentDialog : public QDialog {
    Q_OBJECT
public:
    explicit StudentDialog(QWidget* parent = nullptr);
    explicit StudentDialog(const std::shared_ptr<Student>& student, QWidget* parent = nullptr);
    std::shared_ptr<Student> student() const;

private slots:
    void updateDynamicFields();
    void validateAndAccept();

private:
    void buildUi();
    void fillFromStudent(const std::shared_ptr<Student>& student);
    void applyStyle();

    QLineEdit* idEdit_{};
    QLineEdit* nameEdit_{};
    QDoubleSpinBox* gpaSpin_{};
    QComboBox* typeCombo_{};
    QLabel* extraLabel_{};
    QLineEdit* extraEdit_{};
    QLabel* yearLabel_{};
    QSpinBox* yearSpin_{};
    std::shared_ptr<Student> student_;
};

#endif // STUDENTDIALOG_H
