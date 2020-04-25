set ( LIB_NAME QtUI )

set ( CMAKE_AUTOMOC ON )
set ( CMAKE_AUTORCC ON )
set ( CMAKE_AUTOUIC ON )

if ( CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#find_package ( Qt5 COMPONENTS Widgets REQUIRED )

set ( HEADER_FILES 
    "MainWindow.h"
    "HierarchyTaskList.h"
    "HierarchyTaskListModel.h"
    "TaskNoteEditor.h"
    "ProjectManager.h"
    "ProjectListModel.h"
    "LineEditDialog.h"
    )

set ( SOURCE_FILES 
    "MainWindow.cpp"
    "HierarchyTaskList.cpp"
    "HierarchyTaskListModel.cpp"
    "TaskNoteEditor.cpp"
    "ProjectManager.cpp"
    "ProjectListModel.cpp"
    "LineEditDialog.cpp"
    )

add_library( ${LIB_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} )

target_include_directories ( ${LIB_NAME} 
PRIVATE
    ${CMAKE_HOME_DIRECTORY}/sources/tasks_core
    )


target_link_libraries ( ${LIB_NAME}
PRIVATE
    Qt5::Widgets
    TasksCore
    )

add_subdirectory( tests )
