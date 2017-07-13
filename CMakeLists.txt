cmake_minimum_required (VERSION 3.0)

project ( Automatic_Differentiation )

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-O0 -g -Wall -Wpedantic")

add_library( Nodes
    Nodes.cpp
)
target_include_directories( Nodes
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable( demo 
    main.cpp
)
target_link_libraries( demo
    LINK_PUBLIC Nodes
)

add_custom_target( run
    COMMAND ./demo
    DEPENDS demo
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)
