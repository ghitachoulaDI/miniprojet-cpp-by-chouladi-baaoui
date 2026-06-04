#include "StudentDialog.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../models/UndergraduateStudent.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

StudentDialog::StudentDialog(QWidget* parent) : QDialog(parent) {
    buildUi();
    applyStyle();
}

StudentDialog::StudentDialog(const std::shared_ptr<Student>& student, QWidget* parent) : QDialog(parent) {
    buildUi();
    fillFromStudent(student);
    applyStyle();
}

std::shared_ptr<Student> StudentDialog::student() const { return student_; }

void StudentDialog::buildUi() {
    setWindowTitle("Fiche etudiant");
    setMinimumWidth(460);

    auto* mainLayout = new QVBoxLayout(this);
    auto* title = new QLabel("Ajouter / Modifier un etudiant", this);
    title->setObjectName("dialogTitle");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    auto* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);

    idEdit_ = new QLineEdit(this);
    idEdit_->setValidator(new QIntValidator(1000, 999999999, this));
    idEdit_->setPlaceholderText("Ex : 1001");

    nameEdit_ = new QLineEdit(this);
    nameEdit_->setPlaceholderText("Nom complet");

    gpaSpin_ = new QDoubleSpinBox(this);
    gpaSpin_->setRange(0.0, 4.0);
    gpaSpin_->setDecimals(2);
    gpaSpin_->setSingleStep(0.10);

    typeCombo_ = new QComboBox(this);
    typeCombo_->addItems({"Licence", "Master", "Doctorat"});

    extraLabel_ = new QLabel(this);
    extraEdit_ = new QLineEdit(this);
    yearLabel_ = new QLabel("Annee", this);
    yearSpin_ = new QSpinBox(this);
    yearSpin_->setRange(1, 8);

    form->addRow("ID", idEdit_);
    form->addRow("Nom", nameEdit_);
    form->addRow("GPA", gpaSpin_);
    form->addRow("Type", typeCombo_);
    form->addRow(extraLabel_, extraEdit_);
    form->addRow(yearLabel_, yearSpin_);
    mainLayout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText("Valider");
    buttons->button(QDialogButtonBox::Cancel)->setText("Annuler");
    mainLayout->addWidget(buttons);

    connect(typeCombo_, &QComboBox::currentTextChanged, this, &StudentDialog::updateDynamicFields);
    connect(buttons, &QDialogButtonBox::accepted, this, &StudentDialog::validateAndAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &StudentDialog::reject);
    updateDynamicFields();
}

void StudentDialog::updateDynamicFields() {
    const QString type = typeCombo_->currentText();
    const bool phd = type == "Doctorat";
    yearLabel_->setVisible(phd);
    yearSpin_->setVisible(phd);

    if (type == "Licence") {
        extraLabel_->setText("Filiere");
        extraEdit_->setPlaceholderText("Ex : Informatique");
    } else if (type == "Master") {
        extraLabel_->setText("Sujet");
        extraEdit_->setPlaceholderText("Ex : Intelligence Artificielle");
    } else {
        extraLabel_->setText("Superviseur");
        extraEdit_->setPlaceholderText("Ex : Dr. Hadj");
    }
}

void StudentDialog::validateAndAccept() {
    try {
        if (idEdit_->text().trimmed().isEmpty() || nameEdit_->text().trimmed().isEmpty() || extraEdit_->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Veuillez remplir tous les champs obligatoires.");
            return;
        }
        const int id = idEdit_->text().toInt();
        const std::string name = nameEdit_->text().trimmed().toStdString();
        const double gpa = gpaSpin_->value();
        const std::string extra = extraEdit_->text().trimmed().toStdString();
        const QString type = typeCombo_->currentText();

        if (type == "Licence") {
            student_ = std::make_shared<UndergraduateStudent>(name, id, gpa, extra);
        } else if (type == "Master") {
            student_ = std::make_shared<GraduateStudent>(name, id, gpa, extra);
        } else {
            student_ = std::make_shared<PhDStudent>(name, id, gpa, extra, yearSpin_->value());
        }
        accept();
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Erreur de validation", ex.what());
    }
}

void StudentDialog::fillFromStudent(const std::shared_ptr<Student>& student) {
    if (!student) return;
    idEdit_->setText(QString::number(student->getId()));
    idEdit_->setReadOnly(true);
    nameEdit_->setText(QString::fromStdString(student->getName()));
    gpaSpin_->setValue(student->getGpa());
    typeCombo_->setCurrentText(QString::fromStdString(student->getType()));
    typeCombo_->setEnabled(false);

    if (auto u = std::dynamic_pointer_cast<UndergraduateStudent>(student)) {
        extraEdit_->setText(QString::fromStdString(u->getMajor()));
    } else if (auto g = std::dynamic_pointer_cast<GraduateStudent>(student)) {
        extraEdit_->setText(QString::fromStdString(g->getResearchTopic()));
    } else if (auto p = std::dynamic_pointer_cast<PhDStudent>(student)) {
        extraEdit_->setText(QString::fromStdString(p->getSupervisor()));
        yearSpin_->setValue(p->getYearOfStudy());
    }
    updateDynamicFields();
}

void StudentDialog::applyStyle() {
    setStyleSheet(R"(
        QDialog { background: #f8fafc; font-size: 13px; }
        QLabel#dialogTitle { color: #1e3a8a; font-size: 18px; font-weight: 700; margin: 8px; }
        QLineEdit, QComboBox, QDoubleSpinBox, QSpinBox { padding: 8px; border: 1px solid #cbd5e1; border-radius: 8px; background: white; }
        QPushButton { padding: 8px 15px; border-radius: 8px; background: #1e40af; color: white; font-weight: 600; }
        QPushButton:hover { background: #1d4ed8; }
    )");
}
