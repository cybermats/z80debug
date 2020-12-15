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

set (SOURCE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    chunks.cpp
    chunks.h
    commands.cpp
    commands.h
    qhexedit.cpp
    qhexedit.h
    registermodel.cpp
    registermodel.h
    registeritem.cpp
    registeritem.h
    simulator.cpp
    simulator.h
    loaddialog.cpp
    loaddialog.h
    loaddialog.ui
    mapreader.cpp
    mapreader.h
    breakpoint.cpp
    breakpoint.h
    codewidget.cpp
    codewidget.h
    options.cpp
    options.h
    )



add_executable(Z80Debug ${SOURCE})

target_link_libraries(Z80Debug PRIVATE Qt5::Widgets libz80.so)
