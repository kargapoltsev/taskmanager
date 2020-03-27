set ( LIB_NAME TasksCore )

set ( HEADER_FILES 
    "Task.h"
#    "Project.h"
#    "TasksRepository.h"
)

set ( SOURCE_FILES 
    "Task.cpp"
#    "Project.cpp"
#    "TasksRepository.cpp"
)

add_library( ${LIB_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} )

add_subdirectory( tests )
