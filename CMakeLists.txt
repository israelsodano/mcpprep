cmake_minimum_required(VERSION 3.22)
project(mcpprep VERSION 1.0.0)

set(CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_BINARY_DIR}")

include_directories("src")
file(GLOB SOURCES
    "src/*.h"
)
add_library(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    VERSION ${PROJECT_VERSION})


find_package(Boost 1.82.0 REQUIRED)
find_package(mongocxx 3.8.0 REQUIRED)
find_package(simdjson 3.2.2 REQUIRED)

include_directories(${mongocxx_INCLUDE_DIRS}
    ${simdjson_LIBRARIES}
    ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    ${simdjson_LIBRARIES}
    ${mongocxx_LIBRARIES}
    ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})