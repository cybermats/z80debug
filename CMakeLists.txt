cmake_minimum_required(VERSION 3.5)

project(Z80Debug LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCE
        src/main.cpp
        src/ui/mainwindow.cpp
        src/ui/mainwindow.h
        src/ui/mainwindow.ui
        src/qhexedit/chunks.cpp
        src/qhexedit/chunks.h
        src/qhexedit/commands.cpp
        src/qhexedit/commands.h
        src/qhexedit/qhexedit.cpp
        src/qhexedit/qhexedit.h
        src/simulator/registermodel.cpp
        src/simulator/registermodel.h
        src/simulator/registeritem.cpp
        src/simulator/registeritem.h
        src/simulator/simulator.cpp
        src/simulator/simulator.h
        src/ui/loaddialog.cpp
        src/ui/loaddialog.h
        src/ui/loaddialog.ui
        src/simulator/mapreader.cpp
        src/simulator/mapreader.h
        src/simulator/breakpoint.h
        src/ui/codewidget.cpp
        src/ui/codewidget.h
        src/options.cpp
        src/options.h
        src/ui/highlighter.cpp
        src/ui/highlighter.h)


add_executable(Z80Debug ${SOURCE})

target_link_libraries(Z80Debug PRIVATE Qt5::Widgets libz80.so)
