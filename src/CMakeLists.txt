cmake_minimum_required(VERSION 3.5)

project(Scientific_calculator_V1.0 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

# compilation flags
add_compile_options(-Wall -Werror -Wextra -pedantic -g)

# set project source directories
set(PROJECT_SOURCES
        main.cc
        controller/controller.h
        view/view.cc
        view/view.h
        view/qcustomplot.cc
        view/qcustomplot.h
        view/view.ui
)

# add model static library (libmodel.a)
add_library( _model STATIC model/model.cc model/model.h model/lib/functions.h )

# add executable w/o static library libmodel.a
add_executable( Scientific_calculator_V1.0 ${PROJECT_SOURCES} )

# link static library libmodel.a and Qt
target_link_libraries( Scientific_calculator_V1.0 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets )
target_link_libraries( Scientific_calculator_V1.0 PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport )
target_link_libraries( Scientific_calculator_V1.0 PRIVATE _model )

###########################################################
#   adding model as subproject for testing and coverage   #
###########################################################
include_directories( model )
add_subdirectory( model )
