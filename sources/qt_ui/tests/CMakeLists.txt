set ( TEST_NAME ${LIB_NAME}_Test )

set ( CMAKE_AUTOMOC ON )
set ( CMAKE_AUTORCC ON )
set ( CMAKE_AUTOUIC ON )

if ( CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set ( SOURCE_FILES 
    "main.cpp"
    "UiTest.h"
    "UiTest.cpp"
   )

add_executable( ${TEST_NAME} ${SOURCE_FILES} )

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_include_directories ( ${TEST_NAME}
PRIVATE
    ..
    ${CMAKE_HOME_DIRECTORY}/sources/tasks_core
    )

target_link_libraries( ${TEST_NAME}
PRIVATE
    Qt5::Widgets
    Qt5::Test
    TasksCore
    ${LIB_NAME}
    )
