QT += core gui widgets charts
CONFIG += c++17 warn_on
TEMPLATE = app
TARGET = StudentManager

SOURCES += \
    src/main.cpp \
    src/models/Student.cpp \
    src/models/UndergraduateStudent.cpp \
    src/models/GraduateStudent.cpp \
    src/models/PhDStudent.cpp \
    src/services/StudentManager.cpp \
    src/persistence/PersistenceManager.cpp \
    src/gui/MainWindow.cpp \
    src/gui/StudentDialog.cpp

HEADERS += \
    src/models/Student.h \
    src/models/UndergraduateStudent.h \
    src/models/GraduateStudent.h \
    src/models/PhDStudent.h \
    src/services/StudentManager.h \
    src/exceptions/StudentExceptions.h \
    src/persistence/PersistenceManager.h \
    src/gui/MainWindow.h \
    src/gui/StudentDialog.h

FORMS += \
    src/gui/MainWindow.ui \
    src/gui/StudentDialog.ui

DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
