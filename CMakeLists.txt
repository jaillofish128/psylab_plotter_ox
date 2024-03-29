cmake_minimum_required(VERSION 3.10)

project(BloodOxy_draw)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    about.cpp
    darw.cpp
    histowindow.cpp
    libpsyclass.cpp
    main.cpp
    mainwindow.cpp
    odi.cpp
    qcustomplot.cpp
)
set(HEADERS
    about.h
    darw.hpp
    histowindow.h
    libpsyclass.h
    mainwindow.h
    odi.hpp
    qcustomplot.h)
set(UI
    about.ui
    histowindow.ui
    mainwindow.ui)

add_executable(BloodOxy_draw ${SOURCES} ${HEADERS} ${UI})
qt5_use_modules(BloodOxy_draw Core Network PrintSupport Widgets)
