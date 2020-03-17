cmake_minimum_required(VERSION 2.8)

project( TaskManager )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

if( CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package( Qt5 COMPONENTS Widgets REQUIRED )


add_executable( ${PROJECT_NAME}
    "main.cpp"

    )

target_link_libraries( ${PROJECT_NAME}
    Qt5::Widgets
    )
