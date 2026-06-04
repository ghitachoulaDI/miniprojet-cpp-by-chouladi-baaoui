#include "MainWindow.h"
#include "StudentDialog.h"
#include "../exceptions/StudentExceptions.h"
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>
#include <algorithm>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), persistence_("data/students.txt") {
    buildUi();
    persistence_.load(manager_);
    refreshTable();
    applyTheme();
}

void MainWindow::buildUi() {
    setWindowTitle("StudentManager - Systeme de Gestion des Etudiants");
    resize(1200, 720);

    auto* toolbar = addToolBar("Actions");
    toolbar->setMovable(false);
    toolbar->addAction("Ajouter", this, &MainWindow::addStudent);
    toolbar->addAction("Modifier", this, &MainWindow::editStudent);
    toolbar->addAction("Supprimer", this, &MainWindow::deleteStudent);
    toolbar->addAction("Rafraichir", this, &MainWindow::refreshTable);
    toolbar->addAction("Trier GPA", this, &MainWindow::sortByGpa);
    toolbar->addAction("Trier Nom", this, &MainWindow::sortByName);
    toolbar->addAction("Exporter CSV", this, &MainWindow::exportCsv);
    toolbar->addAction("Histogramme GPA", this, &MainWindow::showGpaHistogram);
    toolbar->addAction("Mode sombre/clair", this, &MainWindow::toggleTheme);

    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);

    searchEdit_ = new QLineEdit(this);
    searchEdit_->setPlaceholderText("Recherche temps reel par nom, ID, type, filiere, sujet ou superviseur...");
    mainLayout->addWidget(searchEdit_);

    auto* filters = new QFormLayout();
    typeFilter_ = new QComboBox(this);
    typeFilter_->addItems({"Tous", "Licence", "Master", "Doctorat"});
    minGpa_ = new QDoubleSpinBox(this);
    minGpa_->setRange(0.0, 4.0);
    minGpa_->setDecimals(2);
    minGpa_->setValue(0.0);
    maxGpa_ = new QDoubleSpinBox(this);
    maxGpa_->setRange(0.0, 4.0);
    maxGpa_->setDecimals(2);
    maxGpa_->setValue(4.0);
    filters->addRow("Type", typeFilter_);
    filters->addRow("GPA min", minGpa_);
    filters->addRow("GPA max", maxGpa_);
    mainLayout->addLayout(filters);

    table_ = new QTableWidget(this);
    table_->setColumnCount(6);
    table_->setHorizontalHeaderLabels({"ID", "Nom", "Type", "GPA", "Bourse", "Detail"});
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    mainLayout->addWidget(table_, 1);

    statsLabel_ = new QLabel(this);
    mainLayout->addWidget(statsLabel_);
    setCentralWidget(central);
    statusBar()->showMessage("Pret");

    connect(searchEdit_, &QLineEdit::textChanged, this, &MainWindow::applyFilters);
    connect(typeFilter_, &QComboBox::currentTextChanged, this, &MainWindow::applyFilters);
    connect(minGpa_, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &MainWindow::applyFilters);
    connect(maxGpa_, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &MainWindow::applyFilters);
    connect(table_, &QTableWidget::cellDoubleClicked, this, &MainWindow::editStudent);
}

void MainWindow::populateTable(const std::vector<StudentManager::StudentPtr>& students) {
    table_->setRowCount(static_cast<int>(students.size()));
    for (int row = 0; row < static_cast<int>(students.size()); ++row) {
        const auto& s = students[static_cast<std::size_t>(row)];
        table_->setItem(row, 0, new QTableWidgetItem(QString::number(s->getId())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));
        table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s->getType())));
        table_->setItem(row, 3, new QTableWidgetItem(QString::number(s->getGpa(), 'f', 2)));
        table_->setItem(row, 4, new QTableWidgetItem(QString::number(s->computeScholarship(), 'f', 2)));
        table_->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(s->extraFieldValue())));
    }
    updateStats();
}

std::vector<StudentManager::StudentPtr> MainWindow::filteredStudents() const {
    const QString text = searchEdit_->text().trimmed().toLower();
    const QString type = typeFilter_->currentText();
    const double minG = minGpa_->value();
    const double maxG = maxGpa_->value();

    std::vector<StudentManager::StudentPtr> result;
    for (const auto& s : manager_.sortByName()) {
        if (s->getGpa() < minG || s->getGpa() > maxG) continue;
        if (type != "Tous" && QString::fromStdString(s->getType()) != type) continue;
        const QString hay = QString("%1 %2 %3 %4 %5")
            .arg(s->getId())
            .arg(QString::fromStdString(s->getName()))
            .arg(QString::fromStdString(s->getType()))
            .arg(QString::fromStdString(s->extraFieldLabel()))
            .arg(QString::fromStdString(s->extraFieldValue()))
            .toLower();
        if (text.isEmpty() || hay.contains(text)) result.push_back(s);
    }
    return result;
}

void MainWindow::refreshTable() {
    populateTable(manager_.sortByName());
    applyFilters();
    statusBar()->showMessage("Tableau actualise", 2500);
}

void MainWindow::applyFilters() {
    if (minGpa_->value() > maxGpa_->value()) {
        maxGpa_->setValue(minGpa_->value());
    }
    populateTable(filteredStudents());
}

void MainWindow::addStudent() {
    StudentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            manager_.add(dialog.student());
            autoSave();
            refreshTable();
            statusBar()->showMessage("Etudiant ajoute avec succes.", 3000);
        } catch (const std::exception& ex) {
            QMessageBox::critical(this, "Ajout impossible", ex.what());
        }
    }
}

