add_executable( ${PROJECT_NAME}
    "main.cpp"
    )

#find_package ( Qt5 COMPONENTS Widgets REQUIRED )

target_include_directories ( ${PROJECT_NAME} 
PRIVATE
#    ${Qt5Widgets_INCLUDES}
    ${CMAKE_HOME_DIRECTORY}/sources/tasks_core
#    ${CMAKE_HOME_DIRECTORY}/sources/qt_ui
    )

target_link_libraries( ${PROJECT_NAME}
PRIVATE
#    Qt5::Widgets
    TasksCore
#    QtUi
    )
