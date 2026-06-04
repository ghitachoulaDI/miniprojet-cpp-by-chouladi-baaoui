cmake_minimum_required(VERSION 3.16)
project(StudentManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/gui)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

set(MODEL_SOURCES
    src/models/Student.cpp
    src/models/UndergraduateStudent.cpp
    src/models/GraduateStudent.cpp
    src/models/PhDStudent.cpp
)

set(SERVICE_SOURCES
    src/services/StudentManager.cpp
    src/persistence/PersistenceManager.cpp
)

add_executable(StudentManager
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.ui
    src/gui/StudentDialog.cpp
    src/gui/StudentDialog.h
    src/gui/StudentDialog.ui
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
)

target_include_directories(StudentManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(StudentManager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)

add_custom_command(TARGET StudentManager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:StudentManager>/data
)

add_executable(StudentManagerTests
    tests/test_main.cpp
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
)

target_include_directories(StudentManagerTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

enable_testing()
add_test(NAME StudentManagerBusinessTests COMMAND StudentManagerTests)
