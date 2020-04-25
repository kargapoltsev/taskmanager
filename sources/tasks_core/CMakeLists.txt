set ( LIB_NAME TasksCore )

set ( HEADER_FILES 
    "Task.h"
    "Project.h"
    "DataStore.h"
    "CoreExeptions.h"
)

set ( SOURCE_FILES 
    "Task.cpp"
    "Project.cpp"
    "DataStore.cpp"
)

add_library( ${LIB_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} )

add_subdirectory( tests )
