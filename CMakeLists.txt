cmake_minimum_required(VERSION 3.10)

project(time_tracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Test)



set(PROJECT_SOURCES
        src/main.cpp
        src/Application.h
        src/Application.cpp
        ${TS_FILES}
)

add_subdirectory(tests)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(time_tracker
        MANUAL_FINALIZATION
        #CORE
        src/core/ThemeManager.h
        src/core/ThemeManager.cpp
        src/core/Task.h
        src/core/Task.cpp
        src/core/TaskManager.h
        src/core/TaskManager.cpp
        src/core/TaskSettingsDialog.h
        src/core/TaskSettingsDialog.cpp
        src/core/DatabaseManager.h
        src/core/DatabaseManager.cpp
        #UI
        src/ui/MainWindow.h
        src/ui/MainWindow.cpp
        src/ui/SideMenu.h
        src/ui/SideMenu.cpp
        src/ui/TimerWindow.h
        src/ui/TimerWindow.cpp
        #COMPONENTS
        src/ui/components/DayCard.h
        src/ui/components/DayCard.cpp
        #PAGES
        src/ui/pages/BasePage.h
        src/ui/pages/BasePage.cpp
        src/ui/pages/TaskPage.h
        src/ui/pages/TaskPage.cpp
        src/ui/pages/HistoryPage.h
        src/ui/pages/HistoryPage.cpp
        src/ui/pages/SettingsPage.h
        src/ui/pages/SettingsPage.cpp
        src/ui/pages/ProfilePage.h
        src/ui/pages/ProfilePage.cpp
        src/ui/pages/StartPage.h
        src/ui/pages/StartPage.cpp
        #TESTS
        #tests/test_db.cpp
        ${PROJECT_SOURCES}
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
            src/ui/MainWindow.h
            src/ui/MainWindow.cpp
            src/ui/SideMenu.h
            src/ui/SideMenu.cpp
        )
    endif()
endif()

target_link_libraries(time_tracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Test)

set_target_properties(time_tracker PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    #WIN32_EXECUTABLE TRUE
)

install(TARGETS time_tracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(time_tracker)
endif()