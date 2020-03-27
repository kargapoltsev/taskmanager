set ( TEST_NAME ${LIB_NAME}_Test )

set ( SOURCE_FILES 
    "main.cpp"
    "TaskTests.cpp"
#    "ProjectTests.cpp"
    )

add_executable( ${TEST_NAME} ${SOURCE_FILES} )

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

target_include_directories ( ${TEST_NAME}
PRIVATE
    ..
    )

target_link_libraries( ${TEST_NAME}
PRIVATE
    gtest
    ${LIB_NAME}
    )