void MainWindow::editStudent() {
    try {
        const int id = selectedStudentId();
        StudentDialog dialog(manager_.findById(id), this);
        if (dialog.exec() == QDialog::Accepted) {
            manager_.update(id, dialog.student());
            autoSave();
            refreshTable();
            statusBar()->showMessage("Etudiant modifie avec succes.", 3000);
        }
    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Modification impossible", ex.what());
    }
}

void MainWindow::deleteStudent() {
    try {
        const int id = selectedStudentId();
        const auto answer = QMessageBox::question(this, "Confirmation", "Supprimer l'etudiant selectionne ?");
        if (answer == QMessageBox::Yes) {
            manager_.remove(id);
            autoSave();
            refreshTable();
            statusBar()->showMessage("Etudiant supprime.", 3000);
        }
    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Suppression impossible", ex.what());
    }
}

void MainWindow::sortByGpa() { populateTable(manager_.sortByGpa()); }
void MainWindow::sortByName() { populateTable(manager_.sortByName()); }

void MainWindow::exportCsv() {
    const QString path = QFileDialog::getSaveFileName(this, "Exporter CSV", "students_export.csv", "CSV (*.csv)");
    if (path.isEmpty()) return;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export CSV", "Impossible de creer le fichier CSV.");
        return;
    }
    QTextStream out(&file);
    out << "ID;Nom;Type;GPA;Bourse;Detail\n";
    for (const auto& s : manager_.sortByName()) {
        out << s->getId() << ';' << QString::fromStdString(s->getName()) << ';'
            << QString::fromStdString(s->getType()) << ';' << QString::number(s->getGpa(), 'f', 2) << ';'
            << QString::number(s->computeScholarship(), 'f', 2) << ';' << QString::fromStdString(s->extraFieldValue()) << '\n';
    }
    statusBar()->showMessage("Export CSV termine.", 3000);
}

void MainWindow::showGpaHistogram() {
    int low = 0, mid = 0, high = 0, excellent = 0;
    for (const auto& s : manager_.getAllVector()) {
        const double g = s->getGpa();
        if (g < 2.0) ++low;
        else if (g < 3.0) ++mid;
        else if (g < 3.5) ++high;
        else ++excellent;
    }

    auto* set = new QBarSet("Etudiants");
    *set << low << mid << high << excellent;
    auto* series = new QBarSeries();
    series->append(set);

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques - Histogramme des GPA");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    auto* axisX = new QBarCategoryAxis();
    axisX->append({"< 2.0", "2.0 - 2.99", "3.0 - 3.49", ">= 3.5"});
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis();
    axisY->setRange(0, std::max({1, low, mid, high, excellent}));
    axisY->setTitleText("Nombre d'etudiants");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->hide();

    auto* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    auto* window = new QMainWindow(this);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setWindowTitle("Histogramme GPA");
    window->resize(720, 460);
    window->setCentralWidget(view);
    window->show();
}

void MainWindow::toggleTheme() {
    darkMode_ = !darkMode_;
    applyTheme();
}

void MainWindow::updateStats() {
    statsLabel_->setText(QString("Nombre d'etudiants : %1 | Moyenne GPA : %2 | Licence : %3 | Master : %4 | Doctorat : %5")
        .arg(manager_.count())
        .arg(manager_.averageGpa(), 0, 'f', 2)
        .arg(manager_.countByType("Licence"))
        .arg(manager_.countByType("Master"))
        .arg(manager_.countByType("Doctorat")));
}

int MainWindow::selectedStudentId() const {
    const int row = table_->currentRow();
    if (row < 0 || !table_->item(row, 0)) {
        throw StudentNotFoundException(-1);
    }
    return table_->item(row, 0)->text().toInt();
}

void MainWindow::autoSave() {
    persistence_.save(manager_);
}

void MainWindow::applyTheme() {
    const QString light = R"(
        QMainWindow, QWidget { background: #f8fafc; color: #0f172a; font-size: 13px; }
        QLineEdit, QComboBox, QDoubleSpinBox { padding: 8px; border: 1px solid #cbd5e1; border-radius: 8px; background: white; }
        QTableWidget { background: white; alternate-background-color: #f1f5f9; gridline-color: #cbd5e1; border: 1px solid #cbd5e1; border-radius: 8px; }
        QHeaderView::section { background: #1e3a8a; color: white; padding: 8px; font-weight: 700; }
        QToolBar { background: #e2e8f0; spacing: 7px; }
        QToolButton { padding: 7px 12px; border-radius: 7px; background: #ffffff; }
        QToolButton:hover { background: #dbeafe; }
        QLabel { padding: 5px; }
    )";
    const QString dark = R"(
        QMainWindow, QWidget { background: #0f172a; color: #e2e8f0; font-size: 13px; }
        QLineEdit, QComboBox, QDoubleSpinBox { padding: 8px; border: 1px solid #475569; border-radius: 8px; background: #1e293b; color: #e2e8f0; }
        QTableWidget { background: #1e293b; alternate-background-color: #334155; color: #f8fafc; gridline-color: #475569; border: 1px solid #475569; border-radius: 8px; }
        QHeaderView::section { background: #020617; color: white; padding: 8px; font-weight: 700; }
        QToolBar { background: #020617; spacing: 7px; }
        QToolButton { padding: 7px 12px; border-radius: 7px; background: #1e293b; color: #f8fafc; }
        QToolButton:hover { background: #334155; }
        QLabel { padding: 5px; }
    )";
    qApp->setStyleSheet(darkMode_ ? dark : light);
}
