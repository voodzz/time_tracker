cmake_minimum_required(VERSION 3.5)

project(time_tracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        timetracker.cpp
        timetracker.h
        timetracker.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(time_tracker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        qss/black.qss
        qss/white.qss
        recources.qrc
        classes/Task.h
        classes/Task.cpp
        classes/TaskSettingsDialog.h
        classes/TaskSettingsDialog.cpp
        classes/TimerDialog.h
        classes/TimerDialog.cpp
        classes/DeadlineDialog.h
        classes/DeadlineDialog.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET time_tracker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(time_tracker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(time_tracker
            ${PROJECT_SOURCES}
            qss/black.qss
            qss/white.qss
            resources.qrc
            classes/Task.h
            classes/Task.cpp
            classes/TaskSettingsDialog.h
            classes/TaskSettingsDialog.cpp
            classes/TimerDialog.h
            classes/TimerDialog.cpp
            classes/DeadlineDialog.h
            classes/DeadlineDialog.cpp
        )
    endif()
endif()

target_link_libraries(time_tracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(time_tracker PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS time_tracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(time_tracker)
endif()