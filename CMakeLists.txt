cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(UsingBoostCompute LANGUAGES C CXX VERSION 0.0.1)

find_package(BoostCompute REQUIRED)
find_package(Boost 1.54 REQUIRED)
find_package(OpenCL REQUIRED)
find_package(TiMemory REQUIRED)

#------------------------------------------------------------------------------#

function(found_message PACKAGE_NAME)
    if(NOT "${${PACKAGE_NAME}_INCLUDE_DIRS}" STREQUAL "")
        message(STATUS "${PACKAGE_NAME} include: ${${PACKAGE_NAME}_INCLUDE_DIRS}")
    endif()
    if(NOT "${${PACKAGE_NAME}_LIBRARIES}" STREQUAL "")
        message(STATUS "${PACKAGE_NAME} libraries: ${${PACKAGE_NAME}_LIBRARIES}")
    endif()
endfunction(found_message PACKAGE_NAME)

#------------------------------------------------------------------------------#

found_message(Boost)
found_message(BoostCompute)
found_message(OpenCL)
found_message(TiMemory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

include_directories(
    SYSTEM ${BoostCompute_INCLUDE_DIRS}
    SYSTEM ${OpenCL_INCLUDE_DIRS}
    SYSTEM ${Boost_INCLUDE_DIRS}
    ${TiMemory_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/source)

file(GLOB headers ${PROJECT_SOURCE_DIR}/source/*.hpp)
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)

option(BUILD_SHARED_LIBS "Build shared library" ON)
add_library(boost-compute ${headers} ${sources})
target_link_libraries(boost-compute
    ${BoostCompute_LIBRARIES} ${OpenCL_LIBRARIES} ${TiMemory_LIBRARIES})
set_target_properties(boost-compute PROPERTIES
    SOVERSION                   ${PROJECT_VERSION_MAJOR}
    VERSION                     ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE   ${BUILD_SHARED_LIBS})

add_executable(hello hello.cpp)
target_link_libraries(hello boost-compute)
add_executable(transform transform.cpp)
target_link_libraries(transform boost-compute)

include(GNUInstallDirs)

# C/C++ compiled libraries
install(TARGETS boost-compute
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT ${PROJECT_NAME}LibraryDepends)

# C/C++ compiled binaries
install(TARGETS hello transform
    DESTINATION ${CMAKE_INSTALL_BINDIR})

# C/C++ development headers
install(FILES ${headers}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}LibraryDepends
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
